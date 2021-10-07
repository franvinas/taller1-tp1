#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "common_socket.h"
#include "common_game_state.h"

typedef struct {
    socket_t sk;
    socket_t peer_sk;
} protocol_t;

int protocol_create(protocol_t *self);

int protocol_destroy(protocol_t *self);

int protocol_listen(protocol_t *self, const char *port);

int protocol_new_client(protocol_t *self);

int protocol_free_client(protocol_t *self);

int protocol_connect(protocol_t *self, const char *host, const char *port);

int protocol_server_send(protocol_t *self, const game_state_t *game_state);

int protocol_server_recv(protocol_t *self, char *c);

int protocol_client_send(protocol_t *self, char c);

int protocol_client_recv(protocol_t *self, 
                         char *partial_word,
                         unsigned char *tries_left,
                         bool *game_over);

#endif
