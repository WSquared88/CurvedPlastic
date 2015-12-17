#include <iostream>

#include "Cube.h"
#include "ShaderHelpers.h"

Cube::Cube(void)
{

}

Cube::~Cube()
{

}

Cube::Cube(Shape* s, GLint index, GLushort faces[], int numFaces, vec3 startPos, float startRot, float startSpin, vec3 startScale, vec3 c, vector<vec3> vertices, bool isW)
{
	sqr = s;
	shaderIndex = index;
	currentPos = startPos;
	rotateVec = vec3(0, 1, 0);
	forward = vec3(sin(rotNum), 0, cos(rotNum));
	rotNum = startRot;
	spinSpeed = startSpin;
	scaleVec = startScale;
	color = c;
	isVisible = true;
	isWall = isW;
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

void Cube::print()
{
	cout << "Center: " << OBB.c.x << ", " << OBB.c.y << ", " << OBB.c.z << endl;
	cout << "Halfwidths: " << OBB.halfWidths.x << ", " << OBB.halfWidths.y << ", " << OBB.halfWidths.z << endl;
}

void Cube::Update(float dt)
{
	//std::cout << "delta time " << dt << std::endl;
	rotNum += spinSpeed;


	forward = vec3(sin(rotNum), 0, cos(rotNum));
	//float rotAngle = acos(dot(velocity, forward) / length(velocity) * length(forward));
	velocity += force * dt;
	velocity += velocity * -.1f;
	//velocity = vec3(cos(rotAngle) * velocity.x + sin(rotAngle) * velocity.z, velocity.y, -sin(rotAngle) * velocity.x + cos(rotAngle) * velocity.z);
	currentPos += velocity * dt;
	force = vec3(0, 0, 0);

	//TODO multiply by dat mass
	//vec3 surfaceNormal = CloseToWall();

	/*if (surfaceNormal != vec3(0, 0, 0))
	{
	vec3 vNorm = surfaceNormal * (dot(velocity, surfaceNormal));
	velocity -= vNorm * 2.0f;
	}*/

	//BoundsCheck();

	worldMatrix = translate(currentPos) * rotate(rotNum, rotateVec) * scale(scaleVec);
	//OBB.c = vec3(translate(currentPos) * rotate(spinSpeed, rotateVec) * vec4(OBB.c, 0));
	OBB.axes[0] = normalize(vec3(rotate(spinSpeed, rotateVec) * vec4(OBB.axes[0], 0)));
	OBB.axes[1] = normalize(vec3(rotate(spinSpeed, rotateVec) * vec4(OBB.axes[1], 0)));
	OBB.axes[2] = normalize(vec3(rotate(spinSpeed, rotateVec) * vec4(OBB.axes[2], 0)));
	//SetAABB();
	//SetOBB();
	//updateOBB();
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
	OBB.c = vec3(translate(currentPos) * rotate(rotNum, rotateVec) * vec4((OBB.maxX + OBB.minX) / 2, (OBB.maxY + OBB.minY) / 2, (OBB.maxZ + OBB.minZ) / 2, 0));
	OBB.axes.push_back(normalize(vec3(rotate(rotNum, rotateVec) * vec4(1, 0, 0, 0))));
	OBB.axes.push_back(normalize(vec3(rotate(rotNum, rotateVec) * vec4(0, 1, 0, 0))));
	OBB.axes.push_back(normalize(vec3(rotate(rotNum, rotateVec) * vec4(0, 0, 1, 0))));
	OBB.halfWidths = vec3(scale(scaleVec) * vec4(abs((OBB.maxX - OBB.minX) / 2), abs((OBB.maxY - OBB.minY) / 2), abs((OBB.maxZ - OBB.minZ) / 2), 0));
	std::cout << "Min x " << OBB.minX << endl;
	std::cout << "Max x " << OBB.maxX << endl;
	std::cout << "Min y " << OBB.minY << endl;
	std::cout << "Max y " << OBB.maxY << endl;
	std::cout << "Min z " << OBB.minZ << endl;
	std::cout << "Max z " << OBB.maxZ << endl;
	std::cout << endl;
}

bool Cube::TestOBBOBB(Cube &B)
{
	float ra, rb;
	mat3 R, AbsR;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			R[i][j] = dot(this->OBB.axes[i], B.OBB.axes[j]);

		}
	}

	vec3 t = vec3(this->worldMatrix * vec4(this->OBB.c, 1) - B.worldMatrix * vec4(B.OBB.c, 1));
	t = vec3(dot(t, this->OBB.axes[0]), dot(t, this->OBB.axes[1]), dot(t, this->OBB.axes[2]));

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			AbsR[i][j] = abs(R[i][j]) + FLT_EPSILON;
		}
	}

	for (int i = 0; i < 3; i++)
	{
		ra = this->OBB.halfWidths[i];
		rb = B.OBB.halfWidths[0] * AbsR[i][0] + B.OBB.halfWidths[1] * AbsR[i][1] + B.OBB.halfWidths[2] * AbsR[i][2];
		if (abs(t[i]) > ra + rb)
		{
			return false;
		}
	}

	for (int i = 0; i < 3; i++)
	{
		ra = this->OBB.halfWidths[0] * AbsR[0][i] + this->OBB.halfWidths[1] * AbsR[1][i] + this->OBB.halfWidths[2] * AbsR[2][i];
		rb = B.OBB.halfWidths[i];
		if (abs(t[0] * R[0][i] + t[1] * R[1][i] + t[2] * R[2][i]) > ra + rb)
		{
			return false;
		}
	}

	//Testing Ax X Bx
	ra = this->OBB.halfWidths[1] * AbsR[2][0] + this->OBB.halfWidths[2] * AbsR[1][0];
	rb = B.OBB.halfWidths[1] * AbsR[0][2] + B.OBB.halfWidths[2] * AbsR[0][1];

	if (abs(t[2] * R[1][0] - t[1] * R[2][0]) > ra + rb)
	{
		return false;
	}

	//Testing Ax X By
	ra = this->OBB.halfWidths[1] * AbsR[2][1] + this->OBB.halfWidths[2] * AbsR[1][1];
	rb = B.OBB.halfWidths[0] * AbsR[0][2] + B.OBB.halfWidths[2] * AbsR[0][0];
	if (abs(t[2] * R[1][1] - t[1] * R[2][1]) > ra + rb)
	{
		return false;
	}

	//Testing Ax X Bz
	ra = this->OBB.halfWidths[1] * AbsR[2][2] + this->OBB.halfWidths[2] * AbsR[1][2];
	rb = B.OBB.halfWidths[0] * AbsR[0][1] + B.OBB.halfWidths[1] * AbsR[0][0];
	if (abs(t[2] * R[1][2] - t[1] * R[2][2]) > ra + rb)
	{
		return false;
	}

	//Testing Ay X Bx
	ra = this->OBB.halfWidths[0] * AbsR[2][0] + this->OBB.halfWidths[2] * AbsR[0][0];
	rb = B.OBB.halfWidths[1] * AbsR[1][2] + B.OBB.halfWidths[2] * AbsR[1][1];
	if (abs(t[0] * R[2][0] - t[2] * R[0][0]) > ra + rb)
	{
		return false;
	}

	//Testing Ay X By
	ra = this->OBB.halfWidths[0] * AbsR[2][1] + this->OBB.halfWidths[2] * AbsR[0][1];
	rb = B.OBB.halfWidths[0] * AbsR[1][2] + B.OBB.halfWidths[2] * AbsR[1][0];
	if (abs(t[0] * R[2][1] - t[2] * R[0][1]) > ra + rb)
	{
		return false;
	}

	//Testing Ay X Bz
	ra = this->OBB.halfWidths[0] * AbsR[2][2] + this->OBB.halfWidths[2] * AbsR[0][2];
	rb = B.OBB.halfWidths[0] * AbsR[1][1] + B.OBB.halfWidths[1] * AbsR[1][0];
	if (abs(t[0] * R[2][2] - t[2] * R[0][2]) > ra + rb)
	{
		return false;
	}

	//Testing Az X Bx
	ra = this->OBB.halfWidths[0] * AbsR[1][0] + this->OBB.halfWidths[1] * AbsR[0][0];
	rb = B.OBB.halfWidths[1] * AbsR[2][2] + B.OBB.halfWidths[2] * AbsR[2][1];
	if (abs(t[1] * R[0][0] - t[0] * R[1][0]) > ra + rb)
	{
		return false;
	}

	//Testing Az X By
	ra = this->OBB.halfWidths[0] * AbsR[1][1] + this->OBB.halfWidths[1] * AbsR[0][1];
	rb = B.OBB.halfWidths[0] * AbsR[2][2] + B.OBB.halfWidths[2] * AbsR[2][0];
	if (abs(t[1] * R[0][1] - t[0] * R[1][1]) > ra + rb)
	{
		return false;
	}

	//Testing Az X Bz
	ra = this->OBB.halfWidths[0] * AbsR[1][2] + this->OBB.halfWidths[1] * AbsR[0][2];
	rb = B.OBB.halfWidths[0] * AbsR[2][1] + B.OBB.halfWidths[1] * AbsR[2][0];
	if (abs(t[1] * R[0][2] - t[0] * R[1][2]) > ra + rb)
	{
		return false;
	}

	return true;
}


void Cube::updateOBB()
{
	OBB.c = vec3(translate(currentPos) * rotate(rotNum, rotateVec) * vec4((OBB.maxX + OBB.minX) / 2, (OBB.maxY + OBB.minY) / 2, (OBB.maxZ + OBB.minZ) / 2, 0));
	OBB.axes[0] = normalize(vec3(rotate(rotNum, rotateVec ) * vec4(1, 0, 0, 0)));
	OBB.axes[1] = normalize(vec3(rotate(rotNum, rotateVec ) * vec4(0, 1, 0, 0)));
	OBB.axes[2] = normalize(vec3(rotate(rotNum, rotateVec ) * vec4(0, 0, 1, 0)));
	OBB.halfWidths = vec3(scale(scaleVec) * vec4(abs((OBB.maxX - OBB.minX) / 2), abs((OBB.maxY - OBB.minY) / 2), abs((OBB.maxZ - OBB.minZ) / 2), 0));
	//std::cout << "Center: " << OBB.c.x << ", " << OBB.c.y << ", " << OBB.c.z << endl;
	/*for (int i = 0; i < 8; i++)
	{
		vec3 v = OBB.c 
		//std::cout << "Corner " << i << 
	}*/
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