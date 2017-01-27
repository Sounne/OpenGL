
// Directive de compilation 'pragma' 
// ici force le linkage avec la lib specifiee en parametre
#if _WIN32
#define FREEGLUT_LIB_PRAGMAS	1
#pragma comment (lib, "freeglut.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "isartlib.lib")
#endif

#include <iostream>
#include <cassert>

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "../common/GLShader.h"

GLShader BlitProgram;
// Framebuffer Object et render target (texture)
GLuint FBO;
GLuint TexFBO;
GLuint DepthFBO;

bool CreateFramebuffer(int width, int height)
{
	// le renderbuffer se distingue de la texture car en ecriture seule
	glGenRenderbuffers(1, &DepthFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, DepthFBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

	// creation de la texture qui va servir de color buffer pour le rendu hors ecran
	glGenTextures(1, &TexFBO);
	glBindTexture(GL_TEXTURE_2D, TexFBO);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// creation du framebuffer pour le rendu hors ecran
	glGenFramebuffers(1, &FBO);
	// GL_FRAMEBUFFER = read/write, GL_READ_FRAMEBUFFER = read only, GL_DRAW_FRAMEBUFFER = write only
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	// attache la texture en tant que color buffer #0
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TexFBO, 0);
	// attache le render buffer en tant que depth buffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, DepthFBO);
	// Switch vers le framebuffer par defaut (celui cree par GLUT ou le contexte de rendu)
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	// necessite #include <cassert>
	assert(status == GL_FRAMEBUFFER_COMPLETE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	return true;
}

void DestroyFramebuffer()
{
	glDeleteRenderbuffers(1, &DepthFBO);
	glDeleteFramebuffers(1, &FBO);
	glDeleteTextures(1, &TexFBO);
}

GLShader BasicProgram;
GLuint VBO;
GLuint IBO;
GLuint VAO;
GLuint TexDragon;

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

// format X,Y,Z, NX, NY, NZ, U, V = 8 floats par vertex
#include "DragonData.h"

int numVertices = _countof(DragonVertices);
int numIndices = _countof(DragonIndices);

void Initialize()
{
	glewInit();

	glGenTextures(1, &TexDragon);
	glBindTexture(GL_TEXTURE_2D, TexDragon);
	int w, h, c;
	uint8_t* imageData = stbi_load("dragon.png", &w, &h, &c, STBI_rgb_alpha);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	stbi_image_free(imageData);
	// par default le mode de min filter est GL_NEAREST_MIPMAP_LINEAR
	// ce qui necessite de definir tous les niveaux de details (LOD)
	// on peut forcer le filtrage a etre en nearest ou linear pour desactiver le mip mapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// ...ou bien generer les mipmap
	//glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	// extension .v, .vs, .vert, *.glsl ou autre
	BasicProgram.LoadShader(GL_VERTEX_SHADER, "basicFBO.vs");
	// extension .f, .fs, .frag, *.glsl ou autre
	BasicProgram.LoadShader(GL_FRAGMENT_SHADER, "basicFBO.fs");
	BasicProgram.Create();

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);			// <- ATTENTION, un VAO enregistre les parametres suivants:
									// glBindBuffer(GL_(ELEMENT)_ARRAY_BUFFER
									// glEnable/DisableVertexAttribArray()
									// glVertexAttribPointer()

	glGenBuffers(1, &VBO);
	// notez la relation entre _ARRAY_ et DrawArrays
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER
		, numVertices * sizeof(float), DragonVertices
		, GL_STATIC_DRAW);
	
	glGenBuffers(1, &IBO);
	// notez la relation entre _ELEMENT_ARRAY_ et DrawElements
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER
		, numIndices * sizeof(uint16_t), DragonIndices
		, GL_STATIC_DRAW);
	

	
	auto programID = BasicProgram.Get();
	// VAO et VBO deja bind plus
	auto positionLocation = glGetAttribLocation(programID, "a_Position");
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, false, 8 * sizeof(float), 0);
	glEnableVertexAttribArray(positionLocation);
	auto normalLocation = glGetAttribLocation(programID, "a_Normal");
	glVertexAttribPointer(normalLocation, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(normalLocation);
	auto texcoordsLocation = glGetAttribLocation(programID, "a_TexCoords");
	glVertexAttribPointer(texcoordsLocation, 2, GL_FLOAT, false, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(texcoordsLocation);

	// toujours reinitialiser le VAO par defaut (0) avant de bind un autre buffer
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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

	CreateFramebuffer(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

void Shutdown()
{
	DestroyFramebuffer();

	BasicProgram.Destroy();
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);
}

void Update() {
	glutPostRedisplay();
}

void Render() {
	glEnable(GL_DEPTH_TEST);

	// redirige TOUTES les commandes de rendu vers le FBO donc hors ecran
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClearColor(1.f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	BasicProgram.Bind();
	// sans inline la ligne precedente donnerait ceci:
	// temp = CALL MEMBER FUNCTION GET of GLShader
	// programID = temp
	// avec inline : programID = BasicProgram._Program 
	auto programID = BasicProgram.Get();
	

	float timeInSeconds = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	auto timeLocation = glGetUniformLocation(programID, "u_Time");
	static float previousTime = 0.0f;
	float deltaTime = previousTime - timeInSeconds;
	previousTime = timeInSeconds;
	static float rotationSpeed;
	static float speed = 36.0f;
	rotationSpeed += deltaTime * speed;
	rotationSpeed = fmod(rotationSpeed, 360.0f);
	glUniform1f(timeLocation, timeInSeconds);

	// defini sur quelle unite de texture (texture unit) on va "bind" la texture
	uint32_t texUnit = 0;
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, TexDragon);
	auto textureLocation = glGetUniformLocation(programID, "u_Texture");
	glUniform1i(textureLocation, texUnit);

	glBindVertexArray(VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//auto positionLocation = glGetAttribLocation(programID, "a_Position");
	//glVertexAttribPointer(positionLocation, 3, GL_FLOAT, false, 8 * sizeof(float), 0);
	//glEnableVertexAttribArray(positionLocation);
	// idem mais avec un Index Buffer (IBO)
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, nullptr);
	//glDisableVertexAttribArray(positionLocation);
	glBindVertexArray(0);

	// zero: bind du framebuffer par defaut soit le framebuffer de GLUT ici (backbuffer etc...)
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// todo: configurer les framebuffers d'entree et sortie pour que le blit 
	// sache quel fbo lire et vers quel fbo ecrire
	int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
	int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, windowWidth, windowHeight, 0, 0, windowWidth/2, windowHeight/2
						, GL_COLOR_BUFFER_BIT, GL_NEAREST);
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
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

	glutCreateWindow("basic FBO");
	
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