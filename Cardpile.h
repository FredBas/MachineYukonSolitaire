#ifndef CARDPILE_H
#define CARDPILE_H

#include "Card.h"
#include "Definitions.h"

typedef struct Cardpile {
    Card *top;
    int size;
} Cardpile;

void shuffleDeck(Cardpile *deck, int split);

int checkDuplicate(Card *deck, int numCards);

void printCardList(Card *head);

void freeCardList(Card *head);

Card *createDeckFromFile(char *filename);

#endif // CARDPILE_H