//======================================================================================
//	Ed Kurlyak 2023 Volume Fog OpenGL
//======================================================================================

#ifndef _VOLUME_FOG_
#define _VOLUME_FOG_

#include "OpenGLHeader.h"

#include "glm/glm.hpp"

using glm::mat4;
using glm::mat3;
using glm::vec3;

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

enum { A, B, C, D, E, F, G, H };

class CMeshManager
{
public:
	CMeshManager();
	~CMeshManager();

	void Init_MeshManager();
	void Draw_MeshManager();
	void Update_MeshManager();
	
private:
	GLchar* Load_Shader_As_String(char *Fn);

	float m_ZFar = 100.0f;

	CTimer m_Timer;

	mat4 m_MatVP;

	GLuint m_FsCube;
	GLuint m_FsQuad;

	GLuint m_ProgramHandleDepth;
	GLuint m_ProgramHandleFog;

	GLuint m_FrameBufferFront;
	GLuint m_FrameBufferBack;

	GLuint m_RenderedTextureFront;
	GLuint m_RenderedTextureBack;
};

#endif