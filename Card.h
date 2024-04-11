#ifndef CARD_H
#define CARD_H

#include <stdbool.h>

typedef enum {
    H, D, C, S
} Suit;
typedef enum {
    A = 1, T = 10, J = 11, Q = 12, K = 13
} Rank;

typedef struct Card {
    Rank rank;
    Suit suit;
    bool isFaceUp;
    char symbol;
    struct Card *next;
} Card;

bool isInSequence(Card card1, Card card2);

bool isSameSuit(Card card1, Card card2);

bool canBePlacedBottom(Card card1, Card card2);

bool canBePlacedFoundation(Card card1, Card card2);

void printCard(Card *card); // Add this line

#endif // CARD_H