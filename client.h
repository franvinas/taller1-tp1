#ifndef CLIENT_H
#define CLIENT_H

#include "common_protocol.h"

typedef struct {
    protocol_t protocol;
} client_t;

/*
 *  Constructor
 */
int client_create(client_t *self, const char *host, const char *port);


/*
 *  Destructor
 */
int client_destroy(client_t *self);

/*
 *  El cliente corre con la interfaz de usuario que permite jugar al ahorcado mientras
 *  se comunica con el servidor.
 */
int client_run(client_t *self);

#endif
