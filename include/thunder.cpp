#include "thunder.hpp"
#include <vector>

PhysicsBodyRec::PhysicsBodyRec(PhysicsEnvironment* env, float x, float y){
    env->objects.push_back(*this);
}

PhysicsEnvironment::PhysicsEnvironment(float gravity){
    this->gravity = gravity;
}
