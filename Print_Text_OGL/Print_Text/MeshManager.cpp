//======================================================================================
// Ed Kurlyak 2021 Render To Texture OpenGL
//======================================================================================

#include "OpenGLHeader.h"
#include "MeshManager.h"


CMeshManager::CMeshManager()
{

}

CMeshManager::~CMeshManager()
{
	glDeleteProgram(m_ProgramHandlePass0);
}

void CMeshManager::Init_MeshManager()
{
	Load_BMP();

	glewInit();

	// Initialize VBO
	glGenBuffers(1, &Text2DVertexBufferID);
	glGenBuffers(1, &Text2DUVBufferID);

	/*
	VERT AND FRAG SHADER PASS1
	*/
	GLuint VertShaderPass0 = glCreateShader( GL_VERTEX_SHADER );
	if ( 0 == VertShaderPass0)
	{
		MessageBox(NULL, "Error creating vertex shader", "Info", MB_OK);
	}

	const GLchar* ShaderCodePass1V = Load_Shader_As_String ((char*)".\\Shader\\pass0.vert");

	const GLchar* CodeArray[] = {ShaderCodePass1V};
	glShaderSource (VertShaderPass0, 1, CodeArray, NULL);

	glCompileShader(VertShaderPass0);

	GLint Result;
	glGetShaderiv( VertShaderPass0, GL_COMPILE_STATUS, &Result);
	if(GL_FALSE == Result)
	{
		MessageBox(NULL, "Vert shader compilation failed", "Info", MB_OK);

		GLint LogLen;
		glGetShaderiv(VertShaderPass0, GL_INFO_LOG_LENGTH, &LogLen);
		if(LogLen > 0)
		{
			char * Log = (char *) malloc(LogLen);
			GLsizei Written;
			glGetShaderInfoLog(VertShaderPass0, LogLen, &Written, Log);
			char Buff[1024];
			sprintf_s(Buff, 1024, "Shader Log:\n%s", Log);
			MessageBox(NULL, Buff, "Info", MB_OK);
		}
	}

	GLuint FragShaderPass0 = glCreateShader( GL_FRAGMENT_SHADER );
	if ( 0 == FragShaderPass0 )
	{
		MessageBox(NULL, "Error creating frag shader", "Info", MB_OK);
	}

	const GLchar*	ShaderCodePass1F = Load_Shader_As_String ((char*)".\\Shader\\pass0.frag");
	const GLchar* 	CodeArray_f[] = {ShaderCodePass1F};
	glShaderSource (FragShaderPass0, 1, CodeArray_f, NULL);

	glCompileShader(FragShaderPass0);

	glGetShaderiv( FragShaderPass0, GL_COMPILE_STATUS, &Result);
	if(GL_FALSE == Result)
	{
		MessageBox(NULL, "Frag shader compilation failed", "Info", MB_OK);

		GLint LogLen;
		glGetShaderiv(FragShaderPass0, GL_INFO_LOG_LENGTH, &LogLen);
		if(LogLen > 0)
		{
			char * Log = (char *) malloc(LogLen);
			GLsizei Written;
			glGetShaderInfoLog(FragShaderPass0, LogLen, &Written, Log);
			char Buff[1024];
			sprintf_s(Buff, 1024, "Shader Log:\n%s", Log);
			MessageBox(NULL, Buff, "Info", MB_OK);
		}
	}

	delete[] ShaderCodePass1V;
	delete[] ShaderCodePass1F;

	m_ProgramHandlePass0 = glCreateProgram();

	if( 0 == m_ProgramHandlePass0)
	{
		MessageBox(NULL, "Error creating programm object", "Info", MB_OK);
	}

	glAttachShader(m_ProgramHandlePass0, VertShaderPass0);
	glAttachShader(m_ProgramHandlePass0, FragShaderPass0);
	
	glLinkProgram(m_ProgramHandlePass0);

	glDetachShader(m_ProgramHandlePass0, VertShaderPass0);
	glDetachShader(m_ProgramHandlePass0, FragShaderPass0);

	glDeleteShader(VertShaderPass0);
	glDeleteShader(FragShaderPass0);

	//ограничиваем FPS на 30 кадров в сек.
	m_Timer.Timer_Start(30);
}

