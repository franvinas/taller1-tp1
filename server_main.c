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

    if (server_create(&server, argv[1]) != 0) {
        return 1;
    }
    
    if (server_run(&server, atoi(argv[2]), argv[3]) != 0) {
        server_destroy(&server);
        return 1;
    }

    server_print_summary(&server);
    
    if (server_destroy(&server) != 0) {
        return 1;
    }

    return 0;
}
