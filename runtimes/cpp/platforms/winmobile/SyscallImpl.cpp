/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

#define WIN32_LEAN_AND_MEAN

#include "config_platform.h"
#include <windows.h>
#include <gx.h>
#include <Tchar.h>
#include <aygshell.h>

#include <sms.h>

#include <Wininet.h>
#include <winsock2.h>
#include <string>
#include <vector>
#include <map>
#include <time.h>
#include <math.h>
#include <helpers/fifo.h>
#include <helpers/log.h>
#include <helpers/helpers.h>
#include <helpers/smartie.h>
#include <helpers/cpp_defs.h>
#include <MemStream.h>
#include <FileStream.h>
#include "Syscall.h"
#include "TextOutput.h"
#include "ImageLoad.h"
#include <helpers/CPP_IX_GUIDO.h>
#include "netImpl.h"
#define NETWORKING_H
#include "networking.h"
#include <bluetooth/discovery.h>
#include <base_errors.h>
using namespace MoSyncError;
#include "wce_errors.h"
#include "wce_helpers.h"
#include <winioctl.h>

//#include "audio.h"
#include "AudioEngine.h"
#include "AudioChannel.h"

// blah
#include <helpers/CPP_IX_AUDIOBUFFER.h>
#include "BufferAudioSource.h"

#include <core/core.h>

// ifdef streaming?
#include <helpers/CPP_IX_STREAMING.h>
#include <helpers/CPP_IX_AUDIOBUFFER.h>

#if (_WIN32_WCE > 0x420) 
#define MA_PROF_SUPPORT_LOCATIONAPI
#endif

#if (_WIN32_WCE >= 0x502) 
#include <ddraw.h>
IDirectDraw * g_pDD = NULL;
IDirectDrawSurface * g_pDDSPrimary = NULL;		
DDSURFACEDESC ddsd;
/*
namespace Base {
BOOL InitDDraw();
BOOL CloseDDraw();
}
*/
#endif

#ifdef MA_PROF_SUPPORT_LOCATIONAPI
#include <Service.h>
//#include <helpers/CPP_IX_LOCATION.h>
#include <Gpsapi.h>
#endif

//#ifdef MA_PROF_SUPPORT_WLAN
#include <helpers/CPP_IX_WLAN.h>
#include "WifiPeek.h"

#include "Vibration.h"

extern "C" __declspec(dllimport) BOOL KernelIoControl(
    DWORD dwIoControlCode, 
    LPVOID lpInBuf, 
    DWORD nInBufSize, 
    LPVOID lpOutBuf, 
    DWORD nOutBufSize, 
    LPDWORD lpBytesReturned);

bool exited = false;
CRITICAL_SECTION exitMutex;

//Convert a char array to a TCHAR array 
void convertCharToTCHAR(char *c, TCHAR* t) 
{ 
	int i = 0; 
	while (c[i] != '\0'){ 
		t[i] = (TCHAR) c[i]; 
		i++; } 
	t[i] = (TCHAR) c[i]; 
} 
//Convert TCHAR array to a char array 
void convertTCHARToChar(char *c, TCHAR* t) 
{ 
	int i = 0; 
	while (t[i] != '\0'){ 
		c[i] = (char) t[i]; i++; 
	} 
	c[i] = (char) t[i]; 
}

extern ThreadPool gThreadPool;

namespace Base {

	//***************************************************************************
	//Defines and declarations
	//***************************************************************************
	void closeNetwork();

	void InitFullScreen();
	void InitWindowed();
	BOOL InitGraphics();
	void CloseGraphics();
	static void MAUpdateScreen();
	static void textBoxEventHook(UINT msg, WPARAM wParam, LPARAM lParam);

	BOOL VibrationStop();

//	void VideoHandleEvent(VideoStream *stream);

	Syscall* gSyscall;

	enum GraphicsMode 
	{
		GRAPHICSMODE_GX,
		GRAPHICSMODE_GDI,
		GRAPHICSMODE_DDRAW // not used yet
	};

	GraphicsMode graphicsMode;
	Image::PixelFormat pixelFormat;
	Image *backBuffer = NULL;
	static Image *sInternalBackBuffer = NULL;
	Image *currentDrawSurface = NULL;
	unsigned char* screen = NULL;
	unsigned int screenPitchX, screenPitchY;
	MAHandle drawTargetHandle = HANDLE_SCREEN;

	bool gGraphicsActive = true;

	uint realColor;
	uint currentColor;
	uint oldColor;
	uint redMask;
	uint redShift;
	uint redBits;
	uint greenMask;
	uint greenShift;
	uint greenBits;
	uint blueMask;
	uint blueShift;
	uint blueBits;

	int gStartTime;

	static ResourceArray gResourceArray;

	//static int gStoreId = 1;
	//static std::map<int, std::string> gStores;

	static CircularFifo<MAEvent, EVENT_BUFFER_SIZE> gEventFifo;
	static bool gEventOverflow	= false;
	static bool gClosing		= false;

	byte silentWave[] = {82,73,70,70,40,0,0,0,87,65,86,69,102,109,116,32,16,0,0,0,1,0,2,0,
		68,-84,0,0,16,-79,2,0,4,0,16,0,100,97,116,97,4,0,0,0,76,1,-9,1};

	int gVolume = -1;
	//byte *gCurrentSound = NULL;
	Stream* gCurrentSoundSource = NULL;

	void MALibQuit();

	HINSTANCE g_hInst		= NULL;							// MAHandle to the application instance
	HWND g_hwndMain			= NULL;							// MAHandle to the application main window
	TCHAR g_szTitle[80]		= TEXT ("MoRE"),			// Application main window name
	g_szClassName[80]		= TEXT ("MoRE class");	// Main window class name

	extern CRITICAL_SECTION vibrationCS;
	extern UINT_PTR vibrationId;

	// change this to use pairs??
	typedef void (*WinMobileEventCallback)(void);
	std::vector<WinMobileEventCallback> winMobileEventCallbacks;
	std::vector<HANDLE> winMobileEventHandles;

	void addWinMobileEvent(HANDLE event, WinMobileEventCallback callback) {
		winMobileEventHandles.push_back(event);
		winMobileEventCallbacks.push_back(callback);
	}

	void removeWinMobileEvent(HANDLE event) {
		for(unsigned int i = 0; i < winMobileEventHandles.size(); i++) {
			if(winMobileEventHandles[i] == event) {
				winMobileEventCallbacks.erase(winMobileEventCallbacks.begin()+i);
				winMobileEventHandles.erase(winMobileEventHandles.begin()+i);
				return;
			}
		}
	}

	//***************************************************************************
	// Resource loading
	//***************************************************************************
	
	Image* Syscall::loadImage(MemStream& s) 
	{
		int len;
		TEST(s.length(len));
		Image* bitmap;
		if(loadImageFromStream(&bitmap, s.ptrc(), len)!=S_OK)
			MYASSERT(0, WCEERR_IMAGE_LOAD_FAILED);
		return bitmap;
	}
	
	Image* Syscall::loadSprite(void* surface, ushort left, ushort top,
		ushort width, ushort height, ushort cx, ushort cy) 
	{
		return NULL;
	}
	
	//***************************************************************************
	// Helpers
	//***************************************************************************
	void InitAppName() 
	{
		TCHAR modname[_MAX_PATH];
		DWORD size = GetModuleFileName(GetModuleHandle(NULL), modname, _MAX_PATH);

		/* trim exe part */
		int startOfExe = size;
		while(startOfExe>0 && modname[startOfExe]!='.') startOfExe--;
		if(startOfExe) modname[startOfExe] = 0;

		/* trim directory part */
		int startOfModName = size;
		while(startOfModName>0 && modname[startOfModName]!='\\') startOfModName--;
		if(startOfModName) startOfModName++;

		/* generate titles and class names */
		wsprintf(g_szTitle, L"%s", &modname[startOfModName]);
		wsprintf(g_szClassName, L"%s class", &modname[startOfModName]);

		LOG("AppName: %s\n", &modname[startOfModName]);
	}

	// win mobile specific network initialization
	WSADATA wsaData = {0};
	bool initNetwork() 
	{
		// Initialize Winsock.
		INT iRet;
		iRet = WSAStartup( MAKEWORD(2,2), &wsaData);
		if(iRet != NO_ERROR )
		{
			LOG("initNetwork: WSAStartup returned error code %d\n", WSAGetLastError());
			return false;
		}
		return true;
	}
	
	// win mobile specific network cleanup
	void closeNetwork() 
	{
		WSACleanup();
	}

#define NUMBER_KEYS(m) m(0) m(1) m(2) m(3) m(4)	m(5) m(6) m(7) m(8) m(9)
#define DIRECT_KEYS(m) m(LEFT) m(RIGHT) m(UP) m(DOWN)
#define MULTI_KEYS(m) m(FIRE, RCTRL) m(FIRE, LCTRL) m(STAR, KP_MULTIPLY) m(POUND, KP_DIVIDE)\
	m(SOFTLEFT, LSHIFT) m(SOFTRIGHT, RSHIFT)

	int currentKeyState = 0;

	static int MAConvertKey(int wmkey)
	{
		if(wmkey>='A'&&wmkey<='Z') {
			return (wmkey-'A')+'a';
		} else
		if(wmkey>='0'&&wmkey<='9') {
			return wmkey;
		}

		switch(wmkey) {
		
			case VK_TTALK:
			case VK_TSOFT1: return MAK_SOFTLEFT;
			case VK_TEND:
			case VK_TSOFT2: return MAK_SOFTRIGHT;

			case VK_T0: return MAK_0;
			case VK_T1: return MAK_1;			
			case VK_T2: return MAK_2;
			case VK_T3: return MAK_3;
			case VK_T4: return MAK_4;
			case VK_T5: return MAK_5;
			case VK_T6: return MAK_6;
			case VK_T7: return MAK_7;
			case VK_T8: return MAK_8;
			case VK_T9: return MAK_9;
			case VK_TPOUND: return MAK_POUND;
			case VK_TSTAR: return MAK_STAR;
			case VK_TLEFT: return MAK_LEFT;
			case VK_TUP: return MAK_UP;
			
#ifdef WIN32_PLATFORM_WFSP
			case VK_TACTION: return MAK_FIRE;
#else
			case VK_ACTION: return MAK_FIRE;
#endif

			case VK_TRIGHT: return MAK_RIGHT;
			case VK_TDOWN: return MAK_DOWN;
			case VK_TBACK: return MAK_CLEAR;
			case VK_SPACE: return MAK_SPACE;

#ifndef WIN32_PLATFORM_WFSP
			case VK_RETURN: return MAK_RETURN;
#endif

			case VK_BACK: return MAK_BACKSPACE;
			case VK_TAB: return MAK_TAB;

			case VK_LSHIFT: return MAK_LSHIFT;
			case VK_RSHIFT: return MAK_RSHIFT;
			case VK_LMENU: return MAK_LALT;
			case VK_RMENU: return MAK_RALT;
			case VK_LCONTROL: return MAK_LCTRL;
			case VK_RCONTROL: return MAK_RCTRL;

			default: return MAK_UNKNOWN;
		}
	}

	static int MAConvertKeyBit(int wmkey)
	{
		switch(wmkey) {
			case VK_TTALK:
			case VK_TSOFT1: return MAKB_SOFTLEFT;
			case VK_TEND:
			case VK_TSOFT2: return MAKB_SOFTRIGHT;
			case VK_T0: return MAKB_0;
			case VK_T1: return MAKB_1;			
			case VK_T2: return MAKB_2;
			case VK_T3: return MAKB_3;
			case VK_T4: return MAKB_4;
			case VK_T5: return MAKB_5;
			case VK_T6: return MAKB_6;
			case VK_T7: return MAKB_7;
			case VK_T8: return MAKB_8;
			case VK_T9: return MAKB_9;
			case VK_TPOUND: return MAKB_POUND;
			case VK_TSTAR: return MAKB_STAR;
			case VK_TLEFT: return MAKB_LEFT;
			case VK_TUP: return MAKB_UP;
			case VK_TACTION: return MAKB_FIRE;
			case VK_TRIGHT: return MAKB_RIGHT;
			case VK_TDOWN: return MAKB_DOWN;
			case VK_TBACK: return MAKB_CLEAR;
			default: return 0;
		}
	}

	static void MAHandleKeyEvent(int wmkey, int eventType) 
	{
		int mak = MAConvertKey(wmkey);

		if(!gEventOverflow) {
			if(gEventFifo.count() + 2 == EVENT_BUFFER_SIZE) {	//leave space for Close event
				gEventOverflow = true;
				gEventFifo.clear();
				LOG("EventBuffer overflow!\n");
			}

			if(eventType==EVENT_TYPE_KEY_PRESSED) {
				currentKeyState |= MAConvertKeyBit(wmkey);
			} else {
				currentKeyState &= ~MAConvertKeyBit(wmkey);
			}

			/* put event in event queue */
			MAEvent event;
			event.type = eventType;
			event.key = mak;
			event.nativeKey = wmkey;
			gEventFifo.put(event);
		}
	}

	static void MAHandleCharEvent(uint character) 
	{
		MAEvent event;
		event.type = EVENT_TYPE_CHAR;
		event.character = character;
		gEventFifo.put(event);
	}

	static void MAHandlePointerEvent(int x, int y, int eventType) 
	{
		//y+=24;
		if(!gEventOverflow) {
			if(gEventFifo.count() + 2 == EVENT_BUFFER_SIZE) {	//leave space for Close event
				gEventOverflow = true;
				gEventFifo.clear();
				LOG("EventBuffer overflow!\n");
			}
			/* put event in event queue */
			MAEvent event;
			event.type = eventType;
			event.point.x = x;
			event.point.y = y;
			gEventFifo.put(event);
		}
	}

