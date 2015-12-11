#include <glew.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <iostream>
#include <fstream>

#include "ShaderHelpers.h"

using namespace std;
using namespace glm;

char* loadTextFile(const char* file)
{
	GLchar* shaderCode;
	int length = 0;

	ifstream vertexFile(file, ios::binary);
	if (vertexFile.is_open())
	{
		vertexFile.seekg(0, ios::end);
		length = (int)vertexFile.tellg();
		vertexFile.seekg(0, ios::beg);

		shaderCode = new GLchar[length + 1];
		vertexFile.read(shaderCode, length);
		shaderCode[length] = 0;

		vertexFile.close();

		return shaderCode;
	}

	cout << "Shader loading failed" << endl;
	return 0;
}

GLuint loadShader(const char* file, GLenum shaderType)
{
	const GLchar* data = loadTextFile(file);

	if (!data)
	{
		return 0;
	}

	GLuint shaderIndex = glCreateShader(shaderType);
	glShaderSource(shaderIndex, 1, (const GLchar**)&data, 0);
	glCompileShader(shaderIndex);

	delete[] data;

	//TODO add error checking

	return shaderIndex;
}

GLuint loadShaderProgram(const char* vertexFile, const char* fragmentFile)
{
	GLuint vertexIndex = loadShader(vertexFile, GL_VERTEX_SHADER);

	if (!vertexIndex)
	{
		return 0;
	}

	GLuint fragmentIndex = loadShader(fragmentFile, GL_FRAGMENT_SHADER);

	if (!fragmentIndex)
	{
		return 0;
	}

	GLuint shaderIndex = glCreateProgram();
	glAttachShader(shaderIndex, vertexIndex);
	glAttachShader(shaderIndex, fragmentIndex);
	glLinkProgram(shaderIndex);

	//TODO add error checking

	return shaderIndex;
}

void setShaderColor(GLuint shaderIndex, const char* uniformName, GLfloat red, GLfloat green, GLfloat blue)
{
	GLint uniformIndex = glGetUniformLocation(shaderIndex, uniformName);
	glProgramUniform4f(shaderIndex, uniformIndex, red, green, blue, 1.0f);
}

void setShaderMatrix(GLuint shaderIndex, const char* uniformName, const mat4 &matrix)
{
	GLint uniformIndex = glGetUniformLocation(shaderIndex, uniformName);
	glProgramUniformMatrix4fv(shaderIndex, uniformIndex, 1, GL_FALSE, &matrix[0][0]);
}