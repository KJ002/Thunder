#pragma once
#include <vector>


class PhysicsEnvironment;
class PhysicsBodyRec;

class PhysicsBodyRec{
public:
    PhysicsBodyRec(PhysicsEnvironment*, float, float);
};

class PhysicsEnvironment{
public:
    float gravity;

    std::vector<PhysicsBodyRec> objects;

    PhysicsEnvironment(float);
};
