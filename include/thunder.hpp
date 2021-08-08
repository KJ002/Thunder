#pragma once

#include <chrono>
#include <cmath>
#include <cstddef>
#include <vector>

struct Vec2{
  double x;
  double y;

  Vec2 operator+(const Vec2& other) const{
    return (Vec2){x+other.x, y+other.y};
  }

  Vec2 operator-(const Vec2& other) const{
    return (Vec2){x-other.x, y-other.y};
  }

  Vec2 operator*(const Vec2& other) const{
    return (Vec2){x*other.x, y*other.y};
  }

  Vec2 operator/(const Vec2& other) const{
    return (Vec2){x/other.x, y/other.y};
  }

  bool operator>(const Vec2& other) const{
    if ((x*y) > (other.x*other.y))
      return true;
    return false;
  }

  bool operator<(const Vec2& other) const{
    return !operator>(other);
  }
};

std::vector<unsigned int> range(unsigned int, unsigned int);

Vec2 max(Vec2, float);
Vec2 max(Vec2, Vec2);
float min(float, float);
Vec2 min(Vec2, float);
Vec2 min(Vec2, Vec2);

Vec2 abs(Vec2);

float length(Vec2 v);

float signedDstToBox(Vec2, Vec2, Vec2);

class PhysicsEnvironment;
class PhysicsBodyRec;

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

  //Vec2 dimentions;
  Vec2 position;
  Vec2 velocity;
  double mass;
  double rotation;
  double angularVelocity;

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
