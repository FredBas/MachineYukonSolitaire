#include <stdio.h>
#include "GUI.h"
#include "raylib.h"
#include "GameInitialization.h"

const float cardHeight = 100;
const float cardWidth = cardHeight * 0.7159090909f;

bool isDragging = false;
Vector2 dragOffset = {0}; // Offset to maintain relative position while dragging
Card *draggedCard = NULL;

Suit suitFromASCII(int ascii) {
    switch (ascii) {
        case 67:
            return C;
        case 68:
            return D;
        case 72:
            return H;
        case 83:
            return S;
        default:
            return -1;
    }
}

int rankFromASCII(int ascii) {
    switch (ascii) {
        case 65:
            return 1;
        case 50:
            return 2;
        case 51:
            return 3;
        case 52:
            return 4;
        case 53:
            return 5;
        case 54:
            return 6;
        case 55:
            return 7;
        case 56:
            return 8;
        case 57:
            return 9;
        case 84:
            return 10;
        case 74:
            return 11;
        case 81:
            return 12;
        case 75:
            return 13;
        default:
            return -1;
    }
}

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
    faceDownCard.height = (int) cardHeight;
    faceDownCard.width = (int) cardWidth;

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
            int iterator = 52;
            int nullCardCounter = 0;


            for (int i = 0; i < iterator; i++) {
                Card *card = getCardAt(tableau[i % 7], row);
                if (card == NULL) {
                    x += 86;
                    iterator++;
                    nullCardCounter++;
                } else {
                    int sourceIndex = -1;
                    Rectangle cardRect = {x, y, cardWidth, cardHeight};
                    bool isMouseOverCard = CheckCollisionPointRec(GetMousePosition(), cardRect);
                    if (isMouseOverCard && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && *phase == play) {
                        // If the left mouse button is pressed over the card, start dragging
                        for (int j = 0; j < 7; ++j) {
                            if (GetMouseX() > 15 + j * 86 && GetMouseX() < 15 + j * 86 + cardWidth) {
                                sourceIndex = j;
                                printf("Source index: %d\n", sourceIndex);
                                break;
                            }
                        }
                        isDragging = true;
                        draggedCard = card;
                        dragOffset.x = GetMouseX() - x;
                        dragOffset.y = GetMouseY() - y;
                    }
                    if (isDragging && draggedCard != NULL) {
                        bool isTopCard = true;
                        for (int j = 0; j < 7; ++j) {
                            if (tableau[j]->top != draggedCard) {
                                isTopCard = false;
                                break;
                            }
                        }
                        if (isTopCard) {
                            float offsetX = GetMouseX() - dragOffset.x;
                            float offsetY = GetMouseY() - dragOffset.y;

                            draggedCard->x = offsetX;
                            draggedCard->y = offsetY;

                            for (int j = 0; j < 7; ++j) {
                                Card *currentCard = draggedCard->next;
                                while (currentCard != NULL) {
                                    currentCard->x = offsetX;
                                    currentCard->y = offsetY + (currentCard->y + 20);
                                    currentCard = currentCard->next;
                                }
                            }
                        } else {
                            draggedCard->x = GetMouseX() - dragOffset.x;
                            draggedCard->y = GetMouseY() - dragOffset.y;
                        }
                    }
                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && *phase == play) {
                        if (isDragging) {
                            isDragging = false;
                            int destinationTableau = -1;
                            int foundationIndex = -1;
                            if (GetMouseX() > 800 && GetMouseX() < 800 + cardWidth && GetMouseY() > 100 &&
                                GetMouseY() < 100 + cardHeight * 4 + 4 * 15) {
                                for (int j = 0; j < 4; ++j) {
                                    if (GetMouseY() > 100 + j * 115 && GetMouseY() < 100 + j * 115 + cardHeight) {
                                        foundationIndex = j;
                                        printf("Foundation index: %d\n", foundationIndex);
                                        break;
                                    }
                                }
                                if (foundationIndex != -1) {
                                    if (foundation[foundationIndex]->top == NULL && rankFromASCII(draggedCard->rank)==1) {
                                        moveCard(tableau[sourceIndex], foundation[foundationIndex], draggedCard);
                                        draggedCard = NULL;
                                    } else if (canBePlacedFoundation(*foundation[foundationIndex]->top, *draggedCard)) {
                                        moveCard(tableau[sourceIndex], foundation[foundationIndex], draggedCard);
                                        draggedCard = NULL;
                                    }
                                }
                            } else {
                                for (int j = 0; j < 7; ++j) {
                                    if (GetMouseX() > 15 + j * 86 && GetMouseX() < 15 + j * 86 + cardWidth) {
                                        destinationTableau = j;
                                        printf("Destination index: %d\n", destinationTableau);
                                        break;
                                    }
                                }
                                if (destinationTableau != -1) {
                                    if (tableau[destinationTableau]->top == NULL ||
                                        canBePlacedBottom(*draggedCard, *tableau[destinationTableau]->top)) {
                                        printf("Source: %d, Destination: %d\n", sourceIndex, destinationTableau);
                                        moveCard(tableau[sourceIndex], tableau[destinationTableau], draggedCard);
                                        draggedCard = NULL;
                                    }
                                }
                            }
                        }
                    }

                    if (card->isFaceUp) {
                        Texture2D texture = cardToTexture(*card, textures);
                        if (isDragging && draggedCard == card) {
                            DrawTexture(texture, GetMouseX() - dragOffset.x, GetMouseY() - dragOffset.y, WHITE);
                        } else {
                            DrawTexture(texture, x, y, WHITE);
                        }
                        x += texture.width + 15;

                    } else {
                        DrawTexture(faceDownCard, x, y, WHITE);
                        x += faceDownCard.width + 15;
                    }
                }
                if (i % 7 == 6) {
                    if (nullCardCounter == 7) {
                        break;
                    } else {
                        nullCardCounter = 0;
                    }
                    x = 15;
                    y += 20;
                    row++;
                }
            }
            x = 800;
            y = 100;
            for (int i = 0; i < 4; ++i) {
                DrawRectangle(x, y, faceDownCard.width, faceDownCard.height, BLACK);
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
    switch (suitFromASCII(card.suit)) {
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
        Texture2D textureToReturn = *textures[rankFromASCII(card.rank) - 1][suitNumber];
        textureToReturn.height = cardHeight;
        textureToReturn.width = cardWidth;
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
