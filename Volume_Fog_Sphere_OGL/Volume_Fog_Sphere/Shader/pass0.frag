#version 330

out vec4 FragColor;
  
in vec2 TextCoord;

in float Fog_Val;

uniform sampler2D OurTexture;
 
void main()
{

	vec4 ResColor = texture(OurTexture, TextCoord);

	float FogVal = Fog_Val / 12000.0f;

	float InvFogVal = 1.0f - FogVal;

	vec4 SrcAlpha = vec4(0, 223.0f / 255.0f * FogVal, 191.0f / 255.0f * FogVal, 1.0f);

	vec4 InvSrcAlpha = vec4(ResColor.x * InvFogVal,
			ResColor.y * InvFogVal, ResColor.z * InvFogVal, 1.0f);

	ResColor = SrcAlpha + InvSrcAlpha;
	//ResColor += SrcAlpha;

	FragColor = ResColor;
}