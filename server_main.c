#include <stdio.h>
#include <string.h>
#include "server_hangman.h"
#include "common_socket.h"

int main(int argc, const char *argv[]) {
    int game_over = 0;
    int msg_len;
    char msg[20];
    char *word = "casa";
    char c;
    
    socket_t server_socket;
    socket_t client_socket;
    hangman_t hangman;
        
    socket_create(&server_socket);
    socket_bind_and_listen(&server_socket, "127.0.0.1", "7777");
    socket_accept(&server_socket, &client_socket);
    
    hangman_create(&hangman, word);
    
    while (!game_over) {
        socket_receive(&client_socket, &c, 1);
        game_over = hangman_try_letter(&hangman, c);
        msg_len = hangman_get_msg(&hangman, msg);
        socket_send(&client_socket, msg, msg_len);
    }

    hangman_destroy(&hangman);

    socket_destroy(&server_socket);

    return 0;
}
