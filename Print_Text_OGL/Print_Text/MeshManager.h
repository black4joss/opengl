//======================================================================================
// Ed Kurlyak 2021 Render To Texture OpenGL
//======================================================================================

#ifndef _MESH_MANAGER_
#define _MESH_MANAGER_

#include "glm/glm.hpp"
#include "vector"

using namespace std;

using glm::mat4;
using glm::mat3;
using glm::vec3;
using glm::vec2;

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
	void Print_Text(char* BuffText, float PosX, float PosY, float SizeX, float SizeY);
	
	CTimer m_Timer;

	GLuint  m_TexID;

	mat4 m_MatMVP;

	GLuint m_ProgramHandlePass0;

	unsigned int Text2DVertexBufferID;
	unsigned int Text2DUVBufferID;

};

#endif