#pragma once

#include <glew.h>

class Shape
{
	public:
		Shape(void);
		Shape(GLfloat vertices[], GLsizei numVert, GLint index, const char* texture);
		~Shape(void);
		GLuint VBO;
		GLuint VAO;
		GLsizei numVerts;
		GLuint shaderIndex;
		GLint offset;
		GLint scale;
		void Draw(float x, float y, float xScale, float yScale);
};