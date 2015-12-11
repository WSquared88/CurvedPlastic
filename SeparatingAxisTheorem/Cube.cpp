#include <iostream>

#include "Cube.h"
#include "ShaderHelpers.h"

Cube::Cube(void)
{

}

Cube::Cube(Shape* s, GLint index, GLushort faces[], int numFaces, vec3 startPos, float startRot, float startSpin, vec3 startScale, vec3 c, vector<vec3> vertices)
{
	sqr = s;
	shaderIndex = index;
	currentPos = startPos;
	rotateVec = vec3(1, 0, 0);
	rotNum = startRot;
	spinSpeed = startSpin;
	scaleVec = startScale;
	color = c;
	isVisible = false;
	verts = vertices;

	worldMatrix = translate(currentPos) * rotate(rotNum, rotateVec) * scale(scaleVec);

	GLuint indexBufferID;
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * numFaces, faces, GL_STATIC_DRAW);

	SetOBB();
	SetAABB();
	SetBoundingSphere();
}

void Cube::Update(float dt)
{
	//std::cout << "delta time " << dt << std::endl;
	rotNum += spinSpeed;
	/*velocity += force * dt;
	velocity += velocity * -.1f;
	currentPos += velocity * dt;
	force = vec3(0, 0, 0);*/

	//TODO multiply by dat ass
	//vec3 surfaceNormal = CloseToWall();

	/*if (surfaceNormal != vec3(0, 0, 0))
	{
	vec3 vNorm = surfaceNormal * (dot(velocity, surfaceNormal));
	velocity -= vNorm * 2.0f;
	}*/

	//BoundsCheck();

	worldMatrix = translate(currentPos) * rotate(rotNum, rotateVec) * scale(scaleVec);
	//SetAABB();
	//SetOBB();
}

void Cube::Draw()
{
	setShaderMatrix(shaderIndex, "worldMatrix", worldMatrix);
	//setShaderColor(shaderIndex, "color", color.x, color.y, color.z);
	sqr->Draw(0, 0, 1, 1);
}

void Cube::SetOBB()
{
	for (int i = 0; i < verts.size(); i++)
	{
		if (verts[i].x > OBB.maxX)
		{
			OBB.maxX = verts[i].x;
		}

		if (verts[i].x < OBB.minX)
		{
			OBB.minX = verts[i].x;
		}

		if (verts[i].y > OBB.maxY)
		{
			OBB.maxY = verts[i].y;
		}

		if (verts[i].y < OBB.minY)
		{
			OBB.minY = verts[i].y;
		}

		if (verts[i].z > OBB.maxZ)
		{
			OBB.maxZ = verts[i].z;
		}

		if (verts[i].z < OBB.minZ)
		{
			OBB.minZ = verts[i].z;
		}
	}

	std::cout << "Min x " << OBB.minX << endl;
	std::cout << "Max x " << OBB.maxX << endl;
	std::cout << "Min y " << OBB.minY << endl;
	std::cout << "Max y " << OBB.maxY << endl;
	std::cout << "Min z " << OBB.minZ << endl;
	std::cout << "Max z " << OBB.maxZ << endl;
	std::cout << endl;
}

void Cube::SetAABB()
{
	vec4 tempVert;
	Box tempAABB;

	for (int i = 0; i < verts.size(); i++)
	{
		tempVert = vec4(verts[i], 0);
		vec4 mutatedVert = worldMatrix * tempVert;

		if (mutatedVert.x > tempAABB.maxX)
		{
			tempAABB.maxX = mutatedVert.x;
		}

		if (mutatedVert.x < tempAABB.minX)
		{
			tempAABB.minX = mutatedVert.x;
		}

		if (mutatedVert.y > tempAABB.maxY)
		{
			tempAABB.maxY = mutatedVert.y;
		}

		if (mutatedVert.y < tempAABB.minY)
		{
			tempAABB.minY = mutatedVert.y;
		}

		if (mutatedVert.z > tempAABB.maxZ)
		{
			tempAABB.maxZ = mutatedVert.z;
		}

		if (mutatedVert.z < tempAABB.minZ)
		{
			tempAABB.minZ = mutatedVert.z;
		}
	}

	std::cout << "AABB" << endl;
	std::cout << "Min x " << tempAABB.minX << endl;
	std::cout << "Max x " << tempAABB.maxX << endl;
	std::cout << "Min y " << tempAABB.minY << endl;
	std::cout << "Max y " << tempAABB.maxY << endl;
	std::cout << "Min z " << tempAABB.minZ << endl;
	std::cout << "Max z " << tempAABB.maxZ << endl;
	std::cout << endl;
}

void Cube::SetBoundingSphere()
{
	vec3 randVert = verts[0];// verts[rand() % verts.size()];
	float dist = FLT_MIN;
	int firstVert;
	int secondVert;
	float radius;
	vec3 center;
	bool AllNotInSphere = true;

	for (int i = 0; i < verts.size(); i++)
	{
		float curDist = length(randVert-verts[i]);
		if (randVert != verts[i] && curDist > dist)
		{
			dist = curDist;
			firstVert = i;
		}
	}

	dist = FLT_MIN;

	for (int i = 0; i < verts.size(); i++)
	{
		float curDist = length(verts[firstVert] - verts[i]);
		if (verts[firstVert] != verts[i] && curDist > dist)
		{
			dist = curDist;
			secondVert = i;
		}
	}

	center = (verts[firstVert] - verts[secondVert]);
	center /= 2;
	radius = length(center);
	center += verts[secondVert];

	while (AllNotInSphere)
	{
		AllNotInSphere = false;
		for (int i = 0; i < verts.size(); i++)
		{
			if (length(center - verts[i]) > radius)
			{
				AllNotInSphere = true;
				vec3 diam = verts[i] - center;
				diam /= 2;
				radius = length(diam);
				center += diam;
			}
		}
	}

	cout << "Center x " << center.x << endl;
	cout << "Center y " << center.y << endl;
	cout << "Center z " << center.z << endl;
	cout << "Radius " << radius << endl;
	cout << endl;
}