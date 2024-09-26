#include "raylib.h"
#include <stdio.h>
#include <math.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128
#define SCALE 4
#define WINDOW_WIDTH SCREEN_WIDTH * SCALE
#define WINDOW_HEIGHT SCREEN_HEIGHT * SCALE

#define TILE_SIZE 8

#define MAX(a,b) ((a) > (b) ? a : b)
#define MIN(a,b) ((a) < (b) ? a : b)

typedef struct Lane {
    float velocity;
    float offset;
    char objects[64];
    char type;
} Lane;

// N - no sprite, L - land, W - water, R - road
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

// N - cannot move here, F - finish, W - water, L - land, R - road
Lane lanes[16] = {  {0, 0, "................................................................", 'N'}, // Score
                    {0, 0, "................................................................", 'N'}, // Extra land
                    {0, 0, "................................................................", 'F'}, // Finish
                    {0, 0, "................................................................", 'W'}, // Logs
                    {0, 0, "................................................................", 'W'}, // Logs
                    {0, 0, "................................................................", 'W'}, // Logs
                    {0, 0, "................................................................", 'W'}, // Logs
                    {1, 0, "1223....1223....1223....1223....1223....1223....1223....1223....", 'W'}, // Logs
                    {0, 0, "................................................................", 'L'}, // Land
                    {0, 0, "................................................................", 'R'}, // Road
                    {0, 0, "................................................................", 'R'}, // Road
                    {0, 0, "................................................................", 'R'}, // Road
                    {0, 0, "................................................................", 'R'}, // Road
                    {-5, 0, "X...X...X...X...X...X...X...X...X...X...X...X...X...X...X...X...", 'R'}, // Road
                    {0, 0, "................................................................", 'L'}, // Land
                    {0, 0, "................................................................", 'N'}, // Lives
                    };

int main() {
    Vector2 playerPosition = {8, 14};
    float gameTime = 0;

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Frogger");

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

    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        float deltaTime = GetFrameTime();
        gameTime += deltaTime;

        if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
            switch (lanes[(int)playerPosition.y].type) {
            case 'L':
            case 'R': playerPosition.x = MAX(playerPosition.x - 1, 0); break;  
            }
        }
        if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
            switch (lanes[(int)playerPosition.y].type) {
            case 'L':
            case 'R': playerPosition.x = MIN(playerPosition.x + 1, 15); break;  
            }
        }
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
            switch (lanes[(int)(playerPosition.y - 1)].type) {
            case 'L':
            case 'R': playerPosition.y = playerPosition.y - 1; break;  
            }
        }
        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
            switch (lanes[(int)(playerPosition.y + 1)].type) {
            case 'L':
            case 'R': playerPosition.y = playerPosition.y + 1; break;  
            }
        }

        for (int y = 0; y < 16; y++) {
            if (lanes[y].velocity != 0.0f) {
                lanes[y].offset = lanes[y].offset + deltaTime * lanes[y].velocity;
                if (lanes[y].offset < 0) {lanes[y].offset = 64 - abs(lanes[y].offset);}
                else if (lanes[y].offset >= 64) {lanes[y].offset = fmod(lanes[y].offset, 64);}
            }
        }

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

            for (int y = 0; y < 16; y++) {
                if (lanes[y].type == 'R') {
                    for (int x = 0; x < 64; x++) {
                        if (lanes[y].objects[x] == 'X') {
                            float xPosition = fmod(x + lanes[y].offset + 1, 64) - 1;
                            DrawTexturePro(tileset, carSource, (Rectangle){xPosition * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE}, (Vector2){0, 0}, 0, WHITE);
                        }
                    }
                } else if (lanes[y].type == 'W') {
                    for (int x = 0; x < 64; x++) {
                        Rectangle sourceToDraw = {0, 0, 0, 0};
                        switch (lanes[y].objects[x]) {
                            case '1': sourceToDraw = frontLogSource; break;
                            case '2': sourceToDraw = middleLogSource; break;
                            case '3': sourceToDraw = endLogSource; break;
                        }
                        if (sourceToDraw.width) {
                            float xPosition = fmod(x + lanes[y].offset + 1, 64) - 1;
                            DrawTexturePro(tileset, sourceToDraw, (Rectangle){xPosition * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE}, (Vector2){0, 0}, 0, WHITE);
                        }
                    }
                }
            }

            if (lanes[(int)playerPosition.y].type == 'L' || lanes[(int)playerPosition.y].type == 'R') {
                DrawTexturePro(tileset, playerSource, (Rectangle){playerPosition.x * TILE_SIZE, playerPosition.y * TILE_SIZE, TILE_SIZE, TILE_SIZE}, (Vector2){0, 0}, 0, WHITE);
            }

            EndTextureMode();

            DrawTexturePro(screen.texture, screenSource, screenDestination, (Vector2){0, 0}, 0, WHITE);
            DrawFPS(0, 0);

        EndDrawing();

    }

    return 0;
}