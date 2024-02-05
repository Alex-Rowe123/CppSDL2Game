#pragma once
#include <math.h>
class Vector2D
{
public:
	float x, y;
	Vector2D();
	Vector2D(float x, float y);
	Vector2D operator+(Vector2D const& _val);
	Vector2D operator-(Vector2D const& _val);
	Vector2D operator*(Vector2D const& _val);
	Vector2D operator*(float const& _val);
	Vector2D operator/(float const& _val);
	float GetMagnitude();
	Vector2D GetUnit();
};

