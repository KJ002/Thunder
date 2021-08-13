#include "raylib.h"
#include "include/thunder.hpp"

#include <iostream>

int main(){

  /* Setup Bodies */

  PhysicsEnvironment env(0, 10);

  PhysicsBodyRec playerPhysicsBody(&env, (Vec2){190, 200}, (Vec2){20, 20}, (Vec2){0, 0}, 10, 0, 100);
  Rectangle playerRenderBody = {(float)playerPhysicsBody.position.x-10,
                              (float)playerPhysicsBody.position.y-10,
                              20.f,
                              20.f};

  PhysicsBodyRec testPhysicsBody(&env, (Vec2){100, 100}, (Vec2){20, 20}, (Vec2){0, 0}, 20, 0, 0);
  Rectangle testRenderBody = {(float)testPhysicsBody.position.x-10,
                              (float)testPhysicsBody.position.y-10,
                              20.f,
                              20.f};

  /* Window Setup */

  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "Thunder Test Environment");

  SetTargetFPS(60);

  /* Test Linear Velocity */

  while (!WindowShouldClose()){

    if (IsKeyDown(KEY_W)) playerPhysicsBody.position.y -= 2;
    if (IsKeyDown(KEY_A)) playerPhysicsBody.position.x -= 2;
    if (IsKeyDown(KEY_S)) playerPhysicsBody.position.y += 2;
    if (IsKeyDown(KEY_D)) playerPhysicsBody.position.x += 2;


    /* Update Physics Environment  */

    env.update();
    env.checkCollisions();

    /* Update Render Body With Physics Body */

    playerRenderBody.x = (float)playerPhysicsBody.position.x;
    playerRenderBody.y = (float)playerPhysicsBody.position.y;

    /* Draw Render Object */

    BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawRectanglePro(playerRenderBody,
                         (Vector2){10, 10},
                         playerPhysicsBody.rotation,
                         BLUE);

        DrawRectangleRec(testRenderBody, RED);
    EndDrawing();
  }

  CloseWindow();

  return 0;

}
