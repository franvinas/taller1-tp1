#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "server_hangman.h"
#include "common_socket.h"

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

void print_summary(unsigned int victories, unsigned int defeats) {
    printf("Resumen:\n");
    printf("\tVictorias: %u\n", victories);
    printf("\tDerrotas: %u\n", defeats);
}

int main(int argc, const char *argv[]) {
    char *word = NULL;
    unsigned int victories = 0;
    unsigned int defeats = 0;
    bool victory;
    FILE *words_repository;
    ssize_t line_len;
    size_t buffer_line_size;
    socket_t server_socket;
    socket_t client_socket;
    hangman_t hangman;

    if (argc != 4) {
        printf("Error en la cantidad de argumentos\n");
        printf("El servidor se ejecuta de la siguiente manera:\n");
        printf("./server <port> <N> <path al repositorio de palabras>\n");
        return -1;
    }

    words_repository = fopen(argv[3], "r");
    if (words_repository == NULL) {
        printf("Error al abrir el archivo\n");
        return -1;
    }

    socket_create(&server_socket);
    socket_bind_and_listen(&server_socket, "127.0.0.1", argv[1]);
    
    while ((line_len = getline(&word, &buffer_line_size, words_repository)) > 1) {
        socket_accept(&server_socket, &client_socket);
        
        word[line_len - 1] = '\0';
        hangman_create(&hangman, word, atoi(argv[2]));
        victory = play_hangman(&hangman, &client_socket);
        if (victory)
            victories++;
        else
            defeats++;
        hangman_destroy(&hangman);
    }

    print_summary(victories, defeats);

    free(word);
    socket_destroy(&server_socket);
    fclose(words_repository);

    return 0;
}
