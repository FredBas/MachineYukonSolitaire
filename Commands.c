#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "GameInitialization.h"
#include "Cardpile.h"
#include "Commands.h"
#include "GUI.h"

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

void printUIMessages(char *lastCommand, char *message[]) {
    printf("LAST Command: %s\n", lastCommand);
    if (*message != NULL) {
        printf("Message: %s\n", *message);
    } else printf("Message: \n");
    printf("INPUT > ");
}

void initialTableauPrinter() {
    printf("__________________________________________________________________________________");
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
    char *message[50] = {0};
    // Create a copy of the command string because strtok modifies the original string
    char *commandCopy = strdup(command);
    lastCommand = commandCopy;
    char *movecmdCopy = strdup(command);

    char *cmd = strtok(commandCopy, " ");
    char *movecmd = strtok(movecmdCopy, " -> ");

    if (*currentPhase == welcome && strcmp(cmd, "GUI") == 0) {
        drawGUI(tableau, foundation, deck, currentPhase);
        return;
    }

    if (strcmp(cmd, "LD") == 0) {
        if (*currentPhase == play) {
            message[0] = "Command not available during a game. For a list of available commands, type HELP";
            printUIMessages(lastCommand, message);
            return;
        }
        *currentPhase = startup;
        char *filename = strtok(NULL, " ");
        if (filename == NULL) {
            filename = "unshuffledDeck.txt";
        }
        initializeStartup(tableau, foundation, deck, filename);
        if (deck->top == NULL) {
            message[0] = "Error: Deck not loaded";
            printUIMessages(lastCommand, message);
            return;
        }
        clearTableau(tableau);
        startupPopulateTableau(tableau, copyDeck(deck->top));
        printUI(tableau, foundation);
        message[0] = "Deck loaded successfully";
        printUIMessages(lastCommand, message);
    } else if (strcmp(cmd, "SW") == 0) {
        if (deck->top == NULL) {
            message[0] = "Error: No deck loaded. Please load a deck using the LD command.";
            printUIMessages(lastCommand, message);
            return;
        }

        if (*currentPhase == play || *currentPhase == welcome) {
            message[0] = (*currentPhase == play)
                      ? "Command not available during a game. For a list of available commands, type HELP" :
                      "Command not available in welcome phase. For a list of available commands, type HELP";
            printUIMessages(lastCommand, message);
            return;
        }

        showTableauCardsStartup(tableau);
        printUI(tableau, foundation);
        message[0] = "Cards shown successfully";
        printUIMessages(lastCommand, message);

    } else if (strcmp(cmd, "SI") == 0) {
        if (*currentPhase == play || *currentPhase == welcome) {
            message[0] = (*currentPhase == play)
                      ? "Command not available during a game. For a list of available commands, type HELP" :
                      "Command not available in welcome phase. For a list of available commands, type HELP";
            printUIMessages(lastCommand, message);
            return;
        }
        char *splitStr = strtok(NULL, " ");
        clearTableau(tableau);
        if (splitStr != NULL && isValidSplitInput(splitStr) == true) {
            int split = atoi(splitStr); // Convert the split string to an integer
            shuffleDeckSplit(deck, split);
            message[0] = "Deck shuffled with specified split.";
        } else if (splitStr == NULL) {
            message[0] = "No split input provided. Shuffling deck with random split.";
            shuffleDeckSplit(deck, 0);
        } else if (isValidSplitInput(splitStr) == false) {
            message[0] = "Error: Invalid split input. Please enter a number between 1 and 52.";
        } else {
            shuffleDeckSplit(deck, 0);
            message[0] = "Shuffling deck with random split.";
        }
        startupPopulateTableau(tableau, copyDeck(deck->top));
        printUI(tableau, foundation);
        printUIMessages(lastCommand, message);
    } else if (strcmp(cmd, "SR") == 0) {
        if (*currentPhase == play || *currentPhase == welcome) {
            message[0] = (*currentPhase == play)
                      ? "Command not available during a game. For a list of available commands, type HELP" :
                      "Command not available in welcome phase. For a list of available commands, type HELP";
            printUIMessages(lastCommand, message);
            return;
        }
        clearTableau(tableau);
        shuffleRandom(deck);
        startupPopulateTableau(tableau, copyDeck(deck->top));
        printUI(tableau, foundation);
        message[0] = "Deck shuffled randomly";
        printUIMessages(lastCommand, message);
    } else if (strcmp(cmd, "SD") == 0) {
        lastCommand = "SD";
        if (*currentPhase == play || *currentPhase == welcome) {
            message[0] = (*currentPhase == play)
                      ? "Command not available during a game. For a list of available commands, type HELP" :
                      "Command not available in welcome phase. For a list of available commands, type HELP";
            printUIMessages(lastCommand, message);
            return;
        }
        char *filename = strtok(NULL, " ");
        if (filename == NULL) {
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
        if (*currentPhase == play || *currentPhase == welcome) {
            message[0] = (*currentPhase == play)
                      ? "Unavailable command: You are already playing a game. For a list of available commands, type HELP"
                      :
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
        printUI(tableau, foundation);
        message[0] = "Transitioned to play phase";
        printUIMessages(lastCommand, message);

    } else if (strcmp(cmd, "Q") == 0) {
        //lastCommand = "Q";
        if (*currentPhase == welcome || *currentPhase == startup) {
            message[0] = (*currentPhase == welcome)
                      ? "Command not available in welcome phase. For a list of available commands, type HELP" :
                      "Unavailable command: You are already in the startup phase. For a list of available commands, type HELP";
            printUIMessages(lastCommand, message);
            return;
        }

        *currentPhase = startup;
        clearTableau(tableau);
        startupPopulateTableau(tableau, copyDeck(deck->top));
        printUI(tableau, foundation);
        message[0] = "Transitioned to startup phase";
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

    else if (strcmp(movecmd, "C1") == 0) {
        if (*currentPhase == welcome) {
            message[0] = "Command not available in welcome phase. For a list of available commands, type HELP";
            printUIMessages(lastCommand, message);
            return;
        } else if (*currentPhase == startup) {
            message[0] = "Unavailable command: You are in the startup phase For a list of available commands, type HELP";
            printUIMessages(lastCommand, message);
            return;
        }
        int sourceIndex = 0;
        char *destination = strtok(NULL, " -> ");
        if (destination != NULL) {
            if (destination[0] == 'F') {
                moveToFoundation(sourceIndex, tableau, foundation, destination, message);
            } else if (strcmp(destination, "C1") == 0) {
                message[0] = "Invalid destination";
                printUIMessages(lastCommand, message);
                return;
            } else if (destination[0] == 'C') {
                moveBottomCardToTableau(sourceIndex, tableau, destination, message);
            }
        } else {
            message[0] = "Error: Invalid destination";
            printUIMessages(lastCommand, message);
        }
        printUI(tableau, foundation);
        printUIMessages(lastCommand, message);
    } else if (strcmp(movecmd, "C2") == 0) {
        if (*currentPhase == welcome) {
            message[0] = "Command not available in welcome phase. For a list of available commands, type HELP";
            printUIMessages(lastCommand, message);
            return;
        } else if (*currentPhase == startup) {
            message[0] = "Unavailable command: You are already in the startup phase For a list of available commands, type HELP";
            printUIMessages(lastCommand, message);
            return;
        }
        int sourceIndex = 1;
        char *destination = strtok(NULL, " -> ");
        if (destination != NULL) {
            if (destination[0] == 'F') {
                moveToFoundation(sourceIndex, tableau, foundation, destination, message);
            } else if (strcmp(destination, "C2") == 0) {
                message[0] = "Invalid destination";
                printUIMessages(lastCommand, message);
                return;
            } else if (destination[0] == 'C') {
                moveBottomCardToTableau(sourceIndex, tableau, destination, message);
            }
        } else {
            message[0] = "Error: Invalid destination";
        }
        printUI(tableau, foundation);
        printUIMessages(lastCommand, message);
    } else if (strcmp(movecmd, "C3") == 0) {
        if (*currentPhase == welcome) {
            message[0] = "Command not available in welcome phase. For a list of available commands, type HELP";
            printUIMessages(lastCommand, message);
            return;
        } else if (*currentPhase == startup) {
            message[0] = "Unavailable command: You are already in the startup phase For a list of available commands, type HELP";
            printUIMessages(lastCommand, message);
            return;
        }
        int sourceIndex = 2;
        char *destination = strtok(NULL, " -> ");
        if (destination != NULL) {
            if (destination[0] == 'F') {
                moveToFoundation(sourceIndex, tableau, foundation, destination, message);
            } else if (strcmp(destination, "C3") == 0) {
                message[0] = "Invalid destination";
                printUIMessages(lastCommand, message);
                return;
            } else if (destination[0] == 'C') {
                moveBottomCardToTableau(sourceIndex, tableau, destination, message);
            }
        } else {
            message[0] = "Error: Invalid destination";
        }
        printUI(tableau, foundation);
        printUIMessages(lastCommand, message);
    } else if (strcmp(movecmd, "C4") == 0) {
        if (*currentPhase == welcome) {
            message[0] = "Command not available in welcome phase. For a list of available commands, type HELP";
            printUIMessages(lastCommand, message);
            return;
        } else if (*currentPhase == startup) {
            message[0] = "Unavailable command: You are already in the startup phase For a list of available commands, type HELP";
            printUIMessages(lastCommand, message);
            return;
        }
        int sourceIndex = 3;
        char *destination = strtok(NULL, " -> ");
        if (destination != NULL) {
            if (destination[0] == 'F') {
                moveToFoundation(sourceIndex, tableau, foundation, destination, message);
            } else if (strcmp(destination, "C4") == 0) {
                message[0] = "Invalid destination";
                printUIMessages(lastCommand, message);
                return;
            } else if (destination[0] == 'C') {
                moveBottomCardToTableau(sourceIndex, tableau, destination, message);
            }
        } else {
            message[0] = "Error: Invalid destination";
        }
        printUI(tableau, foundation);
        printUIMessages(lastCommand, message);
    } else if (strcmp(movecmd, "C5") == 0) {
        if (*currentPhase == welcome) {
            message[0] = "Command not available in welcome phase. For a list of available commands, type HELP";
            printUIMessages(lastCommand, message);
            return;
        } else if (*currentPhase == startup) {
            message[0] = "Unavailable command: You are already in the startup phase For a list of available commands, type HELP";
            printUIMessages(lastCommand, message);
            return;
        }
        int sourceIndex = 4;
        char *destination = strtok(NULL, " -> ");
        if (destination != NULL) {
            if (destination[0] == 'F') {
                moveToFoundation(sourceIndex, tableau, foundation, destination, message);
            } else if (strcmp(destination, "C5") == 0) {
                message[0] = "Invalid destination";
                printUIMessages(lastCommand, message);
                return;
            } else if (destination[0] == 'C') {
                moveBottomCardToTableau(sourceIndex, tableau, destination, message);
            }
        } else {
            message[0] = "Error: Invalid destination";
        }
        printUI(tableau, foundation);
        printUIMessages(lastCommand, message);
    } else if (strcmp(movecmd, "C6") == 0) {
        if (*currentPhase == welcome) {
            message[0] = "Command not available in welcome phase. For a list of available commands, type HELP";
            printUIMessages(lastCommand, message);
            return;
        } else if (*currentPhase == startup) {
            message[0] = "Unavailable command: You are already in the startup phase For a list of available commands, type HELP";
            printUIMessages(lastCommand, message);
            return;
        }
        int sourceIndex = 5;
        char *destination = strtok(NULL, " -> ");
        if (destination != NULL) {
            if (destination[0] == 'F') {
                moveToFoundation(sourceIndex, tableau, foundation, destination, message);
            } else if (strcmp(destination, "C6") == 0) {
                message[0] = "Invalid destination";
                printUIMessages(lastCommand, message);
                return;
            } else if (destination[0] == 'C') {
                moveBottomCardToTableau(sourceIndex, tableau, destination, message);
            }
        } else {
            message[0] = "Error: Invalid destination";
        }
        printUI(tableau, foundation);
        printUIMessages(lastCommand, message);
    } else if (strcmp(movecmd, "C7") == 0) {
        if (*currentPhase == welcome) {
            message[0] = "Command not available in welcome phase. For a list of available commands, type HELP";
            printUIMessages(lastCommand, message);
            return;
        } else if (*currentPhase == startup) {
            message[0] = "Unavailable command: You are already in the startup phase For a list of available commands, type HELP";
            printUIMessages(lastCommand, message);
            return;
        }
        int sourceIndex = 6;
        char *destination = strtok(NULL, " -> ");
        if (destination != NULL) {
            if (destination[0] == 'F') {
                moveToFoundation(sourceIndex, tableau, foundation, destination, message);
            } else if (strcmp(destination, "C7") == 0) {
                message[0] = "Invalid destination";
                printUIMessages(lastCommand, message);
                return;
            } else if (destination[0] == 'C') {
                moveBottomCardToTableau(sourceIndex, tableau, destination, message);
            }
        } else {
            message[0] = "Error: Invalid destination";
        }
        printUI(tableau, foundation);
        printUIMessages(lastCommand, message);
    } else if (movecmd[0] == 'F') {
        if (*currentPhase == welcome) {
            message[0] = "Command not available in welcome phase. For a list of available commands, type HELP";
            printUIMessages(lastCommand, message);
            return;
        } else if (*currentPhase == startup) {
            message[0] = "Unavailable command: You are already in the startup phase For a list of available commands, type HELP";
            printUIMessages(lastCommand, message);
            return;
        }
        int sourceIndex = movecmd[1] - '1';
        if (foundation[sourceIndex]->top == NULL) {
            message[0] = "Error: No card in foundation";
            printUIMessages(lastCommand, message);
            return;
        }
        char *destination = strtok(NULL, " -> ");
        if (destination != NULL) {
            if (destination[0] == 'F') {
                int destinationIndex = destination[1] - '1';
                if (foundation[destinationIndex]->top == NULL ||
                    canBePlacedBottom(*foundation[sourceIndex]->top, *foundation[destinationIndex]->top)) {
                    moveToFoundation(sourceIndex, foundation, foundation, destination, message);
                } else {
                    message[0] = "Error: Move not valid";
                }
            } else if (destination[0] == 'C') {
                if (tableau[destination[1] - '1']->top == NULL) {
                    if (foundation[sourceIndex]->top->rank == 'K') {
                        moveCard(foundation[sourceIndex], tableau[destination[1] - '1'], foundation[sourceIndex]->top);
                        message[0] = "Card moved";
                    } else {
                        message[0] = "Error: Move not valid";
                    }
                } else if (canBePlacedBottom(*foundation[sourceIndex]->top, *tableau[destination[1] - '1']->top)) {
                    moveCard(foundation[sourceIndex], tableau[destination[1] - '1'], foundation[sourceIndex]->top);
                    message[0] = "Card moved";
                } else {
                    message[0] = "Error: Move not valid";
                }
            } else {
                message[0] = "Error: Invalid destination";
            }
        } else {
            message[0] = "Error: Invalid destination";
        }
        printUI(tableau, foundation);
        printUIMessages(lastCommand, message);
    } else if (movecmd[0] == 'C' && movecmd[2] == ':') {
        if (*currentPhase == welcome) {
            message[0] = "Command not available in welcome phase. For a list of available commands, type HELP";
            printUIMessages(lastCommand, message);
            return;
        } else if (*currentPhase == startup) {
            message[0] = "Unavailable command: You are already in the startup phase For a list of available commands, type HELP";
            printUIMessages(lastCommand, message);
            return;
        }
        int sourceIndex = movecmd[1] - '1';
        if (tableau[sourceIndex]->top == NULL) {
            message[0] = "Error: No card in tableau";
            printUIMessages(lastCommand, message);
            return;
        }
        char *cardStrCopy = strdup(movecmd + 3);
        char *cardStr = strtok(cardStrCopy, " -> ");
        Rank rank = cardStr[0];
        if (rank == 'A' || rank == 'T' || rank == 'J' || rank == 'Q' || rank == 'K') {
            rank = rankValue(rank);
        } else {
            rank = rank - '0';
        }
        Suit suit = cardStr[1];
        printf("Rank: %d, Suit: %c\n", rank, suit);
        Card *card = getCardAtTableau(tableau[sourceIndex], rank, suit);
        if (card == NULL) {
            message[0] = "Error: Card not found";
            printUIMessages(lastCommand, message);
            return;
        } else {
            printf("Card found: %c%c\n", card->rank, card->suit);
        }
        char *destination;
        destination = commandCopy + 7;
        printf("Destination: %s\n", destination);
        if (destination != NULL && destination[0] == 'C') {
            moveMultipleCardsToTableau(sourceIndex, tableau, destination, card, message);
        } else {
            message[0] = "Error: Invalid destination";
        }
        printUI(tableau, foundation);
        printUIMessages(lastCommand, message);
        free(cardStrCopy);
    } else {
        message[0] = "Error: Invalid command";
        printUIMessages(lastCommand, message);
    }
    free(commandCopy);
}
