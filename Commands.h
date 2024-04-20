#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdbool.h>

typedef enum {
    welcome,
    startup,
    play,
}gamePhase;

bool isValidSplitInput(char *str);
void printUIMessages(char *lastcommands, char *message);
void initialTableauPrinter();
void commandHandler(const char *command, Cardpile **tableau, Cardpile **foundation, Cardpile *deck, gamePhase *currentPhase);

#endif // COMMANDS_H