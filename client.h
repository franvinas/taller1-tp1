#include "common_socket.h"

#ifndef CLIENT_H
#define CLIENT_H

typedef struct {
    socket_t sk;
} client_t;

/*
 *  Constructor
 */
int client_create(client_t *self);


/*
 *  Destructor
 */
int client_destroy(client_t *self);

/*
 *  El cliente se conecta al servidor dado por la dirección "host" en el puerto "port"
 */
int client_connect(client_t *self, const char *host, const char *port);

/*
 *  El cliente corre con la interfaz de usuario que permite jugar al ahorcado mientras
 *  se comunica con el servidor.
 */
int client_run(client_t *self);

#endif