	bool MAProcessEvents() //returns true if maWait should return
	{	
		MSG msg;
		HACCEL hAccel = 0;

		while (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE)) {

			// I think I have to do this for oem keys.
			if(gGraphicsActive &&
				(msg.message == WM_KEYDOWN || msg.message == WM_KEYUP)
				&& msg.wParam == VK_PROCESSKEY)
			{
				msg.wParam = ImmGetVirtualKey(msg.hwnd);
			}

			TranslateMessage (&msg);
			LOGD("Message: 0x%08X, 0x%04X, 0x%08X, 0x%08X\n",
				msg.hwnd, msg.message, msg.wParam, msg.lParam);
			DispatchMessage (&msg);

			// dispatch softkey events to textBox, since they aren't handled normally.
			if(msg.message == WM_KEYDOWN &&
				(msg.wParam == VK_F1 || msg.wParam == VK_F2 ||
				msg.wParam == VK_ESCAPE))	//backspace key
			{
				textBoxEventHook(msg.message, msg.wParam, msg.lParam);
			}

			if(msg.message == WM_TIMER) {
				return true;
			}
		}

		return false;
	}

#define EVENT_CLOSE_TIMEOUT 2000
#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp)) 

	VOID CALLBACK CloseCallback(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) 
	{
		GLE(KillTimer(hwnd, idEvent));
		LOG("The application failed to respond to the Close Event and will be terminated.\n");
		MoSyncExit(303);
	}

	void Close() {
		gClosing = true;
		MAEvent event;
		event.type = EVENT_TYPE_CLOSE;
		gEventFifo.put(event);
		GLE(SetTimer(g_hwndMain, 1, EVENT_CLOSE_TIMEOUT, CloseCallback));		
	}

	void Resume() {
		if(gGraphicsActive) return;
		LOG("Resume\n");
#if _WIN32_WCE < 0x502	
		if(graphicsMode == GRAPHICSMODE_GX) GXResume();
#else
		g_pDD->SetCooperativeLevel(g_hwndMain, DDSCL_FULLSCREEN);
		g_pDD->RestoreAllSurfaces();
#endif
		gGraphicsActive = true;
	}

	void Suspend() {
		if(!gGraphicsActive) return;
		LOG("Suspend\n");
#if _WIN32_WCE < 0x502				
		if(graphicsMode == GRAPHICSMODE_GX) GXSuspend();
#else
		g_pDD->SetCooperativeLevel(g_hwndMain, DDSCL_NORMAL); 
#endif
		gGraphicsActive = false;
	}

DWORD GetScreenOrientation()
{
    DWORD retVal ;
    HKEY hKey;
    DWORD dataSize = sizeof(DWORD);
    RegOpenKeyEx(HKEY_LOCAL_MACHINE,TEXT("System\\GDI\\Rotation"),NULL,NULL,&hKey);
    RegQueryValueEx(hKey,TEXT("Angle"),NULL,NULL,(LPBYTE)&retVal, &dataSize);
    RegCloseKey(hKey);
    return retVal;
}

	//***************************************************************************
	//Initialization
	//***************************************************************************
	LRESULT CALLBACK WndProc (HWND hwnd, UINT umsg, WPARAM wParam, 
		LPARAM lParam)
	{
		LOGD("WndProc(0x%08x, 0x%04x, 0x%08x, 0x%08x)\n", hwnd, umsg, wParam, lParam);
		if(wParam == VK_F24) return DefWindowProc (hwnd, umsg, wParam, lParam);

		switch (umsg)
		{
			// Add cases such as WM_CREATE, WM_COMMAND, WM_PAINT if you don't 
			// want to pass these messages along for default processing.
		case WM_CREATE:
			ShowWindow (hwnd, SW_SHOW);
			UpdateWindow (hwnd);
			return 0;

		case WM_CLOSE:
			Close();
			return 0;

		case WM_DESTROY:
			CloseGraphics();
			PostQuitMessage (0);
			return 0;

		case WM_LBUTTONDOWN: 	// The user pressed the screen.
			MAHandlePointerEvent(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), EVENT_TYPE_POINTER_PRESSED);			
			return 0;
		case WM_LBUTTONUP: 	// The user released the stylus from the screen.
			MAHandlePointerEvent(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), EVENT_TYPE_POINTER_RELEASED);
			return 0;
		case WM_MOUSEMOVE:
			MAHandlePointerEvent(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), EVENT_TYPE_POINTER_DRAGGED);
			return 0;
		
		//case WM_GESTURE:
		//	return 0;

		case WM_KEYDOWN:
			if(wParam == VK_THOME) {
				//ShowWindow(hwnd, SW_MINIMIZE);
				//ShowWindow(hwnd, SW_HIDE);
				//EnableWindow(hwnd, FALSE);
				//SetWindowPos(hwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_HIDEWINDOW);
				
				//MoSyncExit(303);
				Suspend();
				InitWindowed();
				ShowWindow(g_hwndMain, SW_MINIMIZE);
				return 0;
			}

			if(wParam == VK_TTALK) break;

			if(lParam&0x40000000) return 0; // check if it has been repeated more than once
			//if(wParam == VK_TBACK) {
			//	GLE(PostMessage(hwnd, WM_CLOSE, 0, 0));
			//}

			MAHandleKeyEvent(wParam, EVENT_TYPE_KEY_PRESSED);
			return 0;

		case WM_CHAR:
			MAHandleCharEvent(wParam);
			return 0;

		case WM_KEYUP:
			if(wParam == VK_F24) break;
			MAHandleKeyEvent(wParam, EVENT_TYPE_KEY_RELEASED);
			return 0;
		
		case WM_ADD_EVENT:
			{
				LOGDT("WM_ADD_EVENT");
				MAEvent* pe = (MAEvent*)wParam;
				gEventFifo.put(*pe);
				delete pe;
			}
			return 0;

		case WM_DEFLUX_BINARY:
			LOGDT("WM_DEFLUX_BINARY");
			SYSCALL_THIS->resources.extract_RT_FLUX(lParam);
			ROOM(SYSCALL_THIS->resources.dadd_RT_BINARY(lParam, (Stream*)wParam));
			return 0;

		case WM_SETFOCUS:
			{
				//InitGraphics();
				//InitFullScreen();
				MAEvent event;
				event.type = EVENT_TYPE_FOCUS_GAINED;
				gEventFifo.put(event);
				Resume();
				//InitFullScreen();
				MAUpdateScreen();
				//SetForegroundWindow(hwnd);
			}
			return 0;

		case WM_KILLFOCUS:
			{
				//CloseGraphics();
				//InitWindowed();
				MAEvent event;
				event.type = EVENT_TYPE_FOCUS_LOST;
				gEventFifo.put(event);
				Suspend();
				//InitWindowed();
				//ShowWindow(g_hwndMain, SW_MINIMIZE);
			}
			return 0;
		case WM_CANCELMODE:
			Suspend();
			InitWindowed();
			ShowWindow(g_hwndMain, SW_MINIMIZE);
			return 0;
#if 0
		case WM_GRAPHNOTIFY:
			VideoHandleEvent((VideoStream*)lParam);
			break;
#endif
		case WM_SETTINGCHANGE:
			if (SETTINGCHANGE_RESET == wParam) {
				MAEvent event;
				event.type = EVENT_TYPE_SCREEN_CHANGED;
				gEventFifo.put(event);

				if(sInternalBackBuffer) {
					delete sInternalBackBuffer;
					sInternalBackBuffer = 0;
				}

				InitGraphics();
			}
			return 0;
		case WM_SIZE:
			/*
			DWORD orientation = GetScreenOrientation();
			MAEvent event;
			event.type = EVENT_TYPE_SCREEN_CHANGED;
			gEventFifo.put(event);

			InitDDraw();
			*/
			return 0;

			/*
		case WM_MOVE:
		case WM_WINDOWPOSCHANGED:
		case 0x0046: //WM_WINDOWPOSCHANGING:
			return 0;
			*/


		}


		return DefWindowProc (hwnd, umsg, wParam, lParam);
	}



	BOOL InitApplication (HINSTANCE hInstance)
	{
	  WNDCLASS wndclass;

	  wndclass.style = CS_HREDRAW | CS_VREDRAW;
	  wndclass.lpfnWndProc = (WNDPROC)WndProc;
	  wndclass.cbClsExtra = 0;
	  wndclass.cbWndExtra = 0;
	  wndclass.hIcon = NULL;
	  wndclass.hInstance = hInstance;
	  wndclass.hCursor = NULL;
	  wndclass.hbrBackground = NULL; //(HBRUSH) GetStockObject (WHITE_BRUSH);
	  wndclass.lpszMenuName = NULL;
	  wndclass.lpszClassName = g_szClassName;
	  
	  return RegisterClass (&wndclass);
	}


	BOOL InitInstance (HINSTANCE hInstance, int iCmdShow)
	{
	  g_hInst = hInstance;
	
	  HWND hwnd = NULL;

	  /*
	  hwnd = CreateWindowEx (
					WS_EX_NODRAG,
					*/
	  hwnd = CreateWindow (
					  g_szClassName,  // Registered class name         
					  g_szTitle,      // Application window name
					  0,			  // Window style
					  CW_USEDEFAULT,  // Horizontal position of the window
					  CW_USEDEFAULT,  // Vertical position of the window
					  CW_USEDEFAULT,  // Window width
					  CW_USEDEFAULT,  // Window height
					  NULL,           // MAHandle to the parent window
					  NULL,           // MAHandle to the menu the identifier
					  hInstance,      // MAHandle to the application instance
					  NULL);          // Pointer to the window-creation data



	  g_hwndMain = hwnd;

	  SetForegroundWindow(hwnd);

	  // If it failed to create the window, return FALSE.
	  if (!hwnd)
		return FALSE;

//	  ShowWindow (hwnd, iCmdShow);
//	  UpdateWindow (hwnd);
	  return TRUE;
	}

#define MENU_HEIGHT 26

	HWND TaskBarFind( void )
	{
		// Look for Smartphone version first, in case other is around (paranoia?)
		HWND hwnd = ::FindWindow( _T( "Tray" ), NULL );     // Smartphone
		if (!hwnd)
		{
			// Eric R. Balch (microsoft.public.vc.vcce 15 Jan 99) says next line works for CE.
			// BZ found it works for Pocket PC, old Microsoft Windows CE platforms, and generic CE
			hwnd = ::FindWindow( _T( "HHTaskBar" ), NULL );
			if(hwnd) return hwnd;
		} else return hwnd;
		// "Big" Windows
		// (thanks to Shing Yip, comp.os.ms-windows.programmer.win32, 21 Jul 99)
		return ::FindWindow( _T( "Shell_TrayWnd" ), NULL );
	}   // PFWTaskBarFind


	void InitFullScreen() {
		RECT rc;


		HWND hwndTaskbar = TaskBarFind();
		GetWindowRect(hwndTaskbar, &rc);
		int taskBarHeight = (rc.bottom-rc.top);

		//hide task bar and other icons
		SHFullScreen(g_hwndMain, SHFS_HIDETASKBAR | SHFS_HIDESTARTICON | 
					 SHFS_HIDESIPBUTTON);

        // Next resize the main window to the size of the screen.
        SetRect(&rc, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
		MoveWindow(g_hwndMain, rc.left, rc.top-taskBarHeight, rc.right-rc.left, rc.bottom-rc.top+taskBarHeight, TRUE);

		::ShowWindow(hwndTaskbar, SW_HIDE);
	}

	void InitWindowed() {
		RECT rc;

		//show task bar and other icons
		SHFullScreen(g_hwndMain, SHFS_SHOWTASKBAR | SHFS_SHOWSTARTICON | 
					 SHFS_SHOWSIPBUTTON);

        // Next resize the main window to the size of the work area.
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, FALSE);
		MoveWindow(g_hwndMain, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, TRUE);

		HWND hwndTaskbar = TaskBarFind();
		::ShowWindow(hwndTaskbar, SW_SHOW);
	}

