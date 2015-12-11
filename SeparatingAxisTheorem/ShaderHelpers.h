#pragma once

#include <glew.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>

using namespace glm;

char* loadTextFile(const char* file);

GLuint loadShader(const char* file, GLenum shaderType);

GLuint loadShaderProgram(const char* vertexFile, const char* fragmentFile);

void setShaderColor(GLuint shaderIndex, const char* uniformName, GLfloat red, GLfloat green, GLfloat blue);

void setShaderMatrix(GLuint shaderIndex, const char* uniformName, const mat4 &matrix);