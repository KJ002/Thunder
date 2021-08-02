#include "raylib.h"

int main(){
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "test");

    SetTargetFPS(60);

    while (!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawRectangle(190, 200, 20, 20, BLUE);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
