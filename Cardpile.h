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

void moveCard(Cardpile *from, Cardpile *to, Card *card);

void moveToFoundation(int sourcIndex, Cardpile **tableau, Cardpile **foundation, const char *destination, char *message[]);

void moveBottomCardToTableau(int sourceIndex, Cardpile **from, const char *destination, char *message[]);

void moveMultipleCardsToTableau(int sourceIndex, Cardpile **tableau, const char *destination, Card *card, char *message[]);

Card* getCardAtTableau(Cardpile *pile, Rank rank, Suit suit);

#endif // CARDPILE_H