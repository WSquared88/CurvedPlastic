#pragma once

#include <glm\glm.hpp>
#include <vector>

#include "Shape.h"

using namespace glm;
using namespace std;

class Cube
{
	struct Box
	{
		float minX = FLT_MAX;
		float maxX = FLT_MIN;
		float minY = FLT_MAX;
		float maxY = FLT_MIN;
		float minZ = FLT_MAX;
		float maxZ = FLT_MIN;
	};

	struct Sphere
	{
		float radius;
		float center;
	};

public:
	Cube(void);
	Cube(Shape* s, GLint index, GLushort faces[], int numFaces, vec3 startPos, float startRot, float startSpin, vec3 startScale, vec3 c, vector<vec3> vertices);
	~Cube(void);
	void Update(float dt);
	void Draw();
	void SetOBB();
	void SetAABB();
	void SetBoundingSphere();
	Shape* sqr;
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
	vector<vec3> verts;
	Box OBB;
	Box AABB;
	Sphere boundingSphere;
};