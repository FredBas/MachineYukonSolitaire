#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdbool.h>

void commandHandler(const char *command, Cardpile **tableau, Cardpile **foundation, Cardpile *deck);

#endif // COMMANDS_H