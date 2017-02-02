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

		glGenBuffers(1, &this->m_GPUStorage.VAO);
		glGenVertexArrays(1, &m_RenderVAO);
		glBindVertexArray(m_RenderVAO);
		glUniformBlockBinding(program, block_index, binding_point);

		if (this->back_end == PARTICLE_BACK_END::CPU)
		{
		/*	glBindVertexArray(this->m_GPUStorage.VAO);
			glBindBuffer(GL_ARRAY_BUFFER, this->m_GPUStorage.VBO);
			glBufferData(GL_ARRAY_BUFFER, _count * stride, nullptr, GL_DYNAMIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr);
			glEnableVertexAttribArray(0);*/

			glGenVertexArrays(1, &m_TransformVAO);
			glBindBuffer(GL_ARRAY_BUFFER, m_GPUStorage.VBO[0]);
			glBufferData(GL_ARRAY_BUFFER, _count * stride, nullptr, GL_DYNAMIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr);
			glEnableVertexAttribArray(0);
		}

		m_CurrentSource = 0;
		glBindBuffer(GL_ARRAY_BUFFER, m_GPUStorage.VBO[0]);
		Vec3 * data = (Vec3*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

		if (this->back_end == PARTICLE_BACK_END::CPU)
		{
			Particle particule;

			/*glGenVertexArrays(1, &m_TransformVAO);
			glBindBuffer(GL_ARRAY_BUFFER, m_GPUStorage.VBO[0]);
			glBufferData(GL_ARRAY_BUFFER, _count * stride, nullptr, GL_DYNAMIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr);
			glEnableVertexAttribArray(0);*/

			float randx = (rand() / (float)RAND_MAX*30.f - 15.f);
			float randy = (rand() / (float)RAND_MAX * 5.f - 2.5f);
			float randz = (rand() / (float)RAND_MAX * 5.f - 2.5f);

			this->m_particule.push_back(particule);
			particule.position = { randx, randy, (float)rand() };
			particule.velocity = { 0.0f, 0.f, 0.f };
			particule.color.w = 1.0f;

			*data++ = { randx, randy, randz };
			*data++ = { 0.f, 0.f, 0.f };
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}
}

auto ParticleSystem::Destroy() -> void
{
	if (back_end != CPU)
	{
		glDeleteBuffers(2, m_GPUStorage.VBO);
		glDeleteVertexArrays(1, &m_TransformVAO);
	}

	else
	{
		glDeleteBuffers(1, m_GPUStorage.VBO);
		glDeleteVertexArrays(1, &m_RenderVAO);
	}

	m_particule.clear();
}

void ParticleTransformVarying(int program)
{
	//	A appeler avant le glLinkProgram();
	const char * varyings[] = { "o_Position", "o_Velocity" };
	glTransformFeedbackVaryings(program, 2, varyings, GL_INTERLEAVED_ATTRIBS);
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
		Particle & particule = this->m_particule[index];
		particule.position += particule.velocity * _deltatime + acceleration_2;
		particule.velocity += acceletation;

		if (particule.position.y < -5.f)
			particule.velocity.y = -2.0f * particule.position.y;
	}

	if (back_end == PARTICLE_BACK_END::TRANSFORM_FEEDBACK)
	{
		m_TransformShader.Bind();
		auto program_id = m_TransformShader.Get();
		glBindVertexArray(m_TransformVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_GPUStorage.VBO[m_CurrentSource]);
		auto destination_vbo = m_GPUStorage.VBO[m_CurrentSource ^ 1];
		glBindBufferBase(GL_TRANSFORM_FEEDBACK, 0, destination_vbo);
		glEnable(GL_RASTERIZER_DISCARD);
		glBeginTransformFeedback(GL_POINTS);
		glDrawArrays(GL_POINTS, 0, m_count);
		glEndTransformFeedback();
		glDisable(GL_RASTERIZER_DISCARD);
	}
}

auto ParticleSystem::Render() -> void
{
	//this->m_particule.Bind();

	if (this->back_end == PARTICLE_BACK_END::CPU)
	{
	/*	glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, this->m_GPUStorage.VBO[0]);
		Vec3 * vertices = (Vec3*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

		for (auto index = 0; index < this->m_particule.size(); ++index)
		{
			vertices[index] = this->m_particule[index].position;
		}

		glUnmapBuffer(GL_ARRAY_BUFFER);

		glBindVertexArray(this->m_GPUStorage.VAO);
		glDrawArrays(GL_POINTS, 0, this->m_count);*/

	}
	
	else
	{
		glBindVertexArray(m_TransformVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_GPUStorage.VBO[m_CurrentSource]);
		m_CurrentSource ^= 1;
	}
}
