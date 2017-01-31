struct Vec3
{
	Vec3();
	Vec3(float _x, float _y, float _z);

	float x, y, z;

	auto operator*(float)->Vec3;
	auto operator*=(float)->Vec3;
	auto operator+(Vec3)->Vec3;
	auto operator+=(Vec3)->Vec3;

private:
	float values[3] = { x, y, z };
};

