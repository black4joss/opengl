#version 330

out vec4 FragColor;
  
in vec2 tex;

uniform sampler2D ourTexture_Front;
uniform sampler2D ourTexture_Back;

uniform vec3 FogColor={0.5f,0.5f,0.5f}; 

uniform float FogFactor = 12.0f;
 
void main()
{

	float front = texture(ourTexture_Front, tex).r;
	float back = texture(ourTexture_Back, tex).r;

	float k = (back-front) * FogFactor;

	FragColor = vec4(FogColor*k,1.0f);
    
}