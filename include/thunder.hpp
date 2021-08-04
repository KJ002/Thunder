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

  Vec2 position;
  Vec2 velocity;
  double mass;
  double rotation;
  double angularVelocity;

  PhysicsBodyRec(PhysicsEnvironment*, Vec2, Vec2, double, double, double);
  void update();
  void applyForce(Vec2);
};

class PhysicsEnvironment{
public:
  float gravity;

  std::vector<PhysicsBodyRec*> objects;

  PhysicsEnvironment(float);
  void update();
};
