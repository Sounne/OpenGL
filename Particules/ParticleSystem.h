#pragma once

#include <iostream>
#include <vector>

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "Vec3.h"
#include "Vec4.h"
#include "GLShader.h"

struct Obstacle
{
	Vec3 position;
	float radius;
};

struct Particule
{
	Vec3 position;
	Vec3 velocity;
	Vec4 color;
};

struct ParticleSystem
{
	Vec3 position;
	Vec3 velocity;
	Vec3 color;
	GLShader m_particuleshader;

	struct GPUParticuleSystem
	{
		uint32_t VAO;
		uint32_t VBO;

	} m_GPUStorage;
	
	std::vector<Particule> m_particule;
	std::vector<Obstacle> m_obstacle;
	int back_end;
	int m_count;
	float previous_time = 0.f;
	bool g_candraw = false;

	auto Emit(uint32_t _count) -> void;
	auto Destroy() -> void;
	auto Update(float _deltatime) -> void;
	auto Render() -> void;
};

enum PARTICULE_BACK_END
{
	CPU = 0,
	TRANSFORM_FEEDBACK,
	COMPUTE
};

