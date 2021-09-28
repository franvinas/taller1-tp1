#include <stdio.h>
#include <stdbool.h>
#include "client.h"

int main(int argc, const char *argv[]) {
    client_t client;
    
    if (argc != 3) {
        printf("Error en la cantidad de argumentos\n");
        printf("El cliente se ejecuta de la siguiente manera:\n");
        printf("./client <host> <port>\n");
        return 1;
    }

    if (client_create(&client) != 0) {
        return 1;
    }

    if (client_connect(&client, argv[1], argv[2]) != 0) {
        return 1;
    }

    if (client_run(&client) != 0) {
        return 1;
    }

    if (client_destroy(&client) != 0) {
        return 1;
    }

    return 0;
}
