#version 330

layout (location = 0) in vec3 aPos;

out vec2 tex;

void main()
{
	vec2 oPos1 = sign(aPos.xy);
	vec4 oPos = vec4(oPos1.xy,0, 1);
	gl_Position = oPos;

	tex.x=0.5* (1+oPos.x);
	tex.y=0.5* (1+oPos.y); 
}