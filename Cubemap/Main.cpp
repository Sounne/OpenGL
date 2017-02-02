
// Directive de compilation 'pragma' 
// ici force le linkage avec la lib specifiee en parametre
#if _WIN32
#define FREEGLUT_LIB_PRAGMAS	1
#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION

#pragma comment (lib, "freeglut.lib")
#pragma comment (lib, "glew32.lib")
#endif

#include <iostream>
#include <fstream>

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "dds.h"
#include "Skybox.h"
#include "../common/GLShader.h"
#include "stb/stb_image.h"
#include "glm/glm.hpp"

void Initialize()
{
	glewInit();
}

void Update() 
{
	glutPostRedisplay();
}

void Render() {
}

void Resize(int, int ) {}

auto Look() -> void
{
	glutReshapeFunc(Resize);
	glutIdleFunc(Update);
	glutDisplayFunc(Render);
	glutMainLoop();
}

GLShader shader;
GLuint VAO;

auto Init() -> void
{
	glewInit();
	shader.LoadShader(GL_VERTEX_SHADER, "Skybox.vs");
	shader.LoadShader(GL_VERTEX_SHADER, "Skybox.vs");
	shader.Create();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	int width = 1280, height = 720;
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1280, 720);
	glutCreateWindow("Cubemap");
	glutInitDisplayMode(GL_RGBA | GL_DOUBLE);
	glewInit();

	Skybox skybox = Skybox();
	skybox.cube_map_texture = skybox.LoadCubeMap(1200, 720);

	auto positionLocation = glGetAttribLocation(shader.Get(), "position");
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, false, 8 * sizeof(float), 0);
	glEnableVertexAttribArray(positionLocation);

	glm::mat4 view_matrix = glm::mat4(
		0.f, 0.f, 0.f, 0.f,
		0.f, 0.f, 0.f, 0.f,
		0.f, 0.f, 0.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	);

	auto view_location = glGetUniformLocation(shader.Get(), "view");

	glDepthMask(GL_FALSE);
	// ... Set view and projection matrix
	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.cube_map_texture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);
	// ... Draw rest of the scene

	Look();

	return 1;
}

