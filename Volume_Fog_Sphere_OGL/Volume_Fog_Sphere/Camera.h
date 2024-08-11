//======================================================================================
//	Ed Kurlyak 2023 Volume Fog OpenGL
//======================================================================================

#ifndef _CAMERA_
#define _CAMERA_

#include "OpenGLHeader.h"

#include "glm/glm.hpp"

using glm::mat4;
//using glm::mat3;
using glm::vec3;
//using glm::vec2;

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

class CFirstPersonCamera
{
public:

	CFirstPersonCamera(){};
	~CFirstPersonCamera(){};
	void Init_Camera(int Width, int Height);
	mat4 Frame_Move(float fTime);
	mat4 Matrix_Rotation_Axis(vec3 VecIn, float Angle);
	vec3 Vec3_Transform(vec3 VecIn, mat4 MatIn) ;
	vec3 Vec3_Scale(vec3 VecIn, float ValIn);
	vec3 Vec3_Normalize(vec3 VecIn);
	vec3 Vec3_Cross(vec3 VecIn1, vec3 VecIn2);
	vec3 Vec3_Add(vec3 VecIn11, vec3 VecIn2);
	float Vec3_Dot(vec3 VecIn1, vec3 VecIn2);

	vec3 VecCamPos;

private:
	vec3 VecRight, VecUp, VecLook;
	int ScreenWidth, ScreenHeight;
};

#endif