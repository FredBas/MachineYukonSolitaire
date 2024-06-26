// C
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Definitions.h"
#include "Cardpile.h"
#include "MemoryManagement.h"
#include "Commands.h"
#include "GUI.h"


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
    char *message[50] ={"Welcome to Yukon Solitaire! For GUI type 'GUI' else type 'LD' to start. For a list of commands, type 'HELP'."};
    initialTableauPrinter();
    printUIMessages(empty,message);

    // Main game loop
    while (true) {
        fgets(command, 20, stdin);
        command[strcspn(command, "\n")] = 0;
        commandHandler(command, tableau, foundation, deck, &currentPhase);

        int cardsInFoundations = 0;
        for (int i = 0; i < NUMBER_OF_FOUNDATIONS; i++) {
            cardsInFoundations += foundation[i]->size;
        }
        if (cardsInFoundations == DECK_SIZE) {
            printf("\n\nCongratulations! You have won the game!\n");
            break;
        }
    }
    // Cleanup
    cleanup(&deck);


    return 0;
}