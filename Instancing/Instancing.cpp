#include "GL/glew.h"

#include "GLFW/glfw3.h"

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

GLuint screenWidth = 800, screenHeight = 600;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();

Camera camera(glm::vec3(0.0f, 0.0f, 155.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;
	glewInit();

	glViewport(0, 0, screenWidth, screenHeight);

	glEnable(GL_DEPTH_TEST);

	Shader planetShader("shaders/advanced.vs", "shaders/advanced.frag");
	Shader instanceShader("shaders/instancing.vs", "shaders/instancing.frag");

	Model rock("objects/rock.obj");
	Model planet("objects/planet.obj");

	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)screenWidth / (GLfloat)screenHeight, 1.0f, 10000.0f);
	planetShader.Use();
	glUniformMatrix4fv(glGetUniformLocation(planetShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	instanceShader.Use();
	glUniformMatrix4fv(glGetUniformLocation(instanceShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	GLuint amount = 100000;
	glm::mat4* modelMatrices;
	modelMatrices = new glm::mat4[amount];
	srand(glfwGetTime());
	GLfloat radius = 150.0f;
	GLfloat offset = 25.0f;
	for (GLuint i = 0; i < amount; i++)
	{
		glm::mat4 model;
		GLfloat angle = (GLfloat)i / (GLfloat)amount * 360.0f;
		GLfloat displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat x = sin(angle) * radius + displacement;
		displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat y = -2.5f + displacement * 0.4f; 
		displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		GLfloat scale = (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));

		GLfloat rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		modelMatrices[i] = model;
	}

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

	for (GLuint i = 0; i < rock.meshes.size(); i++)
	{
		GLuint VAO = rock.meshes[i].VAO;
		glBindVertexArray(VAO);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}

	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();
		Do_Movement();

		glClearColor(0.03f, 0.03f, 0.03f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		planetShader.Use();
		glUniformMatrix4fv(glGetUniformLocation(planetShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
		instanceShader.Use();
		glUniformMatrix4fv(glGetUniformLocation(instanceShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));

		planetShader.Use();
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -5.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(glGetUniformLocation(planetShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		planet.Draw(planetShader);

		instanceShader.Use();
		glBindTexture(GL_TEXTURE_2D, rock.textures_loaded[0].id);
		for (GLuint i = 0; i < rock.meshes.size(); i++)
		{
			glBindVertexArray(rock.meshes[i].VAO);
			glDrawElementsInstanced(GL_TRIANGLES, rock.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
			glBindVertexArray(0);
		}

		glfwSwapBuffers(window);
	}

	delete[] modelMatrices;

	glfwTerminate();
	return 0;
}

#pragma region "User input"

void Do_Movement()
{
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

#pragma endregion