#if (_WIN32_WCE < 0x502)
	BOOL InitGdi() 
	{

		RawFrameBufferInfo rfbi;
		HDC hdc;
		bool retval;
		retval=false;
		hdc=GetDC(g_hwndMain);
		if(hdc)
		{
			if(ExtEscape(hdc, GETRAWFRAMEBUFFER, 0, 0, 
				sizeof(RawFrameBufferInfo), (char *) &rfbi))
			{
				int screenWidth=rfbi.cxPixels; //store width
				int screenHeight=rfbi.cyPixels; //store height
				screenPitchX=rfbi.cxStride; //store xpitch
				screenPitchY=rfbi.cyStride; //store ypitch
				int bytesPerPixel=screenPitchX;//rfbi.wBPP>>3; //store bits per pixel value
				screen=(unsigned char*)rfbi.pFramePointer; //store pointer
				retval=true;

				if(rfbi.wFormat==FORMAT_555) {
					pixelFormat = Image::PIXELFORMAT_RGB555;
				}
				else if(rfbi.wFormat==FORMAT_565)
				{
					pixelFormat = Image::PIXELFORMAT_RGB565;
				}
				else {
					retval = false;
					LOG("InitGdi: unsupported pixel format.\n");
				}

				if(retval != false) {
					backBuffer = new Image(screenWidth, screenHeight, /*screenWidth*bytesPerPixel*/screenPitchY, pixelFormat);
					graphicsMode = GRAPHICSMODE_GDI;
				} else {
					LOG("InitGdi: ExtEscape failed.\n");
				}
			}
			ReleaseDC(g_hwndMain,hdc);
		}
		else {
			LOG("InitGdi: Could not find DC.\n");
		}

		return retval;	
	}

	BOOL CloseGdi() 
	{
		delete backBuffer;
		
		return TRUE;
	}

	BOOL InitGapi() 
	{
		if(!GXOpenDisplay(g_hwndMain,GX_FULLSCREEN)) {
			LOG("GXOpenDisplay failed.\n");
			return FALSE;
		}

		if(!GXOpenInput()) {
			LOG("GXOpenInput failed.\n");
			return FALSE;
		}

		GXDisplayProperties displayProps;
		displayProps = GXGetDisplayProperties();
		int sw=GetSystemMetrics(SM_CXSCREEN);
		int sh=GetSystemMetrics(SM_CYSCREEN);
		if(sw!=displayProps.cxWidth || sh!=displayProps.cyHeight)
		 return false;

		int screenWidth = displayProps.cxWidth;
		int screenHeight = displayProps.cyHeight;
		screenPitchX = displayProps.cbxPitch;
		screenPitchY = displayProps.cbyPitch;
		int bitsPerPixel = displayProps.cBPP;
		int bytesPerPixel = screenPitchX; //(bitsPerPixel>>3);
		
		if(displayProps.ffFormat&kfDirect444) {
			pixelFormat = Image::PIXELFORMAT_RGB444;
		}
		else if(displayProps.ffFormat&kfDirect555) {
			pixelFormat = Image::PIXELFORMAT_RGB555;
		}
		else if(displayProps.ffFormat&kfDirect565) {	
			pixelFormat = Image::PIXELFORMAT_RGB565;
		}
		else if(displayProps.ffFormat&kfDirect888) {			
			pixelFormat = Image::PIXELFORMAT_RGB888;
		}

		backBuffer = new Image(screenWidth, screenHeight, /*screenWidth*bytesPerPixel*/screenPitchY, pixelFormat);

		graphicsMode = GRAPHICSMODE_GX;

		return TRUE;
	}

	BOOL CloseGapi() {
		if(!GXCloseDisplay()) {
			LOG("GXCloseDisplay failed.\n");
			return FALSE;
		}

		if(!GXCloseInput()) {
			LOG("GXCloseInput failed.\n");
			return FALSE;
		}

		delete backBuffer;

		return TRUE;
	}
#else
	void DDrawUpdateBackbufferInfo(DDSURFACEDESC &ddsd);

	int findShift(int x) {
		int i = 0;
		while(!(x&0x1)) {x>>=1; i++;}
		return i;
	}

	int findNumBits(int x) {
		int y = findShift(x);
		x>>=y;
		int i = 0;
		while((x&0x1)) {x>>=1; i++;}
		return i;
	}

	void DDrawUpdateBackbufferInfo(DDSURFACEDESC &ddsd) {
		backBuffer->alpha = NULL;
		backBuffer->data = (unsigned char*)ddsd.lpSurface;

		backBuffer->bitsPerPixel = ddsd.ddpfPixelFormat.dwRGBBitCount;
		backBuffer->bytesPerPixel = (ddsd.ddpfPixelFormat.dwRGBBitCount+7)/8; //ddsd.lXPitch;
		backBuffer->pitch = ddsd.dwWidth*backBuffer->bytesPerPixel;//(abs(ddsd.lPitch)<abs(ddsd.lXPitch))?abs(ddsd.lXPitch):abs(ddsd.lPitch);
		backBuffer->redMask = ddsd.ddpfPixelFormat.dwRBitMask;
		backBuffer->greenMask = ddsd.ddpfPixelFormat.dwGBitMask;
		backBuffer->blueMask = ddsd.ddpfPixelFormat.dwBBitMask;
		backBuffer->redShift = findShift(ddsd.ddpfPixelFormat.dwRBitMask);
		backBuffer->greenShift = findShift(ddsd.ddpfPixelFormat.dwGBitMask);
		backBuffer->blueShift = findShift(ddsd.ddpfPixelFormat.dwBBitMask);
		backBuffer->redBits = findNumBits(ddsd.ddpfPixelFormat.dwRBitMask);
		backBuffer->greenBits = findNumBits(ddsd.ddpfPixelFormat.dwGBitMask);
		backBuffer->blueBits = findNumBits(ddsd.ddpfPixelFormat.dwBBitMask);		
		backBuffer->shouldFreeData = false;
		backBuffer->width = ddsd.dwWidth;
		backBuffer->height = ddsd.dwHeight;
		backBuffer->clipRect.x = 0;
		backBuffer->clipRect.y = 0;
		backBuffer->clipRect.width = ddsd.dwWidth;
		backBuffer->clipRect.height = ddsd.dwHeight;

		if(backBuffer->bitsPerPixel == 16) {
			if(backBuffer->greenBits == 5)
				backBuffer->pixelFormat = Image::PIXELFORMAT_RGB555;
			else 
				backBuffer->pixelFormat = Image::PIXELFORMAT_RGB565;
		}
		else if(backBuffer->bitsPerPixel == 24 || backBuffer->bitsPerPixel == 32) {
			backBuffer->pixelFormat = Image::PIXELFORMAT_RGB888;
		}
		pixelFormat = backBuffer->pixelFormat;
	}

	void DDrawCreateBackbuffer(DDSURFACEDESC &ddsd) {
		DDrawUpdateBackbufferInfo(ddsd);
		backBuffer->shouldFreeData = true;
		backBuffer->data = (unsigned char*)new unsigned char[backBuffer->pitch*backBuffer->height];
	}

	BOOL CloseDDraw();

	BOOL InitDDraw() 
	{
		bool currentIsBB = false;
		if(currentDrawSurface == backBuffer) currentIsBB = true;

		CloseDDraw();

		HRESULT hRet; 
		hRet = DirectDrawCreate(NULL, &g_pDD, NULL); 
		if(hRet != DD_OK) return FALSE;

		// Get exclusive mode 
		hRet = g_pDD->SetCooperativeLevel(g_hwndMain, DDSCL_FULLSCREEN); 
		if(hRet != DD_OK) return FALSE;

		memset(&ddsd, 0, sizeof(ddsd)); 
		ddsd.dwSize = sizeof(ddsd); 

		// backbuffer and flipping isn't available on most phones, but should be used if it is available..
		ddsd.dwFlags = DDSD_CAPS; //| DDSD_BACKBUFFERCOUNT; 
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE; // | DDSCAPS_FLIP; 
		//ddsd.dwBackBufferCount = 1; 

		hRet = g_pDD->CreateSurface(&ddsd, &g_pDDSPrimary, NULL); 
		if(hRet != DD_OK) return FALSE;

		hRet = g_pDDSPrimary->Lock(NULL, &ddsd, DDLOCK_WAITNOTBUSY, NULL);
		if(hRet != DD_OK) return FALSE;
		backBuffer = new Image();
		DDrawCreateBackbuffer(ddsd);
		hRet = g_pDDSPrimary->Unlock(NULL);

		graphicsMode = GRAPHICSMODE_DDRAW;

		if(currentIsBB)
			currentDrawSurface = backBuffer;

		return TRUE;  
	}

	BOOL CloseDDraw() 
	{
		if(g_pDDSPrimary) {
			g_pDDSPrimary->Release();
			g_pDDSPrimary = NULL;
		}
		if(g_pDD) {
			g_pDD->Release();
			g_pDD = NULL;
		}
		if(backBuffer) {
			//delete backBuffer->data;
			delete backBuffer;
			backBuffer = NULL;
		}
		return TRUE;
	}

	void DDrawFlip() {
		HRESULT hRet;
		while( 1 )
		{
			hRet = g_pDDSPrimary->Flip(NULL, 0 );
			if( hRet == DD_OK ) {
				break;
			}
			if( hRet == DDERR_SURFACELOST ) {
				g_pDDSPrimary->Restore();
			}
			if( hRet != DDERR_WASSTILLDRAWING ) {
				break;
			}
		}
	}
