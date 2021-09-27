#include <stdio.h>
#include <stdlib.h>
#include "server.h"


int main(int argc, const char *argv[]) {
    server_t server;

    if (argc != 4) {
        printf("Error en la cantidad de argumentos\n");
        printf("El servidor se ejecuta de la siguiente manera:\n");
        printf("./server <port> <N> <path al repositorio de palabras>\n");
        return 1;
    }

    server_create(&server, argv[1], atoi(argv[2]), argv[3]);
    
    server_run(&server);

    server_print_summary(&server);
    
    server_destroy(&server);

    return 0;
}
