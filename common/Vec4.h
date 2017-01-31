#pragma once
struct Vec4
{
	Vec4();
	Vec4(float _x, float _y, float _z, float _w);
	~Vec4();

	float x, y, z, w;

private:
	float value[4] = { x, y, z, w };
};

