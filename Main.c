// C
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Definitions.h"
#include "Cardpile.h"
#include "MemoryManagement.h"
#include "Commands.h"


int main() {
    // Initialize game

    char command[20];
    Cardpile *tableau[NUMBER_OF_TABLEAUS];

    for (int i = 0; i < NUMBER_OF_TABLEAUS; ++i) {
        tableau[i] = safeMalloc(sizeof(Cardpile), "Memory allocation failed for tableau");
        tableau[i]->bottom = NULL;
        tableau[i]->top = NULL;
        tableau[i]->size = 0;
    }
    Cardpile *foundation[NUMBER_OF_FOUNDATIONS];
    for (int i = 0; i < NUMBER_OF_FOUNDATIONS; ++i) {
        foundation[i] = safeMalloc(sizeof(Cardpile), "Memory allocation failed for foundation");
        foundation[i]->bottom = NULL;
        foundation[i]->top = NULL;
        foundation[i]->size = 0;
    }
    Cardpile *deck = safeMalloc(sizeof(Cardpile), "Memory allocation failed for deck");
    deck->bottom = NULL;
    deck->top = NULL;
    deck->size = 0;

    printf("\nWelcome to Yukon Solitaire!\nTo begin the startup phase, load a deck using the 'LD' command.\n\n(For a list of commands, type 'HELP')\n\n");

    // Main game loop
    while (true) {
        fgets(command, 20, stdin);
        command[strcspn(command, "\n")] = 0; // Remove the newline character
        commandHandler(command, tableau, foundation, deck);
    }

    // Cleanup
    cleanup(&deck);

    return 0;
}