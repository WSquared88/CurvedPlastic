#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 vecTexUV;
layout(location = 2) in vec3 vecNorm;
uniform vec3 offset;
uniform vec3 scale;
uniform mat4 worldMatrix;
uniform mat4 cameraMatrix;
out vec2 fragTexUV;
out vec3 fragPos;
out vec3 fragNorm;

void main()
{
	gl_Position = cameraMatrix * worldMatrix * vec4(position * scale + offset, 1);
	fragTexUV = vecTexUV;
	fragPos = gl_Position.xyz;
	fragNorm = normalize(vecNorm);
}