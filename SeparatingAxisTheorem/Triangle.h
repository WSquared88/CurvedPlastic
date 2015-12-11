#pragma once

#include <glm\glm.hpp>

#include "Shape.h"

using namespace glm;

class Triangle
{
	public:
		Triangle(void);
		Triangle(Shape* s, GLint index, vec3 startPos, float startRot, float startSpin, vec3 startScale, vec3 c);
		~Triangle(void);
		void Update(float dt);
		void Draw();
		void BoundsCheck();
		vec3 CloseToWall();
		Shape* tri;
		mat4 worldMatrix;
		GLint shaderIndex;
		vec3 currentPos;
		float rotNum;
		float spinSpeed;
		vec3 rotateVec;
		vec3 scaleVec;
		vec3 color;
		bool isVisible;
		vec3 velocity;
		vec3 force;
};