#ifndef VECTOR_HPP_INCLUDED
#define VECTOR_HPP_INCLUDED
#include<iostream>
#include<math.h>

using namespace std;

class Vector
{
public:
    float x, y;
    Vector(float xComponent, float yComponent);
    Vector();
    void print(Vector &v);
    Vector scalarMult(Vector &v, float i);
    Vector vectorAdd(Vector &a, Vector &b);
    Vector vectorSubtract(Vector &a, Vector &b);
    float vectorMagnitude(Vector &v);
    Vector vectorPerp(Vector &v);
    Vector vectorRotate(Vector &v, float deg);
    Vector vectorNormalize(Vector &v);
    float dotProduct(Vector &a, Vector &b);
    float crossProduct(Vector &a, Vector &b);
    float scalarProjection(Vector &v, Vector &i);
    Vector vectorProjection(Vector &v, Vector &i);
    Vector vectorReflection(Vector &i, Vector &n);
};
#endif // VECTOR_HPP_INCLUDED
