#include <stdio.h>
#include "common_socket.h"

int main(int argc, const char *argv[]) {
    socket_t client_socket;
    char *buffer = "hola";
    size_t length = sizeof(buffer);
    socket_create(&client_socket);
    socket_connect(&client_socket, "127.0.0.1", "7777");
    
    socket_send(&client_socket, buffer, length);

    

    return 0;
}
