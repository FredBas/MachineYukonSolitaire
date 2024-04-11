#include "Definitions.h"
#include "Cardpile.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

void shuffleDeck(Cardpile *deck, int split) {
    // Split the deck into two piles
    Card *pile1 = deck->top;
    Card *current = pile1;
    for (int i = 0; i < split - 1; ++i) {
        current = current->next;
    }
    Card *pile2 = current->next;
    current->next = NULL;

    // Shuffle the piles
    Cardpile shuffled;
    shuffled.top = NULL;
    shuffled.size = 0;
    srand(time(NULL)); // Seed for random number generation

    while (pile1 != NULL && pile2 != NULL) {
        if (rand() % 2 == 0) { // Randomly select from pile1
            Card *temp = pile1;
            pile1 = pile1->next;
            temp->next = shuffled.top;
            shuffled.top = temp;
        } else { // Randomly select from pile2
            Card *temp = pile2;
            pile2 = pile2->next;
            temp->next = shuffled.top;
            shuffled.top = temp;
        }
    }

    // Add remaining cards from the non-empty pile to the shuffled pile
    if (pile1 != NULL) {
        current = pile1;
    } else {
        current = pile2;
    }
    while (current != NULL) {
        Card *temp = current;
        current = current->next;
        temp->next = shuffled.top;
        shuffled.top = temp;
    }

    // Update the stock with the shuffled cards
    deck->top = shuffled.top;
    deck->size = DECK_SIZE;
}

int checkDuplicate(Card *deck, int numCards) {
    for (int i = 0; i < numCards - 1; i++) {
        for (int j = i + 1; j < numCards; j++) {
            if (deck[i].rank == deck[j].rank && deck[i].suit == deck[j].suit) {
                printf("Error: Duplicate card found (%d%c) on line %d.\n", deck[i].rank, deck[i].suit, j + 1);
                return 1;
            }
        }
    }
    return 0;
}

void printCardList(Card *head) {
    while (head != NULL) {
        printf("%c%c ", head->rank, head->suit);
        head = head->next;
    }
    printf("\n");
}

void freeCardList(Card *head) {
    Card *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

Card *createDeckFromFile(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: File '%s' not found\n", filename);
        return NULL;
    }

    Card *head = NULL;
    Card *prev = NULL;
    char card[CARD_SIZE + 1];
    int i = 0;
    while (fgets(card, sizeof(card), file) && i < DECK_SIZE) {
        if (strlen(card) != CARD_SIZE) {
            printf("Error: Invalid card format on line %d.\n", i + 1);
            fclose(file);
            return NULL;
        }
        if (card[strlen(card) - 1] == '\n') {
            card[strlen(card) - 1] = '\0';
        }
        Card *newCard = (Card *) malloc(sizeof(Card));
        if (newCard == NULL) {
            printf("Error: Memory allocation failed.\n");
            fclose(file);
            return NULL;
        }
        newCard->rank = card[0];
        newCard->suit = card[strlen(card) - 1];
        newCard->next = NULL;
        if (head == NULL) {
            head = newCard;
        } else {
            prev->next = newCard;
        }
        prev = newCard;
        i++;
    }
    fclose(file);

    // Check if the file contains the expected number of cards
    if (i != 52) {
        printf("Error: File does not contain 52 cards.\n");
        return NULL;
    }

    return head;
}