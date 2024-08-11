#version 330

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTextCoord;
 

out vec2 TextCoord;

uniform mat4 MVP;

void main()
{

vec2 VertPos = aPos - vec2(400.0f, 300.0f);

VertPos.y = -1.0f * VertPos.y;

VertPos = VertPos / vec2(400.0f, 300.0f);

gl_Position = vec4(VertPos.x, VertPos.y, 0.0f ,1.0);

TextCoord = aTextCoord;


}