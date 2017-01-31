#include "Vec3.h"

Vec3::Vec3()
{
	this->x = this->values[0];
}

Vec3::Vec3(float _x, float _y, float _z)
{
	this->x = _x;
	this->y = _y;
	this->z = _z;
}

auto Vec3::operator*(float _value) -> Vec3
{
	this->x *= _value;
	this->y *= _value;
	this->z *= _value;

	return *this;
}

auto Vec3::operator+(Vec3 _value) -> Vec3
{
	this->x += _value.x;
	this->y += _value.y;
	this->z += _value.z;

	return *this;
}

auto Vec3::operator+=(Vec3 _value)->Vec3
{
	*this += _value;

	return *this;
}
