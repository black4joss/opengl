//======================================================================================
//	Ed Kurlyak 2023 Volume Fog OpenGL
//======================================================================================

#include "OpenGLHeader.h"
#include "Camera.h"

float CFirstPersonCamera::Vec3_Dot(vec3 VecIn1, vec3 VecIn2)
{
	return VecIn1.x * VecIn2.x + VecIn1.y * VecIn2.y + VecIn1.z * VecIn2.z;
}
vec3 CFirstPersonCamera::Vec3_Add(vec3 VecIn1, vec3 VecIn2)
{
	vec3 VecOut;

	VecOut.x = VecIn1.x + VecIn2.x;
	VecOut.y = VecIn1.y + VecIn2.y;
	VecOut.z = VecIn1.z + VecIn2.z;

	return VecOut;
}

vec3 CFirstPersonCamera::Vec3_Cross(vec3 VecIn1, vec3 VecIn2)
{
	vec3 VecOut;

	VecOut.x = VecIn1.y * VecIn2.z - VecIn1.z * VecIn2.y;
	VecOut.y = VecIn1.z * VecIn2.x - VecIn1.x * VecIn2.z;
	VecOut.z = VecIn1.x * VecIn2.y - VecIn1.y * VecIn2.x;

	return VecOut;
}

vec3 CFirstPersonCamera::Vec3_Normalize(vec3 VecIn)
{
	vec3 VecOut;

	float vlen = sqrtf( (VecIn.x * VecIn.x) + (VecIn.y * VecIn.y) + (VecIn.z * VecIn.z));

	VecOut.x = VecIn.x/vlen;
	VecOut.y = VecIn.y/vlen;
	VecOut.z = VecIn.z/vlen;

	return VecOut;
}

vec3 CFirstPersonCamera::Vec3_Scale(vec3 VecIn, float ValIn)
{
	vec3 VecOut;

	VecOut.x = VecIn.x * ValIn;
	VecOut.y = VecIn.y * ValIn;
	VecOut.z = VecIn.z * ValIn;

	return VecOut;
}

vec3 CFirstPersonCamera::Vec3_Transform(vec3 VecIn, mat4 MatIn)
{

	vec3 VecOut;

	float w = 1.0;

	VecOut.x = VecIn.x * MatIn[0][0] + VecIn.y * MatIn[1][0] + VecIn.z * MatIn[2][0] + 1*MatIn[3][0];
	VecOut.y = VecIn.x * MatIn[0][1] + VecIn.y * MatIn[1][1] + VecIn.z * MatIn[2][1] + 1*MatIn[3][1];
	VecOut.z = VecIn.x * MatIn[0][2] + VecIn.y * MatIn[1][2] + VecIn.z * MatIn[2][2] + 1*MatIn[3][2];
	//w = VecIn.x * MatIn[0][3] + VecIn.y * MatIn[1][3] + VecIn.z * MatIn[2][3] + 1*MatIn[3][3];

	/*
	VecOut.x = VecOut.x / w;
	VecOut.y = VecOut.y / w;
	VecOut.z = VecOut.z / w;
	*/

	return VecOut;

}

mat4 CFirstPersonCamera::Matrix_Rotation_Axis(vec3 VecIn, float Angle)
{
		float x = VecIn.x;
		float y = VecIn.y;
		float z = VecIn.z;
	
		float s = sinf(Angle);
		float c = cosf(Angle);
		float omc = (float)1.0f - c;

		float xomc = x * omc;
		float yomc = y * omc;
		float zomc = z * omc;

		float xxomc = x * xomc;
		float xyomc = x * yomc;
		float xzomc = x * zomc;
		
		float yyomc = y * yomc;
		float yzomc = y * zomc;
		float zzomc = z * zomc;

		float xs = x * s;
		float ys = y * s;
		float zs = z * s;

		return mat4(xxomc + c,  xyomc + zs, xzomc - ys, 0.0f,
			xyomc - zs, yyomc + c,  yzomc + xs, 0.0f,
			xzomc + ys, yzomc - xs, zzomc + c, 0.0f,
			0.0f ,0.0f, 0.0f, 1.0f);
}

