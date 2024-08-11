//======================================================================================
//	Ed Kurlyak 2023 Volume Fog OpenGL
//======================================================================================

#include "MeshManager.h"

CMeshManager::CMeshManager()
{
}

CMeshManager::~CMeshManager()
{
	glDeleteProgram(m_ProgramHandleDepth);
	glDeleteProgram(m_ProgramHandleFog);

	glDeleteFramebuffers(1, &m_FrameBufferDepth);

	glDeleteTextures(1, &m_RenderedTextureFront);
	glDeleteTextures(1, &m_RenderedTextureBack);

}

void CMeshManager::Init_MeshManager()
{
	//модель куба имеет
	//8 вершин
	//3 компоненты x,y,z каждая вершина
	//12 треугольников
	//3 вершины каждый треугольник
	
	/*
		CUBE MODEL VERTICES

		FRONT FACE		BACK FACE
		C - D			G - H
		|   |			|	|
		A - B			E - F
	*/

	float VertBuff[8 * 3] = {
	-4.0, -4.0, 4.0,	//A
	 4.0, -4.0, 4.0,	//B
	-4.0,  4.0, 4.0,	//C
	 4.0,  4.0, 4.0,	//D

	-4.0, -4.0,  -4.0,	//E
	 4.0, -4.0,  -4.0,	//F
	-4.0,  4.0,  -4.0,	//G
	 4.0,  4.0,  -4.0 }; //H

	unsigned int IndexBuff[36] = {
		//front face	
		A, D, C,
		A, B, D,

		//back face
		G, F, E,
		G, H, F,

		//left face
		E, C, G,
		E, A, C,

		//right face
		B, H, D,
		B, F, H,

		//top face
		C, H, G,
		C, D, H,

		//bottom face
		E, B, A,
		E, F, B };

	glewInit();

	GLuint VboVertex;
    GLuint VboIndices;

	glGenBuffers(1, &VboVertex);
    glBindBuffer(GL_ARRAY_BUFFER, VboVertex);
    glBufferData(GL_ARRAY_BUFFER, 24 * 3 * sizeof(float), VertBuff, GL_STATIC_DRAW);
	
    glGenBuffers(1, &VboIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(unsigned int) , IndexBuff, GL_STATIC_DRAW);

	glGenVertexArrays( 1, &m_FsCube );
	glBindVertexArray(m_FsCube);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboIndices);
	
	glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VboVertex);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);

	float SAQuadVert[] = { 
        -1.0f,  1.0f,
        -1.0f, -1.0f,
         1.0f, -1.0f,

        -1.0f,  1.0f,
         1.0f, -1.0f,
         1.0f,  1.0f,
    };
	
	GLuint VboSAQuad;

	glGenBuffers(1, &VboSAQuad);
    glBindBuffer(GL_ARRAY_BUFFER, VboSAQuad);
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), SAQuadVert, GL_STATIC_DRAW);

	glGenVertexArrays( 1, &m_FsQuad );
    glBindVertexArray(m_FsQuad);

	glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VboSAQuad);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindVertexArray(0);

	//----------------------
	//DEPTH SHADER
	//-----------------------

	GLuint VertShaderDepth;
	GLuint FragShaderDepth;
	
	VertShaderDepth = glCreateShader( GL_VERTEX_SHADER );
	if ( 0 == VertShaderDepth )
	{
		MessageBox(NULL, "Error creating vertex shader", "Info", MB_OK);
	}

	const GLchar* ShaderCodeDepthV = Load_Shader_As_String ((char*)".\\Shader\\depth.vert");
	const GLchar* CodeArrayDepthV[] = {ShaderCodeDepthV};

	glShaderSource (VertShaderDepth, 1, CodeArrayDepthV, NULL);

	glCompileShader(VertShaderDepth);

	GLint Result;
	glGetShaderiv( VertShaderDepth, GL_COMPILE_STATUS, &Result);
	if(GL_FALSE == Result)
	{
		MessageBox(NULL, "Vert shader compilation failed", "Info", MB_OK);

		GLint LogLen;
		glGetShaderiv(VertShaderDepth, GL_INFO_LOG_LENGTH, &LogLen);
		if(LogLen > 0)
		{
			char * Log = (char *) malloc(LogLen);
			GLsizei Written;
			glGetShaderInfoLog(VertShaderDepth, LogLen, &Written, Log);
			char Buff[1024];
			sprintf_s(Buff, 1024, "Shader Log:\n%s", Log);
			MessageBox(NULL, Buff, "Info", MB_OK);
		}
	}
	
	FragShaderDepth = glCreateShader( GL_FRAGMENT_SHADER );
	if ( 0 == FragShaderDepth )
	{
		MessageBox(NULL, "Error creating frag shader", "Info", MB_OK);
	}

	const GLchar*	ShaderCodeDepthF = Load_Shader_As_String ((char*)".\\Shader\\depth.frag");
	const GLchar* 	CodeArrayDepthF[] = {ShaderCodeDepthF};
	
	glShaderSource (FragShaderDepth, 1, CodeArrayDepthF, NULL);

	glCompileShader(FragShaderDepth);

	glGetShaderiv( FragShaderDepth, GL_COMPILE_STATUS, &Result);
	if(GL_FALSE == Result)
	{
		MessageBox(NULL, "Frag shader compilation failed", "Info", MB_OK);

		GLint LogLen;
		glGetShaderiv(FragShaderDepth, GL_INFO_LOG_LENGTH, &LogLen);
		if(LogLen > 0)
		{
			char * Log = (char *) malloc(LogLen);
			GLsizei Written;
			glGetShaderInfoLog(FragShaderDepth, LogLen, &Written, Log);
			char Buff[1024];
			sprintf_s(Buff, 1024, "Shader Log:\n%s", Log);
			MessageBox(NULL, Buff, "Info", MB_OK);
		}
	}

	delete[] ShaderCodeDepthV;
	delete[] ShaderCodeDepthF;

	//-------------------
	//FOG SHADER
	//-------------------
	GLuint VertShaderFog;
	GLuint FragShaderFog;

	VertShaderFog = glCreateShader( GL_VERTEX_SHADER );
	if ( 0 == VertShaderFog )
	{
		MessageBox(NULL, "Error creating vertex shader fog", "Info", MB_OK);
	}

	const GLchar* ShaderCodeFogV = Load_Shader_As_String ((char*)".\\Shader\\fog.vert");
	const GLchar* CodeArrayFogV[] = { ShaderCodeFogV };
	
	glShaderSource (VertShaderFog, 1, CodeArrayFogV, NULL);

	glCompileShader(VertShaderFog);

	glGetShaderiv( VertShaderFog, GL_COMPILE_STATUS, &Result);
	if(GL_FALSE == Result)
	{
		MessageBox(NULL, "Vert shader fog compilation failed", "Info", MB_OK);

		GLint LogLen;
		glGetShaderiv(VertShaderFog, GL_INFO_LOG_LENGTH, &LogLen);
		if(LogLen > 0)
		{
			char * Log = (char *) malloc(LogLen);
			GLsizei Written;
			glGetShaderInfoLog(VertShaderFog, LogLen, &Written, Log);
			char Buff[1024];
			sprintf_s(Buff, 1024, "Shader fog Log:\n%s", Log);
			MessageBox(NULL, Buff, "Info", MB_OK);
		}
	}

	FragShaderFog = glCreateShader( GL_FRAGMENT_SHADER );
	if ( 0 == FragShaderFog )
	{
		MessageBox(NULL, "Error creating frag fog shader", "Info", MB_OK);
	}

	const GLchar* ShaderCodeFogF = Load_Shader_As_String ((char*)".\\Shader\\fog.frag");
	const GLchar* CodeArrayFogF[] = { ShaderCodeFogF };
	
	glShaderSource (FragShaderFog, 1, CodeArrayFogF, NULL);

	glCompileShader(FragShaderFog);

	glGetShaderiv( FragShaderFog, GL_COMPILE_STATUS, &Result);
	if(GL_FALSE == Result)
	{
		MessageBox(NULL, "Frag shader fog compilation failed", "Info", MB_OK);

		GLint LogLen;
		glGetShaderiv(FragShaderFog, GL_INFO_LOG_LENGTH, &LogLen);
		if(LogLen > 0)
		{
			char * Log = (char *) malloc(LogLen);
			GLsizei Written;
			glGetShaderInfoLog(FragShaderFog, LogLen, &Written, Log);
			char Buff[1024];
			sprintf_s(Buff, 1024, "Shader fog Log:\n%s", Log);
			MessageBox(NULL, Buff, "Info", MB_OK);
		}
	}

	delete[] ShaderCodeFogV;
	delete[] ShaderCodeFogF;

	m_ProgramHandleDepth = glCreateProgram();
	if( 0 == m_ProgramHandleDepth)
	{
		MessageBox(NULL, "Error creating programm object", "Info", MB_OK);
	}

	m_ProgramHandleFog = glCreateProgram();
	if( 0 == m_ProgramHandleFog)
	{
		MessageBox(NULL, "Error creating programm object", "Info", MB_OK);
	}

	glAttachShader(m_ProgramHandleDepth, VertShaderDepth);
	glAttachShader(m_ProgramHandleDepth, FragShaderDepth);
	
	glLinkProgram(m_ProgramHandleDepth);

	glDetachShader(m_ProgramHandleDepth, VertShaderDepth);
	glDetachShader(m_ProgramHandleDepth, FragShaderDepth);

	glDeleteShader(VertShaderDepth);
	glDeleteShader(FragShaderDepth);

	glAttachShader(m_ProgramHandleFog, VertShaderFog);
	glAttachShader(m_ProgramHandleFog, FragShaderFog);

	glLinkProgram(m_ProgramHandleFog);

	glDetachShader(m_ProgramHandleFog, VertShaderFog);
	glDetachShader(m_ProgramHandleFog, FragShaderFog);
	
	glDeleteShader(VertShaderFog);
	glDeleteShader(FragShaderFog);

	//----------------------
	//FRONT
	//----------------------
	
	glGenFramebuffers(1, &m_FrameBufferDepth);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferDepth);

	glGenTextures(1, &m_RenderedTextureFront);
	glBindTexture(GL_TEXTURE_2D, m_RenderedTextureFront);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 800, 600, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_RenderedTextureFront, 0);

	//----------------------
	//BACK
	//----------------------
	
	glGenTextures(1, &m_RenderedTextureBack);
	glBindTexture(GL_TEXTURE_2D, m_RenderedTextureBack);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 800, 600, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_RenderedTextureBack, 0);
		
	glDrawBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		return;
	}

	//ограничиваем FPS на 30 кадров в сек.
	m_Timer.Timer_Start(30);

}

