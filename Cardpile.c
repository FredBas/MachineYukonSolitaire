#include "Definitions.h"
#include "Cardpile.h"
#include "Card.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

Card* getCardAt(Cardpile *pile, int cardPosition) {
    Card *card = pile->bottom;
    for (int i = 0; i < cardPosition; i++) {
        if (card == NULL) {
            return NULL;
        }
        card = card->next;
    }
    return card;
}

Card *getCardAtFoundation(Cardpile *pile) {
    Card *card = pile->top;
    return card;
}

Card *getCardAtTableauBottom(Cardpile *pile) {
    Card *card = pile->top;
    return card;
}

Card* getCardAtTableau(Cardpile *pile, Rank rank, Suit suit) {
Card *card = pile->top;
    while (card != NULL) {
        if (card->rank == rank && card->suit == suit) {
            return card;
        }
        card = card->next;
    }
    return NULL;
}

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

    // Alternately take the top card from each pile until one pile is exhausted
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

    // Reverse the shuffled pile to place remaining cards at the bottom
    Card *prev = NULL, *curr = shuffled.top, *next;
    while (curr != NULL) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    shuffled.top = prev;

    // Update the stock with the shuffled cards
    deck->top = shuffled.top;
    deck->size = DECK_SIZE;
}

void shuffleRandom(Cardpile *deck) {
    if (deck == NULL) {
        printf("Error: Deck is NULL.\n");
        return;
    }
    if (deck->top == NULL) {
        printf("Error: deck->top is NULL.\n");
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
        int j = rand() % (DECK_SIZE); // Generate a random index

        // Swap cards[i] and cards[j]
        Card *temp = cards[j];
        cards[j] = cards[i];
        cards[i] = temp;
    }
    // Reconstruct the deck from the shuffled array
    deck->top = cards[0];
    current = deck->top;
    for (int i = 1; i < DECK_SIZE; ++i) {
        current->next = cards[i];
        current = current->next;
    }
    current->next = NULL; // Make sure the last card points to NULL
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

    while (fgets(line, sizeof(line), file)) {
        if (strlen(line) >= 2) {
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
            newCard->isFaceUp = false;
            if (head == NULL) {
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

void saveDeckToFile(Card *head, char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: File '%s' not found.\n", filename);
        return;
    }

    Card *current = head;
    while (current != NULL) {
        fprintf(file, "%c%c\n", current->rank, current->suit);
        current = current->next;
    }
    fclose(file);
    printf("Deck saved to file '%s'.\n", filename);
}

Card *copyDeck(Card *head) {
    Card *copyHead = NULL;
    Card **nextPtr = &copyHead;
    Card *current = head;
    while (current != NULL) {
        Card *newCard = (Card *) malloc(sizeof(Card));
        if (newCard == NULL) {
            printf("Memory allocation failed for card\n");
            freeCardList(copyHead);
            return NULL;
        }
        newCard->rank = current->rank;
        newCard->suit = current->suit;
        newCard->next = NULL;
        newCard->isFaceUp = current->isFaceUp;
        *nextPtr = newCard;
        nextPtr = &(newCard->next);
        current = current->next;
    }
    return copyHead;
}

void clearTableau(Cardpile *tableau[]) {
    for (int i = 0; i < NUMBER_OF_TABLEAUS; ++i) {
        tableau[i]->top = NULL;
        tableau[i]->bottom = NULL;
        tableau[i]->size = 0;
    }
}

void showTableauCardsStartup(Cardpile *tableau[]) {
    Card *currentCard;
    for (int i = 0; i < NUMBER_OF_TABLEAUS; ++i) {
        currentCard = tableau[i]->bottom;
        while (currentCard != NULL) {
            currentCard->isFaceUp = true;
            currentCard = currentCard->next;
        }
    }
}
void moveToFoundation(int sourceIndex, Cardpile **tableau, Cardpile **foundation, const char *destination) {
    int foundationIndex = destination[1] - '1'; // Convert from char to int (0-based)
    if (foundationIndex >= 0 && foundationIndex < NUMBER_OF_FOUNDATIONS) {
        Card *tableauCard = tableau[sourceIndex]->top;
        Card *foundationCard = foundation[foundationIndex]->top;
        if (foundationCard == NULL || canBePlacedFoundation(*tableauCard, *foundationCard)) {
            if (tableauCard->prev != NULL) {
                tableau[sourceIndex]->top = tableauCard->prev;
                tableau[sourceIndex]->top->next = NULL;
                tableau[sourceIndex]->top->isFaceUp = true;
            } else {
                tableau[sourceIndex]->bottom = NULL;
                tableau[sourceIndex]->top = NULL;
            }
            tableau[sourceIndex]->size--;

            if (foundationCard != NULL) {
                foundation[foundationIndex]->top->next = tableauCard;
                tableauCard->prev = foundation[foundationIndex]->top;
            } else {
                foundation[foundationIndex]->bottom = tableauCard;
            }
            foundation[foundationIndex]->top = tableauCard;
            tableauCard->next = NULL;
            foundation[foundationIndex]->size++;
        }
    }
}

void moveBottomCardToTableau(int sourceIndex, Cardpile **tableau, const char *destination) {
    int tableauIndex = destination[1] - '1'; // Convert from char to int (0-based)
    if (tableauIndex >= 0 && tableauIndex < NUMBER_OF_TABLEAUS) {
        Card *tableauCard = tableau[sourceIndex]->top;
        Card *tableauCard2 = tableau[tableauIndex]->top;
        if (tableauCard2 == NULL || canBePlacedBottom(*tableauCard, *tableauCard2)) {
            if (tableauCard->prev != NULL) {
                tableau[sourceIndex]->top = tableauCard->prev;
                tableau[sourceIndex]->top->next = NULL;
                tableau[sourceIndex]->top->isFaceUp = true;
            } else {
                tableau[sourceIndex]->bottom = NULL;
                tableau[sourceIndex]->top = NULL;
            }
            tableau[sourceIndex]->size--;

            if (tableauCard2 != NULL) {
                tableau[tableauIndex]->top->next = tableauCard;
                tableauCard->prev = tableau[tableauIndex]->top;
            } else {
                tableau[tableauIndex]->bottom = tableauCard;
            }

            tableau[tableauIndex]->top = tableauCard;
            tableauCard->next = NULL;
            tableau[tableauIndex]->size++;

        } else {
            printf("Error: Move not valid\n");
        }
    }
}

void moveMultipleCardsToTableau(int sourceIndex, Cardpile **tableau, const char *destination, Card *card) {
    int destinationIndex = destination[1] - '1'; // Convert from char to int (0-based)
    if (destinationIndex >= 0 && destinationIndex < NUMBER_OF_TABLEAUS) {
        Card *tableauCard = tableau[sourceIndex]->top;
        Card *tableauCard2 = tableau[destinationIndex]->top;
        for (int i = 0; i < tableau[sourceIndex]->size; i++) {
            if (tableauCard->suit == card->suit && tableauCard->rank == card->rank) {
                break;
            } else {
                tableauCard = tableauCard->prev;
            }
        }
        if (tableauCard == NULL) {
            printf("Error: Card not found\n");
            return;
        }
        if (tableauCard2 == NULL || canBePlacedBottom(*tableauCard, *tableauCard2)) {
            if (tableauCard->prev != NULL) {
                tableau[sourceIndex]->top = tableauCard->prev;
                tableau[sourceIndex]->top->next = NULL;
                tableau[sourceIndex]->top->isFaceUp = true;
            } else {
                tableau[sourceIndex]->bottom = NULL;
                tableau[sourceIndex]->top = NULL;
            }
            tableau[sourceIndex]->size--;

            if (tableauCard2 != NULL) {
                tableau[destinationIndex]->top->next = tableauCard;
                tableauCard->prev = tableau[destinationIndex]->top;
            } else {
                tableau[destinationIndex]->bottom = tableauCard;
            }
            tableau[destinationIndex]->top = tableauCard;
            Card *current = tableauCard->next;
            while (current != NULL) {
                tableau[destinationIndex]->top = current;
                current = current->next;
                tableau[sourceIndex]->size--;
                tableau[destinationIndex]->size++;
            }
            tableau[destinationIndex]->top->next = NULL;
        } else {
            printf("Error: Move not valid\n");
        }
    } else {
        printf("Error: Invalid destination\n");
    }
}
