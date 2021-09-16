#include <stdio.h>
#include <string.h>
#include "server_hangman.h"

int main(int argc, const char *argv[]) {
    hangman_t hangman;
    char word[4] = "casa";
    char c;
    int game_over = 0;
    
    hangman_create(&hangman, word);

    while (!game_over) {
        if (c != '\n')
            printf("Enter character: ");
        c = getchar();
        if (c != '\n')
            game_over = hangman_try_letter(&hangman, c);
        
    }
    
    hangman_destroy(&hangman);


}