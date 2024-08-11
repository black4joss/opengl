//======================================================================================
// Ed Kurlyak 2021 Render To Texture OpenGL
//======================================================================================

#ifndef _MESH_MANAGER_
#define _MESH_MANAGER_

#include "glm/glm.hpp"

using glm::mat4;
using glm::mat3;
using glm::vec3;

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

class CMeshManager
{
public:
	CMeshManager();
	~CMeshManager();

	void Init_MeshManager();
	void Draw_MeshManager();
	void Update_MeshManager();
	
private:
	bool Load_BMP();
	GLchar* Load_Shader_As_String(char *fn);
	
	CTimer m_Timer;

	GLuint  m_TexID;

	mat4 m_MatMVP;

	GLuint m_ProgramHandlePass0;
	GLuint m_ProgramHandlePass1;

	GLuint m_FrameBuffer;
	GLuint m_RenderedTexture;
	//наша текстура глубины без нее сцена будет
	//рисоваться с искажениями (без учета глубины)
	//для куба это не актуально, но на более
	//сложных сценах будут искажения
	//если не добавить эту текстуру
	GLuint m_DepthTexture;


	GLuint m_fsCube;
	GLuint m_fsQuad;
};

#endif