#include "GameInitialization.h"
#include "Cardpile.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void startupPopulateTableau(Cardpile *tableau[], Card *head) {
    Card *card = head;
    for (int i = 0; i < DECK_SIZE; i++) {
        if (card == NULL) {
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

void playPopulateTableau(Cardpile *tableau[], Card *head) {
    int numCardsInTableau[NUMBER_OF_TABLEAUS] = {1,6,7,8,9,10,11};
    Card *card = head;
    tableau[0]->top = card;
    tableau[0]->bottom = card;
    card->isFaceUp = true;
    card = card->next;

    int row = 0;
    int i = 1;
    while (card != NULL) {
        if (row == 0) {
            tableau[i]->bottom = card;
            card->prev = NULL;
        } else {
            card->prev = tableau[i]->top;
            tableau[i]->top->next = card;
        }

        tableau[i]->top = card;
        if (numCardsInTableau[i] - 6 < row) {
            card->isFaceUp = true;
        } else {
            card->isFaceUp = false;
        }
        i++;
        if (i == NUMBER_OF_TABLEAUS) {
            if (row < 5) {i = 1;}
            else if (row == 5) {i = 2;}
            else if (row == 6) {i = 3;}
            else if (row == 7) {i = 4;}
            else if (row == 8) {i = 5;}
            else if (row == 9) {i = 6;}
            row++;
        }
        card = card->next;
    }
    for (int i = 0; i < NUMBER_OF_TABLEAUS; i++) {
        tableau[i]->size = numCardsInTableau[i];
        tableau[i]->top->next = NULL;
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


void initializeStartup(Cardpile *tableau[], Cardpile **foundation, Cardpile *deck, char* filename) {

    char filepath[sizeof(char)*50];
    snprintf(filepath, sizeof(filepath), "../%s", filename);

    // Read cards from file
    deck->top = createDeckFromFile(filepath);

    if (deck->top == NULL) {
        printf("Error: Failed to read cards from file.\n");
        return;
    }
}


