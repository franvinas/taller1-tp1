#include "server.h"
#include "common_socket.h"
#include <stdio.h>
#include <stdlib.h>

#define LISTEN_BACKLOG 50

/***********************
    Metodos privados
************************/

bool play_hangman(hangman_t *hangman, socket_t * client_socket) {
    int msg_len;
    int game_over = 0;
    char c;
    char msg[100];

    msg_len = hangman_get_msg(hangman, msg);
    socket_send(client_socket, msg, msg_len);

    while (!game_over) {
        socket_receive(client_socket, &c, 1);
        game_over = hangman_try_letter(hangman, c);
        msg_len = hangman_get_msg(hangman, msg);
        socket_send(client_socket, msg, msg_len);
    }
    return (msg[0] & 0x3F) > 0;
}

/***********************
    Metodos publicos
************************/

int server_create(server_t *self, 
                  const char *port, 
                  int tries, 
                  const char *words_repo_name) {
    bool pasive = true;
    self->words_repo = fopen(words_repo_name, "r");
    if (self->words_repo == NULL) {
        printf("Error al abrir el archivo\n");
        return 1;
    }

    socket_create(&self->sk);
    socket_bind_and_listen(&self->sk, "127.0.0.1", 
                            port, LISTEN_BACKLOG, pasive);

    self->victories = 0;
    self->defeats = 0;
    self->tries = tries;
    return 0;
}

int server_destroy(server_t *self) {
    socket_destroy(&self->sk);
    fclose(self->words_repo);
    return 0;
}

int server_run(server_t *self) {
    socket_t client_socket;
    hangman_t hangman;
    bool victory;
    char *word = NULL;
    ssize_t line_len;
    size_t buffer_size;
    while ((line_len = getline(&word, &buffer_size, self->words_repo)) > 1) {
        socket_accept(&self->sk, &client_socket);
        
        word[line_len - 1] = '\0';
        hangman_create(&hangman, word, self->tries);
        victory = play_hangman(&hangman, &client_socket);
        if (victory)
            self->victories++;
        else
            self->defeats++;
        hangman_destroy(&hangman);
        socket_close(&client_socket);
    }

    free(word);
    return 0;
}

void server_print_summary(server_t *self) {
    printf("Resumen:\n");
    printf("\tVictorias: %u\n", self->victories);
    printf("\tDerrotas: %u\n", self->defeats);
}

