#include "GameUI.h"
#include <stdbool.h>
#include <stdio.h>

void printUI(Cardpile *tableau[], Cardpile *foundation[]) {
    printf("\nC1\tC2\tC3\tC4\tC5\tC6\tC7\n\n");
    int row = 0;
    int iterator = 52;
    for (int i = 0; i < iterator; i++) {
        Card *card = getCardAt(tableau[i % 7], row);
        if (card == NULL) {
            printf("  \t");
            iterator++;
        } else {
            printCard(card);
        }

        if (i % 7 == 6) {
            if (row < 7 && row % 2 == 0) {
                if (foundation[row / 2]->size == 0) {
                    printf("\t\t[]\tF%d\n", row / 2 + 1);
                } else {
                    foundation[row / 2]->top->isFaceUp = true;
                    printCard(foundation[row / 2]->top);
                    printf("\tF%d\n", row / 2 + 1);
                }
            } else {
                printf("\n");
            }
            row++;
        }
    }

    printf("\n\n");

}




