#include <stdbool.h>
#include <stdlib.h>
#include "Definitions.h"
#include "Cardpile.h"
#include "MemoryManagement.h"
#include "GameInitialization.h"

int main() {
    // Initialize game
    Cardpile *tableau[NUMBER_OF_TABLEAUS];
    for (int i = 0; i < NUMBER_OF_TABLEAUS; ++i) {
        tableau[i] = malloc(sizeof(Cardpile));
    }
    Cardpile *foundation[NUMBER_OF_FOUNDATIONS];
    for (int i = 0; i < NUMBER_OF_FOUNDATIONS; ++i) {
        foundation[i] = malloc(sizeof(Cardpile));
    }
    Cardpile *deck;

    initializeStartup(tableau, foundation, deck);

    // Main game loop
    while (true) {
        // Update

        // Draw
        system("cls");

        // Draw cards
        //displayTableau(tableau);

        // Draw foundation, stock, etc.

        // Input handling and game logic here

        // Exit condition
        break; // For now, just break the loop to exit
    }

    // Cleanup
    cleanup(&deck);

    return 0;
}
