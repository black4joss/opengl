#version 330

layout (location = 0) in vec3 aPos;

out float TexDepth;

uniform mat4 MVP;
uniform float ZFar;

void main()
{
	vec4 PositionT = MVP * vec4(aPos,1.0f);

	gl_Position = PositionT;

	TexDepth=PositionT.w/ZFar;
}