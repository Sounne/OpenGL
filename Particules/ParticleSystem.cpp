#include "ParticleSystem.h"

auto ParticleSystem::Emit(uint32_t _count) -> void
{
	this->m_count = _count;

	if (this->m_GPUStorage.VAO = 0)
	{
		GLsizei stride = sizeof(Vec3);

		this->m_particuleshader.LoadShader(GL_VERTEX_SHADER, "Particule.vs");
		this->m_particuleshader.LoadShader(GL_FRAGMENT_SHADER, "Particule.fs");
		this->m_particuleshader.Create();

		auto program = this->m_particuleshader.Get();
		auto block_index = 0, binding_point = 0;

		glUniformBlockBinding(program, block_index, binding_point);
		glGenBuffers(1, &this->m_GPUStorage.VAO);

		if (this->back_end == PARTICULE_BACK_END::CPU)
		{
			glBindVertexArray(this->m_GPUStorage.VAO);
			glBindBuffer(GL_ARRAY_BUFFER, this->m_GPUStorage.VBO);
			glBufferData(GL_ARRAY_BUFFER, _count * stride, nullptr, GL_DYNAMIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr);
			glEnableVertexAttribArray(0);
		}

		if (this->back_end == PARTICULE_BACK_END::CPU)
		{
			Particule particule;

			float randx = (rand() / (float)RAND_MAX*30.f - 15.f);
			float randy = (rand() / (float)RAND_MAX * 5.f - 2.5f);
			float randz = (rand() / (float)RAND_MAX * 5.f - 2.5f);

			this->m_particule.push_back(particule);
			particule.position = { randx, randy, (float)rand() };
			particule.velocity = { 0.0f, 0.f, 0.f };
			particule.color.w = 1.0f;
		}
	}
}

auto ParticleSystem::Destroy() -> void
{
}

auto ParticleSystem::Update(float _deltatime) -> void
{
	auto current_time = glutGet(GLUT_ELAPSED_TIME);
	auto delta = current_time - this->previous_time;
	this->previous_time = current_time;
	auto elapsed_time = delta / 1000.f;

	glutPostRedisplay();
	this->g_candraw = true;

	Vec3 acceletation = { 0.9f, -9.8f, 0.f };
	Vec3 acceleration_2 = (acceletation * _deltatime) * 0.5f;

	for (auto index = 0; index < this->m_particule.size(); ++index)
	{
		Particule & particule = this->m_particule[index];
		particule.position += particule.velocity * _deltatime + acceleration_2;
		particule.velocity += acceletation;

		if (particule.position.y < -5.f)
			particule.velocity.y = -2.0f * particule.position.y;
	}
}

auto ParticleSystem::Render() -> void
{
	//this->m_particule.Bind();

	if (this->back_end == PARTICULE_BACK_END::CPU)
	{
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, this->m_GPUStorage.VBO);
		Vec3 * vertices = (Vec3*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

		for (auto index = 0; index < this->m_particule.size(); ++index)
		{
			vertices[index] = this->m_particule[index].position;
		}

		glUnmapBuffer(GL_ARRAY_BUFFER);

		glBindVertexArray(this->m_GPUStorage.VAO);
		glDrawArrays(GL_POINTS, 0, this->m_count);
	}
}
