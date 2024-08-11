#version 330

out vec4 FragColor;

in vec4 eyeCoords;
in vec3 tnorm;


uniform vec3 DiffuseLightColor = { 1.0f, 1.0f, 0.5f };
uniform float SpotPower = 75.0f;
uniform vec3 LightPosition = { 0.0f, 0.0f, 20.0f };

 vec3 Get_Spot_Light(vec3 tNormal, vec4 PosW)
{

	vec3 LightDirection = vec3( 0.0f, 0.0f, -1.0f );

	vec3 ToLightPos = LightPosition - PosW.xyz;

	ToLightPos = normalize(ToLightPos);

	LightDirection = normalize(LightDirection);
	
    float SpotFactor = pow(max(dot(-ToLightPos, LightDirection), 0.0f), SpotPower);

	return DiffuseLightColor * SpotFactor;
}


void main()
{

	vec3 ResColor = Get_Spot_Light(tnorm, eyeCoords);

    FragColor = vec4(ResColor, 1.0);
}