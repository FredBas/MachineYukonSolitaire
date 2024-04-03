#include "raylib.h"
#include <stdlib.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define CARD_WIDTH 72
#define CARD_HEIGHT 96

#define DECK_SIZE 52
#define FOUNDATION_SIZE 4
#define TABLEAU_SIZE 7
#define STOCK_SIZE 24

typedef enum { HEARTS, DIAMONDS, CLUBS, SPADES } Suit;
typedef enum { ACE = 1, JACK = 11, QUEEN = 12, KING = 13 } Rank;

typedef struct {
    Rank rank;
    Suit suit;
    bool isFaceUp;
    Rectangle rect;
    Color color;
} Card;

typedef struct {
    Card cards[DECK_SIZE];
    int size;
} Stack;

int main() {
    // Initialization
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Yukon Solitaire");

    // Initialize game
    Stack tableau[TABLEAU_SIZE];
    Stack foundation[FOUNDATION_SIZE];
    Stack stock;

    // Initialize cards
    int cardIndex = 0;
    for (int i = 0; i < TABLEAU_SIZE; ++i) {
        for (int j = 0; j < i + 1; ++j) {
            Card card = { .rank = j + 1, .suit = i % 4, .isFaceUp = false };
            card.rect = (Rectangle){ (SCREEN_WIDTH - CARD_WIDTH) / 2, (SCREEN_HEIGHT - CARD_HEIGHT) / 2, CARD_WIDTH, CARD_HEIGHT };
            card.color = (card.suit == HEARTS || card.suit == DIAMONDS) ? RED : BLACK;
            tableau[i].cards[j] = card;
        }
        tableau[i].size = i + 1;
        cardIndex += i + 1;
    }

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose()) {
        // Update

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw cards
        for (int i = 0; i < TABLEAU_SIZE; ++i) {
            for (int j = 0; j < tableau[i].size; ++j) {
                Card card = tableau[i].cards[j];
                DrawRectangleRec(card.rect, card.color);
            }
        }

        // Draw foundation, stock, etc.

        EndDrawing();
    }

    // Cleanup
    CloseWindow();

    return 0;
}