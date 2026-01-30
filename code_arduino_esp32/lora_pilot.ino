#include <Arduino.h>
#include <cstdio>

#include "define.h"
#include "network.h"
#include "serial_driver.h"
#include "lora_pilot.h"

void lora_setup(void)
{
    serial_init();

    serial_write(AT_DR1);
    serial_read_until("DR", 2);
    serial_empty_buffer();

    serial_write(DEVEUI);
    serial_read_until("ID", 2);
    serial_empty_buffer();

    serial_write(APPEUI);
    serial_read_until("ID", 2);
    serial_empty_buffer();

    serial_write(APPKEY);
    serial_read_until("KEY", 3);
    serial_empty_buffer();

    serial_write(AT_DR2);
    serial_read_until("DR", 2);
    serial_empty_buffer();
    serial_read_until("DR", 2);
    serial_empty_buffer();

}

int lora_connect(void)
{
    serial_empty_buffer();
    
    serial_write("AT+JOIN");
    int status = serial_read_until("Done", 4);
    
    serial_empty_buffer();
    Serial.println();

    return status;
}

int send_net_scan(net_scan_t *scan)
{
    serial_empty_buffer();
    
    int status = 1;

    /*Envoie des adresses mac, s'arrete si on a envoyé tous les réseaux détecter ou qu'on a atteint la limite définie*/
    for (int i = 0; (i < scan -> nb_network) && (i < MAX_SENT_NETWORK); i++)
    {
        Serial.println(scan -> networks[i]);
        serial_write(scan -> networks[i]);
        status = serial_read_until("Done", 4);

        if (status == 0)
        {
            return status;
        }
    }

    //char tail[9];
    //sscanf(tail, "Done.%03d", (scan -> nb_network > MAX_SENT_NETWORK) ? MAX_SENT_NETWORK : scan -> nb_network);
    //tail[8] = '\0';
    //serial_write(tail);
    //status = serial_read_until("Done", 4);

    serial_empty_buffer();

    return status;
}

int lora_send_msg(net_scan_t **scan_buffer, int start, int end, int remaining_time)
{
    int time = millis();
    int status = 1;

    int i = start;

    for (; i != end && (millis() - time <= remaining_time); (i >= SCAN_BUFFER_SIZE) ? i = 0 : i++)
    {
        status = send_net_scan(scan_buffer[i]);

        if (status == 0)
        {
            return i;
        }

        free_scan(scan_buffer[i]);
    }

    return i;
}