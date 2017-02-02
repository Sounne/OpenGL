
#if _WIN32
#define FREEGLUT_LIB_PRAGMAS	1
#pragma comment (lib, "freeglut.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "isartlib.lib")
#endif

#include <iostream>
#include <cassert>
#include <vector>

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "../common/GLShader.h"
#include "Skybox.h"

GLShader BlitProgram;
GLuint FBO;
GLuint TexFBO;
GLuint DepthFBO;


class Test
{
public:
	Test() = default;
	Test(int a) {
		tab.reserve(a);
	};
	Test(Test& ref) {
		tab = ref.tab;		
	}
	~Test() {}
	Test(Test&& ref) : tab(std::move(ref.tab)) 
	{
	}
	void operator=(Test& ref) {
		tab = ref.tab;
	}
	void operator=(Test&& ref) {
		tab = ref.tab;// std::move(ref.tab);
	}
	
	std::vector<int> tab;
};

Test makeTemp(int size)
{
	Test temp(6);
	return temp;
}


bool CreateFramebuffer(int width, int height)
{
	Test* a;
	Test b;

	a = new Test(8);		
	b = *a;
	b = makeTemp(6);

	Test c = *a;
	Test d(*a);
	
	std::vector<int> vA;
	glGenRenderbuffers(1, &DepthFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, DepthFBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glGenTextures(1, &TexFBO);
	glBindTexture(GL_TEXTURE_2D, TexFBO);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TexFBO, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, DepthFBO);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	int* pa = (int*)0x1234;
	int* pb = std::move(pa);

	BasicProgram.LoadShader(GL_VERTEX_SHADER, "Cours.vs");
	BasicProgram.LoadShader(GL_FRAGMENT_SHADER, "Cours.fs");
	BasicProgram.Create();
	BasicProgram.ptr = (int*)0x1234;
	BlitProgram = std::move(BasicProgram);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER
		, numVertices * sizeof(float), DragonVertices
		, GL_STATIC_DRAW);
	
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER
		, numIndices * sizeof(uint16_t), DragonIndices
		, GL_STATIC_DRAW);
	

	
	auto programID = BasicProgram.Get();
	auto positionLocation = glGetAttribLocation(programID, "a_Position");
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, false, 8 * sizeof(float), 0);
	glEnableVertexAttribArray(positionLocation);
	auto normalLocation = glGetAttribLocation(programID, "a_Normal");
	glVertexAttribPointer(normalLocation, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(normalLocation);
	auto texcoordsLocation = glGetAttribLocation(programID, "a_TexCoords");
	glVertexAttribPointer(texcoordsLocation, 2, GL_FLOAT, false, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(texcoordsLocation);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	Skybox skybox;
	skybox.Init();
	skybox.cube_map_texture = skybox.LoadCubeMap(1200, 720);

	CreateFramebuffer(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	
	BlitProgram.LoadShader(GL_VERTEX_SHADER, "blit.vs");
	BlitProgram.LoadShader(GL_FRAGMENT_SHADER, "blit.fs");
	BlitProgram.Create();

	
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

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClearColor(1.f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	BasicProgram.Bind();
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

	uint32_t texUnit = 0;
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, TexDragon);
	auto textureLocation = glGetUniformLocation(programID, "u_Texture");
	glUniform1i(textureLocation, texUnit);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, nullptr);
	glBindVertexArray(0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	BlitProgram.Bind();
	timeLocation = glGetUniformLocation(BlitProgram.Get(), "u_Time");
	glUniform1f(timeLocation, timeInSeconds);

	glBindTexture(GL_TEXTURE_2D, TexFBO);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	auto positionLocation = glGetAttribLocation(BlitProgram.Get(), "a_Position");
	static const float quad[] = {
		-1.0f, 1.0f,
		-1.0f, -1.0f,
		1.0f, 1.0f,
		1.0f, -1.0f
	};
	glVertexAttribPointer(positionLocation, 2, GL_FLOAT, false, 2 * sizeof(float), quad);
	glEnableVertexAttribArray(positionLocation);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glutSwapBuffers();	
}

void Resize(int, int ) {}

//int main(int argc, char* argv[])
//{
//	glutInit(&argc, argv);
//	glutInitWindowSize(1280, 720);
//	glutInitWindowPosition(100, 100);
//	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
//
//	glutCreateWindow("basic Post Process");
//	
//	Initialize();
//	
//	glutReshapeFunc(Resize);
//	glutIdleFunc(Update);
//	glutDisplayFunc(Render);
//#if FREEGLUT
//	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE
//			, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
//#endif
//	glutMainLoop();
//
//	Shutdown();
//
//	return 1;
//}