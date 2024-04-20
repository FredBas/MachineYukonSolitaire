#ifndef CARDPILE_H
#define CARDPILE_H

#include "Card.h"
#include "Definitions.h"

typedef struct Cardpile {
    Card *top;
    Card *bottom;
    int size;
} Cardpile;

Card * getCardAtTableauBottom(Cardpile *pile);

Card * getCardAtFoundation(Cardpile *pile);

void shuffleDeckSplit(Cardpile *deck, int split);

void shuffleRandom(Cardpile *deck);

int checkDuplicate(Card *deck, int numCards);

void printCardList(Card *head);

void freeCardList(Card *head);

Card *createDeckFromFile(char *filename);

void saveDeckToFile(Card *head, char *filename);

Card *copyDeck(Card *head);

void clearTableau(Cardpile *tableau[]);

void showTableauCardsStartup(Cardpile *tableau[]);

Card* getCardAt(Cardpile *pile, int cardPosition);

#endif // CARDPILE_H