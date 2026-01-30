#include <Arduino.h>

#include "define.h"
#include "network.h"
#include "serial_driver.h"
#include "lora_pilot.h"

#define DEBUG

/* setup et loop */
void setup(void)
{
  Serial.begin(115200);
  lora_setup();
  setup_station();
}

/*Créé le buffer*/
net_scan_t *scan_buffer[SCAN_BUFFER_SIZE];
int begin = 0;
int end = 0;

uint64_t last_time_scan = 0;
uint64_t last_time_sent = 0;

void loop(void)
{
  /*atempt connection to the lora network*/
  lora_connect();
  serial_read_until("failed", 6);
  serial_empty_buffer();
    
  /*update times variables*/
  uint64_t current_time = millis();
  uint64_t time_scan = current_time - last_time_scan;
  uint64_t time_sent = current_time - last_time_sent;

  if (time_scan >= TIME_SCAN)
  {
    last_time_scan = current_time;
    scan_buffer[end] = scan_network();

    end = (end + 1) % SCAN_BUFFER_SIZE;

    if (end == begin)
    {
      free_scan(scan_buffer[begin]);
      begin = (begin + 1) % SCAN_BUFFER_SIZE;
    }

    Serial.printf("begin : %d | end : %d\n", begin, end);
  }

  if ((time_sent >= TIME_SENT) && (begin != end))
  {
    last_time_sent = current_time;
    
    int status = send_net_scan(scan_buffer[begin]);
      
    /*If sending was successfull*/
    if (status)
    {
      free_scan(scan_buffer[begin]);
      begin += 1;
    }

    Serial.printf("begin : %d | end : %d\n", begin, end);
  }
}