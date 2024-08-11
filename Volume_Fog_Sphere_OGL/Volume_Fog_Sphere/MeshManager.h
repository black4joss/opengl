//======================================================================================
//	Ed Kurlyak 2023 Volume Fog OpenGL
//======================================================================================

#ifndef _MESH_MANAGER_
#define _MESH_MANAGER_

#include "Camera.h"

#include "glm/glm.hpp"

using glm::mat4;
//using glm::mat3;
using glm::vec3;
using glm::vec2;

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

class CMeshManager
{
public:
	CMeshManager();
	~CMeshManager();

	void Init_MeshManager(UINT Width, UINT Height);
	void Draw_MeshManager();
	void Update_MeshManager();
	
private:
	bool Load_BMP(char* Filename, int TexNum);
	bool Load_Mesh(char* Filename, int TexNum);
	GLchar* Load_Shader_As_String(char *fn);
	
	CTimer m_Timer;

	CFirstPersonCamera m_Camera;

	mat4 m_MatMVP;

	GLuint m_ProgramHandlePass0;
	GLuint m_ProgramHandlePass1;

	GLuint m_FrameBuffer;
	GLuint m_RenderedTexture;
	//наша текстура глубины без нее сцена будет
	//рисоваться с искажениями (без учета глубины)
	GLuint m_DepthTexture;

	//мешь состоит из 12 файлов геометрии
	//и 12 текстур BMP
	int VertCountArray[12];
	GLuint m_fsMesh[12];
	GLuint m_TexID[12];
	GLuint m_fsQuad;
};

#endif