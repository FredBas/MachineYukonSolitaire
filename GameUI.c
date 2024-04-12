#include "GameUI.h"
#include <stdbool.h>
#include <stdio.h>

void printUI(Cardpile *tableau, Cardpile *foundation) {
    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\n\n");
    int row = 0;
    for (int i = 0; i < DECK_SIZE; i++) {
        Card *card = tableau[i % 7].bottom;
        if (card == NULL) {
            printf("Error: Not enough cards in the tableau.\n");
            return;
        }
        card->isFaceUp = false;
        printCard(card);
        tableau[i % 7].bottom = card->next;
        if (i % 7 == 6) {
            if (row % 2 == 0) {
                if (foundation[row / 2].size == 0) {
                    printf("\t\t[]\tF%d\n", row / 2 + 1);
                } else {
                    foundation[row / 2].top->isFaceUp = true;
                    printCard(foundation[row / 2].top);
                    printf("\tF%d\n", row / 2 + 1);
                }
            } else {
                printf("\n");
            }
            row++;
        }
    }

    printf("\n\n");
    printf("LAST Command: \n");
    printf("Message: \n");
    printf("INPUT > ");
}
