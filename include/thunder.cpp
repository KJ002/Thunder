#include "thunder.hpp"

#include <cmath>
#include <vector>
#include <chrono>

#define time_now std::chrono::system_clock::now

/* PhysicsBodyRec */

PhysicsBodyRec::PhysicsBodyRec(PhysicsEnvironment* env, Vec2 position, Vec2 velocity,
                               double mass, double rotation, double angularVelocity){
  this->position = position;
  this->velocity = velocity;
  this->mass = mass;
  this->rotation = rotation;
  this->angularVelocity = angularVelocity;

  env->objects.push_back(this);
}

void PhysicsBodyRec::update(){
  if (lastUpdate.isSet){
    std::chrono::duration<double> dur = time_now() - this->lastUpdate.time;

    this->position.x += this->velocity.x * dur.count();
    this->position.y += this->velocity.y * dur.count();

    this->rotation += this->angularVelocity * dur.count();
  }

  this->lastUpdate.time = time_now();
  this->lastUpdate.isSet = true;
}

void PhysicsBodyRec::update(double gravity, double weight){
  this->gravity = gravity;
  this->weight = weight;
}

void PhysicsBodyRec::applyForce(Vec2 force){
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

    i->update(this->gravity, weight);
  }
}
