#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define DECK_SIZE 52
#define NUMBER_OF_FOUNDATIONS 4
#define NUMBER_OF_TABLEAUS 7

typedef enum { HEARTS, DIAMONDS, CLUBS, SPADES } Suit;
typedef enum { ACE = 1, T = 10, JACK = 11, QUEEN = 12, KING = 13 } Rank;

typedef struct {
    Rank rank;
    Suit suit;
    bool isFaceUp;
    char symbol;
} Card;

typedef struct {
    Card cards[DECK_SIZE];
    int size;
} Stack;

void initialize(Stack tableau[], Stack foundation[], Stack *stock) {
    // Initialize cards
    int cardIndex = 0;
    for (int i = 0; i < NUMBER_OF_TABLEAUS; ++i) {
        int cardsInTableau = (i == 0) ? 1 : (6 + i - 1); // Adjusting cards for each tableau
        for (int j = 0; j < cardsInTableau; ++j) {
            Card card = { .rank = j + 1, .suit = i % 4, .isFaceUp = false };
            card.symbol = (char)('0' + card.rank); // using numbers for ranks
            tableau[i].cards[j] = card;
        }
        tableau[i].size = cardsInTableau;
        cardIndex += cardsInTableau;
    }
}

void displayTableau(Stack tableau[]) {
    for (int i = 0; i < NUMBER_OF_TABLEAUS; ++i) {
        printf("Tableau %d: ", i + 1);
        for (int j = 0; j < tableau[i].size; ++j) {
            Card card = tableau[i].cards[j];
            if (card.isFaceUp) {
                printf("%c ", card.symbol);
            } else {
                printf("* ");
            }
        }
        printf("\n");
    }
}

int main() {
    // Initialize game
    Stack tableau[NUMBER_OF_TABLEAUS];
    Stack foundation[NUMBER_OF_FOUNDATIONS];
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

    return 0;
}