#endif

	BOOL InitGraphics() {
#if (_WIN32_WCE >= 0x502)
		if(!InitDDraw()) {
			LOG("InitDDraw failed.\n");
			return FALSE;
		}
#else
		if(!InitGapi()) {
			LOG("InitGapi failed. Trying InitGdi instead.\n");
			if(!InitGdi()) {
				LOG("InitGdi failed.\n");
				return FALSE;
			}
		}
#endif
		return TRUE;
	}

	void CloseGraphics() 
	{
#if _WIN32_WCE < 0x502
		switch(graphicsMode) {
			case GRAPHICSMODE_GX:
				if(!CloseGapi()) {
					LOG("Could not close gapi\n");
				}
				break;
			case GRAPHICSMODE_GDI:
				if(!CloseGdi()) {
					LOG("Could not close gdi\n");
				}
				break;
		}
#else
		CloseDDraw();
#endif
	}


	template<int bpp>
	void copyPixels(byte *dst, byte *src, int w, int h, int dstPitchX, int dstPitchY, int srcPitchX, int srcPitchY) {
		for(int i = 0; i < h; i++)
		{
			byte *dst_s = dst;
			byte *src_s = src;
			for(int j = 0; j < w; j++)
			{
				byte *dst_p = dst_s;
				byte *src_p = src_s;
				//memcpy(dst_s, src_s, bpp);
				switch(bpp) {
					case 4: *dst_p++ = *src_p++;
					case 3: *dst_p++ = *src_p++;
					case 2: *dst_p++ = *src_p++;
					case 1: *dst_p++ = *src_p++;
				}
				dst_s+=dstPitchX;
				src_s+=srcPitchX;
			}
			dst+=dstPitchY;
			src+=srcPitchY;
		}
	}

	static void MAUpdateScreen() 
	{
		if(GetForegroundWindow()!=g_hwndMain || gGraphicsActive==false) return;

#if (_WIN32_WCE < 0x502)
		if(graphicsMode == GRAPHICSMODE_GX) {
			screen = (unsigned char*)GXBeginDraw();
		}
#else
		HRESULT res = g_pDDSPrimary->Lock(NULL, &ddsd, DDLOCK_WAITNOTBUSY, NULL);
		if(res != DD_OK) {
			return;
		}

		screen = (unsigned char*) ddsd.lpSurface;
		screenPitchX = ddsd.lXPitch;
		screenPitchY = ddsd.lPitch;
#endif
		int w = backBuffer->width;
		int h = backBuffer->height;
		int spitch = backBuffer->pitch;
		int bpp = backBuffer->bytesPerPixel;

		if( (screenPitchX==bpp)&& (screenPitchY==spitch)) {
			memcpy(screen, backBuffer->data, spitch*h);
		} else	{
			// slow copy.... 
			byte *dst = screen;
			byte *src = backBuffer->data;

			switch(bpp) {
				case 4: copyPixels<4>(dst, src, w, h, screenPitchX, screenPitchY, bpp, spitch); break;
				case 3: copyPixels<3>(dst, src, w, h, screenPitchX, screenPitchY, bpp, spitch); break;
				case 2: copyPixels<2>(dst, src, w, h, screenPitchX, screenPitchY, bpp, spitch); break;
				case 1: copyPixels<1>(dst, src, w, h, screenPitchX, screenPitchY, bpp, spitch); break;
			}
		}

#if (_WIN32_WCE < 0x502)
		if(graphicsMode == GRAPHICSMODE_GX) {
			if(!GXEndDraw()) {
				LOG("An error occured when trying to lock the screen: "
					"error code %d\n", GetLastError());
			}
		}
#else
		g_pDDSPrimary->Unlock(NULL);
#endif

#if 0
		g_pDDSPrimary->Unlock(NULL);
		g_pDDSPrimary->Flip(NULL, 0);
		g_pDDSPrimary->Lock(NULL, &ddsd, DDLOCK_WAITNOTBUSY, NULL);
		DDrawUpdateBackbufferInfo(ddsd);
#endif

	}

	static bool MALibInit(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd) {
		InitAppName();

		// use this to prevent application from running twice
		if (FindWindow(g_szClassName, g_szTitle)){
			LOG("Already found an instance of this program, running that one instead.\n");
			SetForegroundWindow(FindWindow(g_szClassName, g_szTitle));
			 return false;
		}

		HRES(CoInitializeEx(NULL, COINIT_MULTITHREADED));

		if(!InitApplication(hInstance)) {
			LOG("InitApplication failed.\n");
			return false;
		}

		if(!InitInstance(hInstance, nShowCmd)) {
			LOG("InitInstance failed.\n");
			return false;
		}

		InitFullScreen();

		if(!InitGraphics()) return false;

		currentDrawSurface = backBuffer;

		redMask = backBuffer->redMask;
		greenMask = backBuffer->greenMask;
		blueMask = backBuffer->blueMask;
		redShift = backBuffer->redShift;
		greenShift = backBuffer->greenShift;
		blueShift = backBuffer->blueShift;
		redBits = backBuffer->redBits;
		greenBits = backBuffer->greenBits;
		blueBits = backBuffer->blueBits;

		// init various optimization stuff
		initMulTable();
		initRecipLut();

		atexit(MALibQuit);

		InitializeCriticalSection(&exitMutex);
		InitializeCriticalSection(&vibrationCS);

		MANetworkInit();
		initNetwork();

		BLUETOOTH(MABtInit)();

		/*
		if(!openAudio(44100, 16, 2)) {
			LOG("Audio could not be initialized.\n");
			return false;
		}
		*/
		if(AudioEngine::init()!=0) {
			LOG("Audio could not be initialized.\n");
			return false;
		}

		gStartTime = maGetMilliSecondCount();

		maSetColor(0);
		maFillRect(0, 0, backBuffer->width, backBuffer->height);
		maUpdateScreen();

		AllKeys(TRUE);

		return true;
	}

	static void MALibQuit() {
		LOG("MALibQuit\n");

		// make sure it is stopped.
		VibrationStop();

		MANetworkClose();
		closeNetwork();
		
		//closeAudio();
		AudioEngine::close();

		//if(gCurrentSound) {
			//delete []gCurrentSound;
		//}

		AllKeys(FALSE);

		InitWindowed();
		CloseGraphics();
		CoUninitialize();

		LOG("PostQuitMessage(0)\n");
		PostQuitMessage (0);
	}


	//***************************************************************************
	// Syscall class
	//***************************************************************************

	Syscall::Syscall(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
	{
		gSyscall = this;
		bool ret = MALibInit(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
		if(ret == false) 
		{
			// if program is already open or something failed...
			exit(0);
		}
		init();
	}

	void Syscall::platformDestruct() {
		MALibQuit();
	}

	//***************************************************************************
	// Proper syscalls
	//***************************************************************************
	SYSCALL(void, maSetClipRect(int left, int top, int width, int height))
	{
		currentDrawSurface->clipRect.x = left;
		currentDrawSurface->clipRect.y = top;
		currentDrawSurface->clipRect.width = width;
		currentDrawSurface->clipRect.height = height;
	}

	SYSCALL(void, maGetClipRect(MARect *rect))
	{
		gSyscall->ValidateMemRange(rect, sizeof(MARect));		
		rect->left = currentDrawSurface->clipRect.x;
		rect->top = currentDrawSurface->clipRect.y;
		rect->width = currentDrawSurface->clipRect.width;
		rect->height = currentDrawSurface->clipRect.height;
	}

#define CONVERT_TO_NATIVE_COLOR_FROM_ARGB(col) \
	((((col)&0x00ff0000)>>(16-(redShift-(8-redBits))))&redMask) | \
	((((col)&0x0000ff00)>>(8-(greenShift-(8-greenBits))))&greenMask) | \
	((((col)&0x000000ff)>>(0-(blueShift-(8-blueBits))))&blueMask)

#define CONVERT_TO_ARGB_FROM_NATIVE_COLOR(col) \
	rgb16_to_rgb32(col, redMask, greenMask, blueMask, redShift, greenShift, blueShift, redBits, greenBits, blueBits)
//	((((((col)&redMask)>>redShift)<<(8-redBits))<<16)&0x00ff0000)|\
//	((((((col)&greenMask)>>greenShift)<<(8-greenBits))<<8)&0x0000ff00)|\
//	((((((col)&blueMask)>>blueShift)<<(8-blueBits)))&0x000000ff)\
//	(0xff000000)

	SYSCALL(int, maSetColor(int argb)) {
		oldColor = currentColor;
		currentColor = argb;
		realColor =	CONVERT_TO_NATIVE_COLOR_FROM_ARGB(argb);
		return oldColor;
	}

	SYSCALL(void, maPlot(int posX, int posY)) {
		currentDrawSurface->drawPoint(posX, posY, realColor);
	}

	SYSCALL(void, maLine(int x0, int y0, int x1, int y1)) {
		currentDrawSurface->drawLine(x0, y0, x1, y1, realColor);
	}

	SYSCALL(void, maFillRect(int left, int top, int width, int height)) {
		currentDrawSurface->drawFilledRect(left, top, width, height, realColor);
	}

	SYSCALL(void, maFillTriangleStrip(const MAPoint2d *points, int count)) {
		SYSCALL_THIS->ValidateMemRange(points, sizeof(MAPoint2d) * count);
		CHECK_INT_ALIGNMENT(points);
		MYASSERT(count >= 3, ERR_POLYGON_TOO_FEW_POINTS);
		for(int i = 2; i < count; i++) {
			currentDrawSurface->drawTriangle(
				points[i-2].x,
				points[i-2].y,
				points[i-1].x,
				points[i-1].y,
				points[i].x,
				points[i].y,
				realColor);
		}
	}

	SYSCALL(void, maFillTriangleFan(const MAPoint2d *points, int count)) {
		SYSCALL_THIS->ValidateMemRange(points, sizeof(MAPoint2d) * count);
		CHECK_INT_ALIGNMENT(points);
		MYASSERT(count >= 3, ERR_POLYGON_TOO_FEW_POINTS);
		for(int i = 2; i < count; i++) {
			currentDrawSurface->drawTriangle(
				points[0].x,
				points[0].y,
				points[i-1].x,
				points[i-1].y,
				points[i].x,
				points[i].y,
				realColor);
		}
	}

	SYSCALL(MAExtent, maGetTextSize(const char* str)) {
		SIZE s = TextOutput::getTextSize(str, false);
		return EXTENT(s.cx, s.cy);
	}

	SYSCALL(MAExtent, maGetTextSizeW(const char* str)) {
		SIZE s = TextOutput::getTextSize(str, true);
		return EXTENT(s.cx, s.cy);
	}

	SYSCALL(void, maDrawText(int left, int top, const char* str)) {
		TextOutput::drawText(currentDrawSurface, left, top, str, realColor, false);
	}

	SYSCALL(void, maDrawTextW(int left, int top, const wchar* str)) {
		TextOutput::drawText(currentDrawSurface, left, top, str, realColor, true);
	}

#define WHITE 0xffffff
#define BLACK 0
#define PERIOD_MS (1000)
	void Syscall::drawSplash() {
		// decode message ("mosync non-commercial")
		char str[64];
		int n;
		int msg[] = 
		{
			0x8814,0xccd5,0x155d8,0x231e,0xac31,0x13544,0x471c,0xadae,
			0xd04e,0xaeac,0x204c5,0x1383e,0xd24a,0x8e87,0x8f06,0x17e7c,
			0x13aa9,0x13b38,0x2087d,0xf7f4,0x6fdf,0
		};

		for(n=0;n<64;n++)
		{
			// Decode
			if(!msg[n])
				break;
			str[n] = ((msg[n]-(127*n))*8191)%131071;
		}
		str[n] = 0;

		// show mosync non-commercial text
		int startTime = maGetMilliSecondCount();
		while(maGetMilliSecondCount() < startTime + PERIOD_MS) 
		{
			ClipRect oldRect = currentDrawSurface->clipRect;
			currentDrawSurface->clipRect.x = 0;
			currentDrawSurface->clipRect.y = 0;
			currentDrawSurface->clipRect.width = backBuffer->width;
			currentDrawSurface->clipRect.height = backBuffer->height;
			TextOutput::drawText(currentDrawSurface, 0, 0, str, BLACK, false);
			TextOutput::drawText(currentDrawSurface, 1, 1, str, WHITE, false);
			currentDrawSurface->clipRect = oldRect;
			maUpdateScreen();
		}

		maSetColor(0);
		maFillRect(0, 0, backBuffer->width, backBuffer->height);
	}

	SYSCALL(void, maUpdateScreen()) {
		if(gClosing)
			return;
		MAProcessEvents();
		MAUpdateScreen();
	}

	SYSCALL(void, maResetBacklight()) {
		// send a non-existing key to trigger the backlight timer :)
		keybd_event(VK_F24, 0, KEYEVENTF_KEYUP, 0);
	}

	SYSCALL(MAExtent, maGetScrSize()) {
		return EXTENT(backBuffer->width, backBuffer->height);
	}

	SYSCALL(void, maDrawImage(MAHandle image, int left, int top)) {
		Image* img = gSyscall->resources.get_RT_IMAGE(image);	
		currentDrawSurface->drawImage(left, top, img);
	}

	SYSCALL(void, maDrawRGB(const MAPoint2d* dstPoint, const void* src, const MARect* srcRect,
		int scanlength)) {
		gSyscall->ValidateMemRange(dstPoint, sizeof(MAPoint2d));
		gSyscall->ValidateMemRange(srcRect, sizeof(MARect));
		gSyscall->ValidateMemRange(src, scanlength*srcRect->height*4);

		unsigned char *alpha = new unsigned char[srcRect->width*srcRect->height];
		unsigned char *img = new unsigned char[srcRect->width*srcRect->height*
			currentDrawSurface->bytesPerPixel];
		unsigned int *src_ptr = (unsigned int*) src;
		unsigned char *alpha_ptr = (unsigned char*) alpha;
		unsigned char *img_ptr = (unsigned char*) img;

		switch(currentDrawSurface->bytesPerPixel)
		{
			case 2:
			for(int j = 0; j < srcRect->height; j++) {
				for(int i = 0; i < srcRect->width; i++) {
					*((unsigned short*)img_ptr) = (unsigned short) CONVERT_TO_NATIVE_COLOR_FROM_ARGB(*src_ptr);
					*alpha_ptr = ((*src_ptr)>>24)&0xff;
					alpha_ptr++;
					img_ptr+=currentDrawSurface->bytesPerPixel;
					src_ptr++;
				}
				src_ptr += -srcRect->width + scanlength;
			}
			break;
			case 4:
			for(int j = 0; j < srcRect->height; j++) {
				for(int i = 0; i < srcRect->width; i++) {
					unsigned int col = CONVERT_TO_NATIVE_COLOR_FROM_ARGB(*src_ptr);
					*((unsigned int*)img_ptr) = (unsigned int) CONVERT_TO_NATIVE_COLOR_FROM_ARGB(*src_ptr);
					*alpha_ptr = ((*src_ptr)>>24)&0xff;
					alpha_ptr++;
					img_ptr+=currentDrawSurface->bytesPerPixel;
					src_ptr++;
				}
				src_ptr += -srcRect->width + scanlength;
			}
			break;
			default:
				BIG_PHAT_ERROR(WCEERR_UNSUPPORTED_BPP);
		}

		Image *image = new Image(img, alpha, srcRect->width, srcRect->height, srcRect->width*currentDrawSurface->bytesPerPixel, pixelFormat, false, true);
		currentDrawSurface->drawImage(dstPoint->x, dstPoint->y, image);
		delete image;
	}

	SYSCALL(void, maDrawImageRegion(MAHandle image, const MARect* src, const MAPoint2d* dstTopLeft,
		int transformMode))
	{
		gSyscall->ValidateMemRange(dstTopLeft, sizeof(MAPoint2d));
		gSyscall->ValidateMemRange(src, sizeof(MARect));	
		Image* img = gSyscall->resources.get_RT_IMAGE(image);
		ClipRect srcRect = {src->left, src->top, src->width, src->height};
		currentDrawSurface->drawImageRegion(dstTopLeft->x, dstTopLeft->y, &srcRect, img, transformMode);
	}

	SYSCALL(MAExtent, maGetImageSize(MAHandle image)) {
		Image* img = gSyscall->resources.get_RT_IMAGE(image);
		return EXTENT(img->width, img->height);
	}

#define BIG_PHAT_SOURCE_RECT_ERROR {BIG_PHAT_ERROR(WCEERR_SOURCE_RECT_OOB)}

	SYSCALL(void, maGetImageData(MAHandle image, void* dst, const MARect* src, int scanlength)) {
		gSyscall->ValidateMemRange(src, sizeof(MARect));
		Image* img = gSyscall->resources.get_RT_IMAGE(image);
		int x = src->left;
		int y = src->top;
		int width = src->width;
		int height = src->height;
		gSyscall->ValidateMemRange(dst, src->height*scanlength);

		if(x < img->clipRect.x) {BIG_PHAT_SOURCE_RECT_ERROR;}
		else if(x > img->clipRect.x + img->clipRect.width) {BIG_PHAT_SOURCE_RECT_ERROR;}
		if(y < img->clipRect.y) {BIG_PHAT_SOURCE_RECT_ERROR;}
		else if(y > img->clipRect.y + img->clipRect.height) {BIG_PHAT_SOURCE_RECT_ERROR;}
		if(x + width < img->clipRect.x) {BIG_PHAT_SOURCE_RECT_ERROR;}
		else if(x + width > img->clipRect.x + img->clipRect.width) {BIG_PHAT_SOURCE_RECT_ERROR;}
		if(y+height < img->clipRect.y) {BIG_PHAT_SOURCE_RECT_ERROR;}
		else if(y+height > img->clipRect.y + img->clipRect.height) {BIG_PHAT_SOURCE_RECT_ERROR;}

		if(width < 0 || height < 0) return;

		unsigned int *dst_ptr = (unsigned int*) dst;
		unsigned char* src_ptr = (unsigned char*)&img->data[x*img->bytesPerPixel + y*img->pitch];

		bool has_alpha = false;
		unsigned char* src_alpha;
		if(img->alpha) {
			has_alpha = true;
			src_alpha = &img->alpha[x + y*img->width];
		}

		switch(img->bytesPerPixel) {
			case 2:
				{
					unsigned short* src_scan;
					for(int j = 0; j < height; j++) {
						src_scan = (unsigned short*) src_ptr;
						unsigned char *alpha_scan;
						if(has_alpha) 
							alpha_scan = &src_alpha[j*img->width];
						for(int i = 0; i < width; i++) {
							int alpha = 0xff << 24;
							if(has_alpha) {
								alpha = alpha_scan[i];
								alpha <<= 24;
							}
							*dst_ptr = CONVERT_TO_ARGB_FROM_NATIVE_COLOR(*src_scan)|alpha;
							src_scan++;
							dst_ptr++;
						}
						src_ptr+=img->pitch;
						dst_ptr+=-width+scanlength; 
					}
				}
				break;
			case 4:
				{
					unsigned int* src_scan;
					for(int j = 0; j < height; j++) {
						src_scan = (unsigned int*) src_ptr;
						unsigned char *alpha_scan;
						if(has_alpha) 
							alpha_scan = &src_alpha[j*img->width];
						for(int i = 0; i < width; i++) {
							int alpha = 0xff << 24;
							if(has_alpha) {
								alpha = alpha_scan[i];
								alpha <<= 24;
							}
							*dst_ptr = CONVERT_TO_ARGB_FROM_NATIVE_COLOR(*src_scan)|alpha;
							src_scan++;
							dst_ptr++;
						}
						src_ptr+=img->pitch;
						dst_ptr+=-width+scanlength; 
					}
				}
				break;
			default:
				BIG_PHAT_ERROR(WCEERR_UNSUPPORTED_BPP);
		}
	}

	SYSCALL(MAHandle, maSetDrawTarget(MAHandle handle)) {
		MAHandle temp = drawTargetHandle;
		if(drawTargetHandle != HANDLE_SCREEN) {
			SYSCALL_THIS->resources.extract_RT_FLUX(drawTargetHandle);
			ROOM(SYSCALL_THIS->resources.add_RT_IMAGE(drawTargetHandle, currentDrawSurface));
			drawTargetHandle = HANDLE_SCREEN;
		}
		if(handle == HANDLE_SCREEN) {
			currentDrawSurface = backBuffer;
		} else {
			Image* img = SYSCALL_THIS->resources.extract_RT_IMAGE(handle);
			currentDrawSurface = img; 
			ROOM(SYSCALL_THIS->resources.add_RT_FLUX(handle, NULL));
		}
		drawTargetHandle = handle;
		return temp;
	}

	SYSCALL(int, maCreateImageFromData(MAHandle placeholder, MAHandle data, int offset, int size)) {
		MYASSERT(size>0, ERR_DATA_OOB);
		Stream* stream = gSyscall->resources.get_RT_BINARY(data);
		Image *bitmap = 0;
		HRESULT hresult;

		if(!stream->ptrc()) {
			// is not a memstream, read it to a buffer and load it.
			MYASSERT(stream->seek(Seek::Start, offset), ERR_DATA_OOB);
			unsigned char *data = new unsigned char[size];
			if(data==NULL) return RES_OUT_OF_MEMORY;

			MYASSERT(stream->read(data, size), ERR_DATA_ACCESS_FAILED);
			hresult = loadImageFromStream(&bitmap, (const void*)data, size);
			delete data;
		} else {
			byte* data = &((byte*)stream->ptrc())[offset];
			hresult = loadImageFromStream(&bitmap, data, size);
		}
		if(hresult == E_OUTOFMEMORY) return RES_OUT_OF_MEMORY;
		else if(hresult == E_INVALIDARG || hresult == E_POINTER) return RES_BAD_INPUT;
		else if(hresult != S_OK) return -3;

		return gSyscall->resources.add_RT_IMAGE(placeholder, bitmap);
	}

	SYSCALL(int, maCreateImageRaw(MAHandle placeholder, const void* src, MAExtent size, int processAlpha)) {
		int width = EXTENT_X(size); int height = EXTENT_Y(size);
		gSyscall->ValidateMemRange(src, width*height*4);
		
		unsigned char *alpha = NULL;
		if(processAlpha) {
			alpha = new unsigned char[width*height];
			if(alpha==NULL) return RES_OUT_OF_MEMORY;
		}
		unsigned int *src_ptr = (unsigned int*)src;
		void *data = 0;
		unsigned char *alpha_ptr = alpha;
		switch(backBuffer->bytesPerPixel) {
			case 2:
				{
					unsigned short *dst_ptr = new unsigned short[width*height];
					if(dst_ptr==NULL) return RES_OUT_OF_MEMORY;

					data = dst_ptr;	
					for(int j = 0; j < height; j++) {
						for(int i = 0; i < width; i++) {
							*dst_ptr++ = CONVERT_TO_NATIVE_COLOR_FROM_ARGB(*src_ptr);
							if(processAlpha) {
								*alpha_ptr++ = ((*src_ptr)&0xff000000)>>24;
							}
							src_ptr++;
						}
					}
				}
				break;
			case 4:
				{
					unsigned int *dst_ptr = new unsigned int[width*height];
					if(dst_ptr==NULL) return RES_OUT_OF_MEMORY;

					data = dst_ptr;		
					for(int j = 0; j < height; j++) {
						for(int i = 0; i < width; i++) {
							*dst_ptr++ = CONVERT_TO_NATIVE_COLOR_FROM_ARGB(*src_ptr);
							if(processAlpha) {
								*alpha_ptr++ = ((*src_ptr)&0xff000000)>>24;
							}
							src_ptr++;
						}
					}
				}
				break;
			default:
				BIG_PHAT_ERROR(WCEERR_UNSUPPORTED_BPP);
		}
		Image *newImage = new Image((unsigned char*)data, alpha, width, height,
			width*backBuffer->bytesPerPixel, backBuffer->pixelFormat, false, true);
		if(newImage==NULL) return RES_OUT_OF_MEMORY;

		return gSyscall->resources.add_RT_IMAGE(placeholder, newImage);
	}

	SYSCALL(int, maCreateDrawableImage(MAHandle placeholder, int width, int height)) {
		MYASSERT(width > 0 && height > 0, ERR_IMAGE_SIZE_INVALID);
		Image *i = new Image(width, height, width*backBuffer->bytesPerPixel, backBuffer->pixelFormat);
		if(i==NULL) return RES_OUT_OF_MEMORY;
		if(!(i->hasData())) { delete i; return RES_OUT_OF_MEMORY; }

		return gSyscall->resources.add_RT_IMAGE(placeholder, i);
	}

	SYSCALL(int, maGetKeys()) 
	{
		if(gClosing)
			return 0;
		MAProcessEvents();
		return currentKeyState;
	}


	SYSCALL(int, maGetEvent(MAEvent *dst)) 
	{
		gSyscall->ValidateMemRange(dst, sizeof(MAEvent));	
		MYASSERT(((uint)dst & 3) == 0, ERR_MEMORY_ALIGNMENT);	//alignment

		MAProcessEvents();

		if(!gClosing)
			gEventOverflow = false;
		if(gEventFifo.count() == 0)
			return 0;
		*dst = gEventFifo.get();

		#define HANDLE_CUSTOM_EVENT(eventType, dataType) if(dst->type == eventType) {\
			memcpy(Core::GetCustomEventPointer(gCore), dst->data, sizeof(dataType));\
			delete (dataType*)dst->data;\
			dst->data = (void*)(int(Core::GetCustomEventPointer(gCore)) - int(gCore->mem_ds)); }

		CUSTOM_EVENTS(HANDLE_CUSTOM_EVENT);

		return 1;
	}

	SYSCALL(void, maWait(int timeout)) 
	{
		if(gClosing)
			return;

		if(gEventFifo.count() != 0)
			return;

		unsigned int timerId = 0;
		if(timeout>0)
			timerId = SetTimer(NULL, 0, timeout, 0);

		while(true) {
			bool ret = MAProcessEvents();
			if(ret) {
				break;
			}
			if(gEventFifo.count() != 0)
				break;

			//MsgWaitForMultipleObjects(NULL, NULL, FALSE, INFINITE, QS_ALLEVENTS);

			if(winMobileEventHandles.size()>0) {
				DWORD retVal = MsgWaitForMultipleObjects(winMobileEventHandles.size(), &winMobileEventHandles[0], FALSE, INFINITE, QS_ALLEVENTS);
				if(retVal != WAIT_FAILED && retVal<WAIT_OBJECT_0+winMobileEventHandles.size()) {
					int index = retVal-WAIT_OBJECT_0;
					if(winMobileEventCallbacks[index]!=NULL) {
						winMobileEventCallbacks[index]();
					}
				}
			}
			else
				MsgWaitForMultipleObjects(NULL, NULL, FALSE, INFINITE, QS_ALLEVENTS);
		}

		if(timerId != 0)
			KillTimer(NULL, timerId);
	}

	// Microsoft defines FILETIME as the number of 100-nanoseconds since 
	// 1 january 1601 00:00:00
	// timeoffset to epoch equals 11644473600 seconds 
	// (yes I calculated that myself;)
	__int64 timeOffset = 11644473600;
	SYSCALL(int, maTime()) 
	{
		SYSTEMTIME st;
		FILETIME ft;
		LARGE_INTEGER li;  
		GetSystemTime(&st);
		SystemTimeToFileTime(&st, &ft);
		li.LowPart = ft.dwLowDateTime;
		li.HighPart = ft.dwHighDateTime;
		return (int)((li.QuadPart/10000000)-timeOffset);
	}

	SYSCALL(int, maLocalTime()) 
	{
		TIME_ZONE_INFORMATION tzi;
		int res = GetTimeZoneInformation(&tzi);
		MYASSERT(res != TIME_ZONE_ID_INVALID, WCEERR_TIMEZONE);
		return (int)(maTime() - (tzi.Bias * 60));
	}

	SYSCALL(int, maGetMilliSecondCount()) 
	{
		return GetTickCount();
	}

	SYSCALL(int, maFreeObjectMemory()) {
		MEMORYSTATUS ms;
		ms.dwLength = sizeof(MEMORYSTATUS);
		GlobalMemoryStatus(&ms);
		return ms.dwAvailPhys;
	}
	SYSCALL(int, maTotalObjectMemory()) {
		MEMORYSTATUS ms;
		ms.dwLength = sizeof(MEMORYSTATUS);
		GlobalMemoryStatus(&ms);
		return ms.dwTotalPhys;
	}

	SYSCALL(int, maVibrate(int ms)) 
	{
		if(ms==0) {
			return VibrationStop();
		} else {
			return VibrationStart(ms);
		}
	}

	SYSCALL(void, maPanic(int result, char* message)) 
	{
		int msgLen = strlen(message);
		WCHAR *wbuffer = new WCHAR[msgLen];
		convertAsciiToUnicode(wbuffer, msgLen, message);
		MessageBox(Base::g_hwndMain, wbuffer, TEXT("User Panic"), MB_ICONERROR|MB_OK);
		delete []wbuffer;
		MoSyncExit(result);
	}

	SYSCALL(int, maPlatformRequest(const char* url)) 
	{
		if(sstrcmp(url, "http://") == 0) {
			SHELLEXECUTEINFO sei;
			ZeroMemory(&sei, sizeof(SHELLEXECUTEINFO));
			sei.cbSize = sizeof(SHELLEXECUTEINFO);
			int strSize = strlen(url)+1;
			sei.lpFile = new TCHAR[strSize]; // memory leak?
			convertAsciiToUnicode((LPWSTR)sei.lpFile, strSize, url);
			sei.nShow = SW_SHOW;
			ShellExecuteEx(&sei);
			return 0;
		} else {
			return IOCTL_UNAVAILABLE;
		}
	}

	void readMimeString(char *mimeString, int len, Stream *src) 
	{
		byte b;
		int mimeIndex = 0;
		do {
			if(mimeIndex>len || !src->readByte(b)) {
				BIG_PHAT_ERROR(WCEERR_SOUND_MIME_CORRUPTED);
			}
			mimeString[mimeIndex++] = b;
		} while(b);
		mimeString[mimeIndex] = '\0';
	}

	/*
	MimeType getMimeType(const char *mimeString) 
	{
		if(	strcmp(mimeString, "audio/x-wav") == 0 ) {
			return MT_WAVE;
		} 
		else if ( strcmp(mimeString, "audio/basic") == 0 ) {
			return MT_AU;
		}
		else if ( strcmp(mimeString, "audio/mpeg") == 0 ) {
			return MT_MP3;
		} else if ( strcmp(mimeString, "audio/midi  ") == 0 ) {
			return MT_MIDI;
		}

		return MT_UNKNOWN;
	}
	*/

	SYSCALL(int, maSoundPlay(MAHandle sound_res, int offset, int size)) 
	{ 
		int chan = 0;
		Stream *res = gSyscall->resources.get_RT_BINARY(sound_res);
		char mimeString[256];
		MYASSERT(res->seek(Seek::Start, offset), ERR_DATA_ACCESS_FAILED);
		Stream* src = res->createLimitedCopy(size);
		MYASSERT(src, ERR_DATA_ACCESS_FAILED);
		readMimeString(mimeString, 256, src);
		//MimeType mimeType = getMimeType(mimeString);

		/*
		if(mimeType == MT_UNKNOWN) {
			//BIG_PHAT_ERROR(WCEERR_SOUND_UNKNOWN_MIME);
		}
		*/

		AudioChannel *chnl = AudioEngine::getChannel(chan);
		chnl->setActive(false);
		if(gCurrentSoundSource) {
			delete gCurrentSoundSource;
		}
		gCurrentSoundSource = src;

		AudioSource *audioSource = chnl->getAudioSource();
		if(audioSource!=NULL) {
			audioSource->close();
			delete audioSource;
		}
		audioSource = AudioEngine::getAudioSource(mimeString, src);
		if(!audioSource) return -1;

		chnl->setAudioSource(audioSource);
		chnl->setActive(true);

		/*
		int pos;
		src->tell(pos);

		if(gVolume == -1) {
			gVolume = 100;
			maSoundSetVolume(gVolume);
		}

		BOOL res;
		byte* lastSound = gCurrentSound;
		if(!src->ptrc()) {
			int encodedSize = size - pos + offset;
			gCurrentSound = new byte[encodedSize];
			src->read(gCurrentSound, encodedSize);
			res = sndPlaySound((LPWSTR)gCurrentSound, SND_ASYNC|SND_MEMORY);
		} else {
			gCurrentSound = NULL;
			res = sndPlaySound((LPWSTR)&(((byte*)src->ptrc())[pos]), SND_ASYNC|SND_MEMORY);
		}

		if(lastSound) {
			delete []lastSound;
		}

		if(res==TRUE) return 0;
		else return -1;
		*/

		/*
		Stream *copy = src->createLimitedCopy(-1);
		initChannel(chan, copy, mimeType);
		startChannel(chan);
		*/

		return 0;
	}

	SYSCALL(void, maSoundStop()) 
	{
		//PlaySound(NULL, NULL, 0);
		/*
		stopChannel(0);
		rewindChannel(0);
		*/
		AudioChannel *chnl = AudioEngine::getChannel(0);
		chnl->setActive(false);
		if(gCurrentSoundSource) {
			delete gCurrentSoundSource;
			gCurrentSoundSource = NULL;
		}
	}

	SYSCALL(int, maSoundIsPlaying()) 
	{
		//BOOL soundNotPlaying=sndPlaySound((LPWSTR)silentWave, SND_ASYNC|SND_MEMORY|SND_NOSTOP);
		//return !soundNotPlaying;
		//return soundIsPlaying(0);
		AudioChannel *chnl = AudioEngine::getChannel(0);
		return (int) chnl->isActive();
	}

	SYSCALL(void, maSoundSetVolume(int vol)) 
	{
		/*
		if(vol<0) vol = 0; 
		if(vol>100) vol = 100;

		//setVolume(0, vol);
		unsigned short cVol = (vol*0xffff)/100; 
		waveOutSetVolume(0, (cVol<<16)|cVol); 
		gVolume = vol;
		*/
		AudioChannel *chnl = AudioEngine::getChannel(0);
		chnl->setVolume((vol*0xffff)/100);
	}

	SYSCALL(int, maSoundGetVolume()) 
	{
		//return getVolume(0);
		//return gVolume;
		AudioChannel *chnl = AudioEngine::getChannel(0);
		return (chnl->getVolume()*100)/0xffff;
	}
	
	SYSCALL(int, maGetBatteryCharge()) 
	{
		SYSTEM_POWER_STATUS_EX status;
		if(GetSystemPowerStatusEx(&status, TRUE) == FALSE) {
			LOG("Could not get battery charge information\n");
			return -2;
		}
		if(status.BatteryLifePercent == 0xff) {
			return -2;
		}

		return status.BatteryLifePercent;
	}

	struct button_lock_in
	{
		DWORD set_or_get;
		DWORD set_value;
	};

	int isKeypadLocked = 0;
	int maKeypadIsLocked() 
	{
		/*
		button_lock_in in = {0x00000001, 0x00000000};
		DWORD result;
		BOOL success = KernelIoControl(0x01012A00, &in, sizeof(in), &result, sizeof(DWORD), NULL);
		return (int)result;
		*/
#if (_WIN32_WCE < 0x502)
		return IOCTL_UNAVAILABLE;
#else
		HWND foregroundWnd = GetForegroundWindow();
		if(foregroundWnd == g_hwndMain) {
			isKeypadLocked = 0;
			return 0;
		}

		BOOL success = SetForegroundWindow(g_hwndMain);
		if(success==TRUE)
			SetForegroundWindow(foregroundWnd);
		isKeypadLocked = success==FALSE;
        return isKeypadLocked;
#endif		
	}

	int maLockKeypad() 
	{
		/*
		button_lock_in in = {0x00000000, 0x00000001};
		BOOL success = KernelIoControl(0x01012A00, &in, sizeof(in), NULL, 0, NULL);
		return success==TRUE;
		*/
		if(maKeypadIsLocked()==0)
#if (_WIN32_WCE >= 0x502)
			SHDeviceLockAndPrompt();
			return 1;
#else
			keybd_event(VK_F24, 0, KEYEVENTF_SILENT, 0);
			//keybd_event(VK_APP6, 0, KEYEVENTF_SILENT, 0);
			return IOCTL_UNAVAILABLE;
#endif
	}

	int maUnlockKeypad() 
	{
		/*
		button_lock_in in = {0x00000000, 0x00000000};
		BOOL success = KernelIoControl(0x01012A00, &in, sizeof(in), NULL, 0, NULL);
		return success==TRUE;
		*/
#if (_WIN32_WCE >= 0x502)
		if(maKeypadIsLocked()==1) {
			SHDeviceLockAndPrompt();			
		}
		return 1;
#else
		return IOCTL_UNAVAILABLE;
#endif
	}



	int maSendTextSMS(const char* dst, const char* msg) {
		SMS_HANDLE hSms;
		SMS_ADDRESS smsAddress;
		SMS_MESSAGE_ID msgID;
		TCHAR ptsMessageProtocol[100];
		HRESULT res;
		HANDLE hSmsEvent;

		wcscpy(ptsMessageProtocol,SMS_MSGTYPE_TEXT);
		res = SmsOpen(ptsMessageProtocol, SMS_MODE_SEND, &hSms, &hSmsEvent);
		if(res!=S_OK) {
			LOG("SmsOpen failed, return code %d.\n", res);
			return -2;
		}
	
		
		DWORD dwReturn = WaitForSingleObject( hSmsEvent,  5000 );
		if(dwReturn == WAIT_ABANDONED || dwReturn == WAIT_TIMEOUT)
		{
		  LOG("In maSendTextSMS: WaitForSingleObject failed, return code %d.\n", dwReturn);
		  SmsClose(hSms);
		  return -2;  // never fails
		}

		smsAddress.smsatAddressType = SMSAT_UNKNOWN;
		convertAsciiToUnicode(smsAddress.ptsAddress, SMS_MAX_ADDRESS_LENGTH, dst);

		TEXT_PROVIDER_SPECIFIC_DATA txtProviderData;
		memset(&txtProviderData, 0, sizeof(TEXT_PROVIDER_SPECIFIC_DATA));
		txtProviderData.dwMessageOptions = PS_MESSAGE_OPTION_NONE;
		txtProviderData.psMessageClass = PS_MESSAGE_CLASS1;
		txtProviderData.psReplaceOption = PSRO_NONE;

		int messageDataLength = strlen(msg);
		LPWSTR messageData = new TCHAR[messageDataLength+1];
		//memcpy(messageData, msg, messageDataLength);
		convertAsciiToUnicode(messageData, messageDataLength+1, msg);

		res = SmsSendMessage(hSms, NULL, &smsAddress, NULL, 
							(byte*)messageData, messageDataLength*2,
							(byte*)&txtProviderData, sizeof(TEXT_PROVIDER_SPECIFIC_DATA), 
							SMSDE_GSM, SMS_OPTION_DELIVERY_NONE, &msgID);

		delete []messageData;

		if(res!=S_OK) {
			LOG("SmsSendMessage failed, return code %d.\n", res);
			SmsClose(hSms);
			return -2;
		}

		SmsClose(hSms);

		return 0;
	}

	void BtWaitTrigger() 
	{
		LOGD("BtWaitTrigger\n");
		MAEvent* ep = new MAEvent;
		ep->type = EVENT_TYPE_BT;
		ep->state = Bluetooth::maBtDiscoveryState();
		PostMessage(g_hwndMain, WM_ADD_EVENT, (WPARAM) ep, 0);
	}


	/*
	VideoStream defaultVideoStream = {0, NULL, NULL, NULL};
	std::map<MAHandle, VideoStream*> videoStreams;
	MAHandle currentHandle = 1;

#define LOG_HRESULT(x) LOG("%s return 0x%x\n", #x, x);

	MAHandle maStartVideoStream(const char *url) {
		VideoStream *stream = new VideoStream;
		*stream = defaultVideoStream;
		stream->handle = currentHandle; 
		videoStreams[currentHandle++] = stream;

		// Create the filter graph manager and render the file.
		CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (void **)&stream->pVideoGraph);
		

		char temp[256];
		getWorkingDirectory(temp, 256);
		strcat(temp, "\\");
		strcat(temp, url);
		WCHAR wurl[256];
		convertAsciiToUnicode(wurl, 256, temp);
		LOG_HRESULT(stream->pVideoGraph->RenderFile(wurl, NULL));

		// Specify the owner window.
		IVideoWindow    *pVidWin = NULL;
		LOG_HRESULT(stream->pVideoGraph->QueryInterface(IID_IVideoWindow, (void **)&pVidWin));

		LOG_HRESULT(pVidWin->put_Owner((OAHWND)g_hwndMain));
		LOG_HRESULT(pVidWin->put_WindowStyle( WS_CHILD | WS_CLIPSIBLINGS));

		// Set the owner window to receive event notices.
		LOG_HRESULT(stream->pVideoGraph->QueryInterface(IID_IMediaEventEx, (void **)&stream->pVideoEvent));
		LOG_HRESULT(stream->pVideoEvent->SetNotifyWindow((OAHWND)g_hwndMain, WM_GRAPHNOTIFY, (long)stream));

		// Run the graph.
		LOG_HRESULT(stream->pVideoGraph->QueryInterface(IID_IMediaControl, (void **)&stream->pVideoMediaControl));
		LOG_HRESULT(stream->pVideoMediaControl->Run());

		return stream->handle;
	}
	*/

	/*
	void VideoCleanUp(VideoStream *stream)
	{
		IVideoWindow    *pVidWin = NULL;
		stream->pVideoGraph->QueryInterface(IID_IVideoWindow, (void **)&pVidWin);
		pVidWin->put_Visible(OAFALSE);
		pVidWin->put_Owner(NULL);

		// Stop the graph.
		stream->pVideoMediaControl->Stop();

		long evCode;
		stream->pVideoEvent->WaitForCompletion(INFINITE, &evCode);

		stream->pVideoMediaControl->Release();
		stream->pVideoEvent->Release();
		stream->pVideoGraph->Release();

		videoStreams.erase(stream->handle);
		delete stream;
	}
	*/

	/*
	void VideoHandleEvent(VideoStream *stream) 
	{
		long evCode, param1, param2;
		HRESULT hr;
		while (hr = stream->pVideoEvent->GetEvent(&evCode, &param1, &param2, 0), SUCCEEDED(hr))
		{ 
			hr = stream->pVideoEvent->FreeEventParams(evCode, param1, param2);
			if ((EC_COMPLETE == evCode) || (EC_USERABORT == evCode))
			{ 
				VideoCleanUp(stream);
				break;
			} 
		} 
	}
	*/

	/**
	* Pauses the stream. Does nothing if the stream is already paused.
	*/
	/*
	void maPauseStream(MAHandle stream) {
		VideoStream *vstream = videoStreams[stream];
		if(vstream->pVideoMediaControl)
			vstream->pVideoMediaControl->Pause();
	}
	*/

	/**
	* Resumes the stream. Does nothing if the stream is already playing.
	*/
	/*
	void maResumeStream(MAHandle stream) {
		VideoStream *vstream = videoStreams[stream];
		if(vstream->pVideoMediaControl)
			vstream->pVideoMediaControl->Run();
	}
	*/

	/**
	* Closes the stream, releasing resources and rendering the handle invalid.
	*/
	/*
	void maCloseStream(MAHandle stream) {
		VideoStream *vstream = videoStreams[stream];
		VideoCleanUp(vstream);
	}
	*/

	SYSCALL(int, maFrameBufferGetInfo(MAFrameBufferInfo *info)) {
		info->bitsPerPixel = backBuffer->bitsPerPixel;
		info->bytesPerPixel = backBuffer->bytesPerPixel;
		info->redMask = backBuffer->redMask;
		info->greenMask = backBuffer->greenMask;
		info->blueMask = backBuffer->blueMask;
		info->sizeInBytes = backBuffer->pitch*backBuffer->height;
		info->width = backBuffer->width;
		info->height = backBuffer->height;
		info->pitch = backBuffer->pitch;
		info->redShift = backBuffer->redShift;
		info->greenShift = backBuffer->greenShift;
		info->blueShift = backBuffer->blueShift;
		info->redBits = backBuffer->redBits;
		info->greenBits = backBuffer->greenBits;
		info->blueBits = backBuffer->blueBits;
		info->supportsGfxSyscalls = 1;

		return 1;
	}

	SYSCALL(int, maFrameBufferInit(const void* addr)) {
		gSyscall->ValidateMemRange(addr, backBuffer->pitch*backBuffer->height);
		if(sInternalBackBuffer!=NULL) return 0;
		sInternalBackBuffer = backBuffer;
		backBuffer = new Image((unsigned char*)addr, NULL, backBuffer->width,
			backBuffer->height, backBuffer->pitch, backBuffer->pixelFormat, false, false);
		currentDrawSurface = backBuffer;
		return 1;
	}

	SYSCALL(int, maFrameBufferClose()) {
		if(sInternalBackBuffer==NULL) return 0;
		delete backBuffer;
		backBuffer = sInternalBackBuffer;
		sInternalBackBuffer = NULL;
		currentDrawSurface = backBuffer;
		return 1;
	}

	void fillBufferCallback() {
		MAEvent* ep = new MAEvent;
		ep->type = EVENT_TYPE_AUDIOBUFFER_FILL;
		PostMessage(g_hwndMain, WM_ADD_EVENT, (WPARAM) ep, 0);
	}

	static int maAudioBufferInit(const MAAudioBufferInfo *ainfo) {
		AudioSource *src = AudioEngine::getChannel(1)->getAudioSource();
		if(src!=NULL) {
			src->close();
			delete src;
		}
		src = new BufferAudioSource(ainfo, fillBufferCallback);
		AudioEngine::getChannel(1)->setAudioSource(src);
		AudioEngine::getChannel(1)->setActive(true);

		return 1;
	}

	static int maAudioBufferReady() {
		BufferAudioSource* as = (BufferAudioSource*)AudioEngine::getChannel(1)->getAudioSource();
		as->ready();
		return 1;
	}

	static int maAudioBufferClose() {

		AudioSource *src = AudioEngine::getChannel(1)->getAudioSource();
		if(src!=NULL) {
			src->close();
			delete src;
		}
		AudioEngine::getChannel(1)->setAudioSource(NULL);
		return 1;
	}


	/*
	MoSyncSemaphore *gAudioSem = NULL;
	void *gMosyncAudioBuffer;
	void maAudioBufferFillCallback(void *pBuffer,long bufferLen) {
		MAEVENT audioEvent;
		audioEvent.type = EVENT_TYPE_AUDIOBUFFER_FILL;
		audioEvent.value = bufferLen;
		gEventFifo.put(audioEvent);	
		gAudioSem->wait();
		memcpy(pBuffer, gMosyncAudioBuffer, bufferLen);
	}

	SYSCALL(int, maAudioBufferInit(MAAudioBufferInfo *info)) {
		closeAudio();
		gMosyncAudioBuffer = gSyscall->GetValidatedMemRange((int)info->buffer, info->bufferSize);
		gAudioSem = new MoSyncSemaphore();
		if(!openAudio(info->sampleRate, info->bitDepth, info->numChannels, info->bufferSize, maAudioBufferFillCallback)) {
			LOG("Audio could not be initialized.\n");
			return 0; 
		}

		return 1;
	}

	SYSCALL(int, maAudioBufferReady()) {
		gAudioSem->post();
		return 1;
	}

	SYSCALL(int, maAudioBufferClose()) {
		closeAudio();
		if(!openAudio(44100, 16, 2)) {
			LOG("Audio could not be initialized.\n");
			return 0;
		}

		delete gAudioSem;
		gAudioSem = NULL;
		return 1;
	}


#define MAX_ACCESS_POINTS 1024
	class WlanDiscovery : public Runnable {
	public:
		WlanDiscovery() {
			InitializeCriticalSection(&accessPointsCS);
		}

		void putEvent(int state) {
			MAEVENT e;
			e.type = EVENT_TYPE_WLAN;
			e.state = state;
			Base::gEventFifo.put(e);
		}

		void putErrorEvent() {
			putEvent(-1);
		}

		void setAccessPointsReady(bool b) {
			CriticalSectionHandler ccect(&accessPointsCS);
			accessPointsReady = b;
		}

		bool isAccessPointsReady() {
			CriticalSectionHandler ccect(&accessPointsCS);
			return accessPointsReady;
		}

		void run() {
			WCHAR adapters[1024];
			DWORD bufSize;	
			setAccessPointsReady(false);
			if(!wifiPeeker.GetAdapters(adapters, bufSize)) {
				putErrorEvent();
				return;
			}
			if(!bufSize) {
				putErrorEvent();
				return;
			}
			WCHAR *adapter = adapters;
			while(*adapter != ',' && *adapter != NULL) adapter++;
			*adapter = NULL;
			adapter = adapters;

			DWORD sizeOfAccessPointsArray = (DWORD)MAX_ACCESS_POINTS*sizeof(BSSIDInfo);
			if(!wifiPeeker.RefreshBSSIDs(adapter) || !wifiPeeker.GetBBSIDs(adapter, accessPoints, sizeOfAccessPointsArray, numAccessPoints)) {
				putErrorEvent();
				return;
			}
	
			setAccessPointsReady(true);
			putEvent(numAccessPoints);
		}

		int getNewAccessPoint(MA_WLAN_AP* dst) {
			if(!isAccessPointsReady()) return 0;
			numAccessPoints--;
			for(int i = 0; i < 6; i++)
				dst->address.a[i] = accessPoints[numAccessPoints].BSSID[i];
			dst->signalStrength = accessPoints[numAccessPoints].RSSI;
			for(int i = 0; i < MIN(dst->nameBufSize, 32); i++) {
				dst->name[i] = (char)accessPoints[numAccessPoints].SSID[i];
			}
			if(numAccessPoints == 0) 
			{
				setAccessPointsReady(false);
			} 

			return 1;
		}

	private:
		BSSIDInfo accessPoints[MAX_ACCESS_POINTS];
		DWORD numAccessPoints;

		CRITICAL_SECTION accessPointsCS;
		bool accessPointsReady;

		CWifiPeek wifiPeeker;
	};

	WlanDiscovery *discovery = NULL;
	int maWlanStartDiscovery() {
		if(discovery == NULL) discovery = new WlanDiscovery();
		gThreadPool.execute(discovery);
		return 0;
	}

	int maWlanGetNewAp(MA_WLAN_AP* dst) {
		char* vmName = dst->name;
		dst->name = (char*)SYSCALL_THIS->GetValidatedMemRange((int)dst->name, dst->nameBufSize);
		int res = discovery->getNewAccessPoint(dst);
		dst->name = vmName;
		return res;	
	}
*/

	SYSCALL(int, maInvokeExtension(int, int, int, int)) {
		BIG_PHAT_ERROR(ERR_FUNCTION_UNIMPLEMENTED);
	}

#ifdef MA_PROF_SUPPORT_LOCATIONAPI
	HANDLE gpsEvent = NULL;
	HANDLE gpsProviderEvent = NULL;
	HANDLE gpsDevice = NULL;

#define fixDoubleEndian(x) (x)
	/*
	double fixDoubleEndian(double d) {
		int *dd = (int*)&d;
		int temp = (*(dd+1));
		*(dd+1) = *dd;
		*dd = temp;
		return *((double*)dd);
	}
	*/

	void locCallback() {
		//int posDataSize = MAX(sizeof(GPS_POSITION), 376);
		//byte *posData = new byte[posDataSize];			
		byte posData[MAX(sizeof(GPS_POSITION), 376)];
		//byte posData[sizeof(GPS_POSITION)];
		int posDataSize = sizeof(posData);
		GPS_POSITION *pos = (GPS_POSITION*)posData;
		memset(posData, 0, posDataSize);
		pos->dwVersion = GPS_VERSION_1;
		pos->dwSize = posDataSize; // 376

		MALocation posEventData = {0};
		MAEvent posEvent;
		posEvent.type = EVENT_TYPE_LOCATION;

retry:
		if(GPSGetPosition(gpsDevice, pos, 1000, 0) == ERROR_SUCCESS) {
			if(pos->dwValidFields&(GPS_VALID_LATITUDE|GPS_VALID_LONGITUDE)) 
			{
				posEventData.lat = fixDoubleEndian(pos->dblLatitude);
				posEventData.lon = fixDoubleEndian(pos->dblLongitude);
				
				if(pos->dwValidFields&GPS_VALID_HORIZONTAL_DILUTION_OF_PRECISION)
					posEventData.horzAcc = fixDoubleEndian(pos->flHorizontalDilutionOfPrecision);

				if(pos->dwValidFields&GPS_VALID_VERTICAL_DILUTION_OF_PRECISION)
					posEventData.vertAcc = fixDoubleEndian(pos->flVerticalDilutionOfPrecision);

				if(pos->dwValidFields&GPS_VALID_ALTITUDE_WRT_ELLIPSOID)
					posEventData.alt = pos->flAltitudeWRTEllipsoid;
				
				if(pos->FixQuality == GPS_FIX_QUALITY_UNKNOWN) {
					posEventData.state = MA_LOC_UNQUALIFIED;
				} else {
					posEventData.state = MA_LOC_QUALIFIED;
				}
			} else {
				posEventData.state = MA_LOC_INVALID;
			}
		} else {
			if(pos->dwSize != 376) {
				pos->dwSize = 376; 
				goto retry;
			}
			posEventData.state = MA_LOC_INVALID;
		}

		MALocation *data = new MALocation;
		*data = posEventData;
		posEvent.data = data;

		gEventFifo.put(posEvent);

		//delete posData;
	}

	void lpsCallback() {
		GPS_DEVICE device = {0};
		device.dwVersion = GPS_VERSION_1;
		device.dwSize = sizeof(device);

		MAEvent lpsEvent;
		lpsEvent.type = EVENT_TYPE_LOCATION_PROVIDER;
		if(GPSGetDeviceState(&device)==ERROR_SUCCESS) {
			switch(device.dwDeviceState) {
				case SERVICE_STATE_ON: lpsEvent.state = MA_LPS_AVAILABLE; break;
				case SERVICE_STATE_OFF:  lpsEvent.state = MA_LPS_OUT_OF_SERVICE; break;
				default: lpsEvent.state = MA_LPS_TEMPORARILY_UNAVAILABLE; break;
			}
		} else {
			lpsEvent.state = MA_LPS_OUT_OF_SERVICE;
		}
		gEventFifo.put(lpsEvent);
	}

	BOOL hasGPS ()
	{
		HKEY hkey;
		TCHAR driver[256];

		if ( RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("System\\CurrentControlSet\\GPS Intermediate Driver\\Drivers"), 0, 0, &hkey) == ERROR_SUCCESS )
		{
			DWORD strLength;
			DWORD type = REG_SZ;
			strLength = sizeof( driver );

			LONG result = RegQueryValueEx( hkey, _T("CurrentDriver"), NULL, &type, (PBYTE)&driver, &strLength );
			RegCloseKey(hkey);

			if ( (result != ERROR_SUCCESS) || (strLength == 0) )
			{
				// No GPS is registered.
				return FALSE;
			}
			else
			{
				// A GPS is available.
				return TRUE;
			}
		}
		else 
		{
			// No GPS available
			return FALSE;
		}
	}



	int maLocationStart() {
		if(gpsDevice != NULL)
			return 0;

		if(hasGPS() == FALSE) 
			return MA_LPS_OUT_OF_SERVICE;

		if((gpsEvent = CreateEvent(NULL, FALSE, FALSE, L"GPSEVENT")) == NULL) {
			return MA_LPS_OUT_OF_SERVICE;
		}

		if((gpsProviderEvent = CreateEvent(NULL, FALSE, FALSE, L"GPSPROVIDEREVENT")) == NULL) {
			CloseHandle(gpsEvent);
			return MA_LPS_OUT_OF_SERVICE;
		}
		
		if((gpsDevice = GPSOpenDevice(gpsEvent, gpsProviderEvent, NULL, 0)) == NULL) {
			CloseHandle(gpsEvent);
			CloseHandle(gpsProviderEvent);
			return MA_LPS_OUT_OF_SERVICE;
		}

		/*
		if(device.dwDeviceState == SERVICE_STATE_OFF) {
			GPSCloseDevice(gpsDevice);
			CloseHandle(gpsEvent);
			gpsDevice = NULL;
			gpsEvent = NULL;
			return MA_LPS_OUT_OF_SERVICE;
		}
		*/

		addWinMobileEvent(gpsEvent, locCallback);
		addWinMobileEvent(gpsProviderEvent, lpsCallback);

		return MA_LPS_AVAILABLE;
	}

	int maLocationStop() {
		if(gpsDevice==NULL) return 0;

		if(GPSCloseDevice(gpsDevice) != ERROR_SUCCESS) {
			// couldn't close gpsDevice
		}

		removeWinMobileEvent(gpsEvent);
		removeWinMobileEvent(gpsProviderEvent);
		CloseHandle(gpsEvent);
		CloseHandle(gpsProviderEvent);
		gpsDevice = NULL;
		gpsEvent = NULL;

		return 0;
	}
