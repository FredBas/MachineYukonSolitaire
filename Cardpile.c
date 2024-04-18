#include "Definitions.h"
#include "Cardpile.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

void shuffleDeckSplit(Cardpile *deck, int split) {
    // If split is not provided (i.e., split is 0), generate a random split
    if (split == 0) {
        srand(time(NULL)); // Seed for random number generation
        split = rand() % (DECK_SIZE - 1) + 1; // Generate a random number between 1 and DECK_SIZE - 1
    }

    // Split the deck into two piles
    Card *pile1 = deck->top;
    deck->top = NULL;
    Card *current = pile1;
    for (int i = 0; i < split - 1; ++i) {
        if (current != NULL) {
            current = current->next;
        } else {
            break;
        }
    }
    Card *pile2 = NULL;
    if (current != NULL) {
        pile2 = current->next;
        current->next = NULL;
    } else {
        printf("Error: Split value is too large.\n");
        return;
    }

    // Shuffle the piles
    Cardpile shuffled;
    shuffled.top = NULL;
    shuffled.size = 0;

    // Alternately take the top card from each pile
    while (pile1 != NULL && pile2 != NULL) {
        Card *temp;
        temp = pile1;
        pile1 = pile1->next;
        temp->next = shuffled.top;
        shuffled.top = temp;

        temp = pile2;
        pile2 = pile2->next;
        temp->next = shuffled.top;
        shuffled.top = temp;
    }

    // Add remaining cards from the non-empty pile to the shuffled pile
    current = pile1 != NULL ? pile1 : pile2;
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

void shuffleRandom(Cardpile *deck) {
    if (deck == NULL) {
        printf("Error: Deck is NULL.\n");
        return;
    }
    if(deck->top == NULL) {
        printf("Error: deck->top is NULL.\n");
        return;
    }
    if(deck->bottom == NULL) {
        printf("Error: deck->bottom is NULL.\n");
        return;
    }
    srand(time(NULL)); // Seed for random number generation

    // Create an array to hold the cards for easy random access
    Card *cards[DECK_SIZE];
    Card *current = deck->top;
    for (int i = 0; i < DECK_SIZE; ++i) {
        cards[i] = current;
        current = current->next;
    }
    // Fisher-Yates shuffle
    for (int i = DECK_SIZE - 1; i > 0; --i) {
        int j = rand() % (i + 1); // Generate a random index

        // Swap cards[i] and cards[j]
        Card *temp = cards[i];
        cards[i] = cards[j];
        cards[j] = temp;
    }

    // Reconstruct the deck from the shuffled array
    deck->top = cards[0];
    current = deck->top;
    for (int i = 1; i < DECK_SIZE; ++i) {
        current->next = cards[i];
        current = current->next;
    }
    current->next = NULL; // Make sure the last card points to NULL
    deck->bottom = current;
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
    char line[CARD_SIZE];
    int i = 0;

    while(fgets(line,sizeof(line),file)){
        if (strlen(line) >=2){
            char rank = line[0];
            char suit = line[1];

            Card *newCard = (Card *) malloc(sizeof(Card));
            if (newCard == NULL) {
                printf("Memory allocation failed for card %d\n", i);
                freeCardList(head);
                return NULL;
            }
            newCard->rank = rank;
            newCard->suit = suit;
            newCard->next = NULL;
            if (head == NULL){
                head = newCard;
            } else {
                prev->next = newCard;
            }
            prev = newCard;
            i++;
        }
    }

    fclose(file);

    // Check if the file contains the expected number of cards
    if (i != 52) {
        printf("Error: File does not contain 52 cards.\n");
        return NULL;
    }

    return head;
}