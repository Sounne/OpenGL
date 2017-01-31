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

struct Particle
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
	GLShader m_TransformShader;
	GLShader m_ComputeShader;

	struct GPUParticleData
	{
		uint32_t VBO;
	};

	struct GPUParticleLayout
	{
		uint32_t VAO;
	};

	//m_GPUStorage m_GPUStorage;
	uint32_t m_RenderVAO;
	uint32_t m_TransformVAO;

	struct GPUParticuleSystem
	{
		uint32_t VAO;
		uint32_t VBO[2];

	} m_GPUStorage;

	std::vector<Particle> m_particule;
	std::vector<Obstacle> m_obstacle;
	int back_end;
	int m_count;
	int m_CurrentSource;
	float previous_time = 0.f;
	bool g_candraw = false;

	auto Emit(uint32_t _count) -> void;
	auto Destroy() -> void;
	auto Update(float _deltatime) -> void;
	auto Render() -> void;
};

enum PARTICLE_BACK_END
{
	CPU = 0,
	TRANSFORM_FEEDBACK,
	COMPUTE
};

