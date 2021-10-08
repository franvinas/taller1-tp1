#include "server_summary.h"
#include <stdio.h>

int summary_create(summary_t *self) {
    self->victories = 0;
    self->defeats = 0;
    return 0;
}

int summary_destroy(summary_t *self) {
    return 0;
}

void summary_new_victory(summary_t *self) {
    self->victories++;
}

void summary_new_defeat(summary_t *self) {
    self->defeats++;
}

void summary_print(summary_t *self) {
    printf("Resumen:\n");
    printf("\tVictorias: %u\n", self->victories);
    printf("\tDerrotas: %u\n", self->defeats);
}
