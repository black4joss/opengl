//======================================================================================
//	Ed Kurlyak 2023 Lighting Fong OpenGL
//======================================================================================

#include "OpenGLHeader.h"
#include "MeshManager.h"


CMeshManager::CMeshManager()
{

}

CMeshManager::~CMeshManager()
{
	glDeleteProgram(m_ProgramHandle);
}

void CMeshManager::Init_MeshManager()
{
	//модель куба имеет
	//24 вершины
	//3 компоненты x,y,z каждая вершина
	//12 треугольников
	//3 вершины каждый треугольник
	

	float VertBuff[24 * 3] = {
		-15.000000,-15.000000,-15.000000,
		-15.000000,-15.000000,15.000000,
		15.000000,-15.000000,15.000000,
		15.000000,-15.000000,-15.000000,
		-15.000000,15.000000,-15.000000,
		15.000000,15.000000,-15.000000,
		15.000000,15.000000,15.000000,
		-15.000000,15.000000,15.000000,
		-15.000000,-15.000000,-15.000000,
		15.000000,-15.000000,-15.000000,
		15.000000,15.000000,-15.000000,
		-15.000000,15.000000,-15.000000,
		15.000000,-15.000000,-15.000000,
		15.000000,-15.000000,15.000000,
		15.000000,15.000000,15.000000,
		15.000000,15.000000,-15.000000,
		15.000000,-15.000000,15.000000,
		-15.000000,-15.000000,15.000000,
		-15.000000,15.000000,15.000000,
		15.000000,15.000000,15.000000,
		-15.000000,-15.000000,15.000000,
		-15.000000,-15.000000,-15.000000,
		-15.000000,15.000000,-15.000000,
		-15.000000,15.000000,15.000000 };

	float NormalBuff[24 * 3] = { 0.000000,-1.000000,0.000000,
	   0.000000,-1.000000,0.000000,
	   0.000000,-1.000000,0.000000,
	   0.000000,-1.000000,0.000000,
	   0.000000,1.000000,0.000000,
	   0.000000,1.000000,0.000000,
	   0.000000,1.000000,0.000000,
	   0.000000,1.000000,0.000000,
	   0.000000,0.000000,-1.000000,
	   0.000000,0.000000,-1.000000,
	   0.000000,0.000000,-1.000000,
	   0.000000,0.000000,-1.000000,
	   1.000000,0.000000,0.000000,
	   1.000000,0.000000,0.000000,
	   1.000000,0.000000,0.000000,
	   1.000000,0.000000,0.000000,
	   0.000000,0.000000,1.000000,
	   0.000000,0.000000,1.000000,
	   0.000000,0.000000,1.000000,
	   0.000000,0.000000,1.000000,
	   -1.000000,0.000000,0.000000,
	   -1.000000,0.000000,0.000000,
	   -1.000000,0.000000,0.000000,
	   -1.000000,0.000000,0.000000 };

	unsigned int IndexBuff[36] = {
		0,2,1,
		2,0,3,
		4,6,5,
		6,4,7,
		8,10,9,
		10,8,11,
		12,14,13,
		14,12,15,
		16,18,17,
		18,16,19,
		20,22,21,
		22,20,23 };

	glewInit();

	GLuint VboVertex;
	GLuint VboNormal;
	GLuint VboIndices;

	glGenBuffers(1, &VboVertex);
	glBindBuffer(GL_ARRAY_BUFFER, VboVertex);
	glBufferData(GL_ARRAY_BUFFER, 24 * 3 * sizeof(float), VertBuff, GL_STATIC_DRAW);

	glGenBuffers(1, &VboNormal);
	glBindBuffer(GL_ARRAY_BUFFER, VboNormal);
	glBufferData(GL_ARRAY_BUFFER, 24 * 3 * sizeof(float), NormalBuff, GL_STATIC_DRAW);

	glGenBuffers(1, &VboIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12 * 3 * sizeof(unsigned int), IndexBuff, GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_fsCube);
	glBindVertexArray(m_fsCube);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboIndices);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VboVertex);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, VboNormal);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);

	GLuint VertShader = glCreateShader(GL_VERTEX_SHADER);
	if (0 == VertShader)
	{
		MessageBox(NULL, "Error creating vertex shader", "Info", MB_OK);
	}

	const GLchar* ShaderCodeV = Load_Shader_As_String((char*)".\\Shader\\light.vert");
	const GLchar* CodeArrayV[] = { ShaderCodeV };
	glShaderSource(VertShader, 1, CodeArrayV, NULL);

	glCompileShader(VertShader);

	GLint Result;
	glGetShaderiv(VertShader, GL_COMPILE_STATUS, &Result);
	if (GL_FALSE == Result)
	{
		MessageBox(NULL, "Vert shader compilation failed", "Info", MB_OK);

		GLint LogLen;
		glGetShaderiv(VertShader, GL_INFO_LOG_LENGTH, &LogLen);
		if (LogLen > 0)
		{
			char* Log = (char*)malloc(LogLen);
			GLsizei Written;
			glGetShaderInfoLog(VertShader, LogLen, &Written, Log);
			char Buff[1024];
			sprintf_s(Buff, 1024, "Shader Log:\n%s", Log);
			MessageBox(NULL, Buff, "Info", MB_OK);
		}
	}

	GLuint FragShader = glCreateShader(GL_FRAGMENT_SHADER);
	if (0 == FragShader)
	{
		MessageBox(NULL, "Error creating frag shader", "Info", MB_OK);
	}

	const GLchar* ShaderCodeF = Load_Shader_As_String((char*)".\\Shader\\light.frag");
	const GLchar* CodeArrayF[] = { ShaderCodeF };
	glShaderSource(FragShader, 1, CodeArrayF, NULL);

	glCompileShader(FragShader);

	glGetShaderiv(FragShader, GL_COMPILE_STATUS, &Result);
	if (GL_FALSE == Result)
	{
		MessageBox(NULL, "Frag shader compilation failed", "Info", MB_OK);

		GLint LogLen;
		glGetShaderiv(FragShader, GL_INFO_LOG_LENGTH, &LogLen);
		if (LogLen > 0)
		{
			char* Log = (char*)malloc(LogLen);
			GLsizei Written;
			glGetShaderInfoLog(FragShader, LogLen, &Written, Log);
			char Buff[1024];
			sprintf_s(Buff, 1024, "Shader Log:\n%s", Log);
			MessageBox(NULL, Buff, "Info", MB_OK);
		}
	}

	delete[] ShaderCodeV;
	delete[] ShaderCodeF;

	m_ProgramHandle = glCreateProgram();

	if (0 == m_ProgramHandle)
	{
		MessageBox(NULL, "Error creating programm object", "Info", MB_OK);
	}

	glAttachShader(m_ProgramHandle, VertShader);
	glAttachShader(m_ProgramHandle, FragShader);

	glLinkProgram(m_ProgramHandle);

	glDetachShader(m_ProgramHandle, VertShader);
	glDetachShader(m_ProgramHandle, FragShader);

	glDeleteShader(VertShader);
	glDeleteShader(FragShader);

	//ограничиваем FPS на 30 кадров в сек.
	m_Timer.Timer_Start(30);
}

