#ifndef GAME_UI_H
#define GAME_UI_H

#include "Cardpile.h"

void printUI(Cardpile *tableau[], Cardpile *foundation[]);

Card* getCardAt(Cardpile *pile, int cardPosition);

#endif // GAME_UI_H