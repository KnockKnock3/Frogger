#include "raylib.h"
#include <stdio.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128
#define SCALE 4
#define WINDOW_WIDTH SCREEN_WIDTH * SCALE
#define WINDOW_HEIGHT SCREEN_HEIGHT * SCALE

#define TILE_SIZE 8

const char background[16][16] = {   "NNNNNNNNNNNNNNNN",
                                    "LLLLLLLLLLLLLLLL",
                                    "LLLLLLLLLLLLLLLL",
                                    "WWWWWWWWWWWWWWWW",
                                    "WWWWWWWWWWWWWWWW",
                                    "WWWWWWWWWWWWWWWW",
                                    "WWWWWWWWWWWWWWWW",
                                    "WWWWWWWWWWWWWWWW",
                                    "LLLLLLLLLLLLLLLL",
                                    "RRRRRRRRRRRRRRRR",
                                    "RRRRRRRRRRRRRRRR",
                                    "RRRRRRRRRRRRRRRR",
                                    "RRRRRRRRRRRRRRRR",
                                    "RRRRRRRRRRRRRRRR",
                                    "LLLLLLLLLLLLLLLL",
                                    "NNNNNNNNNNNNNNNN"};

int main() {
    Vector2 playerPosition = {8, 0};

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Frogger");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        float deltaTime = GetFrameTime();

        RenderTexture2D screen = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
        Rectangle screenSource = {0, -screen.texture.height, screen.texture.width, -screen.texture.height};
        Rectangle screenDestination = (Rectangle){0, 0, (float)(WINDOW_WIDTH), (float)(WINDOW_HEIGHT)};

        Texture2D tileset = LoadTexture("res/tileset.png");
        Rectangle playerSource = {0, 0, TILE_SIZE, TILE_SIZE};
        Rectangle playerFinishedSource = {TILE_SIZE, 0, TILE_SIZE, TILE_SIZE};
        Rectangle landSource = {0, TILE_SIZE, TILE_SIZE, TILE_SIZE};
        Rectangle roadSource = {0, 2 * TILE_SIZE, TILE_SIZE, TILE_SIZE};
        Rectangle waterSource = {0, 3 * TILE_SIZE, TILE_SIZE, TILE_SIZE};
        Rectangle carSource = {0, 4 * TILE_SIZE, TILE_SIZE, TILE_SIZE};
        Rectangle frontLogSource = {0, 5 * TILE_SIZE, TILE_SIZE, TILE_SIZE};
        Rectangle middleLogSource = {TILE_SIZE, 5 * TILE_SIZE, TILE_SIZE, TILE_SIZE};
        Rectangle endLogSource = {2 * TILE_SIZE, 5 * TILE_SIZE, TILE_SIZE, TILE_SIZE};

        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginTextureMode(screen);
            ClearBackground(BLACK);

            for (int y = 0; y < 16; y++) { // Draw Background
                for (int x = 0; x < 16; x++) {
                    Rectangle sourceToDraw = {0, 0, 0, 0};
                    switch (background[y][x]) {
                    case 'L': sourceToDraw = landSource; break;
                    case 'W': sourceToDraw = waterSource; break;
                    case 'R': sourceToDraw = roadSource; break;
                    }
                    if (sourceToDraw.width) {
                        DrawTexturePro(tileset, sourceToDraw, (Rectangle){x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE}, (Vector2){0, 0}, 0, WHITE);
                    }
                }
            }

            EndTextureMode();

            DrawTexturePro(screen.texture, screenSource, screenDestination, (Vector2){0, 0}, 0, WHITE);
            DrawFPS(0, 0);

        EndDrawing();

    }

    printf("Hello World!\n");
    return 0;
}