void CMeshManager::Update_MeshManager()
{
	m_Timer.Calculate_FPS();
	float ElapsedTime = m_Timer.Get_Elapsed_Time();

	static float Angle = 0.0f;

	Angle += ElapsedTime * 20.0f;

	if (Angle > 360.0f)
		Angle = 0.0f;
	
	mat4 MatModel = mat4(1.0f);
	mat4 MatView = mat4(1.0f);
	mat4 MatProjection = mat4(1.0f);

	MatModel *= glm::rotate(glm::radians(Angle), vec3(0.0f, 1.0f, 0.0f));
	MatModel *= glm::rotate(glm::radians(Angle), vec3(1.0f, 0.0f, 0.0f));
	//MatModel *= glm::translate(vec3(0.0f,-0.45f,0.0f));

	MatView = glm::lookAt(vec3(0.0f, 0.0f, 95.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	MatProjection = glm::perspective(glm::radians(45.0f), (GLfloat)800 / (GLfloat)600, 0.1f, 500.0f);

	m_MatWVP = MatProjection * MatView * MatModel;
	m_MatWV = MatView * MatModel;
}


void CMeshManager::Draw_MeshManager()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, 800, 600);

	glClearColor(0.0f, 0.125f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_ProgramHandle);
	glBindVertexArray(m_fsCube);

	GLuint MVP = glGetUniformLocation(m_ProgramHandle, "MVP");
	if (MVP >= 0)
	{
		glUniformMatrix4fv(MVP, 1, GL_FALSE, (const GLfloat*)& m_MatWVP);
	}

	GLuint ModelViewMatrix = glGetUniformLocation(m_ProgramHandle, "ModelViewMatrix");
	if (ModelViewMatrix >= 0)
	{
		glUniformMatrix4fv(ModelViewMatrix, 1, GL_FALSE, (const GLfloat*)& m_MatWV);
	}

	GLuint NormalMatrix = glGetUniformLocation(m_ProgramHandle, "NormalMatrix");
	if (NormalMatrix >= 0)
	{
		glUniformMatrix3fv(NormalMatrix, 1, GL_FALSE, (const GLfloat*) & (mat3(vec3(m_MatWV[0]), vec3(m_MatWV[1]), vec3(m_MatWV[2]))));
	}
	
	glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glUseProgram(0);
}

GLchar* CMeshManager::Load_Shader_As_String(char* Fn)
{

	FILE* Fp;
	char* Content = NULL;

	unsigned int Count = 0;

	if (Fn != NULL)
	{

		errno_t Err;
		Err = fopen_s(&Fp, Fn, "rt");

		if (Err == NULL)
		{

			fseek(Fp, 0, SEEK_END);
			Count = ftell(Fp);
			rewind(Fp);

			if (Count > 0)
			{
				Content = (char*)malloc(sizeof(char) * (Count + 1));
				Count = fread(Content, sizeof(char), Count, Fp);
				Content[Count] = '\0';
			}
			fclose(Fp);
		}
	}
	return Content;
}


