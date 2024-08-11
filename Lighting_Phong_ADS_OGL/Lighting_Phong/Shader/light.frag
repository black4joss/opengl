#version 330

out vec4 FragColor;

in vec4 eyeCoords;
in vec3 tnorm;

uniform vec4 vPositionLight= {0.0f, 0.0f, -30.0f, 1.0f};
uniform vec3 Light_Diffuse = {1.0f, 1.0f, 0.5f};
uniform vec3 Light_Specular = {1.0f, 1.0f, 1.0f};
uniform vec3 Light_Ambient ={ 0.4f, 0.4f, 0.4f};
uniform float Specular_Shininess = 100;
 
void main()
{

	vec3 s = normalize(vec3(vPositionLight - eyeCoords));
	
	vec3 v = normalize(-eyeCoords.xyz);
	vec3 r = reflect( -s, tnorm );
    
	float sDotN = max( dot(s, tnorm), 0.0f);
	vec3 spec = vec3(0.0);
	if( sDotN > 0.0 )
		   //specular
    	   spec = Light_Specular * pow( max( dot(r,v), 0.0 ), Specular_Shininess );

	//diffuse
	vec3 diffuse = Light_Diffuse * sDotN;

	vec3 Color = Light_Ambient + diffuse + spec;

    FragColor = vec4(Color, 1.0);
}