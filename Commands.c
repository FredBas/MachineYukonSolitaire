#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "GameInitialization.h"
#include "Cardpile.h"
#include "Commands.h"

void commandHandler(const char *command, Cardpile **tableau, Cardpile **foundation, Cardpile *deck,
                    gamePhase *currentPhase) {

    char *lastCommand = "";
    // Create a copy of the command string because strtok modifies the original string
    char *commandCopy = strdup(command);
    char *movecmdCopy = strdup(command);
    // Get the first token (the command)
    char *cmd = strtok(commandCopy, " ");
    char *movecmd = strtok(movecmdCopy, " -> ");

    if (strcmp(cmd, "LD") == 0) {
        if (*currentPhase == play) {
            printf("Command not available during a game. For a list of available commands, type HELP\n\n");
            return;
        }
        *currentPhase = startup;
        char *filename = strtok(NULL, " ");
        if (filename != NULL) {
            initializeStartup(tableau, foundation, deck, filename);
            clearTableau(tableau);
            startupPopulateTableau(tableau, copyDeck(deck->top));
            printUI(tableau, foundation);
            lastCommand = "LD";
            printf("LAST Command: %s\n", lastCommand);
            printf("Message: \n");
            printf("INPUT > ");

        } else {
            initializeStartup(tableau, foundation, deck, "unshuffledDeck.txt");
            clearTableau(tableau);
            startupPopulateTableau(tableau, copyDeck(deck->top));
            printUI(tableau, foundation);
            lastCommand = "LD";
            printf("LAST Command: %s\n", lastCommand);
            printf("Message: \n");
            printf("INPUT > ");
        }
    } else if (strcmp(cmd, "SW") == 0) {
        if (*currentPhase == play) {
            printf("Command not available during a game. For a list of available commands, type HELP\n\n");
            return;
        } else if (*currentPhase == welcome) {
            printf("Command not available in welcome phase. For a list of available commands, type HELP\n\n");
            return;
        }
        if (deck->top == NULL) {
            printf("Error: No deck loaded.\n");
            return;
        }
        showTableauCardsStartup(tableau);
        printUI(tableau, foundation);
        lastCommand = "SW";
        printf("LAST Command: %s\n", lastCommand);
        printf("Message: \n");
        printf("INPUT > ");
        // Show cards during startup phase
    } else if (strcmp(cmd, "SI") == 0) {
        if (*currentPhase == play) {
            printf("Command not available during a game. For a list of available commands, type HELP\n\n");
            return;
        } else if (*currentPhase == welcome) {
            printf("Command not available in welcome phase. For a list of available commands, type HELP\n\n");
            return;
        }
        char *splitStr = strtok(NULL, " ");
        clearTableau(tableau);
        if (splitStr != NULL) {
            int split = atoi(splitStr); // Convert the split string to an integer
            shuffleDeckSplit(deck, split);
        } else {
            shuffleDeckSplit(deck, 0);
        }
        startupPopulateTableau(tableau, copyDeck(deck->top));
        printUI(tableau, foundation);
        lastCommand = "SI";
        printf("LAST Command: %s\n", lastCommand);
        printf("Message: \n");
        printf("INPUT > ");
    } else if (strcmp(cmd, "SR") == 0) {
        if (*currentPhase == play) {
            printf("Command not available during a game. For a list of available commands, type HELP\n\n");
            return;
        } else if (*currentPhase == welcome) {
            printf("Command not available in welcome phase. For a list of available commands, type HELP\n\n");
            return;
        }
        clearTableau(tableau);
        shuffleRandom(deck);
        startupPopulateTableau(tableau, copyDeck(deck->top));
        printUI(tableau, foundation);
        lastCommand = "SR";
        printf("LAST Command: %s\n", lastCommand);
        printf("Message: \n");
        printf("INPUT > ");
    } else if (strcmp(cmd, "SD") == 0) {
        if (*currentPhase == welcome) {
            printf("Command not available in welcome phase. For a list of available commands, type HELP\n\n");
            return;
        } else if (*currentPhase == play) {
            printf("Command not available during a game. For a list of available commands, type HELP\n\n");
            return;
        }
        char *filename = strtok(NULL, " ");
        if (filename != NULL) {
            // Save current carddeck to file specified by filename
            char filepath[sizeof(char) * 50];
            snprintf(filepath, sizeof(filepath), "../%s", filename);
            saveDeckToFile(deck->top, filepath);
        } else {
            // Save current carddeck to "cards.txt"
            char filepath[sizeof(char) * 50];
            snprintf(filepath, sizeof(filepath), "../cards.txt");
            saveDeckToFile(deck->top, filepath);
        }
    } else if (strcmp(cmd, "QQ") == 0) {
        // Quit the game
        printf("Goodbye! Thank you for playing Yukon Solitaire...\n");
        exit(0);

    } else if (strcmp(cmd, "P") == 0) {
        if (*currentPhase == welcome) {
            printf("Command not available in welcome phase. For a list of available commands, type HELP\n\n");
            return;
        } else if (*currentPhase == play) {
            printf("Unavailable command: You are already playing a game. For a list of available commands, type HELP\n\n");
            return;
        }
        *currentPhase = play;
        // Transition into PLAY phase. Commands specific to startup phase are no longer available
        //If such a command is attempted, the user will receive an error message stating,
        //“Command not available in the PLAY phase.”
        clearTableau(tableau);
        playPopulateTableau(tableau, copyDeck(deck->top));
        printUI(tableau, foundation);
        lastCommand = "P";
        printf("LAST Command: %s\n", lastCommand);
        printf("Message: \n");
        printf("INPUT > ");

    } else if (strcmp(cmd, "Q") == 0) {
        if (*currentPhase == welcome) {
            printf("Command not available in welcome phase. For a list of available commands, type HELP\n\n");
            return;
        } else if (*currentPhase == startup) {
            printf("Unavailable command: You are already in the startup phase For a list of available commands, type HELP\n\n");
            return;
        }
        *currentPhase = startup;
        clearTableau(tableau);
        startupPopulateTableau(tableau, copyDeck(deck->top));
        printUI(tableau, foundation);
        lastCommand = "Q";
        printf("LAST Command: %s\n", lastCommand);
        printf("Message: \n");
        printf("INPUT > ");
        /*quits the current game and goes back to the STARTUP phase. The memory still
        contains the deck of cards used to play the game that we are quitting. So, if we use the command P
        again after Q, we basically restart the last game. */

    } else if (strcmp(cmd, "HELP") == 0) {
        if (*currentPhase == welcome) {
            printf("LD <filename> - Load deck from file specified by filename during startup phase\n"
                   "QQ - Quit the game\n"
                   "HELP - Display this help message\n");
        }
        if (*currentPhase == startup) {
            printf("LD <filename> - Load deck from file specified by filename during startup phase\n"
                   "SW - Show cards during startup phase\n"
                   "SI <split> - Shuffle deck of cards using a deck-split at the number specified in split during startup phase\n"
                   "SR - Do a random shuffle. Take the top card from the unshuffled pile and insert it at a random position within a new pile, do this until all cards have been randomly inserted into the new pile\n"
                   "SD <filename> - Save current carddeck to file specified by filename\n"
                   "QQ - Quit the game\n"
                   "P - Transition into PLAY phase. Commands specific to startup phase are no longer available\n"
                   "HELP - Display this help message\n");

        } else if (*currentPhase == play) {
            printf("M <source> <destination> - Move card(s) from source to destination\n"
                   "H <source> - Move card(s) from source to foundation\n"
                   "R <source> - Move card(s) from source to tableau\n"
                   "Q - Quits the current game and goes back to the STARTUP phase. The deck of cards used to play the game will not be deleted.\n"
                   "QQ - Quit the game\n"
                   "HELP - Display this help message\n");
        }
    }

        //TODO: Add gamemove commands here

        // Game move commands
        // Follow the format: <from> -> <to>
        // <from>: The source card or pile:
        // A specific card in a column: <column>:<card> (e.g., 'C6:4H' for the 4 of Hearts in column 6).
        // The bottom card in a column: <column> alone indicates the bottom card (e.g., 'C6').
        // The top card from a foundation: Simply use the foundation number (e.g., 'F3').

        // <to>: The destination:
        // To a column's bottom: <column> (e.g., 'C4').
        // To the top of a foundation: <foundation> (e.g., 'F2').

        // Validity Rules:
        // The card moved must exist at the specified location.
        // Moving a card to a column is only valid if the bottom card of that column is one rank higher and
        // of a different suit. A card can only be added to a foundation from the bottom of a column and must follow suit and
        // one rank higher than the top card of the foundation.
        // Only the top card from a foundation can be moved to a column, and it must follow the rank and
        // suit restrictions for columns.
        // If a move is valid, the system returns OK. Otherwise, an error message states that the move is not valid.


    else if (strcmp(movecmd, "C1") == 0) {


    }
    free(commandCopy); // Free the command copy
}
