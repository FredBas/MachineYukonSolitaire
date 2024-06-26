#include "GameUI.h"
#include <stdbool.h>
#include <stdio.h>

void printUI(Cardpile *tableau[], Cardpile *foundation[]) {
    bool emptyRow;
    printf("\n____________________________________________________________________________________");    printf("\nC1\tC2\tC3\tC4\tC5\tC6\tC7\n\n");
    int row = 0;
    int iterator = 52;
    int nullCardCounter = 0;
    for (int i = 0; i < iterator; i++) {
        Card *card = getCardAt(tableau[i % 7], row);
        if (card == NULL) {
            printf("  \t");
            iterator++;
            nullCardCounter++;
        } else {
            emptyRow = false;
            printCard(card);
        }

        if (i % 7 == 6) {
            if (nullCardCounter == 7) {
                emptyRow = true;
            } else {
                nullCardCounter = 0;
            }
            if (row < 7 && row % 2 == 0) {
                if (foundation[row / 2]->size == 0) {
                    printf("\t\t[]\tF%d\n", row / 2 + 1);
                } else {
                    foundation[row / 2]->top->isFaceUp = true;
                    printf("\t\t%c%c\tF%d\n", foundation[row / 2]->top->rank, foundation[row / 2]->top->suit, row / 2 + 1);
                    iterator = iterator - foundation[row / 2]->size;
                }
            } else {
                printf("\n");
            }
            row++;
        }
        if (emptyRow && row > 6) {
            break;
        }

    }

    printf("\n\n");

}




