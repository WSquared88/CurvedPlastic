#include <glew.h>
#include <SOIL.h>

#include "Shape.h"

Shape::Shape(void)
{

}

Shape::Shape(GLfloat vertices[], GLsizei numVert, GLint index, const char* texture)
{
	numVerts = numVert;
	shaderIndex = index;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*numVerts * 8, vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 8, 0);

	GLuint texID = SOIL_load_OGL_texture(texture, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	glBindTexture(GL_TEXTURE_2D, texID);
	glProgramUniform1i(shaderIndex, glGetUniformLocation(shaderIndex, "uniformTex"), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 8, (void*)(sizeof(GL_FLOAT) * 3));
	
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 8, (void*)(sizeof(GL_FLOAT) * 5));

	offset = glGetUniformLocation(shaderIndex, "offset");
	scale = glGetUniformLocation(shaderIndex, "scale");
}

Shape::~Shape(void)
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Shape::Draw(float x, float y, float xScale, float yScale)
{
	glProgramUniform3f(shaderIndex, scale, xScale, yScale, 1);
	glProgramUniform3f(shaderIndex, offset, x, y, 1);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, numVerts);
	//glDrawElements(GL_TRIANGLES, numVerts, GL_UNSIGNED_SHORT, (void*)0);
}