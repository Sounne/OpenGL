
// Directive de compilation 'pragma' 
// ici force le linkage avec la lib specifiee en parametre
#if _WIN32
#define FREEGLUT_LIB_PRAGMAS	1
#define _CRT_SECURE_NO_WARNINGS
#pragma comment (lib, "freeglut.lib")
#pragma comment (lib, "glew32.lib")
#endif

#include <iostream>
#include <fstream>

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "SOIL/src/SOIL.h"
#include "dds.h"
#include "Skybox.h"

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

int main(int argc, char* argv[])
{
	glewInit();
	glutInit(&argc, argv);
	int width = 1280, height = 720;
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1280, 720);
	glutCreateWindow("Cubemap");
	glutInitDisplayMode(GL_RGBA | GL_DOUBLE);

	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

	//unsigned char * top = SOIL_load_image("Skybox/top.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	//unsigned char * bot = SOIL_load_image("Skybox/bottom.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	//unsigned char * front = SOIL_load_image("Skybox/front.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	//unsigned char * back = SOIL_load_image("Skybox/back.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	//unsigned char * left = SOIL_load_image("Skybox/left.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	//unsigned char * right = SOIL_load_image("Skybox/right.jpg", &width, &height, 0, SOIL_LOAD_RGBA);

	return 1;
}

