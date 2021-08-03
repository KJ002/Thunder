#pragma once

#include <chrono>
#include <cstddef>
#include <vector>

class PhysicsEnvironment;
class PhysicsBodyRec;

struct Vec2{
  float x;
  float y;
};

struct time_point{
  std::chrono::time_point<std::chrono::system_clock> time;
  bool isSet = false;
};

class PhysicsBodyRec{
public:
  time_point lastUpdate;

  Vec2 position;
  Vec2 velocity;

  PhysicsBodyRec(PhysicsEnvironment*, Vec2, Vec2);
  void update();
};

class PhysicsEnvironment{
public:
  float gravity;

  std::vector<PhysicsBodyRec*> objects;

  PhysicsEnvironment(float);
  void update();
};
