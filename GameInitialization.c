#include "GameInitialization.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void startupPopulateTableau(Cardpile *tableau, Card *head) {
    for (int i = 0; i < DECK_SIZE; i++) {
        Card *card = head;
        head = head->next;
        card->next = NULL;
        card->prev = NULL;

        int tableauIndex = i % NUMBER_OF_TABLEAUS;
        if (tableau[tableauIndex].top == NULL) {
            tableau[tableauIndex].top = card;
            tableau[tableauIndex].bottom = card;
        } else {
            card->prev = tableau[tableauIndex].top;
            tableau[tableauIndex].top->next = card;
            tableau[tableauIndex].top = card;
        }
    }
}

void initializeStartup(Cardpile **tableau, Cardpile **foundation, Cardpile *deck) {

    // Read cards from file
    deck->top = createDeckFromFile("../unshuffledDeck.txt");

    if (deck->top == NULL) {
        printf("Error: Failed to read cards from file.\n");
        return;
    }

    // Initialize tableau and foundation as empty stacks
    for (int i = 0; i < NUMBER_OF_TABLEAUS; ++i) {
        tableau[i] = malloc(sizeof(Cardpile));
        tableau[i]->bottom = NULL;
        tableau[i]->top = NULL;
        tableau[i]->size = 0;
    }
    for (int i = 0; i < NUMBER_OF_FOUNDATIONS; ++i) {
        foundation[i]->top = NULL;
        foundation[i]->size = 0;
    }

    startupPopulateTableau(*tableau, deck->top);
    printUI(*tableau, *foundation);
    for (int i = 0; i < NUMBER_OF_TABLEAUS; i++) {
        printf("\nTableau %d: ", i);
        Card *card = tableau[i]->bottom;
        while (card != NULL) {
            printf("%c%c ", card->rank, card->suit);
            card = card->next;
        }
    }

}


