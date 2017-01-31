
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

void Initialize()
{
	glewInit();
}

void Update() {
	glutPostRedisplay();
}


void Render() {
	//glutSwapBuffers();
}

void Resize(int, int ) {}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1280, 720);

	glutCreateWindow("Image");
	glutInitDisplayMode(GL_RGBA | GL_DOUBLE);
	
	Initialize();
	
	glutReshapeFunc(Resize);
	glutIdleFunc(Update);
	glutDisplayFunc(Render);
#if FREEGLUT
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE
			, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
#endif
	glutMainLoop();


	return 1;
}