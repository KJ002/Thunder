#include "raylib.h"
#include "include/thunder.hpp"

int main(){
  PhysicsEnvironment env(9.8);
  PhysicsBodyRec rectangle(&env, (Vec2){190, 200}, (Vec2){0, 0}, 10, 0, 360);

  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "test");

  SetTargetFPS(60);

  rectangle.applyForce((Vec2){10000, 0});

  while (!WindowShouldClose()){
    env.update();
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawRectanglePro((Rectangle){(float)rectangle.position.x, (float)rectangle.position.y, 20, 20}, (Vector2){10, 10}, rectangle.rotation, BLUE);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
