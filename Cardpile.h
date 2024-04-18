#ifndef CARDPILE_H
#define CARDPILE_H

#include "Card.h"
#include "Definitions.h"

typedef struct Cardpile {
    Card *top;
    Card *bottom;
    int size;
} Cardpile;

void shuffleDeckSplit(Cardpile *deck, int split);

void shuffleRandom(Cardpile *deck);

int checkDuplicate(Card *deck, int numCards);

void printCardList(Card *head);

void freeCardList(Card *head);

Card *createDeckFromFile(char *filename);

#endif // CARDPILE_H