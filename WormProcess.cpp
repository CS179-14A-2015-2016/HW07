#include "WormProcess.hpp"
#include "Vector.hpp"
#include <random>
#include <ctime>
using namespace std;

mt19937 mt(time(NULL));
uniform_int_distribution<int> dist(1, 250);
Vector comp;

float WormProcess::clamp(float value, float min, float max)
{
	if(value<min) return min;
	if(value>max) return max;
	return value;
}

Vector WormProcess::generateWind()
{
	int xVelo = dist(mt);
	int direction = dist(mt);
	if(direction<=125)
		xVelo *= -1;
	Vector ret(xVelo, 0);
	return ret;
}

Vector WormProcess::initialVelocity(float power, float deg)
{
	static Vector base(power, 0);
	Vector rotate = base.vectorRotate(base, deg);
	Vector ret(rotate.x, -rotate.y);
	return ret;
}

// first is true if it is the first iteration of a verlet sequence; else false
// currPos and prevPos are points, initV is the initial velocity, acc is the acceleration, delta is time difference
Vector WormProcess::verletPhysics(Vector currPos, Vector prevPos, Vector initV, Vector acc, float delta, bool first)
{
	if(first)
	{
		Vector a = initV.scalarMult(initV, delta);
		Vector b = acc.scalarMult(acc, 0.5*delta*delta);
		Vector c = a.vectorAdd(a, b);
		Vector next = c.vectorAdd(c, currPos);
		return next;
	}

	Vector a = acc.scalarMult(acc, delta*delta); // A*d*d
	Vector b = currPos.scalarMult(currPos, 2); // 2Xn
	Vector c = b.vectorSubtract(b, prevPos); // 2Xn-X(n-1)
	Vector next = c.vectorAdd(c, a); // 2Xn-X(n-1) + A*d*d
	return next;
}



bool WormProcess::collissionAABBCircle(Vector circleCenter, float r, Vector rectCenter, float sizeX, float sizeY)
{
	float maxX = rectCenter.x + (sizeX/2.0);
	float minX = rectCenter.x - (sizeX/2.0);
	float minY = rectCenter.y - (sizeY/2.0);
	float maxY = rectCenter.y + (sizeY/2.0);
	float nearX = clamp(circleCenter.x, minX, maxX);
	float nearY = clamp(circleCenter.y, minY, maxY);

	float xDiff = circleCenter.x-nearX;
	float yDiff = circleCenter.y-nearY;
	float distSquared = (xDiff*xDiff) + (yDiff*yDiff);

	return (distSquared < r*r);
}


