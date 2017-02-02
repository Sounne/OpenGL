#pragma once

#include <vector>

#include "GL/glew.h"

class Skybox
{
public:
	Skybox();
	~Skybox();

	auto Init() -> void;
	auto LoadCubeMap(int _width, int _height) ->GLuint;

	std::vector<const GLchar*> faces;
	GLuint cube_map_texture;
};

