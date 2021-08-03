#include "raylib.h"
#include "include/thunder.hpp"

int main(){
  PhysicsEnvironment env(9.8);
  PhysicsBodyRec rectangle(&env, (Vec2){190, 200}, (Vec2){5, 5});

  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "test");

  SetTargetFPS(60);

  while (!WindowShouldClose()){
    env.update();
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawRectangle(rectangle.position.x, rectangle.position.y, 20, 20, BLUE);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
