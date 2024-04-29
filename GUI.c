#include "GUI.h"
#include "raylib.h"

void drawGUI() {
    const int screenWidth = 1200;
    const int screenHeight = 800;
    const Color custGreen = {0, 128, 43, 255};
    const Color custLime = {83, 255, 26, 255};

    InitWindow(screenWidth, screenHeight, "Yukon Solitaire");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawRectangleGradientV(0, 0, screenWidth, screenHeight, custGreen, custLime);
        for (int i = 0; i < 7; ++i) {
            DrawRectangleLines(100 + i * 100, 100, 100, 200, BLACK);
        }
        for (int i = 0; i < 4; ++i) {
            DrawRectangleLines(1000, 300 + i * 100, 100, 200, BLACK);
        }
        EndDrawing();
    }

    CloseWindow();
}
