#include <stdbool.h>
#include <stdlib.h>
#include "Definitions.h"
#include "Cardpile.h"
#include "MemoryManagement.h"
#include "GameInitialization.h"
#include <stdio.h>

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
    Cardpile *deck = malloc(sizeof(Cardpile));

    printf("\nWelcome to Yukon Solitaire!\nTo begin the startup phase, load a deck using the 'LD' command.\n\n(For a list of commands, type 'HELP')\n\n");

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
