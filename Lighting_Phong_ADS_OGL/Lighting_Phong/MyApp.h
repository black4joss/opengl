//======================================================================================
//	Ed Kurlyak 2023 App Class
//======================================================================================

#ifndef _MYAPP_
#define _MYAPP_

#include "OpenGLHeader.h"

#define APPNAME "Lighting Fong OpenGL"
#define CLASSNAME "Sample"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define BPP 32

class CMyApp
{
public:
	int Program_Begin(HINSTANCE	hInstance, int nCmdShow);

private:
	static LRESULT CALLBACK Static_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void Destroy();

	HWND m_hWnd;
	CMeshManager m_MeshManager;

	HDC			hDC;
	HGLRC		hRC;

	HINSTANCE hInstance;

};

#endif