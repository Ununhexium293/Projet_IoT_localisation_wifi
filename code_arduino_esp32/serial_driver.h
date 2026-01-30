#ifndef SERIAL_DRIVER_H
#define SERIAL_DRIVER_H

#define MAX_DELAY_SERIAL 15000

/*initialise les parametre du serial*/
void serial_init(void);

/*écris un string dans la connexion serial*/
void serial_write(char *string);

/*vide le buffer du serial*/
void serial_empty_buffer(void);

/*lis le serial jusqu'a ce que le string apparaisse ou timeout
renvois 0 si timeout, 1 si succes*/
int serial_read_until(char *c, int len);

#endif