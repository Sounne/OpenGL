
// Directive de compilation 'pragma' 
// ici force le linkage avec la lib specifiee en parametre
#if _WIN32
#define FREEGLUT_LIB_PRAGMAS	1
#pragma comment (lib, "freeglut.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "opengl32.lib")
#endif

#include <iostream>

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "../common/GLShader.h"

GLShader BasicProgram;
GLuint VBO;
GLuint IBO;

void Initialize()
{
	glewInit();

	const float g_Triangle[] = {
		-0.8f, 0.8f,		// position 0
		1.0f, 0.0, 0.0f,	// couleur 0
		0.0f, -0.8f,		// position 1
		0.0f, 1.0, 0.0f,	// couleur 1
		0.8f, 0.8f,			// position 2
		0.0f, 0.0, 1.0f,	// couleur 2
		1.0f, -1.0f,		// position 3
		1.0f, 1.0, 1.0f		// couleur 3
	};

	glGenBuffers(1, &VBO);
	// notez la relation en _ARRAY_ et DrawArrays
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER
		, 4*5*sizeof(float), g_Triangle
		, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	uint16_t indices[] = { 0, 1, 2, 1, 3, 2 };
	glGenBuffers(1, &IBO);
	// notez la relation en _ELEMENT_ARRAY_ et DrawElements
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER
		, 2 * 3 * sizeof(uint16_t), indices
		, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	/*std::cout << "Version des pilotes: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Fabricant: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Carte graphique: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "extensions supportees: " << std::endl;
	//std::cout << "Version des pilotes" << glGetString(GL_EXTENSIONS) << std::endl;
	int numExt;
	glGetIntegerv(GL_NUM_EXTENSIONS, &numExt);
	for (auto index = 0; index < numExt; ++index) {
		std::cout << "extension[" << index << "] = " << glGetStringi(GL_EXTENSIONS, index) << std::endl;
	}*/

	// extension .v, .vs, .vert, *.glsl ou autre
	BasicProgram.LoadShader(GL_VERTEX_SHADER, "basic.vs");
	// extension .f, .fs, .frag, *.glsl ou autre
	BasicProgram.LoadShader(GL_FRAGMENT_SHADER, "basic.fs");
	BasicProgram.Create();
	
}

void Shutdown()
{
	//BasicProgram.Destroy();
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);
}

void Update() {
	glutPostRedisplay();
}

void Render() {

	glClearColor(0.f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT); // | GL_DEPTH_BUFFER_BIT
	
	//glColorMask(false, true, true, true);

	BasicProgram.Bind();
	auto programID = BasicProgram.Get();
	// sans inline la ligne precedente donnerait ceci:
	// temp = CALL MEMBER FUNCTION GET of GLShader
	// programID = temp
	// avec inline : programID = BasicProgram._Program 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	auto positionLocation = glGetAttribLocation(programID, "a_Position");
	glVertexAttribPointer(positionLocation, 2, GL_FLOAT, false,
				5 * sizeof(float), 0);// g_Triangle);
	glEnableVertexAttribArray(positionLocation);

	auto colorLocation = glGetAttribLocation(programID, "a_Color");
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, false,
		5 * sizeof(float), (void*)(2 * sizeof(float)));// &g_Triangle[2]);		// alternativement g_Triangle + 2
	glEnableVertexAttribArray(colorLocation);
	//glVertexAttrib3f(colorLocation, 0.5f, 1.0f, 0.5f);
	
	float timeInSeconds = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	auto timeLocation = glGetUniformLocation(programID, "u_Time");
	glUniform1f(timeLocation, timeInSeconds);

	// Rendu en Array
	//glDrawArrays(GL_TRIANGLES, 0, 3);

	// Rendu par indices
	unsigned short indices[] = { 0, 1, 2 };
	// par mesure de protection on "unbind" les IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, indices);
	
	// idem mais avec un Index Buffer (IBO)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	// Seulement sur PC, on doit activer cet etat
	// afin de pouvoir utiliser gl_PointSize
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, nullptr);

	glDisableVertexAttribArray(colorLocation);
	glDisableVertexAttribArray(positionLocation);

	glutSwapBuffers();	
}

void Resize(int, int ) {}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitWindowSize(1280, 720);
	glutInitWindowPosition(100, 100);
	// defini les buffers du framebuffer 
	// GLUT_RGBA :  color buffer 32 bits
	// GLUT_DOUBLE : double buffering
	// GLUT_DEPTH : ajoute un buffer pour le depth test
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

	glutCreateWindow("basic Shader GL");
	
	Initialize();
	
	glutReshapeFunc(Resize);
	glutIdleFunc(Update);
	glutDisplayFunc(Render);
#if FREEGLUT
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE
			, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
#endif
	glutMainLoop();

	Shutdown();

	return 1;
}