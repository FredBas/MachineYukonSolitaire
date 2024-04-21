#include "Card.h"
#include <stdio.h>
#include "Definitions.h"

bool isInSequence(Card card1, Card card2) {
    return rankValue(card1.rank) == rankValue(card2.rank) - 1;
}

bool isSameSuit(Card card1, Card card2) {
    return card1.suit == card2.suit;
}

bool canBePlacedBottom(Card card1, Card card2) {
    bool inSequence = isInSequence(card1, card2);
    bool sameSuit = isSameSuit(card1, card2);
    printf("Card1: %c%c, Card2: %c%c, isInSequence: %d, isSameSuit: %d\n", card1.rank, card1.suit, card2.rank, card2.suit, inSequence, sameSuit);
    return inSequence && !sameSuit;
}

bool canBePlacedFoundation(Card card1, Card card2) {
    bool sameSuit = isSameSuit(card1, card2);
    bool inSequence = isInSequence(card1, card2);
    printf("Card1: %c%c, Card2: %c%c, isInSequence: %d, isSameSuit: %d\n", card1.rank, card1.suit, card2.rank, card2.suit, inSequence, sameSuit);
    return sameSuit && inSequence;
}

void printCard(Card *card) { // Add this function
    if (card->isFaceUp) {
        printf("%c%c\t", card->rank, card->suit);
    } else {
        printf("[]\t");
    }
}

int rankValue(char rank) {
    if (rank >= '2' && rank <= '9') {
        return rank - '0';
    } else {
        switch (rank) {
            case 'A':
                return 1;
            case 'T':
                return 10;
            case 'J':
                return 11;
            case 'Q':
                return 12;
            case 'K':
                return 13;
            default:
                return -1; // Invalid rank
        }
    }
}