#pragma once

#include <chrono>
#include <cstddef>
#include <vector>

class PhysicsEnvironment;
class PhysicsBodyRec;

struct Vec2{
  double x;
  double y;
};

struct Vec3 : public Vec2{
  double z;
};

struct time_point{
  std::chrono::time_point<std::chrono::system_clock> time;
  bool isSet = false;
};

class PhysicsBodyRec{
public:
  time_point lastUpdate;
  std::vector<PhysicsBodyRec*> collidingObjects;
  bool isColliding;

  /* Member Variables Passed On Construction */

  Vec2 dimentions;
  Vec2 position;
  Vec2 velocity;
  double mass;
  double rotation;
  double angularVelocity;
  Vec2 vertices[4];

  /* Member Variabes Passed By Environment */

  double gravity;
  double weight;
  unsigned int pixelMultiplier;

  PhysicsBodyRec(PhysicsEnvironment*, Vec2, Vec2, Vec2, double, double, double);
  void update();
  void update(double, double, unsigned int);
  void applyEnergy(Vec2);
};

class PhysicsEnvironment{
public:
  float gravity;
  unsigned int pixelMultiplier = 10;

  std::vector<PhysicsBodyRec*> objects;

  PhysicsEnvironment(float);
  void setup();
  void update();
  void checkCollisions();
};
