#include <glm\gtx\transform.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

#include "Camera.h"

using namespace glm;

Camera::Camera()
{
	position = vec3(0, 0, 1);
	pitch = 0.0f;
	yaw = M_PI;
	fov = 45.0f;
	speed = 0.005f;
	mouseSpeed = 0.00005f;
}

Camera::~Camera()
{

}

void Camera::turn(float dx, float dy, float dt)
{
	yaw += mouseSpeed * dt * dx;
	pitch += mouseSpeed * dt * dy;
	//glfwSetMousePos(1024 / 2, 768 / 2);
}

vec3 Camera::getLocation()
{
	return position;
}

vec3 Camera::getForward()
{
	return vec3(cos(pitch) * sin(yaw), sin(pitch), cos(pitch) * cos(yaw));
}

mat4 Camera::getLookAt()
{
	return lookAt(position, position+getForward(), getUp());
}

vec3 Camera::getUp()
{
	return cross(getRight(), getForward());
}

vec3 Camera::getRight()
{
	return vec3(sin(yaw - (M_PI/2)), 0, cos(yaw - (M_PI/2)));
}

float Camera::getSpeed()
{
	return speed;
}

mat4 Camera::getProjectionMatrix()
{
	return perspective(fov, 4.0f / 3.0f, 0.1f, 100.0f);
}

mat4 Camera::getViewMatrix()
{
	return lookAt(position, position + getForward(), getUp());
}