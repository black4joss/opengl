#version 330

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTextCoord;

out float Fog_Val;
out vec2 TextCoord;

uniform mat4 MVP;
uniform vec3 gCamPos;

vec3 ComponentProd(vec3 v1, vec3 v2)
{
	return vec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

float Check_Sphere(vec3 vertexPos, vec3 cameraPos)
{
	float fSphereRadius = 4128.0f;

	vec3 vSphereCenter = vec3(46433.0f, 6376.0f, 48650.0f);

	vec3 m_Scale = vec3(1.0f / fSphereRadius, 1.0f / fSphereRadius, 1.0f / fSphereRadius);

	vec3 adjCameraPos = ComponentProd((cameraPos - vSphereCenter), m_Scale);
	vec3 adjVertexPos = ComponentProd((vertexPos - vSphereCenter), m_Scale);

	vec3 adjDistance = adjVertexPos - adjCameraPos;

	float OD = dot(adjDistance, adjCameraPos);
	float D2 = dot(adjDistance, adjDistance);
	float O2 = dot(adjCameraPos, adjCameraPos);


	float radix = OD*OD - D2*(O2 - 1);
	
	if (radix <= 0)
	{
		return 0.0f;
	}

	float sradix = float(sqrt(radix));

	float t1 = (-OD - sradix) / D2;
	float t2 = (-OD + sradix) / D2;

	vec3 v1; 
	vec3 v2; 


	if(t1 >= 0.0f && t1 < 1.0f && t2 > 0.0f && t2 <= 1.0f)
	{
		v1 = cameraPos + t1*(vertexPos - cameraPos);
		v2 = cameraPos + t2*(vertexPos - cameraPos);
	
		vec3 vDist = v2 - v1;

		float val = length(vDist);

		return val;
	}


	if(t1 >= 0.0f && t1 < 1.0f && t2 > 1.0f)
	{

		t2 = 1.0f;

		v1 = cameraPos + t1*(vertexPos - cameraPos);
		v2 = cameraPos + t2*(vertexPos - cameraPos);
	
		vec3 vDist = v2 - v1;

		float val = length(vDist);

		return val;
	}

	if(t1 < 0.0f && t2 > 0.0f && t2 <= 1.0f)
	{
		t1 = 0.0f;
		v1 = cameraPos + t1*(vertexPos - cameraPos);
		v2 = cameraPos + t2*(vertexPos - cameraPos);
	
		vec3 vDist = v2 - v1;

		float val = length(vDist);

		return val;
	}


	if(t1 < 0.0f && t2 > 1.0f)
	{
		vec3 vDist = cameraPos - vertexPos;
		float val = length(vDist);
		return val;
	}


	return 0.0f;
}


void main()
{

	Fog_Val = Check_Sphere(aPos, gCamPos);

	gl_Position = MVP * vec4(aPos,1.0);

	TextCoord = aTextCoord;


}