void CMeshManager::Draw_MeshManager()
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	
	//------------------------------
	//FRONT PASS 1 - DRAW CUBE DEPTH FRONT FACES INTO DEPTH BUFF
	//------------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferDepth);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_RenderedTextureFront, 0);

	glViewport(0,0,800,600);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f );
	glClearDepth(1.0f);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glUseProgram(m_ProgramHandleDepth);
	glBindVertexArray(m_FsCube);
	
	GLuint MatMVP = glGetUniformLocation(m_ProgramHandleDepth, "MVP");
	if( MatMVP >= 0 )
    {
        glUniformMatrix4fv(MatMVP, 1, GL_FALSE, (const GLfloat*)&m_MatVP);
    }

	GLuint ZFar = glGetUniformLocation(m_ProgramHandleDepth, "ZFar");
	if (ZFar >= 0)
	{
		glUniform1f(ZFar, m_ZFar);
	}

	glCullFace(GL_BACK);
	
	glDrawElements(GL_TRIANGLES, 36,GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glUseProgram(0);
	
	//------------------------------
	//FRONT PASS 2 - DRAW CUBE DEPTH BACK FACES INTO DEPTH BUFF
	//------------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferDepth);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_RenderedTextureBack, 0);

	glViewport(0,0,800,600);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f );
	glClearDepth(1.0f);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glUseProgram(m_ProgramHandleDepth);
	glBindVertexArray(m_FsCube);
	
	MatMVP = glGetUniformLocation(m_ProgramHandleDepth, "MVP");
	if( MatMVP >= 0 )
    {
        glUniformMatrix4fv(MatMVP, 1, GL_FALSE, (const GLfloat*)&m_MatVP);
    }

	ZFar = glGetUniformLocation(m_ProgramHandleDepth, "ZFar");
	if (ZFar >= 0)
	{
		glUniform1f(ZFar, m_ZFar);
	}

	glCullFace(GL_FRONT);
	glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glUseProgram(0);
	
	//---------------
	//FOG PASS 3 DRAW SCREEN ALIGNED QUAD
	//---------------
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0,0,800,600);

	glEnable (GL_BLEND);
	glBlendFunc (GL_ONE, GL_ONE);
	
	glDisable (GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	glClearColor(0.0f, 0.125f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(m_ProgramHandleFog);
	glBindVertexArray(m_FsQuad);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_RenderedTextureFront);
	
	GLuint Tex = glGetUniformLocation(m_ProgramHandleFog, "ourTexture_Front");
	if( Tex >= 0 )
    {
        glUniform1i(Tex, 0);
    }
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture( GL_TEXTURE_2D, m_RenderedTextureBack );
	
	Tex = glGetUniformLocation(m_ProgramHandleFog, "ourTexture_Back");
	if( Tex >= 0 )
    {
        glUniform1i(Tex, 1);
    }
	
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
	glBindVertexArray(0);
	glUseProgram(0);
    
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

	MatView = glm::lookAt(vec3(0.0f, 0.0f, 20.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	MatProjection = glm::perspective(glm::radians(45.0f), (GLfloat)800 / (GLfloat)600, 0.1f, m_ZFar);

	m_MatVP = MatProjection * MatView * MatModel;

}

GLchar *CMeshManager::Load_Shader_As_String(char *Fn)
{

    FILE *Fp;
    char *Content = NULL;

    unsigned int Count = 0;

    if (Fn != NULL)
	{

		errno_t Err;
		Err = fopen_s(&Fp, Fn,"rt");

        if (Err == NULL) 
		{
      
		fseek(Fp, 0, SEEK_END);
		Count = ftell(Fp);
		rewind(Fp);

			if (Count > 0)
			{
				Content = (char *)malloc(sizeof(char) * (Count+1));
				Count = fread(Content,sizeof(char),Count,Fp);
				Content[Count] = '\0';
			}
        fclose(Fp);
        }
    }
    return Content;
}
