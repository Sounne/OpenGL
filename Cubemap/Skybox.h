#pragma once

#include <vector>

#include "GL/glew.h"
#include "SOIL/src/SOIL.h"

class Skybox
{
public:
	Skybox();
	~Skybox();

	auto LoadCubeMap(std::vector<const GLchar*> _faces, int _width, int _height) -> void;
};

