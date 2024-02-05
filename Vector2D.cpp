#include "Vector2D.h"

Vector2D::Vector2D()
{
	x = 0;
	y = 0;
}

Vector2D::Vector2D(float _x, float _y)
{
	x = _x;
	y = _y;
}

// add the x and y of both vectors together
Vector2D Vector2D::operator+(Vector2D const& _val)
{
	return Vector2D(x + _val.x, y + _val.y);
}

// subtract the input vector x and y from the x and y
Vector2D Vector2D::operator-(Vector2D const& _val)
{
	return Vector2D(x - _val.x, y - _val .y);
}

// multiply the x and y by the input vector x and y
Vector2D Vector2D::operator*(Vector2D const& _val)
{
	return Vector2D(x * _val.x, y * _val.y);
}

// multiply the x and y by the float value
Vector2D Vector2D::operator*(float const& _val)
{
	return Vector2D(x * _val, y * _val);
}

// divide the x and y by the float value
Vector2D Vector2D::operator/(float const& _val)
{
	return Vector2D(x / _val, y / _val);
}

// get the magnitude (length) of the vector
float Vector2D::GetMagnitude()
{
	return fabs(sqrt((x * x) + (y * y)));
}

// get the unit vector. multiplied by 100 for more accuracy
Vector2D Vector2D::GetUnit()
{
	return Vector2D(((x*100) / GetMagnitude()/100), ((y*100) / GetMagnitude())/100);
}

