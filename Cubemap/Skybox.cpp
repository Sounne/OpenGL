#include "Skybox.h"

#include "stb/stb_image.h"
#include "Vertices.h"

Skybox::Skybox()
{
}

Skybox::~Skybox()
{
}

auto Skybox::Init() -> void
{
	this->faces.push_back("../Skybox/back.jpg");
	this->faces.push_back("../Skybox/front.jpg");
	this->faces.push_back("../Skybox/bottom.jpg");
	this->faces.push_back("../Skybox/top.jpg");
	this->faces.push_back("../Skybox/left.jpg");
	this->faces.push_back("../Skybox/right.jpg");
	this->cube_map_texture = LoadCubeMap(1200, 720);
}

auto Skybox::LoadCubeMap(int _width, int _height) -> GLuint
{
	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
	unsigned char * image;

	for (GLuint i = 0; i < this->faces.size(); ++i)
	{
		image = stbi_load(this->faces[i], &_width, &_height, 0, GL_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return texture_id;
}