#endif

	int maWriteLog(const void* a, int b) {
		gSyscall->ValidateMemRange(a, b);
		LOGBIN(a, b);
		return 0;
	}

	static int maGetSystemProperty(const char* key, char* buf, int size) {
		if(strcmp(key, "mosync.iso-639-1") == 0) {
			LCID lcid = GetUserDefaultLCID();
			WCHAR wbuf[16];

			int res = GetLocaleInfo(lcid, LOCALE_SISO639LANGNAME, wbuf, sizeof(wbuf)/sizeof(wbuf[0]));
			if(!res) {
				// use shaky fallback:
				// according to documentation of LOCAL_SABBREVLANGNAME:
				// Abbreviated name of the language. In most cases, the name is created by taking 
				// the two-letter language abbreviation from ISO Standard 639 and adding a third letter, 
				// as appropriate, to indicate the sublanguage. For example, the abbreviated name for 
				// the language corresponding to the English (United States) locale is ENU.
				res = GetLocaleInfo(lcid, LOCALE_SABBREVLANGNAME, wbuf, sizeof(wbuf)/sizeof(wbuf[0]));
				if(!res) return -2;
				wbuf[0] = towlower(wbuf[0]);
				wbuf[1] = towlower(wbuf[1]);
				wbuf[2] = 0;
				res-=1;
			}
			if(res > size)
				return res;
			size_t sres = wcstombs(buf, wbuf, size);
			DEBUG_ASSERT(sres == res-1);
			return res;
		} else if(strcmp(key, "mosync.winmobile.locale.SABBREVLANGNAME") == 0) {
			LCID lcid = GetUserDefaultLCID();
			WCHAR wbuf[16];
			int res = GetLocaleInfo(lcid, LOCALE_SABBREVLANGNAME, wbuf, sizeof(wbuf)/sizeof(wbuf[0]));
			if(!res) return -2;
			if(res > size)
				return res;
			size_t sres = wcstombs(buf, wbuf, size);
			DEBUG_ASSERT(sres == res-1);
			return res;
		}

		return -2;
	}

	//*****************************************************************************
	// maTextBox
	//*****************************************************************************
	static HWND sTextBoxContainer = NULL, sEditBox = NULL;
	static wchar_t* sTextBoxOutBuf;
	static int sTextBoxOutSize;

	static LRESULT CALLBACK TextBoxWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		static SHACTIVATEINFO sai;
		LOGD("TextBoxWndProc(0x%04x, 0x%08x, 0x%08x)\n", msg, wParam, lParam);
		switch (msg) {
		case WM_CREATE:
			// Initialize the static window state information. The shell helper functions
			// will use this buffer to store their state.
			ZeroMemory(&sai, sizeof(sai));
			sai.cbSize = sizeof(sai);
			ShowWindow(hwnd, SW_MAXIMIZE);
			UpdateWindow(hwnd);
			return TRUE;
		case WM_ACTIVATE:
			// Calling this function when we get WM_ACTIVATE ensures that our
			// application will handle the SIP properly. This function does
			// nothing when we're running on Smartphone.
			SHHandleWMActivate(hwnd, wParam, lParam, &sai, 0);
			break;
		case WM_SETTINGCHANGE:
			// This helper function will resize our main application window when the SIP
			// goes up and down. Try commenting out this function and see how it affects
			// our drawing code. This function is optional, so choose whichever behavior
			// you prefer. Again, this function does nothing on Smartphone.
			SHHandleWMSettingChange(hwnd, wParam, lParam, &sai);
			break;
		case WM_SIZE:
			{
				int width = LOWORD(lParam);
				int height = HIWORD(lParam);
				SetWindowPos(sEditBox, NULL, 0, 0, width, height, 0);
				UpdateWindow(hwnd);
			}
			break;
		case WM_COMMAND:
			{
				WORD id = LOWORD(wParam);
				if(id == IDOK || id == IDCANCEL) {
					// get text
					int res = GetWindowText(sEditBox, sTextBoxOutBuf, sTextBoxOutSize);

					// send event
					MAEvent e;
					e.type = EVENT_TYPE_TEXTBOX;
					e.textboxResult = (id == IDOK) ? MA_TB_RES_OK : MA_TB_RES_CANCEL;
					e.textboxLength = res;
					gEventFifo.put(e);

					// time to close
					LOG("DestroyWindow\n");
					GLE(DestroyWindow(hwnd));
				}
			}
			break;
		case WM_DESTROY:
			sEditBox = sTextBoxContainer = NULL;
			Base::Resume();
			Base::InitFullScreen();
			break;
		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	static void textBoxEventHook(UINT msg, WPARAM wParam, LPARAM lParam) {
		if(sTextBoxContainer) {
			LOGD("textBoxEventHook(0x%04x, 0x%08x, 0x%08x)\n", msg, wParam, lParam);
			switch(msg) {
			case WM_KEYDOWN:
#if 0	// no effect
				SendMessage(SHFindMenuBar(sTextBoxContainer), msg, wParam, lParam);
#else
				if(wParam == VK_F1)
					TextBoxWndProc(sTextBoxContainer, WM_COMMAND, IDOK, 0);
				if(wParam == VK_F2)
					TextBoxWndProc(sTextBoxContainer, WM_COMMAND, IDCANCEL, 0);
				if(wParam == VK_ESCAPE)
					SendMessage(sEditBox, WM_CHAR, VK_BACK, 0);
#endif
			}
		}
	}

	static int maTextBox(const wchar* title, const wchar* inText, wchar* outText,
		int maxSize, int constraints)
	{
		LOG("maTextBox\n");
#if 1
		if(sTextBoxContainer != NULL) {
			// textbox is already active, return error.
			return -2;
		}
		DEBUG_ASSERT(sizeof(wchar) == sizeof(wchar_t));
		sTextBoxOutSize = maxSize;
		sTextBoxOutBuf = (wchar_t*)outText;

		// create overlay window
		// window class
		WNDCLASS wc;
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = TextBoxWndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = g_hInst;
		wc.hIcon = NULL;
		wc.hCursor = 0;
		wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wc.lpszMenuName =  0;
		wc.lpszClassName = TEXT("MoSyncTextBoxContainer");
		RegisterClass(&wc);

		Base::Suspend();
		Base::InitWindowed();

		// the window itself
		sTextBoxContainer = CreateWindow(TEXT("MoSyncTextBoxContainer"), (const wchar_t*)title,
			WS_CAPTION,
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
			NULL, NULL, g_hInst, NULL);
		GLE(sTextBoxContainer);
#else
		Base::Suspend();
		sTextBoxContainer = g_hwndMain;
#endif
		// menu bar
		HMENU hMenu = CreateMenu();
		InsertMenu(hMenu, 0, MF_BYPOSITION, IDOK, L"OK");
		InsertMenu(hMenu, -1, MF_BYPOSITION, IDCANCEL, L"Cancel");
		SHMENUBARINFO mbi = { sizeof(SHMENUBARINFO), sTextBoxContainer, SHCMBF_HMENU,
			(UINT)hMenu, g_hInst, 0, 0, 0, 0 };
		GLE(SHCreateMenuBar(&mbi));
		//LOGD("Menu bar: 0x%08X\n", mbi.hwndMB);
		//LOGD("FindMenuBar: 0x%08X\n", SHFindMenuBar(sTextBoxContainer));
#if 1
		SHINITDLGINFO shidi;
		shidi.dwMask = SHIDIM_FLAGS;
		shidi.hDlg = sTextBoxContainer;
		shidi.dwFlags = SHIDIF_SIZEDLGFULLSCREEN | SHIDIF_SIPDOWN;
		GLE(SHInitDialog(&shidi));
#endif
#if 1	//testing softkeys only
		// create editbox
		sEditBox = CreateWindow(TEXT("EDIT"),	// predefined class 
			(LPCWSTR)inText,
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
			sTextBoxContainer,	// parent window 
			NULL, g_hInst, NULL);
		GLE(sEditBox);

		RECT rect;
		GLE(GetClientRect(sTextBoxContainer, &rect));
		GLE(SetWindowPos(sEditBox, NULL, rect.left, rect.top, rect.right, rect.bottom, 0));
		SetFocus(sEditBox);
#endif
		MAProcessEvents();
		//LOG("FindMenuBar: 0x%08X\n", SHFindMenuBar(sTextBoxContainer));
		//HRES(SHEnableSoftkey(mbi.hwndMB, 0, TRUE, TRUE));
		//HRES(SHEnableSoftkey(mbi.hwndMB, 1, TRUE, TRUE));
		ShowWindow(sTextBoxContainer, SW_SHOW);

		LOG("maTextBox: %i\n", 0);
		return 0;
	}

	//*****************************************************************************
	// maIOCtl
	//*****************************************************************************
	SYSCALL(int, maIOCtl(int function, int a, int b, int c)) 
	{
		switch(function) {
			/*
			//these are the same across all C++ platforms. consider sharing them somehow.
		case maIOCtl_sinh:
			{
				double& d = *GVMRA(double);
				d = ::sinh(d);
				return 0;
			}
		case maIOCtl_cosh:
			{
				double& d = *GVMRA(double);
				d = ::cosh(d);
				return 0;
			}
		case maIOCtl_atan2:
			{
				double& yr = *GVMRA(double);
				double& x = *GVMR(b, double);
				yr = ::atan2(yr, x);
				return 0;
			}
		case maIOCtl_atanh:
			{
				double& d = *GVMRA(double);
				d = (1+d) / (1-d);
				d = 0.5 * log(d);
				return 0;
			}
			*/

		maIOCtl_maWriteLog_case(maWriteLog);
		maIOCtl_maPlatformRequest_case(maPlatformRequest);
		maIOCtl_maSendTextSMS_case(maSendTextSMS);
		maIOCtl_maGetBatteryCharge_case(maGetBatteryCharge);
		maIOCtl_maKeypadIsLocked_case(maKeypadIsLocked);
		maIOCtl_maLockKeypad_case(maLockKeypad);
		maIOCtl_maUnlockKeypad_case(maUnlockKeypad);

		case maIOCtl_maBtStartDeviceDiscovery:
			BLUETOOTH(maBtStartDeviceDiscovery)(BtWaitTrigger, a != 0);
			return 0;
		case maIOCtl_maBtGetNewDevice:
			return SYSCALL_THIS->maBtGetNewDevice(GVMRA(MABtDevice));
		case maIOCtl_maBtStartServiceDiscovery:
			BLUETOOTH(maBtStartServiceDiscovery)(GVMRA(MABtAddr), GVMR(b, MAUUID), BtWaitTrigger);
			return 0;

		maIOCtl_maBtGetNewService_case(SYSCALL_THIS->maBtGetNewService);
		maIOCtl_maBtGetNextServiceSize_case(BLUETOOTH(maBtGetNextServiceSize));
		maIOCtl_maFrameBufferGetInfo_case(maFrameBufferGetInfo);
		maIOCtl_maFrameBufferInit_case(maFrameBufferInit);		
		maIOCtl_maFrameBufferClose_case(maFrameBufferClose);

		maIOCtl_maAudioBufferInit_case(maAudioBufferInit);		
		maIOCtl_maAudioBufferReady_case(maAudioBufferReady);
		maIOCtl_maAudioBufferClose_case(maAudioBufferClose);

#ifdef MA_PROF_SUPPORT_LOCATIONAPI
		case maIOCtl_maLocationStart:
			return maLocationStart();
		case maIOCtl_maLocationStop:
			return maLocationStop();
#endif

		maIOCtl_syscall_case(maFileOpen);

		maIOCtl_syscall_case(maFileExists);
		maIOCtl_syscall_case(maFileClose);
		maIOCtl_syscall_case(maFileCreate);
		maIOCtl_syscall_case(maFileDelete);
		maIOCtl_syscall_case(maFileSize);
#if 0	//todo
		maIOCtl_syscall_case(maFileAvailableSpace);
		maIOCtl_syscall_case(maFileTotalSpace);
		maIOCtl_syscall_case(maFileDate);
		maIOCtl_syscall_case(maFileRename);
		maIOCtl_syscall_case(maFileTruncate);
#endif

		case maIOCtl_maFileWrite:
			return SYSCALL_THIS->maFileWrite(a, SYSCALL_THIS->GetValidatedMemRange(b, c), c);
		case maIOCtl_maFileRead:
			return SYSCALL_THIS->maFileRead(a, SYSCALL_THIS->GetValidatedMemRange(b, c), c);

		maIOCtl_syscall_case(maFileWriteFromData);
		maIOCtl_syscall_case(maFileReadToData);

		maIOCtl_syscall_case(maFileTell);
		maIOCtl_syscall_case(maFileSeek);

		maIOCtl_syscall_case(maFileListStart);
		case maIOCtl_maFileListNext:
			return SYSCALL_THIS->maFileListNext(a, (char*)SYSCALL_THIS->GetValidatedMemRange(b, c), c);
		maIOCtl_syscall_case(maFileListClose);

			/*
		case maIOCtl_maWlanStartDiscovery:
			return maWlanStartDiscovery();
		case maIOCtl_maWlanGetNewAp:
			return maWlanGetNewAp(GVMRA(MA_WLAN_AP));
			*/

			/*
		case maIOCtl_maStartVideoStream:
			return maStartVideoStream(SYSCALL_THIS->GetValidatedStr(a));
		case maIOCtl_maPauseStream:
			maPauseStream((MAHandle)a);
			return 0;
		case maIOCtl_maResumeStream:
			maResumeStream((MAHandle)a);
			return 0;
		case maIOCtl_maCloseStream:
			maCloseStream((MAHandle)a);
			return 0;
			*/

		case maIOCtl_maGetSystemProperty:
			return maGetSystemProperty(SYSCALL_THIS->GetValidatedStr(a),
				(char*)SYSCALL_THIS->GetValidatedMemRange(b, c), c);

			maIOCtl_case(maTextBox);
		}

		return IOCTL_UNAVAILABLE;
	}
}

