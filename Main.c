#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define NUMBER_OF_FOUNDATIONS 4
#define NUMBER_OF_TABLEAUS 7

typedef enum { HEARTS, DIAMONDS, CLUBS, SPADES } Suit;
typedef enum { ACE = 1, T = 10, JACK = 11, QUEEN = 12, KING = 13 } Rank;

typedef struct Card {
    Rank rank;
    Suit suit;
    bool isFaceUp;
    char symbol;
    struct Card* next;
} Card;

typedef struct {
    Card* top;
    int size;
} Stack;

int isBlack(Card card) {
    return card.suit == CLUBS || card.suit == SPADES;
}

int isRed(Card card) {
    return card.suit == HEARTS || card.suit == DIAMONDS;
}

int isOppositeColor(Card card1, Card card2) {
    return isBlack(card1) != isBlack(card2);
}

int isInSequence(Card card1, Card card2) {
    return card1.rank == card2.rank + 1;
}

int canBePlacedBottom(Card card1, Card card2) {
    return isOppositeColor(card1, card2) && isInSequence(card1, card2);
}

int isSameSuit(Card card1, Card card2) {
    return card1.suit == card2.suit;
}

int canBePlacedFoundation(Card card1, Card card2) {
    return isSameSuit(card1, card2) && isInSequence(card1, card2);
}

void push(Stack* stack, Card* card) {
    card->next = stack->top;
    stack->top = card;
    stack->size++;
}

Card* pop(Stack* stack) {
    if (stack->size == 0) {
        return NULL;
    }
    Card* card = stack->top;
    stack->top = card->next;
    stack->size--;
    return card;
}

void initialize(Stack tableau[], Stack foundation[], Stack* stock) {
    // Initialize cards as a linked list
    Card* deck = NULL;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 13; ++j) {
            Card* card = malloc(sizeof(Card));
            card->rank = j + 1;
            card->suit = i;
            card->isFaceUp = false;
            card->symbol = (char)('0' + card->rank);
            card->next = deck;
            deck = card;
        }
    }

    // Shuffle the deck (if needed)

    // Deal cards to tableau

    // Deal remaining cards to stock

    // Initialize tableau and foundation as empty stacks
    for (int i = 0; i < 7; ++i) {
        tableau[i].top = NULL;
        tableau[i].size = 0;
    }
    for (int i = 0; i < 4; ++i) {
        foundation[i].top = NULL;
        foundation[i].size = 0;
    }

    // Set the stock pointer to the top of the deck
    stock->top = deck;
    stock->size = DECK_SIZE;
}

void displayTableau(Stack tableau[]) {
    // Display tableau
    for (int i = 0; i < 7; ++i) {
        printf("Tableau %d: ", i + 1);
        Card* current = tableau[i].top;
        while (current != NULL) {
            if (current->isFaceUp) {
                printf("%c ", current->symbol);
            } else {
                printf("* ");
            }
            current = current->next;
        }
        printf("\n");
    }
}

void cleanup(Stack* stock) {
    // Free the memory allocated for the cards
    Card* current = stock->top;
    while (current != NULL) {
        Card* next = current->next;
        free(current);
        current = next;
    }
}

int main() {
    // Initialize game
    Stack tableau[7];
    Stack foundation[4];
    Stack stock;

    initialize(tableau, foundation, &stock);

    // Main game loop
    while (true) {
        // Update

        // Draw
        system("clear"); // Clear the screen (works on UNIX-like systems)

        // Draw cards
        displayTableau(tableau);

        // Draw foundation, stock, etc.

        // Input handling and game logic here

        // Exit condition
        break; // For now, just break the loop to exit
    }

    // Cleanup
    cleanup(&stock);

    return 0;
}
