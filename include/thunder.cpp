#include "thunder.hpp"

#include <algorithm>
#include <cmath>
#include <iterator>
#include <math.h>
#include <vector>
#include <chrono>
#include <iostream>

#define time_now std::chrono::system_clock::now
#define PI 2*acos(0.0)

#define log(x) std::cout << x << std::endl

double radToDeg(double rad) { return rad / (PI / 180); }
double degToRad(double deg) { return deg * (PI / 180); }

std::vector<unsigned int> range(unsigned int start, unsigned int end){
  std::vector<unsigned int> result;

  for (unsigned int i = start; i < end; i++)
    result.push_back(i);

  return result;
}

Vec2 max(Vec2 z, Vec2 w){
  if (z > w)
    return z;
  return w;
}

Vec2 max(Vec2 z, float w) { return max(z, (Vec2){w, w}); }

Vec2 min(Vec2 z, Vec2 w){
  if (z < w)
    return z;
  return w;
}

Vec2 min(Vec2 z, float w) { return min(z, (Vec2){w, w}); }

Vec2 abs(Vec2 v) { return (Vec2){std::abs(v.x), std::abs(v.y)}; }

float length(Vec2 v) { return std::sqrt(v.x * v.x + v.y * v.y); }

bool shapeOverlap_SAT(PhysicsBodyRec r1, PhysicsBodyRec r2){
  PhysicsBodyRec* p1 = &r1;
  PhysicsBodyRec* p2 = &r2;

  for (auto i : range(0, 1)){
    if (i){
      p1 = &r2;
      p2 = &r1;
    }

    for (int a : range(0, p1->envPoints.size()-1)){
      int b = (a+1)%p1->envPoints.size();

    Vec2 axis = { -(p1->envPoints[b].y - p1->envPoints[a].y), p1->envPoints[b].x - p1->envPoints[a].x };

      float min_r1 = INFINITY, max_r1 = -INFINITY;

      for (Vec2 i : p1->envPoints){
        float dot = i.dotProduct(axis);

        min_r1 = std::min(min_r1, dot);
        max_r1 = std::max(max_r1, dot);
      }

      float min_r2 = INFINITY, max_r2 = -INFINITY;

      for (Vec2 i : p2->envPoints){
        float dot = i.dotProduct(axis);

        min_r2 = std::min(min_r2, dot);
        max_r2 = std::max(max_r2, dot);
      }

      if (!(max_r2 >= min_r1 && max_r1 >= min_r2))
        return false;
    }
  }

  return true;
}

/* PhysicsBodyRec */

PhysicsBodyRec::PhysicsBodyRec(PhysicsEnvironment* env, Vec2 position, Vec2 size, Vec2 velocity,
                               double mass, double rotation, double angularVelocity){

  this->size = size;
  this->position = position;
  this->velocity = velocity;
  this->mass = mass;
  this->rotation = rotation;
  this->angularVelocity = angularVelocity;

  this->points.resize(4);
  this->envPoints.resize(4);

  this->points[0] = (Vec2){-this->size.x/2, this->size.y/2};
  this->points[1] = (Vec2){this->size.x/2, this->size.y/2};
  this->points[2] = (Vec2){this->size.x/2, -this->size.y/2};
  this->points[3] = (Vec2){-this->size.x/2, -this->size.y/2};

  this->gravity = env->gravity;
  this->weight = this->mass * this->gravity;
  this->motionMultiplier = env->motionMultiplier;
  this->linearFriction = (Vec2){env->friction, env->friction};
  this->rotationalFriction = env->friction;


  env->objects.push_back(this);
}

void PhysicsBodyRec::update(){

  if (lastUpdate.isSet){
    // Calculate delta time of object.

    std::chrono::duration<double> dur = time_now() - this->lastUpdate.time;

    // Adjust velocity due to friction.

    if (this->linearFriction.x)
      this->velocity.x += ((this->velocity.x < 0) ?
                           this->linearFriction.x :
                           -this->linearFriction.x) * dur.count();

    if (this->linearFriction.y)
      this->velocity.y += ((this->velocity.y < 0) ?
                           this->linearFriction.y :
                           -this->linearFriction.y) * dur.count();

    if (this->rotationalFriction)
      this->angularVelocity += ((this->angularVelocity < 0) ?
                                this->rotationalFriction :
                                -this->rotationalFriction) * dur.count();

    // Update position based on velocity and time.

    this->position.x += (this->velocity.x*this->motionMultiplier) * dur.count();
    this->position.y += (this->velocity.y + this->gravity) * this->motionMultiplier * dur.count();

    this->rotation += this->angularVelocity * dur.count();
  }

  // Update delta time.

  this->lastUpdate.time = time_now();
  this->lastUpdate.isSet = true;

  // Update position of point relative to centre.

  for (int i = 0; i < 4 && rotation; i++)
    this->points[i].rotate(rotation);

  // Update global position of vertices.

  for (auto i : range(0, 4))
    this->envPoints[i] = this->position + this->points[i];

}

void PhysicsBodyRec::applyEnergy(Vec2 force){
  Vec2 result = {std::sqrt((force.x*2)/this->mass), std::sqrt((force.y*2)/this->mass)};

  this->velocity.x += result.x;
  this->velocity.y += result.y;
}

/* PhysicsEnvironment */

PhysicsEnvironment::PhysicsEnvironment(float gravity, float friction){
  this->gravity = gravity;
  this->friction = friction;
}

void PhysicsEnvironment::update(){
  for (auto i : this->objects)
    i->update();
}

void PhysicsEnvironment::checkCollisions() {
  for (int obj : range(0, this->objects.size())){

    this->objects[obj]->collidingObjects.clear();
    this->objects[obj]->isColliding = false;

    for (int comp : range(1, this->objects.size())){
      int fixedComp = (obj+comp)%this->objects.size();

      if (shapeOverlap_SAT(*this->objects[obj], *this->objects[fixedComp])){
        this->objects[obj]->collidingObjects.push_back(this->objects[fixedComp]);
        this->objects[obj]->isColliding = true;
      }
    }
  }
}
