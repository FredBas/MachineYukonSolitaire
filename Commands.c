#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "GameInitialization.h"

void commandHandler(const char *command, Cardpile **tableau, Cardpile **foundation, Cardpile *deck) {
    char *currentPhase = "startupPhase";

    // Create a copy of the command string because strtok modifies the original string
    char *commandCopy = strdup(command);
    // Get the first token (the command)
    char *cmd = strtok(commandCopy, " ");

    if (strcmp(cmd, "LD") == 0) {
        char *filename = strtok(NULL, " ");
        if (filename != NULL) {
            initializeStartup(tableau, foundation, deck, filename);
            printUI(tableau, foundation, false);

        } else {
            initializeStartup(tableau, foundation, deck, "unshuffledDeck.txt");
            printUI(tableau, foundation, false);
        }
    } else if (strcmp(cmd, "SW") == 0) {
        if (deck->top == NULL) {
            printf("Error: No deck loaded.\n");
            return;
        }
        printUI(tableau, foundation, true);
        // Show cards during startup phase
    } else if (strcmp(cmd, "SI") == 0) {
        char *splitStr = strtok(NULL, " ");
        if (splitStr != NULL) {
            int split = atoi(splitStr); // Convert the split string to an integer
            // Shuffle deck of cards using a deck-split at the number specified in split during startup phase
        } else {
            // Shuffle deck of cards using a deck-split at 26 during startup phase
        }
    } else if (strcmp(cmd, "SR") == 0) {
        /* Do a random shuffle. Take the top card from the unshuffled pile and insert it at a random position
           within a new pile, do this until all cards have been randomly inserted into the new pile */
    } else if (strcmp(cmd, "SD") == 0) {
        char *filename = strtok(NULL, " ");
        if (filename != NULL) {
            // Save current carddeck to file specified by filename
        } else {
            // Save current carddeck to "cards.txt"
        }
    } else if (strcmp(cmd, "QQ") == 0) {
        // Quit the game
    } else if (strcmp(cmd, "P") == 0) {
        // Transition into PLAY phase. Commands specific to startup phase are no longer available
        //If such a command is attempted, the user will receive an error message stating,
        //“Command not available in the PLAY phase.”
    } else if (strcmp(cmd, "Q") == 0) {
        /*quits the current game and goes back to the STARTUP phase. The memory still
        contains the deck of cards used to play the game that we are quitting. So, if we use the command P
        again after Q, we basically restart the last game. */

    } else if (strcmp(cmd, "HELP") == 0) {
        if (currentPhase == "startupPhase") {
            printf("LD <filename> - Load deck from file specified by filename during startup phase\n"
                   "SW - Show cards during startup phase\n"
                   "SI <split> - Shuffle deck of cards using a deck-split at the number specified in split during startup phase\n"
                   "SR - Do a random shuffle. Take the top card from the unshuffled pile and insert it at a random position within a new pile, do this until all cards have been randomly inserted into the new pile\n"
                   "SD <filename> - Save current carddeck to file specified by filename\n"
                   "QQ - Quit the game\n"
                   "P - Transition into PLAY phase. Commands specific to startup phase are no longer available\n"
                   "HELP - Display this help message\n");

        } else if (currentPhase == "playPhase") {
            printf("M <source> <destination> - Move card(s) from source to destination\n"
                   "H <source> - Move card(s) from source to foundation\n"
                   "R <source> - Move card(s) from source to tableau\n"
                   "D - Draw a card from the deck\n"
                   "Q - Quits the current game and goes back to the STARTUP phase. The memory still contains the deck of cards used to play the game that we are quitting. So, if we use the command P again after Q, we basically restart the last game.\n"
                   "QQ - Quit the game completely\n"
                   "HELP - Display this help message\n");
        }

            // Handle unknown command


        //TODO: Add gamemove commands here

        free(commandCopy); // Free the command copy
    }
}