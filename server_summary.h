#ifndef SERVER_SUMMARY_H
#define SERVER_SUMMARY_H

typedef struct {
    unsigned int victories;
    unsigned int defeats;
} summary_t;

int summary_create(summary_t *self);

int summary_destroy(summary_t *self);

void summary_new_victory(summary_t *self);

void summary_new_defeat(summary_t *self);

void summary_print(summary_t *self);

#endif