void CFirstPersonCamera::Init_Camera(int Width, int Height)
{
	VecRight = vec3( 0.0, 0.0, -1.0);
    VecUp = vec3( 0.0, 1.0, 0.0);
    VecLook = vec3( 1.0, 0.0, 0.0);
	VecCamPos =	vec3(46114.0f, 6376.0f, 45417.0f);

	ScreenWidth = Width;
	ScreenHeight = Height;

	ShowCursor(FALSE);
}

mat4 CFirstPersonCamera::Frame_Move(float fTime)
{
	POINT mousePos;
	GetCursorPos(&mousePos);

	SetCursorPos(ScreenWidth/2, ScreenHeight/2);
    
	int nDeltaX=ScreenWidth/2-mousePos.x;
	int nDeltaY=ScreenHeight/2-mousePos.y;
	
	//что бы камера перемещалась вверх- вниз
	//(вертикально) сделайте значение больше нуля
	//float m_fRotationScalerY = 0.025;
	float m_fRotationScalerY = 0.0;
	//для перемещения камеры горизонтально
	float m_fRotationScalerX = 0.1f;

	//движение камеры вверх- вниз
	if(nDeltaY<0) m_fRotationScalerY = -m_fRotationScalerY;
	else if(nDeltaY>0) m_fRotationScalerY = m_fRotationScalerY;
	else if(nDeltaY==0) m_fRotationScalerY = 0;

	mat4 matRotRight = Matrix_Rotation_Axis(VecRight , m_fRotationScalerY);
	VecRight = Vec3_Transform(VecRight, matRotRight);
	VecUp = Vec3_Transform(VecUp, matRotRight);
	VecLook = Vec3_Transform(VecLook, matRotRight);

	//движение камеры влево- вправо
	if(nDeltaX<0) m_fRotationScalerX = -m_fRotationScalerX;
	else if(nDeltaX>0) m_fRotationScalerX = m_fRotationScalerX;
	else if(nDeltaX==0) m_fRotationScalerX = 0;

	vec3 vUpTemp = vec3( 0.0f, 1.0f, 0.0f );
	mat4 matRotUp = Matrix_Rotation_Axis(vUpTemp , m_fRotationScalerX);
	
	VecRight = Vec3_Transform(VecRight, matRotUp);
	VecUp = Vec3_Transform(VecUp, matRotUp);
	VecLook = Vec3_Transform(VecLook, matRotUp);
	
	//реакция на нажатия клавиш
	float ratioMove = 5000;
	vec3 temp = vec3(0, 0, 0);;
	vec3 vAccel = vec3(0, 0, 0);

	if (GetAsyncKeyState('W') & 0xFF00)
	{
		temp = vec3(VecLook.x, 0, VecLook.z);
		temp = Vec3_Scale(temp, -ratioMove);
		vAccel += Vec3_Scale(temp, fTime);
	}

	if (GetAsyncKeyState('S') & 0xFF00)
	{
		temp = vec3(VecLook.x, 0, VecLook.z);
		temp = Vec3_Scale(temp, ratioMove);
		vAccel += Vec3_Scale(temp, fTime);
	}

	if (GetAsyncKeyState('D') & 0xFF00)
	{
		temp = vec3(VecRight.x, 0, VecRight.z);
		temp = Vec3_Scale(temp, ratioMove);
		vAccel += Vec3_Scale(temp, fTime);
	}

	if (GetAsyncKeyState('A') & 0xFF00)
	{
		temp = vec3(VecRight.x, 0, VecRight.z);
		temp = Vec3_Scale(temp, -ratioMove);
		vAccel += Vec3_Scale(temp, fTime);
	}
	
	vAccel.y = 0.0;
	
	VecCamPos = Vec3_Add(VecCamPos,vAccel);
	
	VecLook = Vec3_Normalize(VecLook);
	VecUp = Vec3_Cross(VecLook,VecRight);
	VecUp = Vec3_Normalize(VecUp);
	VecRight = Vec3_Cross(VecUp,VecLook);
	VecRight = Vec3_Normalize(VecRight);
	
	float x = -Vec3_Dot(VecCamPos, VecRight);
	float y = -Vec3_Dot(VecCamPos, VecUp);
	float z = -Vec3_Dot(VecCamPos, VecLook);
	
	mat4 MatView = mat4(VecRight.x, VecUp.x, VecLook.x, 0.0f,
		VecRight.y, VecUp.y, VecLook.y, 0.0f,
		VecRight.z, VecUp.z, VecLook.z, 0.0f,
		x,y,z, 1.0f);
		
	return MatView;
}

