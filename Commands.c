#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "GameInitialization.h"
#include "Cardpile.h"
#include "Commands.h"

bool isValidSplitInput(char *str) {
    int length = strlen(str);
    for (int i = 0; i < length; i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    int num = atoi(str);
    if (num < 1 || num > 52) {
        return false;
    }
    return true;
}

void printUIMessages(char *lastCommand, char *message) {
    printf("LAST Command: %s\n", lastCommand);
    printf("Message: %s\n", message);
    printf("INPUT > ");
}

void initialTableauPrinter() {
    printf("\nC1\tC2\tC3\tC4\tC5\tC6\tC7\n\n");
    printf("  \t  \t  \t  \t  \t  \t  \t\t[]\tF1\n");
    printf("  \t  \t  \t  \t  \t  \t  \t\n");
    printf("  \t  \t  \t  \t  \t  \t  \t\t[]\tF2\n");
    printf("  \t  \t  \t  \t  \t  \t  \t\n");
    printf("  \t  \t  \t  \t  \t  \t  \t\t[]\tF3\n");
    printf("  \t  \t  \t  \t  \t  \t  \t\n");
    printf("  \t  \t  \t  \t  \t  \t  \t\t[]\tF4\n");
    printf("\n");
}

void commandHandler(const char *command, Cardpile **tableau, Cardpile **foundation, Cardpile *deck, gamePhase *currentPhase) {

    char *lastCommand = "";
    char *message = "OK";
    // Create a copy of the command string because strtok modifies the original string
    char *commandCopy = strdup(command);
    char *movecmdCopy = strdup(command);
    // Get the first token (the command)
    char *cmd = strtok(commandCopy, " ");
    char *movecmd = strtok(movecmdCopy, " -> ");

    if (strcmp(cmd, "LD") == 0) {
        lastCommand = "LD";
        if (*currentPhase == play) {
            message = "Command not available during a game. For a list of available commands, type HELP";
            printUIMessages(lastCommand, message);
            return;
        }
        *currentPhase = startup;
        char *filename = strtok(NULL, " ");
        if (filename == NULL) {
            filename = "unshuffledDeck.txt";
        }
        initializeStartup(tableau, foundation, deck, filename);
        clearTableau(tableau);
        startupPopulateTableau(tableau, copyDeck(deck->top));
        printUI(tableau, foundation);
        printUIMessages(lastCommand, message);
    } else if (strcmp(cmd, "SW") == 0) {
        lastCommand = "SW";
        if (deck->top == NULL) {
            message = "Error: No deck loaded. Please load a deck using the LD command.";
            printUIMessages(lastCommand, message);
            return;
        }

        if (*currentPhase == play || *currentPhase == welcome) {
            message = (*currentPhase == play) ? "Command not available during a game. For a list of available commands, type HELP" :
                                                "Command not available in welcome phase. For a list of available commands, type HELP";
            printUIMessages(lastCommand, message);
            return;
        }

        showTableauCardsStartup(tableau);
        printUI(tableau, foundation);
        printUIMessages(lastCommand, message);
        // Show cards during startup phase
    } else if (strcmp(cmd, "SI") == 0) {
        lastCommand = "SI";
        if (*currentPhase == play || *currentPhase == welcome) {
            message = (*currentPhase == play) ? "Command not available during a game. For a list of available commands, type HELP" :
                                                "Command not available in welcome phase. For a list of available commands, type HELP";
            printUIMessages(lastCommand, message);
            return;
        }
        char *splitStr = strtok(NULL, " ");
        clearTableau(tableau);
        if (splitStr != NULL && isValidSplitInput(splitStr) == true) {
            int split = atoi(splitStr); // Convert the split string to an integer
            shuffleDeckSplit(deck, split);
        }else if (splitStr == NULL) {
            message = "No split input provided. Shuffling deck with split 0.";
            shuffleDeckSplit(deck, 0);
        }
        else if (isValidSplitInput(splitStr) == false) {
            message = "Error: Invalid split input. Please enter a number between 1 and 52.";
        } else {
            shuffleDeckSplit(deck, 0);
        }
        startupPopulateTableau(tableau, copyDeck(deck->top));
        printUI(tableau, foundation);
        printUIMessages(lastCommand, message);
    } else if (strcmp(cmd, "SR") == 0) {
        lastCommand = "SR";
        if (*currentPhase == play || *currentPhase == welcome) {
            message = (*currentPhase == play) ? "Command not available during a game. For a list of available commands, type HELP" :
                                                "Command not available in welcome phase. For a list of available commands, type HELP";
            printUIMessages(lastCommand, message);
            return;
        }
        clearTableau(tableau);
        shuffleRandom(deck);
        startupPopulateTableau(tableau, copyDeck(deck->top));
        printUI(tableau, foundation);
        printUIMessages(lastCommand, message);
    } else if (strcmp(cmd, "SD") == 0) {
        lastCommand = "SD";
        if (*currentPhase == play || *currentPhase == welcome) {
            message = (*currentPhase == play) ? "Command not available during a game. For a list of available commands, type HELP" :
                                                "Command not available in welcome phase. For a list of available commands, type HELP";
            printUIMessages(lastCommand, message);
            return;
        }
        char *filename = strtok(NULL, " ");
        if(filename == NULL) {
            filename = "cards.txt";
        }
        char filepath[sizeof(char) * 50];
        snprintf(filepath, sizeof(filepath), "../%s", filename);
        saveDeckToFile(deck->top, filepath);

    } else if (strcmp(cmd, "QQ") == 0) {
        // Quit the game
        printf("Message : Goodbye! Thank you for playing Yukon Solitaire...");
        exit(0);

    } else if (strcmp(cmd, "P") == 0) {
        lastCommand = "P";
        if (*currentPhase == play || *currentPhase == welcome) {
            message = (*currentPhase == play) ? "Unavailable command: You are already playing a game. For a list of available commands, type HELP":
                                                "Command not available in welcome phase. For a list of available commands, type HELP";
            printUIMessages(lastCommand, message);
            return;
        }
        *currentPhase = play;
        // Transition into PLAY phase. Commands specific to startup phase are no longer available
        //If such a command is attempted, the user will receive an error message stating,
        //“Command not available in the PLAY phase.”
        clearTableau(tableau);
        playPopulateTableau(tableau, copyDeck(deck->top));
        printCard(getCardAtTableauBottom(tableau[1]));
        printUI(tableau, foundation);
        printUIMessages(lastCommand, message);

    } else if (strcmp(cmd, "Q") == 0) {
        lastCommand = "Q";
        if (*currentPhase == welcome || *currentPhase == startup) {
            message = (*currentPhase == welcome) ? "Command not available in welcome phase. For a list of available commands, type HELP" :
                                                   "Unavailable command: You are already in the startup phase. For a list of available commands, type HELP";
            printUIMessages(lastCommand, message);
            return;
        }

        *currentPhase = startup;
        clearTableau(tableau);
        startupPopulateTableau(tableau, copyDeck(deck->top));
        printUI(tableau, foundation);
        printUIMessages(lastCommand, message);
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
        if (*currentPhase == welcome) {
            printf("Command not available in welcome phase. For a list of available commands, type HELP\n\n");
            return;
        } else if (*currentPhase == startup) {
            printf("Unavailable command: You are already in the startup phase For a list of available commands, type HELP\n\n");
            return;
        }
        int sourceIndex = 0;
        char *destination = strtok(NULL, " -> ");
        if (destination != NULL) {
            if (destination[0] == 'F') {
                moveToFoundation(sourceIndex, tableau, foundation, destination);
            } else if (strcmp(destination, "C1") == 0) {
                printf("Invalid destination\n");
                return;
            } else if (destination[0] == 'C') {
                moveBottomCardToTableau(sourceIndex, tableau, destination);
            }
        } else {
            printf("Error: Invalid destination\n");
        }
        printUI(tableau, foundation);
        lastCommand = "???";
        printUIMessages(lastCommand, message);
    }
    else if (strcmp(movecmd, "C2") == 0) {
        if (*currentPhase == welcome) {
            printf("Command not available in welcome phase. For a list of available commands, type HELP\n\n");
            return;
        } else if (*currentPhase == startup) {
            printf("Unavailable command: You are already in the startup phase For a list of available commands, type HELP\n\n");
            return;
        }
        int sourceIndex = 1;
        char *destination = strtok(NULL, " -> ");
        if (destination != NULL) {
            if (destination[0] == 'F') {
                moveToFoundation(sourceIndex, tableau, foundation, destination);
            } else if (strcmp(destination, "C2") == 0) {
                printf("Invalid destination\n");
                return;
            } else if (destination[0] == 'C') {
                moveBottomCardToTableau(sourceIndex, tableau, destination);
            }
        } else {
            printf("Error: Invalid destination\n");
        }
        printUI(tableau, foundation);
        lastCommand = "???";
        printUIMessages(lastCommand, message);
    } else if (strcmp(movecmd, "C3") == 0) {
        if (*currentPhase == welcome) {
            printf("Command not available in welcome phase. For a list of available commands, type HELP\n\n");
            return;
        } else if (*currentPhase == startup) {
            printf("Unavailable command: You are already in the startup phase For a list of available commands, type HELP\n\n");
            return;
        }
        int sourceIndex = 2;
        char *destination = strtok(NULL, " -> ");
        if (destination != NULL) {
            if (destination[0] == 'F') {
                moveToFoundation(sourceIndex, tableau, foundation, destination);
            } else if (strcmp(destination, "C3") == 0) {
                printf("Invalid destination\n");
                return;
            } else if (destination[0] == 'C') {
                moveBottomCardToTableau(sourceIndex, tableau, destination);
            }
        } else {
            printf("Error: Invalid destination\n");
        }
        printUI(tableau, foundation);
        lastCommand = "???";
        printUIMessages(lastCommand, message);
    } else if (strcmp(movecmd, "C4") == 0) {
        if (*currentPhase == welcome) {
            printf("Command not available in welcome phase. For a list of available commands, type HELP\n\n");
            return;
        } else if (*currentPhase == startup) {
            printf("Unavailable command: You are already in the startup phase For a list of available commands, type HELP\n\n");
            return;
        }
        int sourceIndex = 3;
        char *destination = strtok(NULL, " -> ");
        if (destination != NULL) {
            if (destination[0] == 'F') {
                moveToFoundation(sourceIndex, tableau, foundation, destination);
            } else if (strcmp(destination, "C4") == 0) {
                printf("Invalid destination\n");
                return;
            } else if (destination[0] == 'C') {
                moveBottomCardToTableau(sourceIndex, tableau, destination);
            }
        } else {
            printf("Error: Invalid destination\n");
        }
        printUI(tableau, foundation);
        lastCommand = "???";
        printUIMessages(lastCommand, message);
    } else if (strcmp(movecmd, "C5") == 0) {
        if (*currentPhase == welcome) {
            printf("Command not available in welcome phase. For a list of available commands, type HELP\n\n");
            return;
        } else if (*currentPhase == startup) {
            printf("Unavailable command: You are already in the startup phase For a list of available commands, type HELP\n\n");
            return;
        }
        int sourceIndex = 4;
        char *destination = strtok(NULL, " -> ");
        if (destination != NULL) {
            if (destination[0] == 'F') {
                moveToFoundation(sourceIndex, tableau, foundation, destination);
            } else if (strcmp(destination, "C5") == 0) {
                printf("Invalid destination\n");
                return;
            } else if (destination[0] == 'C') {
                moveBottomCardToTableau(sourceIndex, tableau, destination);
            }
        } else {
            printf("Error: Invalid destination\n");
        }
        printUI(tableau, foundation);
        lastCommand = "???";
        printUIMessages(lastCommand, message);
    }
    else if (strcmp(movecmd, "C6") == 0) {
        if (*currentPhase == welcome) {
            printf("Command not available in welcome phase. For a list of available commands, type HELP\n\n");
            return;
        } else if (*currentPhase == startup) {
            printf("Unavailable command: You are already in the startup phase For a list of available commands, type HELP\n\n");
            return;
        }
        int sourceIndex = 5;
        char *destination = strtok(NULL, " -> ");
        if (destination != NULL) {
            if (destination[0] == 'F') {
                moveToFoundation(sourceIndex, tableau, foundation, destination);
            } else if (strcmp(destination, "C6") == 0) {
                printf("Invalid destination\n");
                return;
            } else if (destination[0] == 'C') {
                moveBottomCardToTableau(sourceIndex, tableau, destination);
            }
        } else {
            printf("Error: Invalid destination\n");
        }
        printUI(tableau, foundation);
        lastCommand = "???";
        printUIMessages(lastCommand, message);
    }
    else if (strcmp(movecmd, "C7") == 0) {
        if (*currentPhase == welcome) {
            printf("Command not available in welcome phase. For a list of available commands, type HELP\n\n");
            return;
        } else if (*currentPhase == startup) {
            printf("Unavailable command: You are already in the startup phase For a list of available commands, type HELP\n\n");
            return;
        }
        int sourceIndex = 6;
        char *destination = strtok(NULL, " -> ");
        if (destination != NULL) {
            if (destination[0] == 'F') {
                moveToFoundation(sourceIndex, tableau, foundation, destination);
            } else if (strcmp(destination, "C7") == 0) {
                printf("Invalid destination\n");
                return;
            } else if (destination[0] == 'C') {
                moveBottomCardToTableau(sourceIndex, tableau, destination);
            }
        } else {
            printf("Error: Invalid destination\n");
        }
        printUI(tableau, foundation);
        lastCommand = "???";
        printUIMessages(lastCommand, message);
    }

    free(commandCopy); // Free the command copy
}
