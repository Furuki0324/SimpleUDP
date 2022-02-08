#pragma once
#include "Math.h"

class Circle
{
public:
	Circle(float radius);

	Vector2 mCenter;
	float mRadius;
};

class Box
{
public:
	Box(Vector2 min, Vector2 max);
	
	Vector2 mMin, mMax;
};

bool Intersect(const Circle& a, const Circle& b);
bool Intersect(const Circle& a, const Box& b);