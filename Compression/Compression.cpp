
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
#include "dds.h"

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

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1280, 720);

	glutCreateWindow("Compression Image");
	glutInitDisplayMode(GL_RGBA | GL_DOUBLE);
	
	Initialize();

	GLuint texture_id;
	glGenTextures(1, &texture_id);

	glBindTexture(GL_TEXTURE_2D, texture_id);

	uint8_t  * output;
	uint32_t width = 512;
	uint32_t height = 512;

	uint32_t image_size = LoadImageDDS(&output, width, height, "mario.dds");
	//glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB_S3TC_DXT1_EXT, width, height, 0, image_size, &output);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//LoadDDS("mario.dds");

	//FreeImageDDS(&output);

	glutReshapeFunc(Resize);
	glutIdleFunc(Update);
	glutDisplayFunc(Render);
	glutMainLoop();

	return 1;
}

