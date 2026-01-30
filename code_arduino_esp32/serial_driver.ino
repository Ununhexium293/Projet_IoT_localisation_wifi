#include <Arduino.h>

//librairie pour communication en uart
#include <HardwareSerial.h>

HardwareSerial MySerial(2);
const int UART2_Rx = 16;
const int UART2_Tx = 17;

#include "define.h"
#include "serial_driver.h"

void serial_init(void)
{
    MySerial.begin(9600, SERIAL_8N1, UART2_Rx, UART2_Tx);
}

void serial_write(char *string)
{
    MySerial.println(string);
}

void serial_empty_buffer(void)
{
    while (MySerial.available() >= 1)
    {
      Serial.printf("%c", MySerial.read());
    }
}

int serial_read_until(char *c, int len)
{
    int found = 0;

    /*Pour que ça dure longtemps*/
    uint64_t last_time_serial = millis();

    int current_letter = 0;

    while (!found)
    {
        if (millis() - last_time_serial > MAX_DELAY_SERIAL)
        {
            Serial.printf("timeout\n");
            return 0;
        }

        if (MySerial.available() >= 1)
        {

            if (current_letter >= len)
            {
                found = 1;
            }else
            {
                char temp;
                sprintf(&temp, "%c", MySerial.read());
                Serial.printf("%c", temp);

                if (temp == c[current_letter])
                {
                    current_letter += 1;
                }else
                {
                    current_letter = 0;
                }
            }
        }
    }

    return 1;
}