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

    gamePhase currentPhase = welcome;

    char *empty = "";
    printf("\nWelcome to Yukon Solitaire!\n\nFor a list of commands, type 'HELP'.\n\n");
    initialTableauPrinter();
    printUIMessages(empty,empty);

    // Main game loop
    while (true) {
        fgets(command, 20, stdin);
        command[strcspn(command, "\n")] = 0; // Remove the newline character
        commandHandler(command, tableau, foundation, deck, &currentPhase);
    }
    // Cleanup
    cleanup(&deck);

    return 0;
}