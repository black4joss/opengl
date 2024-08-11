//======================================================================================
//	Ed Kurlyak 2023 App Class
//======================================================================================

#include "OpenGLHeader.h"
#include "MyApp.h"

int WINAPI WinMain(HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPSTR lpCmdLine,
					int nCmdShow)
{
	CMyApp App;

	return App.Program_Begin(hInstance, nCmdShow);
}

LRESULT CALLBACK CMyApp::Static_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	if ( uMsg == WM_CREATE )
		SetWindowLong( hWnd, GWL_USERDATA, (LONG)(LONG_PTR)((CREATESTRUCT FAR *)lParam)->lpCreateParams);

	CMyApp *MainWndProc = (CMyApp*)(LONG_PTR)GetWindowLong( hWnd, GWL_USERDATA );
	if (MainWndProc) return MainWndProc->WndProc( hWnd, uMsg, wParam, lParam );
	
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

LRESULT CMyApp::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//проверяем сообщения
	switch (uMsg)
	{
		//мы получили сообщение закрыть приложение
		case WM_CLOSE:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

int CMyApp::Program_Begin(HINSTANCE	hInstance, int nCmdShow)
{
	GLuint		PixelFormat;

	WNDCLASS wcl;

	hInstance = GetModuleHandle(NULL);

	wcl.style			= CS_HREDRAW | CS_VREDRAW;;
	wcl.lpfnWndProc		= (WNDPROC) Static_WndProc;
	wcl.cbClsExtra		= 0L;
	wcl.cbWndExtra		= 0L;
	wcl.hInstance		= GetModuleHandle(NULL);
	wcl.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
	wcl.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wcl.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcl.lpszMenuName	= NULL;
	wcl.lpszClassName	= CLASSNAME;
	
	if(!RegisterClass (&wcl)) return 0;

	m_hWnd = CreateWindow(CLASSNAME, APPNAME,
			  WS_OVERLAPPEDWINDOW,
              0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
              NULL, NULL, hInstance, this);

	if (!m_hWnd) return 0;
	
	RECT WindowRect = {0,0,WINDOW_WIDTH,WINDOW_HEIGHT};

	AdjustWindowRectEx(&WindowRect,
		GetWindowStyle(m_hWnd),
		GetMenu(m_hWnd) != NULL,
		GetWindowExStyle(m_hWnd));

	UINT WidthScreen = GetSystemMetrics(SM_CXSCREEN);
	UINT HeightScreen = GetSystemMetrics(SM_CYSCREEN);

	UINT WidthX = WindowRect.right - WindowRect.left;
	UINT WidthY = WindowRect.bottom - WindowRect.top;

	UINT PosX =  (WidthScreen - WidthX)/2;
	UINT PosY =  (HeightScreen - WidthY)/2;
	
	MoveWindow(m_hWnd,
		PosX,
        PosY,
        WidthX,
        WidthY,
        FALSE);

	static	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		BPP,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		16,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	if (!(hDC = GetDC(m_hWnd)))
	{
		Destroy();
		MessageBox(NULL, "Can't create OpenGL device dontext", "Info", MB_OK);
		return FALSE;
	}

	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))
	{
		Destroy();
		MessageBox(NULL, "Can't find suitable PixelFormat", "Info", MB_OK);
		return FALSE;
	}

	if (!SetPixelFormat(hDC, PixelFormat, &pfd))
	{
		Destroy();
		MessageBox(NULL, "Can't set the PixelFormat", "Info", MB_OK);
		return FALSE;
	}

	if (!(hRC = wglCreateContext(hDC)))
	{
		Destroy();
		MessageBox(NULL, "Can't create OpenGL rendering context", "Info", MB_OK);
		return FALSE;
	}

	if (!wglMakeCurrent(hDC, hRC))
	{
		Destroy();
		MessageBox(NULL, "Can't activate OpenGL rendering context", "Info", MB_OK);
		return FALSE;
	}

	ShowWindow (m_hWnd, nCmdShow);
	UpdateWindow (m_hWnd);
	SetForegroundWindow(m_hWnd);

	m_MeshManager.Init_MeshManager();
	
	MSG msg;

    while (TRUE)
    {
	    if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
        {
		    if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
            DispatchMessage(&msg);
		}
        
		if(GetKeyState(VK_ESCAPE) & 0xFF00)
			break;

		m_MeshManager.Update_MeshManager();
		m_MeshManager.Draw_MeshManager();

		SwapBuffers(hDC);
	}
	
	Destroy();

	return (int) msg.wParam;
}

void CMyApp::Destroy()
{

	if (hRC)
	{
		if (!wglMakeCurrent(NULL, NULL))
		{
			MessageBox(NULL, "Release hDC and hRC failed", "Info", MB_OK);
		}

		if (!wglDeleteContext(hRC))
		{
			MessageBox(NULL, "Release rendering context failed", "Info", MB_OK);
		}
	}
	
	ReleaseDC(m_hWnd, hDC);
	DestroyWindow(m_hWnd);
	UnregisterClass("Sample", hInstance);

	hRC = NULL;
	hDC = NULL;
	m_hWnd = NULL;
	hInstance = NULL;
}
