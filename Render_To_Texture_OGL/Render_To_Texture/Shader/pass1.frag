#version 330

out vec4 FragColor;
  
in vec2 tex;

uniform sampler2D ourTexture;
 
void main()
{
    vec4 front = texture(ourTexture, tex);
	
    FragColor = front;
}