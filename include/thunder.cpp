#include "thunder.hpp"

#include <algorithm>
#include <cmath>
#include <iterator>
#include <vector>
#include <chrono>
#include <iostream>

#define time_now std::chrono::system_clock::now

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

float min(float z, float w){
  if (z < w)
    return z;
  return w;
}

Vec2 min(Vec2 z, Vec2 w){
  if (z < w)
    return z;
  return w;
}

Vec2 min(Vec2 z, float w) { return min(z, (Vec2){w, w}); }

Vec2 abs(Vec2 v) { return (Vec2){std::abs(v.x), std::abs(v.y)}; }

float length(Vec2 v) { return std::sqrt(v.x * v.x + v.y * v.y); }

/* PhysicsBodyRec */

PhysicsBodyRec::PhysicsBodyRec(PhysicsEnvironment* env, Vec2 position, Vec2 size, Vec2 velocity,
                               double mass, double rotation, double angularVelocity){

  this->size = size;
  this->position = position;
  this->velocity = velocity;
  this->mass = mass;
  this->rotation = rotation;
  this->angularVelocity = angularVelocity;

  this->points[0] = (Vec2){-size.x/2, size.y/2};
  this->points[1] = (Vec2){size.x/2, size.y/2};
  this->points[2] = (Vec2){size.x/2, -size.y/2};
  this->points[3] = (Vec2){-size.x/2, -size.y/2};

  env->objects.push_back(this);
}

void PhysicsBodyRec::update(){

  if (lastUpdate.isSet){
    std::chrono::duration<double> dur = time_now() - this->lastUpdate.time;

    this->position.x += (this->velocity.x*this->pixelMultiplier) * dur.count();
    this->position.y += (this->velocity.y + this->gravity) * this->pixelMultiplier * dur.count();

    this->rotation += this->angularVelocity * dur.count();
  }

  this->lastUpdate.time = time_now();
  this->lastUpdate.isSet = true;

  for (auto i : range(0, 3))
    this->envPoint[i] = this->position + this->points[i];

}

void PhysicsBodyRec::update(double gravity, double weight, unsigned int pixelMultiplyer){
  this->gravity = gravity;
  this->weight = weight;
  this->pixelMultiplier = pixelMultiplyer;
}

void PhysicsBodyRec::applyEnergy(Vec2 force){
  Vec2 result = {std::sqrt((force.x*2)/this->mass), std::sqrt((force.y*2)/this->mass)};

  this->velocity.x += result.x;
  this->velocity.y += result.y;
}

/* PhysicsEnvironment */

PhysicsEnvironment::PhysicsEnvironment(float gravity){
  this->gravity = gravity;
}

void PhysicsEnvironment::update(){
  for (auto i : this->objects)
    i->update();
}

void PhysicsEnvironment::setup(){
  for (auto i : this->objects){
    double weight = i->mass * this->gravity;

    i->update(this->gravity, weight, this->pixelMultiplier);
  }
}

void PhysicsEnvironment::checkCollisions() {

}
