#ifndef LORA_PILOT_H
#define LORA_PILOT_H

#define AT_DR1 "AT+DR=" /* Use Yours*/
#define AT_DR2 "AT+DR=" /* Use Yours*/
#define DEVEUI "AT+ID=DevEUI," /* Use Yours*/
#define APPEUI "AT+ID=AppEUI," /* Use Yours*/
#define APPKEY "AT+KEY=APPKEY," /* Use Yours*/

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