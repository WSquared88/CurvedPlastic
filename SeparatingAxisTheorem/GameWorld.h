#pragma once

#include <glfw3.h>

#include <glm\glm.hpp>

using namespace glm;

vec3 getCursorPos(GLFWwindow* windowPtr);
void mouseMove(GLFWwindow* windowPtr, double x, double y);
void mouseClick(GLFWwindow* windowPtr, int button, int action, int mods);
void keyPress(GLFWwindow* windowPtr, int key, int scancode, int action, int mods);