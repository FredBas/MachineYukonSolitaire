#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdbool.h>

typedef enum {
    welcome,
    startup,
    play,
}gamePhase;

void commandHandler(const char *command, Cardpile **tableau, Cardpile **foundation, Cardpile *deck, gamePhase *currentPhase);

#endif // COMMANDS_H