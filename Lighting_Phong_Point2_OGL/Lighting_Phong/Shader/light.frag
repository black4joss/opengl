#version 330

out vec4 FragColor;

in vec4 eyeCoords;
in vec3 tnorm;

uniform vec3 LightPos = { 0.0f, 9.0f, 0.0f };
uniform vec3 DiffuseLightColor = { 1.0f, 1.0f, 0.5f };
uniform vec3 Att = {0.0f, 0.01f, 0.01f};

float CalcAttenuation(float d, float falloffStart, float falloffEnd)
{
	float res = ((falloffEnd-d) / (falloffEnd - falloffStart));
    
    return clamp(res, 0.0f, 1.0f);
}
 
vec3 Get_Point_Light(vec3 tNormal, vec4 PosW)
{
	float FallOfStart = 12;
	float FallOfEnd = 16;

	vec3 ToLightPos = LightPos - PosW.xyz;

	float Dist = length(ToLightPos);

	if(Dist > FallOfEnd)
		return vec3(0,0,0);

	ToLightPos = normalize(ToLightPos);

	vec3 Normal = normalize(tNormal);
	float Dot = max(dot(ToLightPos, Normal), 0.0f);

	float AttVal = CalcAttenuation(Dist, FallOfStart, FallOfEnd);

	return DiffuseLightColor * AttVal * Dot;
}


void main()
{

	vec3 ResColor = Get_Point_Light(tnorm, eyeCoords);

    FragColor = vec4(ResColor, 1.0);
}