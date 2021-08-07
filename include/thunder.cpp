#include "thunder.hpp"

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

/* PhysicsBodyRec */

PhysicsBodyRec::PhysicsBodyRec(PhysicsEnvironment* env, Vec2 dimentions, Vec2 position, Vec2 velocity,
                               double mass, double rotation, double angularVelocity){

  this->dimentions = dimentions;
  this->position = position;
  this->velocity = velocity;
  this->mass = mass;
  this->rotation = rotation;
  this->angularVelocity = angularVelocity;

  // Calculate Vertices

  this->vertices[0] = (Vec2){position.x-(dimentions.x/2), position.y-(dimentions.y/2)};
  this->vertices[1] = (Vec2){position.x+(dimentions.x/2), position.y-(dimentions.y/2)};
  this->vertices[2] = (Vec2){position.x+(dimentions.x/2), position.y+(dimentions.y/2)};
  this->vertices[3] = (Vec2){position.x-(dimentions.x/2), position.y+(dimentions.y/2)};

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

  // Calculate Vertices

  this->vertices[0] = (Vec2){position.x-(dimentions.x/2), position.y-(dimentions.y/2)};
  this->vertices[1] = (Vec2){position.x+(dimentions.x/2), position.y-(dimentions.y/2)};
  this->vertices[2] = (Vec2){position.x+(dimentions.x/2), position.y+(dimentions.y/2)};
  this->vertices[3] = (Vec2){position.x-(dimentions.x/2), position.y+(dimentions.y/2)};

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

void PhysicsEnvironment::checkCollisions(){
  for (int selectedObj = 0; selectedObj < this->objects.size(); selectedObj++){
    this->objects[selectedObj]->isColliding = false;
    std::vector<PhysicsBodyRec*> collidingObjects;

    for (int selectedVert = 0; selectedVert < 4; selectedVert++){

      int intersections = 0;
      Vec2 selected = this->objects[selectedObj]->vertices[selectedVert];

      for (int comparisonObj = 0; comparisonObj < this->objects.size()-1; comparisonObj++){

        bool possibleCollisionIsSet = false;

        int correctComparisonObj;

        (comparisonObj == selectedObj)
          ? correctComparisonObj = (comparisonObj+1)%this->objects.size()
          : correctComparisonObj = comparisonObj;

        for (int comparisonVert = 0; comparisonVert < 4; comparisonVert++){

          Vec2 comparison[2] = {this->objects[correctComparisonObj]->vertices[comparisonVert],
          this->objects[correctComparisonObj]->vertices[(comparisonVert+1)%4]};

          if (((selected.x > comparison[0].x && selected.x < comparison[1].x) ||
               (selected.x < comparison[0].x && selected.x > comparison[1].x) &&
              (selected.y < comparison[0].y && selected.y < comparison[1].y))
          ){
            intersections++;

            if (!possibleCollisionIsSet){
              possibleCollisionIsSet = true;
              collidingObjects.push_back(this->objects[correctComparisonObj]);
            }

          }
        }
      }

      if (intersections%2){
        this->objects[selectedObj]->isColliding = true;
      }

      std::cout << intersections << std::endl;
    }
  }
}
