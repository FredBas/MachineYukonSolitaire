#include <stdbool.h>
#include <stdlib.h>
#include "Definitions.h"
#include "GameUI.h"
#include "Cardpile.h"
#include "Card.h"
#include "MemoryManagement.h"
#include "GameInitialization.h"
#include <stdio.h>
#include "Commands.h"

int main() {
    // Initialize game
    Cardpile *tableau[NUMBER_OF_TABLEAUS];
    char* command = (char*)malloc(20 * sizeof(char));
    if (command == NULL) {
        printf("Memory allocation failed for command\n");
        return 1; // Return an error code
    }

    for (int i = 0; i < NUMBER_OF_TABLEAUS; ++i) {
        tableau[i] = malloc(sizeof(Cardpile));
        tableau[i]->bottom = NULL;
        tableau[i]->top = NULL;
        tableau[i]->size = 0;
        if (tableau[i] == NULL) {
            printf("Memory allocation failed for tableau[%d]\n", i);
            return 1; // Return an error code
        }
    }
    Cardpile *foundation[NUMBER_OF_FOUNDATIONS];
    for (int i = 0; i < NUMBER_OF_FOUNDATIONS; ++i) {
        foundation[i] = malloc(sizeof(Cardpile));
        foundation[i]->bottom = NULL;
        foundation[i]->top = NULL;
        foundation[i]->size = 0;
        if (foundation[i] == NULL) {
            printf("Memory allocation failed for foundation[%d]\n", i);
            return 1; // Return an error code
        }
    }
    Cardpile *deck = malloc(sizeof(Cardpile));
    deck->bottom = NULL;
    deck->top = NULL;
    deck->size = 0;

    if (deck == NULL) {
        printf("Memory allocation failed for deck\n");
        return 1; // Return an error code
    }

    printf("\nWelcome to Yukon Solitaire!\nTo begin the startup phase, load a deck using the 'LD' command.\n\n(For a list of commands, type 'HELP')\n\n");


    // Main game loop
    while (true) {
        scanf("%s", command);
        commandHandler(command, tableau, foundation, deck);

        // Draw

        // Draw cards
        //displayTableau(tableau);

        // Draw foundation, stock, etc.

        // Input handling and game logic here

        // Exit condition
        //break; // For now, just break the loop to exit
    }

    // Cleanup
    cleanup(&deck);

    return 0;
}