void MoSyncExit(int r) 
{
	LOG("MoSyncExit(%d)\n", r);

	EnterCriticalSection(&exitMutex);
	if(!exited) {
		exited = true;
		LeaveCriticalSection(&exitMutex);
		exit(r);
		EnterCriticalSection(&exitMutex);
	}
	LeaveCriticalSection(&exitMutex);
}

void MoSyncErrorExit(int errorCode) 
{
	LOG("ErrorExit %i\n", errorCode);
	char buffer[256];
	char* ptr = buffer + sprintf(buffer, "MoSync Panic\np%i.", errorCode);
	if(gCore) {
#ifdef PUBLIC_DEBUG
		if(SYSCALL_NUMBER_IS_VALID(gCore->currentSyscallId)) {
			ptr += sprintf(ptr, "s%i.", gCore->currentSyscallId);
		}
		if(gRunning) {
			ptr += sprintf(ptr, "i%x.", Core::GetIp(gCore));
		}
#endif
		if(gRunning) {
			char appCode[5];
			for(int i=0; i<4; i++) {
				appCode[i] = (byte)(gCore->Head.AppCode >> ((3-i)*8));
			}
			appCode[4] = 0;
			sprintf(ptr, "%s", appCode);
		}
	}

	CloseGraphics();
	InitWindowed();

	//LOG("%s", buffer);
	WCHAR wbuffer[256];
	convertAsciiToUnicode(wbuffer, 256, buffer);
	MessageBox(Base::g_hwndMain, wbuffer, TEXT("MoSync Panic"), MB_ICONERROR|MB_OK);
	MoSyncExit(errorCode);
}
