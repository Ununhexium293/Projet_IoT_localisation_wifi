#include <Arduino.h>

//librairie pour le wifi
#include <WiFi.h>
#include <esp_wifi.h>

#include "define.h"
#include "network.h"

static void copy_string(char *s1, char *s2)
{
    int i = 0;
    for (; s1[i] != '\0'; i++)
    {
        s2[i] = s1[i];
    }
    s2[i] = '\0';
}

void setup_station(void)
{
    WiFi.mode(WIFI_STA);
}

net_scan_t *scan_network(void)
{
    /*Scan les réseau environant et conserve leur nombre*/
    Serial.println("Scanning networks ...");
    int nbNet = WiFi.scanNetworks();

    /*!!! try to keep the time it happened !!!*/

    if(nbNet < 0)
    {
      Serial.println("Failed to scan for networks.");
      return NULL;
    }else if(nbNet < 3)
    {
      Serial.println("Too few network detected.");
      return NULL;
    }

    /*prépare à stocker les BSSID et RSSI récupéré*/
    net_scan_t *scan = (net_scan_t *) malloc(sizeof(net_scan_t));
    scan -> nb_network = nbNet;    
    scan -> networks = (char **) malloc(sizeof(char *) * scan -> nb_network);

    /*Keep all scanned network in the scan structure*/
    char msg[25] = {0};

    for (int i = 0; i < scan -> nb_network; i++)
    {
        sprintf(msg, "AT+MSG=%02x%02x%02x%02x%02x%02x.%04ld", *WiFi.BSSID(i), *(WiFi.BSSID(i) + 1), *(WiFi.BSSID(i) + 2), *(WiFi.BSSID(i) + 3), *(WiFi.BSSID(i) + 4), *(WiFi.BSSID(i) + 5), WiFi.RSSI(i));
        scan -> networks[i] = (char *) malloc(sizeof(char) * 25);
        copy_string(msg, scan -> networks[i]);
    }

    Serial.printf("Done !\n\n");

    return scan;
}

void free_scan(net_scan_t *scan)
{
    for (int i = 0; i < scan -> nb_network; i++)
    {
        free(scan -> networks[i]);
    }
    free(scan -> networks);
    free(scan);
}