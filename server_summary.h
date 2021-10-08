#ifndef SERVER_SUMMARY_H
#define SERVER_SUMMARY_H

typedef struct {
    unsigned int victories;
    unsigned int defeats;
} summary_t;

/*
 *  Constructor
 */
int summary_create(summary_t *self);

/*
 *  Destructor
 */
int summary_destroy(summary_t *self);

/*
 *  Suma una victoria
 */
void summary_new_victory(summary_t *self);

/*
 *  Suma una derrota
 */
void summary_new_defeat(summary_t *self);

/*
 *  Imprime la cantidad de victorias y cantidad de derrotas con el 
 *  formato adecuado
 */
void summary_print(summary_t *self);

#endif
