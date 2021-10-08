#include "common_protocol.h"
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

#define LISTEN_BACKLOG 50

/***********************
    Metodos publicos
************************/

int protocol_create(protocol_t *self) {
    return socket_create(&self->sk);
}

int protocol_destroy(protocol_t *self) {
    return socket_destroy(&self->sk);
}

int protocol_listen(protocol_t *self, const char *port) {
    return socket_bind_and_listen(&self->sk, NULL, port, LISTEN_BACKLOG);
}

int protocol_new_client(protocol_t *self) {
    return socket_accept(&self->sk, &self->peer_sk);    
}

int protocol_free_client(protocol_t *self) {
    return socket_destroy(&self->peer_sk);
}

int protocol_connect(protocol_t *self, const char *host, const char *port) {
    return socket_connect(&self->sk, host, port);
}

int protocol_server_send(protocol_t *self, const game_state_t *game_state) {
    int word_len = game_state_get_len(game_state);
    int msg_len = word_len + 3;
    unsigned short int word_len_be = htons(word_len);
    char *msg = malloc(msg_len);
    if (msg == NULL) return -1;
    
    msg[0] = (char) (game_state_game_over(game_state) ? 0x80 : 0);
    msg[0] = msg[0] | game_state_get_tries_left(game_state);
    
    memcpy(&msg[1], &word_len_be, 2);
    game_state_get_partial_word(game_state, &msg[3]);

    int s = socket_send(&self->peer_sk, msg, msg_len);
    free(msg);
    return s;
}

int protocol_server_recv(protocol_t *self, char *c) {
    return socket_recv(&self->peer_sk, c, 1);
}

int protocol_client_send(protocol_t *self, char c) {
    return socket_send(&self->sk, &c, 1);
}

int protocol_client_recv(protocol_t *self,
                         char **partial_word,
                         unsigned char *tries_left,
                         bool *game_over) {
    unsigned char game_info = 0;
    uint16_t word_len;
    int s;
    s = socket_recv(&self->sk, (char *) &game_info, 1);
    if (s != 0) return -1;

    *game_over = (game_info & 0x80) != 0;
    *tries_left = (game_info & 0x7F);

    s = socket_recv(&self->sk, (char *) &word_len, 2);
    if (s != 0) return -1;

    word_len = ntohs(word_len);
    if (*partial_word == NULL) {
        *partial_word = malloc(word_len + 1);
        if (*partial_word == NULL) return -1;
        memset(*partial_word, '\0', word_len + 1);
    }

    s = socket_recv(&self->sk, *partial_word, word_len);
    if (s != 0) return -1;

    return 0;
}
