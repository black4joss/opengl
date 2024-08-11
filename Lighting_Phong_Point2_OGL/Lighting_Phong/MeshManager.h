//======================================================================================
//	Ed Kurlyak 2023 Lighting Fong OpenGL
//======================================================================================

#ifndef _MESH_MANAGER_
#define _MESH_MANAGER_

#include "glm/glm.hpp"

using glm::mat4;
using glm::mat3;
using glm::vec3;

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

#include <vector>

struct Vertex
{
	float x, y, z;
};

struct Normal
{
	float x, y, z;
};

class CMeshManager
{
public:
	CMeshManager();
	~CMeshManager();

	void Init_MeshManager();
	void Draw_MeshManager();
	void Update_MeshManager();
	
private:

	GLchar* Load_Shader_As_String(char* Fn);
	void Build_Side(std::vector<Vertex>& VertBuff,
		std::vector<Normal>& NormalBuff,
		std::vector<std::uint32_t>& Indices);

	CTimer m_Timer;

	UINT m_NumCellsPerRow;
	UINT m_NumCellsPerCol;
	float m_CellSpacing;

	UINT m_NumVertsPerRow;
	UINT m_NumVertsPerCol;

	int NumTriangles;
	int NumVert;

	mat4 m_MatWVP;
	mat4 m_MatWV;

	GLuint m_fsCube;

	GLuint m_ProgramHandle;

};

#endif