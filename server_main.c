#include <stdio.h>
#include <string.h>
#include "server_hangman.h"
#include "common_socket.h"

int main(int argc, const char *argv[]) {
    // hangman_t hangman;
    // char word[4] = "casa";
    // char c;
    // int game_over = 0;
    
    // hangman_create(&hangman, word);

    // while (!game_over) {
    //     if (c != '\n')
    //         printf("Enter character: ");
    //     c = getchar();
    //     if (c != '\n')
    //         game_over = hangman_try_letter(&hangman, c);
        
    // }
    
    // hangman_destroy(&hangman);

    socket_t server_socket;
    socket_t client_socket;
    char buffer[10];
    size_t length = 10;
        
    socket_create(&server_socket);
    socket_bind_and_listen(&server_socket, "127.0.0.1", "7777");
    socket_accept(&server_socket, &client_socket);
    
    socket_receive(&client_socket, buffer, length);

    printf("%s\n", buffer);

    return 0;
}
