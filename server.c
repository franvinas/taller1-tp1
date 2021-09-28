#include "server.h"
#include "common_socket.h"
#include <stdio.h>
#include <stdlib.h>

#define LISTEN_BACKLOG 50

/***********************
    Metodos privados
************************/

static bool play_hangman(hangman_t *hangman, socket_t * client_socket) {
    int msg_len;
    int game_over = 0;
    char c;
    char *msg;

    msg_len = hangman_get_msg(hangman, &msg);
    socket_send(client_socket, msg, msg_len);

    while (!game_over) {
        socket_recv(client_socket, &c, 1);
        game_over = hangman_try_letter(hangman, c);
        msg_len = hangman_get_msg(hangman, &msg);
        socket_send(client_socket, msg, msg_len);
    }
    return (msg[0] & 0x3F) > 0;
}

/***********************
    Metodos publicos
************************/

int server_create(server_t *self) {
    socket_create(&self->sk);
    
    self->victories = 0;
    self->defeats = 0;
    return 0;
}

int server_destroy(server_t *self) {
    socket_destroy(&self->sk);
    fclose(self->words_repo);
    return 0;
}

int server_bind_and_listen(server_t *self, const char *port) {
    socket_bind_and_listen(&self->sk, "127.0.0.1", port, LISTEN_BACKLOG);
    return 0;
}

int server_run(server_t *self, int tries, const char *words_repo_name) {
    socket_t client_socket;
    hangman_t hangman;
    char *word = NULL;
    ssize_t line_len;
    size_t buffer_size;

    self->tries = tries;
    self->words_repo = fopen(words_repo_name, "r");
    if (self->words_repo == NULL) {
        printf("Error al abrir el archivo\n");
        return 1;
    }
    
    while ((line_len = getline(&word, &buffer_size, self->words_repo)) > 1) {
        socket_accept(&self->sk, &client_socket);    
        word[line_len - 1] = '\0';
        hangman_create(&hangman, word, self->tries);
        if (play_hangman(&hangman, &client_socket))
            self->victories++;
        else
            self->defeats++;
        hangman_destroy(&hangman);
        socket_destroy(&client_socket);
    }

    free(word);
    return 0;
}

void server_print_summary(server_t *self) {
    printf("Resumen:\n");
    printf("\tVictorias: %u\n", self->victories);
    printf("\tDerrotas: %u\n", self->defeats);
}

