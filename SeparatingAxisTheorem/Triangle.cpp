#include <iostream>

#include "Triangle.h"
#include "ShaderHelpers.h"

Triangle::Triangle(void)
{

}

Triangle::Triangle(Shape* s, GLint index, vec3 startPos, float startRot, float startSpin, vec3 startScale, vec3 c)
{
	tri = s;
	shaderIndex = index;
	currentPos = startPos;
	rotateVec = vec3(0, 0, 1);
	rotNum = startRot;
	spinSpeed = startSpin;
	scaleVec = startScale;
	color = c;
	isVisible = false;
}

void Triangle::Update(float dt)
{
	std::cout << "delta time " << dt << std::endl;
	rotNum += spinSpeed;
	velocity += force * dt;
	velocity += velocity * -.1f;
	currentPos += velocity * dt;
	force = vec3(0, -.00001, 0);

	//TODO multiply by dat ass
	vec3 surfaceNormal = CloseToWall();

	if (surfaceNormal != vec3(0, 0, 0))
	{
		vec3 vNorm = surfaceNormal * (dot(velocity, surfaceNormal));
		velocity -= vNorm * 2.0f;
	}

	//BoundsCheck();

	worldMatrix = translate(currentPos) * rotate(rotNum, rotateVec) * scale(scaleVec);
}

void Triangle::Draw()
{
	setShaderMatrix(shaderIndex, "worldMatrix", worldMatrix);
	setShaderColor(shaderIndex, "color", color.x, color.y, color.z);
	tri->Draw(0, 0, 1, 1);
}

void Triangle::BoundsCheck()
{
	if (currentPos.x > 1)
	{
		currentPos.x = 1;
	}
	else if (currentPos.x < -1)
	{
		currentPos.x = -1;
	}

	if (currentPos.y > 1)
	{
		currentPos.y = 1;
	}
	else if (currentPos.y < -1)
	{
		currentPos.y = -1;
	}
}

vec3 Triangle::CloseToWall()
{
	if (currentPos.x > 1)
	{
		return vec3(-1, 0, 0);
	}
	else if (currentPos.x < -1)
	{
		return vec3(1, 0, 0);
	}

	if (currentPos.y > 1)
	{
		return vec3(0, -1, 0);
	}
	else if (currentPos.y < -1)
	{
		return vec3(0, 1, 0);
	}
	return vec3(0, 0, 0);
}