#include "raylib.h"
#include "include/thunder.hpp"

int main(){

  /* Setup Bodies */

  PhysicsEnvironment env(9.8);

  PhysicsBodyRec testPhysicsBody(&env, (Vec2){190, 200}, (Vec2){0, 0}, 10, 0, 360);
  Rectangle testRenderBody = {(float)testPhysicsBody.position.x, (float)testPhysicsBody.position.y, 20, 20};

  /* Window Setup */

  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "Thunder Test Environment");

  SetTargetFPS(60);

  /* Test Linear Velocity */

  testPhysicsBody.applyForce((Vec2){0, 9.8});

  while (!WindowShouldClose()){

    /* Update Physics Environment  */

    env.update();

    /* Update Render Body With Physics Body */

    testRenderBody.x = (float)testPhysicsBody.position.x;
    testRenderBody.y = (float)testPhysicsBody.position.y;

    /* Draw Render Object */

    BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawRectanglePro((Rectangle){(float)testPhysicsBody.position.x, (float)testPhysicsBody.position.y, 20, 20},
                         (Vector2){10, 10}, testPhysicsBody.rotation, BLUE);
    EndDrawing();
  }

  CloseWindow();

  return 0;

}
