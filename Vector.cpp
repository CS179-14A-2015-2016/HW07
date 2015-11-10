#include<iostream>
#include<math.h>
#include "Vector.hpp"
using namespace std;



//constructor
Vector::Vector(float xComponent, float yComponent)
{
    x = xComponent;
    y = yComponent;
}
Vector::Vector()
{
    x = 0;
    y = 0;
}

void print(Vector &v)
{
	cout << "<" << v.x << ", " << v.y << ">" << endl;
}

Vector Vector::scalarMult(Vector &v, float i)
{
	Vector ret(v.x*i, v.y*i);
	return ret;
}

Vector Vector::vectorAdd(Vector &a, Vector &b)
{
	Vector ret(a.x+b.x, a.y+b.y);
	return ret;
}

Vector Vector::vectorSubtract(Vector &a, Vector &b)
{
	Vector ret(a.x-b.x, a.y-b.y);
	return ret;
}

float Vector::vectorMagnitude(Vector &v)
{
	double temp = (v.x*v.x) + (v.y*v.y);
	return sqrt(temp);
}

Vector Vector::vectorPerp(Vector &v)
{
	Vector ret(-v.y, v.x);
	return ret;
}

Vector Vector::vectorRotate(Vector &v, float deg)
{
	static float pi = 3.14159f;
	float rad = (deg/180.0) * pi;
	float newX = (v.x*cos(rad))-(v.y*sin(rad));
	float newY = (v.y*cos(rad))+(v.x*sin(rad));
	Vector ret(newX, newY);
	return ret;
}

Vector Vector::vectorNormalize(Vector &v)
{
	float mag = 1.f/vectorMagnitude(v);
	return scalarMult(v, mag);
}

float Vector::dotProduct(Vector &a, Vector &b)
{
	float ret = (a.x*b.x) + (a.y*b.y);
	return ret;
}

float Vector::crossProduct(Vector &a, Vector &b)
{
	float ret = a.x*b.y - a.y*b.x;
	return ret;
}

float Vector::scalarProjection(Vector &v, Vector &i)
{
	Vector iHat = vectorNormalize(i);
	return dotProduct(v, iHat);
}

Vector Vector::vectorProjection(Vector &v, Vector &i)
{
	Vector iHat = vectorNormalize(i);
	float mag = scalarProjection(v, i);
	return scalarMult(iHat, mag);
}

Vector Vector::vectorReflection(Vector &i, Vector &n)
{
	Vector bigN = vectorProjection(i, n);
	Vector twiceBigN = scalarMult(bigN, 2);
	return vectorSubtract(i, twiceBigN);
}

int main2()
{
	Vector i(12, 13);
	Vector n(17, -2);
	Vector d;
	print(i);
	d = d.vectorProjection(i, n);
	print(d);
	Vector c;
	c.vectorSubtract(i, d);
	print(c);
	float temp = c.dotProduct(n, c);
	cout << temp << endl;
	return 0;
}
