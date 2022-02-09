#pragma once
#include "Math.h"

class LineSegment
{
public:
	LineSegment(const Vector2& start, const Vector2& end);

	Vector2 mStart, mEnd;
};

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
	Box(const Vector2& min, const Vector2& max);
	
	Vector2 mMin, mMax;
};

bool Intersect(const LineSegment& a, const Box& b);
bool Intersect(const Circle& a, const Circle& b);
bool Intersect(const Circle& a, const Box& b);