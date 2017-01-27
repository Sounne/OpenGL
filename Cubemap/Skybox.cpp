#include "Skybox.h"

#include "SOIL/src/SOIL.h"

Skybox::Skybox()
{
}

Skybox::~Skybox()
{
}

auto Skybox::LoadCubeMap(std::vector<const GLchar*> _faces, int _width, int _height) -> void
{
	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
	unsigned char * image;

	for (GLuint i = 0; i < _faces.size(); ++i)
	{
		image = SOIL_load_image(_faces[i], &_width, &_height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	}
}

