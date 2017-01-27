
// Directive de compilation 'pragma' 
// ici force le linkage avec la lib specifiee en parametre
#if _WIN32
#define FREEGLUT_LIB_PRAGMAS	1
#pragma comment (lib, "freeglut.lib")
#pragma comment (lib, "glew32.lib")
#endif

#include <iostream>

#include "GL/glew.h"
#include "GL/freeglut.h"

//#include "GL/glext.h"
//PFNGLGETSTRINGIPROC glGetStringi = nullptr;

void Initialize()
{
	glewInit();
	// wglGetProcAddress() version specialisee de getProcAddress() 
	// qui recupere un pointeur de fonction dans la dll du driver OpenGL
//	glGetStringi = (PFNGLGETSTRINGIPROC)wglGetProcAddress("glGetStringi");

	std::cout << "Version des pilotes: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Fabricant: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Carte graphique: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "extensions supportees: " << std::endl;
	//std::cout << "Version des pilotes" << glGetString(GL_EXTENSIONS) << std::endl;
	int numExt;
	glGetIntegerv(GL_NUM_EXTENSIONS, &numExt);
	for (auto index = 0; index < numExt; ++index) {
		std::cout << "extension[" << index << "] = " << glGetStringi(GL_EXTENSIONS, index) << std::endl;
	}
}

void Update() {
	glutPostRedisplay();
}


void Render() {
	/*
	glViewport(0, 0, 640, 480);
	glScissor(0, 0, 640, 480);
	glEnable(GL_SCISSOR_TEST);

	glClearColor(rand()/(float)RAND_MAX, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT); // | GL_DEPTH_BUFFER_BIT
	
	glColorMask(false, true, true, true);
	
	static const float g_Triangle[] = {
		-0.8f, 0.8f,		// position 0
		1.0f, 0.0, 0.0f,	// couleur 0
		0.0f, -0.8f,		// position 1
		0.0f, 1.0, 0.0f,	// couleur 1
		0.8f, 0.8f,			// position 2
		0.0f, 0.0, 1.0f		// couleur 2
	};*/

	/*glVertexAttribPointer(0, 2, GL_FLOAT, false,
						5 * sizeof(float), g_Triangle);
	glEnableVertexAttribArray(0);
	//glVertexAttribPointer(3, 3, GL_FLOAT, false,
	//	5 * sizeof(float), &g_Triangle[2]);		// g_Triangle + 2
	//glEnableVertexAttribArray(3);
	glVertexAttrib3f(3, 0.5f, 1.0f, 0.5f);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	//glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(0);*/

	glutSwapBuffers();
}

void Resize(int, int ) {}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1280, 720);
	// defini les buffers du framebuffer 
	// GL_RGBA :  color buffer 32 bits
	// GL_DOUBLE : double buffering
	// GL_DEPTH : ajoute un buffer pour le depth test

	glutCreateWindow("Image");
	glutInitDisplayMode(GL_RGBA | GL_DOUBLE);
	
	Initialize();
	/*
	glutReshapeFunc(Resize);
	glutIdleFunc(Update);
	glutDisplayFunc(Render);
#if FREEGLUT
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE
			, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
#endif*/
	glutMainLoop();

	//Terminate();

	return 1;
}