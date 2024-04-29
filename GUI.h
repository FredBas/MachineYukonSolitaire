#ifndef MACHINEYUKONSOLITAIRE_GUI_H
#define MACHINEYUKONSOLITAIRE_GUI_H

#include "raylib.h"
#include "Cardpile.h"
#include "Commands.h"
#include "Card.h"
#include <string.h>
#include <stdlib.h>

typedef struct {
    int x;
    int y;
    int height;
    int width;
    char* text;
    char *commandToExecute;
    gamePhase phase;
} Button;
void drawGUI(Cardpile *tableau[], Cardpile *foundation[], Card *deck, gamePhase *phase);
void initializeTextures(Texture2D* textures[13][4]);
Texture2D cardToTexture(Card card, Texture2D* textures[13][4]);
void createButtons(Button* buttons[], int amountOfButtons);

#endif //MACHINEYUKONSOLITAIRE_GUI_H
