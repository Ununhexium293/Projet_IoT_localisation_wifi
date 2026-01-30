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

    serial_empty_buffer();

    return status;
}