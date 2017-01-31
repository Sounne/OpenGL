
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

void Initialize()
{
	glewInit();
}

void Update() 
{
	glutPostRedisplay();
}

void Render() 
{
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
	//glewInit();
	//shader.LoadShader(GL_VERTEX_SHADER, "Skybox.vs");
	//shader.LoadShader(GL_VERTEX_SHADER, "Skybox.vs");
	//shader.Create();
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
	skybox.Init();

	auto positionLocation = glGetAttribLocation(shader.Get(), "Position");
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, false, 8 * sizeof(float), 0);
	glEnableVertexAttribArray(positionLocation);

	Look();
	glutMainLoop();

	return 1;
}

