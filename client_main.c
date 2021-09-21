#include <stdio.h>
#include "common_socket.h"

int main(int argc, const char *argv[]) {
    socket_t client_socket;
    char c;
    char msg[10];
    socket_create(&client_socket);
    socket_connect(&client_socket, "127.0.0.1", "7778");
    

    while (1) {
        if (c != '\n')
            printf("Enter character: ");
        c = getchar();
        if (c != '\n') {
            socket_send(&client_socket, &c, 1);
            socket_receive(&client_socket, msg, 1);
        }
    }    

    socket_destroy(&client_socket);

    return 0;
}
