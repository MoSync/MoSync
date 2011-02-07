#include "OpenGLES.h"
#include <windows.h>
#include <SDL/SDL.h>
#include <SDL/SDL_syswm.h>

namespace Base {

	struct SubViewData {
		HWND window;
		HGLRC hRC;	// Permanent Rendering Context
	};

#define CHILD_IDENTIFIER 100

	// can only have one subview for now.
	static bool sSubViewExists = false;

	static HWND sMainWnd;

	static LRESULT CALLBACK WindowProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)	// Evaluate Window Message
		{
		case WM_PAINT:
			{
				return 0;
			}
		}

		return DefWindowProc (hWnd, uMsg, wParam, lParam);		
	}

	static BOOL RegisterWindowClass (CHAR* className, HINSTANCE hInstance)	// Register A Window Class For This Application.
	{	// TRUE If Successful
		// Register A Window Class
		WNDCLASSEX windowClass;	// Window Class
		ZeroMemory(&windowClass, sizeof (WNDCLASSEX));	// Make Sure Memory Is Cleared
		windowClass.cbSize = sizeof (WNDCLASSEX);	// Size Of The windowClass Structure
		windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraws The Window For Any Movement / Resizing
		windowClass.lpfnWndProc = (WNDPROC)(WindowProc);	// WindowProc Handles Messages
		windowClass.hInstance = hInstance;	// Set The Instance
		windowClass.hbrBackground = (HBRUSH)(COLOR_APPWORKSPACE);	// Class Background Brush Color
		windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);	// Load The Arrow Pointer
		windowClass.lpszClassName = "test";	// Sets The Applications Classname
		if (RegisterClassEx (&windowClass) == 0)	// Did Registering The Class Fail?
		{
			return FALSE;	// Return False (Failure)
		}
		return TRUE;	// Return True (Success)
	}

	bool subViewOpen(int left, int top, int width, int height, SubView& out) {

		HWND mainWnd;
		SDL_SysWMinfo info;
		SDL_VERSION(&info.version);
		if(sSubViewExists) {
			return false;
		}

		if (SDL_GetWMInfo(&info))
		{
			mainWnd = info.window;
			//printf("Found gMainWnd");
		}
		else
		{
			//printf("NOT Found gMainWnd");
			return false;
		}

		// Setting window style WS_CLIPCHILDREN prevents drawing on top of the WebView.
		LONG windowStyle = GetWindowLong(mainWnd, GWL_STYLE);
		SetWindowLong(mainWnd, GWL_STYLE, windowStyle | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW); // Not needed: WS_CLIPSIBLINGS

		CHAR className[256];
		if(!GetClassName(mainWnd, className, 256)) 
			return false;

		RegisterWindowClass(className, GetModuleHandle(NULL));

		HWND handle = CreateWindowEx(
			0, 
			className, //"test", 
			(LPCTSTR) "SubView", 
			WS_CHILD | WS_BORDER, 
			left,
			top,
			width,
			height, 
			mainWnd, 
			(HMENU) (int) (CHILD_IDENTIFIER), 
			GetModuleHandle(NULL), 
			NULL
			);

		if(!handle) return false;

		RECT clientRect;
		GetClientRect(handle, &clientRect);

		SubViewData *subViewData = new SubViewData;
		subViewData->window = handle;
		subViewData->hRC = NULL;

		out.data = subViewData;
		out.x = clientRect.left;
		out.y = clientRect.top;
		out.w = clientRect.right-clientRect.left;
		out.h = clientRect.bottom-clientRect.top;
		sSubViewExists = true;

		sMainWnd = mainWnd;


		return true;
	}

	bool subViewClose(const SubView& sv) {
		return false;
	}

	bool openGLInit(const SubView& subView) {
		if(subView.data == NULL) return false;

		SubViewData *subViewData = (SubViewData*)subView.data;
		HWND hWnd = subViewData->window;

		HDC hDC;
		if (!(hDC=GetDC(hWnd)))	return false;

		static	PIXELFORMATDESCRIPTOR pfd=					// pfd Tells Windows How We Want Things To Be
		{
			sizeof(PIXELFORMATDESCRIPTOR),					// Size Of This Pixel Format Descriptor
			1,								// Version Number
			PFD_DRAW_TO_WINDOW |						// Format Must Support Window
			PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
			PFD_DOUBLEBUFFER,						// Must Support Double Buffering
			PFD_TYPE_RGBA,							// Request An RGBA Format
			32,								// Select Our Color Depth
			0, 0, 0, 0, 0, 0,						// Color Bits Ignored
			0,								// No Alpha Buffer
			0,								// Shift Bit Ignored
			0,								// No Accumulation Buffer
			0, 0, 0, 0,							// Accumulation Bits Ignored
			16,								// 16Bit Z-Buffer (Depth Buffer)
			0,								// No Stencil Buffer
			0,								// No Auxiliary Buffer
			PFD_MAIN_PLANE,							// Main Drawing Layer
			0,								// Reserved
			0, 0, 0								// Layer Masks Ignored
		};

		int PixelFormat;	

		if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))				// Did Windows Find A Matching Pixel Format?
		{
			return false;							// Return FALSE
		}

		if(!SetPixelFormat(hDC,PixelFormat,&pfd))				// Are We Able To Set The Pixel Format?
		{
			return false;							// Return FALSE
		}

		if(!(subViewData->hRC=wglCreateContext(hDC))) {
			return false;
		}

		if(!wglMakeCurrent(hDC,subViewData->hRC)) {
			return false;
		}

		ShowWindow(hWnd,SW_NORMAL);						// Show The Window
		SetForegroundWindow(hWnd);						// Slightly Higher Priority
		//	SetFocus(hWnd);								// Sets Keyboard Focus To The Window
		UpdateWindow(hWnd);

		return true;
	}

	bool openGLClose(const SubView& subView) {

		return false;
	}

	bool openGLProcessEvents(const SubView &subView) {
		if(subView.data == NULL) return false;
		SubViewData *subViewData = (SubViewData*)subView.data;
		HWND hWnd = subViewData->window;

		MSG msg;
		while(PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE) > 0) {
			TranslateMessage(&msg);
			if(msg.message == WM_PAINT || msg.message == WM_SETFOCUS)
				ValidateRect(hWnd, NULL);
			DispatchMessage(&msg);
		}

		while(PeekMessage(&msg, sMainWnd, 0, 0, PM_REMOVE) > 0) {
			TranslateMessage(&msg);
			if(msg.message == WM_PAINT || msg.message == WM_SETFOCUS)
				ValidateRect(sMainWnd, NULL);
			DispatchMessage(&msg);
		}

		return true;
	}

	bool openGLSwap(const SubView& subView) {
		if(subView.data == NULL) return false;
		SubViewData *subViewData = (SubViewData*)subView.data;
		HWND hWnd = subViewData->window;
		if(!hWnd) return false;
		HDC hDC=GetDC(hWnd);
		if(!hDC) return false;
		SwapBuffers(hDC);

		/*
		MSG msg;
		while(GetMessage(&msg, hWnd, 0, 0) > 0) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		*/
	
		return true;
	}

}