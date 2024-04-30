#include <stdio.h>
#include "GUI.h"
#include "raylib.h"
#include "GameInitialization.h"

void drawGUI(Cardpile *tableau[], Cardpile *foundation[], Cardpile *deck, gamePhase *phase) {
    const int screenWidth = 1200;
    const int screenHeight = 800;
    const Color custGreen = {0, 128, 43, 255};
    const Color custLime = {83, 255, 26, 255};
    InitWindow(screenWidth, screenHeight, "Yukon Solitaire");
    SetTargetFPS(60);

    Texture2D *textures[13][4];
    for (int i = 0; i < 13; i++) {
        for (int k = 0; k < 4; k++) {
            textures[i][k] = malloc(sizeof(Texture2D));
        }
    }
    initializeTextures(textures);
    Texture2D faceDownCard = LoadTexture("../PNG-cards-1.3/unshown.png");
    faceDownCard.height = 100;
    faceDownCard.width = faceDownCard.height * 0.7159090909;

    int amountOfButtons = 9;
    Button *buttons[amountOfButtons];
    createButtons(buttons, amountOfButtons);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        int x = 0;
        int y = 0;
        DrawRectangleGradientV(x, y, screenWidth, screenHeight, custGreen, custLime);
        x = 15;
        y = 15;

        if (*phase != welcome) {
            for (int i = 0; i < 7; ++i) {
                //DrawRectangleLines(100 + i * 100, 100, 100, 200, BLACK);
                DrawText(TextFormat("C%d", i + 1), x, y, 15, BLACK);
                x += 86;
            }
            x = 15;
            y = 40;

            int row = 0;
            for (int i = 0; i < DECK_SIZE; i++) {
                Card *card = getCardAt(tableau[i % 7], row);
                if (card == NULL) {
                    x += 100;
                } else {
                    if (card->isFaceUp) {
                        Texture2D texture = cardToTexture(*card, textures);
                        DrawTexture(texture, x, y, WHITE);
                        x += texture.width + 15;
                    } else {
                        DrawTexture(faceDownCard, x, y, WHITE);
                        x += faceDownCard.width + 15;
                    }
                }
                if (i % 7 == 6) {
                    x = 15;
                    y += 20;
                    row++;
                }
            }
            x = 800;
            y = 100;
            for (int i = 0; i < 4; ++i) {
                DrawRectangleLines(x, y, faceDownCard.width, faceDownCard.height, BLACK);
                DrawText(TextFormat("F%d", i + 1), x + faceDownCard.width + 15, y, 15, BLACK);
                y += faceDownCard.height + 15;

            }
        }

        for (int i = 0; i < amountOfButtons; i++) {
            if (buttons[i]->phase != *phase) { continue; }
            DrawRectangle(buttons[i]->x, buttons[i]->y, buttons[i]->width, buttons[i]->height, BLACK);
            int textWidth = MeasureText(buttons[i]->text, 20);
            int textX = buttons[i]->x + (buttons[i]->width - textWidth) / 2;
            int textY = buttons[i]->y + (buttons[i]->height - 20) / 2;


            DrawText(buttons[i]->text, textX, textY, 20, GREEN);
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && buttons[i]->x < GetMouseX() &&
                buttons[i]->x + buttons[i]->width > GetMouseX() && buttons[i]->y < GetMouseY() &&
                buttons[i]->y + buttons[i]->height > GetMouseY()) {
                commandHandler(buttons[i]->commandToExecute, tableau, foundation, deck, phase);
            }
        }
        EndDrawing();
    }

    CloseWindow();
}

void initializeTextures(Texture2D *textures[13][4]) {
    char fileToLoad[50];
    for (int i = 0; i < 13; i++) {
        for (int k = 0; k < 4; k++) {
            printf("%d,%d\n", i, k);
            strcpy(fileToLoad, "../PNG-cards-1.3/");

            switch (i) {
                case 0:
                    strcat(fileToLoad, "ace");
                    break;
                case 9:
                    strcat(fileToLoad, "10");
                    break;
                case 10:
                    strcat(fileToLoad, "jack");
                    break;
                case 11:
                    strcat(fileToLoad, "queen");
                    break;
                case 12:
                    strcat(fileToLoad, "king");
                    break;
                default:
                    strcat(fileToLoad, TextFormat("%d", i + 1));
            }
            strcat(fileToLoad, "_of_");
            switch (k) {
                case 0:
                    strcat(fileToLoad, "clubs");
                    break;
                case 1:
                    strcat(fileToLoad, "diamonds");
                    break;
                case 2:
                    strcat(fileToLoad, "hearts");
                    break;
                case 3:
                    strcat(fileToLoad, "spades");
                    break;
            }
            strcat(fileToLoad, ".png");
            Texture2D texture = LoadTexture(fileToLoad);
            *textures[i][k] = texture;
            if (texture.id == 0) k--;
        }
    }
}

