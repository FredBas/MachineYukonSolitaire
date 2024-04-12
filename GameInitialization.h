#ifndef GAME_INITIALIZATION_H
#define GAME_INITIALIZATION_H

#include "Cardpile.h"
#include "Card.h"
#include "GameUI.h"

void startupPopulateTableau(Cardpile *tableau, Card *head);
void initializeStartup(Cardpile **tableau, Cardpile **foundation, Cardpile *deck);

#endif // GAME_INITIALIZATION_H