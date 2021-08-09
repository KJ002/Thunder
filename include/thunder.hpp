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

  double dotProduct(const Vec2& other) const{
    return x*other.x+y*other.y;
  }

  double crossProduct(const Vec2& other) const{
    return x+other.x*y+other.y;
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

class PhysicsEnvironment;
class PhysicsBodyRec;

struct time_point{
  std::chrono::time_point<std::chrono::system_clock> time;
  bool isSet = false;
};

bool shapeOverlap_SAT(PhysicsBodyRec &r1, PhysicsBodyRec &r2);

class PhysicsBodyRec{
public:

  /* Member Variables Updated Due to Env */

  time_point lastUpdate;
  std::vector<PhysicsBodyRec*> collidingObjects;
  bool isColliding;

  /* Member Variables Passed On Construction */

  Vec2 position;                 // Initial Position of Rectangle
  Vec2 size;                     // Size of Rectangle
  Vec2 velocity;                 // Initial Velocity of Rectangle
  double mass;                   // Mass of Rectangle
  double rotation;               // Initial Rotation of Rectangle
  double angularVelocity;        // Initial Angular Velocity of Rectangle

  /* Member Variabes Passed By Environment */

  double gravity;                 // Gravity applied to Rectangle
  double weight;                  // Weight of Rectangle
  unsigned int pixelMultiplier;   // Multiply velocity by this Multiplier

  /* Member Variables Due to Construction Members */

  std::vector<Vec2> points;       // Points of Rectangle Relative to Center
  std::vector<Vec2> envPoints;     // Points of Rectangle Relative to Env

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
