#version 330

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec4 eyeCoords;
out vec3 tnorm;

uniform mat4 MVP;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;

void main()
{

	gl_Position = MVP * vec4(aPos, 1.0f);
	
	eyeCoords = ModelViewMatrix * vec4(aPos, 1.0f);
	
	tnorm = normalize(NormalMatrix * aNormal);
}