#pragma once

#include <glm\glm.hpp>

using namespace glm;

class Camera
{
	public:
		Camera(void);
		~Camera(void);
		void turn(float dx, float dy, float dt);
		vec3 getLocation();
		vec3 getForward();
		mat4 getLookAt();
		vec3 getUp();
		vec3 getRight();
		float getSpeed();
		mat4 getProjectionMatrix();
		mat4 getViewMatrix();
		vec3 position;

	private:
		float yaw; //left/right
		float pitch; //up/down
		float fov;
		float speed;
		float mouseSpeed;
};