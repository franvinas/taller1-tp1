#include "common_socket.h"

#ifndef CLIENT_H
#define CLIENT_H

typedef struct {
    socket_t sk;
} client_t;


int client_create(client_t *self, const char *host, const char *port);

int client_run(client_t *self);

void client_print_summary(client_t *self);

int client_destroy(client_t *self);

#endif
