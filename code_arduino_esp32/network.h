#ifndef NETWORK_H
#define NETWORK_H

typedef struct
{
    int nb_network;
    char **networks;
} net_scan_t;

/*initialise les parametre de l'antenne wifi en mode station pour scanner les réseaux alentour*/
void setup_station(void);

/*scan les réseaux alentour*/
net_scan_t *scan_network(void);

/*libere la mémoire alloué au scan*/
void free_scan(net_scan_t *scan);

#endif