#include "GameInitialization.h"
#include "Cardpile.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void startupPopulateTableau(Cardpile *tableau[], Card *head) {
    Card *card = head;
    for (int i = 0; i < DECK_SIZE; i++) {
        if (card == NULL || card->next == NULL) {
            printf("Error: head or next is NULL\n");
            return;
        }

        int tableauIndex = i % NUMBER_OF_TABLEAUS;
        if (tableau[tableauIndex]->top == NULL) {
            tableau[tableauIndex]->top = card;
            tableau[tableauIndex]->bottom = card;
        } else {
            card->prev = tableau[tableauIndex]->top;
            tableau[tableauIndex]->top->next = card;
            tableau[tableauIndex]->top = card;
        }
        card = card->next;
    }
}

Cardpile* copyTableau(Cardpile *originalTableau) {
    Cardpile *copyTableau = malloc(NUMBER_OF_TABLEAUS * sizeof(Cardpile));

    for (int i = 0; i < NUMBER_OF_TABLEAUS; i++) {
        Cardpile *original = &originalTableau[i];
        Cardpile *copy = &copyTableau[i];

        copy->top = NULL;
        copy->bottom = NULL;
        copy->size = original->size;

        Card *currentOriginalCard = original->bottom;
        while (currentOriginalCard != NULL) {
            Card *newCard = malloc(sizeof(Card));
            newCard->rank = currentOriginalCard->rank;
            newCard->suit = currentOriginalCard->suit;
            newCard->isFaceUp = currentOriginalCard->isFaceUp;
            newCard->next = NULL;
            newCard->prev = copy->top;

            if (copy->top != NULL) {
                copy->top->next = newCard;
            }
            copy->top = newCard;

            if (copy->bottom == NULL) {
                copy->bottom = newCard;
            }

            currentOriginalCard = currentOriginalCard->next;
        }
    }

    return copyTableau;
}


void initializeStartup(Cardpile *tableau[], Cardpile **foundation, Cardpile *deck) {

    // Read cards from file
    deck->top = createDeckFromFile("../unshuffledDeck.txt");

    if (deck->top == NULL) {
        printf("Error: Failed to read cards from file.\n");
        return;
    }

    // Initialize tableau and foundation as empty stacks


    startupPopulateTableau(tableau, deck->top);
    printUI(tableau, foundation, false);
    printUI(tableau, foundation, true);

}


