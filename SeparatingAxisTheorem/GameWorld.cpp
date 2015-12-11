#include <iostream>

#include "GameWorld.h"
#include "Camera.h"

using namespace std;
using namespace glm;

bool mouseButtonHeld;
bool isWireFrame;
Camera* camera;
extern float dt;

vec3 getCursorPos(GLFWwindow* windowPtr)
{
	double xpos;
	double ypos;
	int width;
	int height;
	vec3 cursorPosition;

	glfwGetCursorPos(windowPtr, &xpos, &ypos);
	glfwGetWindowSize(windowPtr, &width, &height);

	cursorPosition.x = 2.0f * ((float)xpos / (float)width) - 1.0f;
	cursorPosition.y = (2.0f * ((float)ypos / (float)height) - 1.0f) * -1.0f;
	cursorPosition.z = 1;

	return cursorPosition;
}

void mouseMove(GLFWwindow* windowPtr, double x, double y)
{
	vec3 cursorPosition = getCursorPos(windowPtr);
	glfwSetCursorPos(windowPtr, 800 / 2, 800 / 2);
	camera->turn((float)((800/2)-x), (float)((800/2)-y), dt);
}

void mouseClick(GLFWwindow* windowPtr, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		mouseButtonHeld = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		mouseButtonHeld = false;
	}
}

void keyPress(GLFWwindow* windowPtr, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_UP || key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		camera->position += camera->getForward() * dt * camera->getSpeed();
	}
	if (key == GLFW_KEY_DOWN || key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		camera->position -= camera->getForward() * dt * camera->getSpeed();
	}
	if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		camera->position += camera->getRight() * dt * camera->getSpeed();
	}
	if (key == GLFW_KEY_LEFT || key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		camera->position -= camera->getRight() * dt * camera->getSpeed();
	}
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		isWireFrame = !isWireFrame;
	}
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		exit(EXIT_SUCCESS);
	}
}