void CMeshManager::Update_MeshManager()
{
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

	MatView = glm::lookAt(vec3(0.0f, 0.0f, 70.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	MatProjection = glm::perspective(glm::radians(45.0f), (GLfloat)800 / (GLfloat)600, 0.1f, 100.0f);

	m_MatMVP = MatProjection * MatView * MatModel;

}

void CMeshManager::Print_Text(char* BuffText, float PosX, float PosY, float SizeX, float SizeY)
{
	unsigned int Length = strlen(BuffText);
	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> UVs;

	for (unsigned int i = 0; i < Length; i++)
	{

		glm::vec2 vertex_up_left =		glm::vec2(PosX + i * SizeX,			PosY);
		glm::vec2 vertex_up_right =		glm::vec2(PosX + i * SizeX + SizeX, PosY);
		glm::vec2 vertex_down_right =	glm::vec2(PosX + i * SizeX + SizeX, PosY + SizeY);
		glm::vec2 vertex_down_left =	glm::vec2(PosX + i * SizeX,			PosY + SizeY);

		vertices.push_back(vertex_up_left);
		vertices.push_back(vertex_down_left);
		vertices.push_back(vertex_down_right);

		vertices.push_back(vertex_up_left);
		vertices.push_back(vertex_down_right);
		vertices.push_back(vertex_up_right);

		char character = BuffText[i];
		float uv_x = (character % 16) / 16.0f;
		float uv_y = (character / 16) / 16.0f;

		//отступ перед буквой 3 пикселя
		float tw1 = 3.0f / 256.0f;
		//ширина буквы 9 пикселей
		float tw2 = 9.0f / 256.0f;

		//ширина клетки 16 пикселей

		glm::vec2 uv_up_left =		glm::vec2(uv_x + tw1,		uv_y);
		glm::vec2 uv_up_right =		glm::vec2(uv_x + tw1 + tw2, uv_y);
		glm::vec2 uv_down_right =	glm::vec2(uv_x + tw1 + tw2, uv_y + 1.0f / 16.0f);
		glm::vec2 uv_down_left =	glm::vec2(uv_x + tw1,		uv_y + 1.0f / 16.0f);
		
		UVs.push_back(uv_up_left);
		UVs.push_back(uv_down_left);
		UVs.push_back(uv_down_right);

		UVs.push_back(uv_up_left);
		UVs.push_back(uv_down_right);
		UVs.push_back(uv_up_right);

	}

	glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glDisable(GL_BLEND);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void CMeshManager::Draw_MeshManager()
{

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	//PASS 1 рисуем куб
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 800, 600);

	glClearColor(0.0f, 0.125f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glEnable (GL_DEPTH_TEST);

	glUseProgram(m_ProgramHandlePass0);
	
	GLuint MatMVP = glGetUniformLocation(m_ProgramHandlePass0, "MVP");
	if( MatMVP >= 0 )
    {
        glUniformMatrix4fv(MatMVP, 1, GL_FALSE, (const GLfloat*)&m_MatMVP);
    }
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture( GL_TEXTURE_2D, m_TexID );

	GLuint Tex = glGetUniformLocation(m_ProgramHandlePass0, "ourTexture");
	if( Tex >= 0 )
    {
        glUniform1i(Tex, 0);
    }


	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	char buff[256];

	//char width/height in screen coordinates
	float char_w = 10.0f;
	float char_h = 20.0f;

	sprintf_s(buff, 256, "Vendor %s", vendor);
	Print_Text(buff, 100.0f, 100.0f + char_h * 0.0f, char_w, char_h);
	
	sprintf_s(buff, 256, "Renderer %s", renderer);
	Print_Text(buff, 100.0f, 100.0f + char_h * 1.0f, char_w, char_h);

	sprintf_s(buff, 256, "Gl Version (string) %s", version);
	Print_Text(buff, 100.0f, 100.0f + char_h * 2.0f, char_w, char_h);
	
	sprintf_s(buff, 256, "GL Version (integer) %d.%d", major, minor);
	Print_Text(buff, 100.0f, 100.0f + char_h * 3.0f, char_w, char_h);
	
	sprintf_s(buff, 256, "GLSL Version %s", glslVersion);
	Print_Text(buff, 100.0f, 100.0f + char_h * 4.0f, char_w, char_h);

	unsigned int UnisgnedIntFPS = m_Timer.Calculate_FPS();

	sprintf_s(buff, 256, "FPS %d", UnisgnedIntFPS);
	Print_Text(buff, 100.0f, 100.0f + char_h * 6.0f, char_w, char_h);

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

bool CMeshManager::Load_BMP()
{
	//открываем BMP файл для чтения в бинарном режиме
	FILE* fp = NULL;
	fopen_s(&fp, "ExportedFont.bmp", "rb");
	if (fp == NULL) printf("Error Open File");

	//читаем заголовок файла текстуры
	BITMAPFILEHEADER bfh;
	fread(&bfh, sizeof(BITMAPFILEHEADER), 1, fp);

	//читаем заголовок файла текстуры
	BITMAPINFOHEADER bih;
	fread(&bih, sizeof(BITMAPINFOHEADER), 1, fp);

	//сдвигаемся от начала BMP файла до начала данных
	fseek(fp, bfh.bfOffBits, SEEK_SET);

	//указатель на массив байт полученных из BMP файла
	unsigned char* pRes;
	pRes = new unsigned char[bih.biHeight * bih.biWidth * 4];
	//читаем из файла rgb данные изображения
	fread(pRes, bih.biHeight * bih.biWidth * 4, 1, fp);

	//загрузили текстуру закрываем файл
	fclose(fp);

	unsigned char* pResTemp;
	pResTemp = new unsigned char[bih.biHeight * bih.biWidth * 4];

	for (int h = 0; h < bih.biHeight; h++)
	{
		for (int w = 0; w < bih.biWidth; w++)
		{
			//переворачиваем BMP файл по вертикали
			int Index1 = (h * bih.biWidth + w) * 4;
			int Index2 = ((bih.biHeight - 1 - h) * bih.biWidth + w) * 4;

			//меняем RGB местами
			pResTemp[Index2 + 0] = pRes[Index1 + 2];
			pResTemp[Index2 + 1] = pRes[Index1 + 1];
			pResTemp[Index2 + 2] = pRes[Index1 + 0];
			pResTemp[Index2 + 3] = pRes[Index1 + 3];
		}
	}

	glGenTextures(1, &m_TexID);
	glBindTexture(GL_TEXTURE_2D, m_TexID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bih.biWidth, bih.biHeight,
		0, GL_RGBA, GL_UNSIGNED_BYTE, pResTemp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	delete[] pRes;
	delete[] pResTemp;

	return TRUE;
}