Texture2D cardToTexture(Card card, Texture2D *textures[13][4]) {
    int suitNumber = -1;
    switch (card.suit) {
        case C:
            suitNumber = 0;
            break;
        case D:
            suitNumber = 1;
            break;
        case H:
            suitNumber = 2;
            break;
        case S:
            suitNumber = 3;
            break;
    }

    if (suitNumber == -1) {
        printf("Error: Suit not found\n");
        return LoadTexture("../PNG-cards-1.3/unshown.png");
    } else {
        Texture2D textureToReturn = *textures[card.rank - 1][suitNumber];
        textureToReturn.height = 100;
        textureToReturn.width = textureToReturn.height * 0.71;
        return textureToReturn;
    }

}

void createButtons(Button *buttons[], int amountOfButtons) {
    for (int i = 0; i < amountOfButtons; i++) {
        buttons[i] = malloc(sizeof(Button));
    }
    int x = 25;
    int y = 700;
    int buttonHeight = 100;
    int buttonWidth = 150;

    buttons[0]->height = buttonHeight;
    buttons[0]->width = buttonWidth;
    buttons[0]->text = "Load";
    buttons[0]->commandToExecute = "LD";
    buttons[0]->x = 350;
    buttons[0]->y = 350;
    buttons[0]->phase = welcome;

    buttons[1]->height = buttonHeight;
    buttons[1]->width = buttonWidth;
    buttons[1]->text = "Show";
    buttons[1]->commandToExecute = "SW";
    buttons[1]->x = x;
    buttons[1]->y = y;
    buttons[1]->phase = startup;

    x += buttonWidth + 50;

    buttons[2]->height = buttonHeight;
    buttons[2]->width = buttonWidth;
    buttons[2]->text = "Split";
    buttons[2]->commandToExecute = "SI";
    buttons[2]->x = x;
    buttons[2]->y = y;
    buttons[2]->phase = startup;

    x += buttonWidth + 50;

    buttons[3]->height = buttonHeight;
    buttons[3]->width = buttonWidth;
    buttons[3]->text = "Shuffle";
    buttons[3]->commandToExecute = "SR";
    buttons[3]->x = x;
    buttons[3]->y = y;
    buttons[3]->phase = startup;

    x += buttonWidth + 50;

    buttons[4]->height = buttonHeight;
    buttons[4]->width = buttonWidth;
    buttons[4]->text = "Save";
    buttons[4]->commandToExecute = "SD";
    buttons[4]->x = x;
    buttons[4]->y = y;
    buttons[4]->phase = startup;

    x += buttonWidth + 50;

    buttons[5]->height = buttonHeight;
    buttons[5]->width = buttonWidth;
    buttons[5]->text = "Play";
    buttons[5]->commandToExecute = "P";
    buttons[5]->x = x;
    buttons[5]->y = y;
    buttons[5]->phase = startup;

    x += buttonWidth + 50;

    buttons[6]->height = buttonHeight;
    buttons[6]->width = buttonWidth;
    buttons[6]->text = "Quit";
    buttons[6]->commandToExecute = "QQ";
    buttons[6]->x = 700;
    buttons[6]->y = 350;
    buttons[6]->phase = welcome;

    buttons[7]->height = buttonHeight;
    buttons[7]->width = buttonWidth;
    buttons[7]->text = "Quit";
    buttons[7]->commandToExecute = "QQ";
    buttons[7]->x = x;
    buttons[7]->y = y;
    buttons[7]->phase = startup;

    buttons[8]->height = buttonHeight;
    buttons[8]->width = buttonWidth;
    buttons[8]->text = "Quit";
    buttons[8]->commandToExecute = "QQ";
    buttons[8]->x = x;
    buttons[8]->y = y;
    buttons[8]->phase = play;
}
