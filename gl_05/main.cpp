#include <iostream>
#include <string>

//GLEW
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shprogram.h"
#include "EnumObjectType.h"
#include "MeshController.h"
#include "Object.h"
#include "Camera.h"
#include "objectSet.h"

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

using namespace std;

const GLuint WIDTH = 1000, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;
#define ROTATE_SPEED	16.0f
#define GEAR_TEETH		20

Camera camera(glm::vec3(0.0f, 1.5f, 3.5f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
	if (glfwInit() != GL_TRUE)
	{
		cout << "GLFW initialization failed" << endl;
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	try
	{
		GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "GKOM - Windmill", nullptr, nullptr);
		if (window == nullptr)
			throw exception("GLFW window not created");
		glfwMakeContextCurrent(window);
		glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

		glfwSetKeyCallback(window, KeyCallback);
		glfwSetCursorPosCallback(window, MouseCallback);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
			throw exception("GLEW Initialization failed");

		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		ShaderProgram theShader("shader.vert", "shader.frag");

		Object grass(grass, "grass.jpg", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		Fence fence(1.5f, 0.5f, -0.8f, 0.0f, 2.0f, 0.0f, 0.0f, 90.0f);
		Object mlyn(coneCut, "mlyn.jpg", 10, 0.8f, 6.0f, 0.0f, 0.001f, 0.0f, 0.0f, 0.0f, 0.0f);
		Object dach(cone, "dach.png", 16, 0.7f, 1.0f, 0.0f, 1.799f, 0.0f, 0.0f, 0.0f, 0.0f);
		Object smiglo1(windmill, "smiglo.jpg", 0.0f, 1.601f, 0.81f, 0.0f, 0.0f, 0.0f);
		Object smiglo2(windmill, "smiglo.jpg", 0.0f, 1.601f, 0.8f, 0.0f, 0.0f, 90.0f);
		Object smiglo3(windmill, "smiglo.jpg", 0.0f, 1.601f, 0.81f, 0.0f, 0.0f, 180.0f);
		Object smiglo4(windmill, "smiglo.jpg", 0.0f, 1.601f, 0.8f, 0.0f, 0.0f, 270.0f);
		Object uchwyt(cylinder, "wood1.png", 16, 0.2f, 0.52f, 0.0f, 1.601f, 0.3f, 90.0f, 0.0f, 0.0f);
		Object lezak(cylinder, "lezak.png", 18, 0.43f, 0.16f, 0.0f, 0.002f, -0.1f, 0.0f, 0.0f, 0.0f);
		Object runner(cylinder, "runner.jpg", 18, 0.34f, 0.16f, 0.0f, 0.163f, -0.1f, 0.0f, 0.0f, 0.0f);
		Object belka(cylinder, "wood1.png", 18, 0.08f, 0.875f, 0.0f, 0.326f, -0.1f, 0.0f, 0.0f, 0.0f);
		Object zebatka(gear, "wood1.png", GEAR_TEETH, 0.3f, 0.10f, 0.0f, 1.601f, 0.2f, 90.0f, 0.0f, 0.0f);
		Object zebatka2(gear, "wood1.png", GEAR_TEETH, 0.3f, 0.10f, 0.0f, 1.201f, -0.1f, 0.0f, 360.0f/GEAR_TEETH, 0.0f);
		Tree tree1(0.9f, 1.8f, 2.2f, 0.0f, 0.1f, 0.0f, 0.0f, 0.0f);
		Tree tree2(1.2f, 2.9f, -1.6f, 0.0f, -2.2f, 0.0f, 0.0f, 0.0f);
		Tree tree3(0.6f, 2.2f, -2.7f, 0.0f, -0.8f, 0.0f, 0.0f, 0.0f);
		Tree tree4(1.1f, 2.9f,  2.2f, 0.0f, -1.7f, 0.0f, 0.0f, 0.0f);
		Tree tree5(0.9f, 2.7f,  0.7f, 0.0f, -2.9f, 0.0f, 0.0f, 0.0f);
		Object tlo(backcylinder, "niebo.jpg", 16, 50.0f, 32.0f, 0.0f, -2.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		
		int programId = theShader.get_programID();

		while (!glfwWindowShouldClose(window))
		{
			GLfloat currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			glfwPollEvents();
			DoMovement();
			glClearColor(0.2f, 0.7f, 0.9f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			theShader.Use();

			glUniform3f(glGetUniformLocation(programId, "lightColor"), 1.0f, 1.0f, 1.0f);
			glUniform3f(glGetUniformLocation(programId, "lightPos"), -2.0f, 4.0f, 3.0f);
			
			grass.draw(programId, camera, SCREEN_WIDTH, SCREEN_HEIGHT);
			fence.draw(programId, camera, SCREEN_WIDTH, SCREEN_HEIGHT);
			mlyn.draw(programId, camera, SCREEN_WIDTH, SCREEN_HEIGHT);
			dach.draw(programId, camera, SCREEN_WIDTH, SCREEN_HEIGHT);
			smiglo1.draw(programId, camera, SCREEN_WIDTH, SCREEN_HEIGHT);
			smiglo1.rotate(programId, 1.0f, ROTATE_SPEED, HORIZONTAL);
			smiglo2.draw(programId, camera, SCREEN_WIDTH, SCREEN_HEIGHT);
			smiglo2.rotate(programId, 1.0f, ROTATE_SPEED, HORIZONTAL);
			smiglo3.draw(programId, camera, SCREEN_WIDTH, SCREEN_HEIGHT);
			smiglo3.rotate(programId, 1.0f, ROTATE_SPEED, HORIZONTAL);
			smiglo4.draw(programId, camera, SCREEN_WIDTH, SCREEN_HEIGHT);
			smiglo4.rotate(programId, 1.0f, ROTATE_SPEED, HORIZONTAL);
			uchwyt.draw(programId, camera, SCREEN_WIDTH, SCREEN_HEIGHT);
			uchwyt.rotate(programId, 1.0f, ROTATE_SPEED, VERTICAL);
			lezak.draw(programId, camera, SCREEN_WIDTH, SCREEN_HEIGHT);
			runner.draw(programId, camera, SCREEN_WIDTH, SCREEN_HEIGHT);
			runner.rotate(programId, 1.0f, ROTATE_SPEED, VERTICAL);
			belka.draw(programId, camera, SCREEN_WIDTH, SCREEN_HEIGHT);
			belka.rotate(programId, 1.0f, ROTATE_SPEED, VERTICAL);
			zebatka.draw(programId, camera, SCREEN_WIDTH, SCREEN_HEIGHT);
			zebatka.rotate(programId, 1.0f, ROTATE_SPEED, VERTICAL);
			zebatka2.draw(programId, camera, SCREEN_WIDTH, SCREEN_HEIGHT);
			zebatka2.rotate(programId, 1.0f, ROTATE_SPEED, VERTICAL);
			tree1.draw(programId, camera, SCREEN_WIDTH, SCREEN_HEIGHT);
			tree2.draw(programId, camera, SCREEN_WIDTH, SCREEN_HEIGHT);
			tree3.draw(programId, camera, SCREEN_WIDTH, SCREEN_HEIGHT);
			tree4.draw(programId, camera, SCREEN_WIDTH, SCREEN_HEIGHT);
			tree5.draw(programId, camera, SCREEN_WIDTH, SCREEN_HEIGHT);
			tlo.draw(programId, camera, SCREEN_WIDTH, SCREEN_HEIGHT);
			glfwSwapBuffers(window);
		}
	}
	catch (exception ex)
	{
		cout << ex.what() << endl;
	}
	glfwTerminate();

	return 0;
}

void DoMovement()
{
	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}
	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}
