#ifndef WORMPROCESS_HPP_INCLUDED
#define WORMPROCESS_HPP_INCLUDED
#include "Vector.hpp"
#include <random>
#include <ctime>

class WormProcess
{
    public:
        float clamp(float value, float min, float max);
        Vector generateWind();
        Vector initialVelocity(float power, float deg);
        Vector verletPhysics(Vector currPos, Vector prevPos, Vector initV, Vector acc, float delta, bool first);
        Vector verlet2(Vector velocity, Vector accel);
        bool collissionAABBCircle(Vector circleCenter, float r, Vector rectCenter, float sizeX, float sizeY);
};

#endif // WORMPROCESS_HPP_INCLUDED
