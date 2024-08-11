#version 330

layout (location = 0) in vec3 aPos;

out vec2 tex;

void main()
{
	vec2 oPos1 = sign(aPos.xy);
	vec4 oPos = vec4(oPos1.xy,0.0f, 1.0f);
	gl_Position = oPos;

	tex.x=0.5f* (1.0f+oPos.x);
	tex.y=0.5f* (1.0f-oPos.y); 
}