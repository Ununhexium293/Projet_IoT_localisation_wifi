#ifndef LORA_PILOT_H
#define LORA_PILOT_H

#define AT_DR1 "AT+DR=EU868"
#define AT_DR2 "AT+DR=DR3"
#define DEVEUI "AT+ID=DevEUI,70B3D57ED0073496"
#define APPEUI "AT+ID=AppEUI,0000000000000000"
#define APPKEY "AT+KEY=APPKEY,6B6C025304DA03A05C84891F66E987E1"

#include "network.h"

/*initialise les parametre de connexion*/
void lora_setup(void);

/*lance la connexion au resau lora*/
int lora_connect(void);

/*permet de lire et envoyer le resultat d'un scan via le reseau lora
retourne 0 si l'envois a échoué, 1 sinon*/
int send_net_scan(net_scan_t *scan);

/*!!! deprecated !!!*/
int lora_send_msg(net_scan_t **scan_buffer, int start, int end, int remaining_time);

#endif