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

#define HAVE_IOCTL_ELLIPSIS

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x502
#include <windows.h>
#include <shellapi.h>

#include "resource.h"
#endif

#ifdef LINUX
#include <gtk/gtk.h>
#define stricmp(x, y) strcasecmp(x, y)
#endif	//LINUX

#ifdef DARWIN
#include "MacDialogs.h"
#endif //DARWIN

#include <math.h>

#include "config_platform.h"

#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_syswm.h>
//#include <SDL/SDL_ffmpeg.h>

#include <FreeImage.h>

#include <string>
#include <map>
#include <time.h>
#include <limits.h>


#include <helpers/fifo.h>
#include <helpers/log.h>
#include <helpers/helpers.h>
#include <helpers/attribute.h>
#include <helpers/smartie.h>
#include <MemStream.h>
#include <FileStream.h>

#include <helpers/CPP_IX_GUIDO.h>
#include <helpers/CPP_IX_STREAMING.h>

// blah
#include <helpers/CPP_IX_AUDIOBUFFER.h>
#include "BufferAudioSource.h"

//#include <bluetooth/discovery.h>

#include "Syscall.h"

#include "fastevents.h"
extern "C" {
#include "SDL_prim.h"
}
//#include "audio.h"
#include "AudioEngine.h"
#include "AudioChannel.h"

#include "sdl_syscall.h"
#include "sdl_errors.h"
#ifdef WIN32
#include "windows_errors.h"
#endif
#include "report.h"
#include "TcpConnection.h"
#include "ConfigParser.h"
#include "sdl_stream.h"
#include "MoSyncDB.h"

#include "Skinning/Screen.h"
#include "Skinning/SkinManager.h"

#ifdef SUPPORT_OPENGL_ES
#include <helpers/CPP_IX_OPENGL_ES.h>
#include <dgles-0.5/GLES/gl.h>
#include "../../generated/gl.h.cpp"
#include "OpenGLES.h"
#endif

namespace Base {

	//***************************************************************************
	//Defines and declarations
	//***************************************************************************

	Syscall* gSyscall = NULL;
	bool gReload = false;

#ifndef __USE_FULLSCREEN__
	static uint screenWidth  = 240;	//176
	static uint screenHeight = 320;	//208
#else
        //320x240 Will not work on
	static uint screenWidth  = 320*2;	//176
	static uint screenHeight = 240*2;	//208
#endif

	static Syscall::STARTUP_SETTINGS gStartupSettings;

	static SDL_Surface *gScreen = NULL, *gDrawSurface = NULL;
	SDL_Surface *gBackBuffer = NULL;
	static int gCurrentUnconvertedColor = 0, gCurrentConvertedColor = 0;
	static TTF_Font *gFont = NULL;
	static MAHandle gDrawTargetHandle = HANDLE_SCREEN;

	static bool gCameraViewFinderActive = false;
	static MAPoint2d gCameraViewFinderPoint, gCameraViewFinderDirection;
	static SDL_TimerID gCameraViewFinderTimer = NULL;

	static CircularFifo<MAEvent, EVENT_BUFFER_SIZE> gEventFifo;
	static bool gEventOverflow = false, gClosing = false;

	static SDL_TimerID gTimerId = NULL;
	static int gTimerSequence;
	static SDL_mutex* gTimerMutex = NULL;
	static bool gShowScreen;

	static SDL_TimerID gExitTimer = NULL;

#ifdef SUPPORT_OPENGL_ES
	static SubView sSubView;
	static bool sOpenGLMode = false;
#endif

	static MoRE::DeviceSkin* sSkin = NULL;

	static bool MALibInit(const Syscall::STARTUP_SETTINGS&);
#ifdef USE_MALIBQUIT
	static void MALibQuit();
#endif
	bool MAMoSyncInit();

	static void MAHandleKeyEventMAK(int mak, bool pressed, int nativeKey=0);
	static void MAHandleKeyEvent(int sdlk, bool pressed);
	static void MASendPointerEvent(int x, int y, int touchId, int type);

	static int maSendToBackground();
	static int maBringToForeground();

	static int maCameraFormatNumber();
	static int maCameraFormat(int index, MA_CAMERA_FORMAT* fmt);
	static int maCameraStart();
	static int maCameraStop();
	static int maCameraSnapshot(int formatIndex, MAHandle placeholder);
	static void cameraViewFinderUpdate();

	static int maGetSystemProperty(const char* key, char* buf, int size);

#ifdef WIN32
	static HFONT gWindowsUnifont = NULL;
	static int maTextBox(const wchar* title, const wchar* inText, wchar* outText,
		int maxSize, int constraints);
#endif

//********************************************************************

#ifdef MOBILEAUTHOR
	SDL_Surface* getMoSyncScreen() {
		return gBackBuffer;
	}
#endif

	//***************************************************************************
	// Syscall class
	//***************************************************************************

	Syscall::Syscall(const STARTUP_SETTINGS& settings)
#ifdef RESOURCE_MEMORY_LIMIT
		: resources(settings.resmem)
#endif
	{
		gStartupSettings = settings;
		gSyscall = this;
		gShowScreen = settings.showScreen;
		init();
#ifdef LINUX
#ifndef DARWIN
		int argc = 0;
		char** argv = NULL;
		gtk_init(&argc, &argv);
#endif
#endif

#ifdef MOBILEAUTHOR
		MAMoSyncInit();
#else
		bool res = MALibInit(settings);
		DEBUG_ASSERT(res);
#endif
	}

	Syscall::Syscall(int width, int height, const STARTUP_SETTINGS& settings)
#ifdef RESOURCE_MEMORY_LIMIT
		: resources(settings.resmem)
#endif
	{
		gStartupSettings = settings;
		gSyscall = this;
		gShowScreen = settings.showScreen;
		init();
#ifdef LINUX
#ifndef DARWIN
		int argc = 0;
		char** argv = NULL;
		gtk_init(&argc, &argv);
#endif
#endif
		screenWidth = width;
		screenHeight = height;
		bool res = MALibInit(settings);
		DEBUG_ASSERT(res);
	}

	void Syscall::platformDestruct() {
#ifdef EMULATOR
		gSyscall->pimClose();
#endif
		MoSyncDBClose();
	}

	//***************************************************************************
	//Initialization
	//***************************************************************************

#ifdef MOBILEAUTHOR
	//TODO: combine with MALibInit to avoid code duplication
	bool MAMoSyncInit() {
		char *mosyncDir = getenv("MOSYNCDIR");
		if(!mosyncDir) {
			LOG("MOSYNCDIR could not be found");
			BIG_PHAT_ERROR(SDLERR_MOSYNCDIR_NOT_FOUND);
		}
		if(!parseConfig(std::string(mosyncDir) + "/skins")) return false;

		//TEST_LTZ(SDL_Init(SDL_INIT_VIDEO));
		atexit(SDL_Quit);

		//TEST_NZ(FE_Init());
		atexit(FE_Quit);

		TEST_Z(gTimerMutex = SDL_CreateMutex());

		TEST_NZ(TTF_Init());
		atexit(TTF_Quit);

		//openAudio();
		AudioEngine::init();

		Bluetooth::MABtInit();
		MANetworkInit(/*broadcom*/);
		atexit(MANetworkClose);

		Uint32 rmask, gmask, bmask, amask;

		SDL_EnableUNICODE(true);


#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
#else
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
#endif

		TEST_Z(gBackBuffer = SDL_CreateRGBSurface(SDL_SWSURFACE, screenWidth, screenHeight,
			32, rmask, gmask, bmask, amask));

		gDrawSurface = gBackBuffer;

		char destDir[256];
		destDir[0] = 0;
		strcpy(destDir, mosyncDir);
		strcat(destDir, "/bin/unifont-5.1.20080907.ttf");

		TEST_Z(gFont = TTF_OpenFont(destDir, 16));

		return true;
	}
#endif

	class MoSyncSkinListener : public MoRE::DeviceSkin::Listener {
	public:
		void onMoSyncKeyPress(int mak) {
			MAHandleKeyEventMAK(mak, true);
		}
		void onMoSyncKeyRelease(int mak) {
			MAHandleKeyEventMAK(mak, false);
		}
		void onMoSyncPointerPress(int x, int y, int touchId) {
			MASendPointerEvent(x, y, touchId, EVENT_TYPE_POINTER_PRESSED);
		}
		void onMoSyncPointerDrag(int x, int y, int touchId) {
			MASendPointerEvent(x, y, touchId, EVENT_TYPE_POINTER_DRAGGED);
		}
		void onMoSyncPointerRelease(int x, int y, int touchId) {
			MASendPointerEvent(x, y, touchId, EVENT_TYPE_POINTER_RELEASED);
		}
	};


#ifdef EMULATOR
	static Uint32 GCCATTRIB(noreturn) SDLCALL TimeoutCallback(Uint32 interval, void*) {
		LOG("TimeoutCallback %i\n", interval);
		MoSyncErrorExit(2);
	}
#endif

	static bool setupScreen(const Syscall::STARTUP_SETTINGS& settings) {
#ifndef MOBILEAUTHOR
#ifdef __USE_SYSTEM_RESOLUTION__
		const SDL_VideoInfo* pVid = SDL_GetVideoInfo();
		if ( pVid != NULL )
		{
			screenWidth  = pVid->current_w;
			screenHeight = pVid->current_h;
		}
#endif

		if(settings.haveSkin) {
			sSkin = MoRE::SkinManager::getInstance()->createSkinFor(&settings.profile);
			if(!sSkin) {
				TEST_Z(gScreen = SDL_SetVideoMode(screenWidth, screenHeight, 32, SDL_SWSURFACE | SDL_ANYFORMAT ));
			} else {
				sSkin->setListener(new MoSyncSkinListener());
				// fix with mobile image
				TEST_Z(gScreen = SDL_SetVideoMode(sSkin->getWindowWidth(), sSkin->getWindowHeight(), 32, SDL_SWSURFACE | SDL_ANYFORMAT));
			}
		} else {
#ifdef __USE_FULLSCREEN__
			TEST_Z(gScreen = SDL_SetVideoMode(screenWidth, screenHeight, 32, SDL_SWSURFACE | SDL_ANYFORMAT | SDL_FULLSCREEN));
#else
			TEST_Z(gScreen = SDL_SetVideoMode(screenWidth, screenHeight, 32, SDL_SWSURFACE | SDL_ANYFORMAT ));
#endif
		}
#endif	//MOBILEAUTHOR

		SDL_PixelFormat* fmt = gScreen->format;

		DUMPINT(fmt->BitsPerPixel);
		TEST_Z(gBackBuffer = SDL_CreateRGBSurface(SDL_SWSURFACE, screenWidth, screenHeight,
			fmt->BitsPerPixel, fmt->Rmask, fmt->Gmask, fmt->Bmask, fmt->Amask));

		MoRE::setWindowSurface(gScreen);
		MoRE::setPhoneScreen(gBackBuffer);
		if(sSkin) {
			sSkin->drawDevice();
			sSkin->drawScreen();
			sSkin->drawMultiTouchSimulation();
			SDL_UpdateRect(gScreen, 0, 0, 0, 0);
		}
		return true;
	}

	static bool MALibInit(const Syscall::STARTUP_SETTINGS& settings) {
		char *mosyncDir = getenv("MOSYNCDIR");
		if(!mosyncDir) {
			LOG("MOSYNCDIR could not be found");
			BIG_PHAT_ERROR(SDLERR_MOSYNCDIR_NOT_FOUND);
		}

		TEST_LTZ(SDL_Init(0));
		atexit(SDL_Quit);

		/**
         * It is important that the SDL subsystems are initialized after
		 * MANetworkInit. Otherwise there is a race condition for a shared
		 * object between one of the SDL audio threads and openssl which causes
		 * SSL_CTX_new to hang in Heap32Next. However, SDL_Init should be
		 * called before MANetworkInit since it relies on SDL-mutexes.
		 */
		MANetworkInit();

		TEST_LTZ(SDL_InitSubSystem(SDL_INIT_VIDEO));

		TEST_NZ(FE_Init());
		atexit(FE_Quit);

		TEST_Z(gTimerMutex = SDL_CreateMutex());

		TEST_NZ(TTF_Init());
		atexit(TTF_Quit);

#ifdef EMULATOR
		gSyscall->pimInit();
#endif

		MoSyncDBInit();

		AudioEngine::init();

		Bluetooth::MABtInit();

		SDL_EnableUNICODE(true);

		if(settings.showScreen) {
			TEST_Z(setupScreen(settings));

			char caption[1024];
			if(settings.id != NULL) {
				int res = _snprintf(caption, 1024, "%s - MoSync", settings.id);
				DEBUG_ASSERT(res > 0 && res < 1024);
			} else {
				strcpy(caption, "MoSync");
			}
			SDL_WM_SetCaption(caption, NULL);

			if(settings.iconPath) {
				SDL_Surface *surf = IMG_Load(settings.iconPath);
				if(surf)
					SDL_WM_SetIcon(surf, NULL);
			}

		} else {
			Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			rmask = 0xff000000;
			gmask = 0x00ff0000;
			bmask = 0x0000ff00;
			amask = 0x000000ff;
#else
			rmask = 0x000000ff;
			gmask = 0x0000ff00;
			bmask = 0x00ff0000;
			amask = 0xff000000;
#endif

			TEST_Z(gBackBuffer = SDL_CreateRGBSurface(SDL_SWSURFACE, screenWidth, screenHeight,
				32, rmask, gmask, bmask, amask));
		}

		gDrawSurface = gBackBuffer;

		char destDir[256];
		destDir[0] = 0;

		strcpy(destDir, mosyncDir);
		strcat(destDir, "/bin/unifont-5.1.20080907.ttf");
		gFont = TTF_OpenFont(destDir, 16);

#ifdef WIN32
		int res = AddFontResourceEx(destDir, FR_PRIVATE, 0);
		LOG("AddFontResourceEx: %i faces added.\n", res);
		gWindowsUnifont = CreateFont(0,0,0,0,0,0,0,0,
			DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
			DEFAULT_PITCH, "unifont");
		if(!gWindowsUnifont) {
			LOG_GLE;
		}
#endif

		if(gFont == NULL) {	//fallback to old font
			LOG("Failed to load font %s. Attempting fallback.\n", destDir);
			strcpy(destDir, mosyncDir);
			strcat(destDir, "/bin/maspec.fon");
			TEST_Z(gFont = TTF_OpenFont(destDir, 8));
		}

#ifdef EMULATOR
		if(settings.timeout != 0) {
			DEBUG_ASSERT(NULL != SDL_AddTimer(settings.timeout * 1000, TimeoutCallback, NULL));
		}
#endif

		return true;
	}

#ifdef USE_MALIBQUIT
	static void MALibQuit() {
		closeAudio();

		MANetworkClose();

		Bluetooth::MABtClose();

		if(gTimerMutex)
			SDL_DestroyMutex(gTimerMutex);

		SDL_FreeSurface(gBackBuffer);

#ifndef MOBILEAUTHOR
		freeMophone();
#endif
	}
#endif	//USE_MALIBQUIT

	//***************************************************************************
	// Resource loading
	//***************************************************************************
	#if 0
	static SDL_Surface* createNew32BitSurface(SDL_Surface *src, int left, int top, int width, int height, bool freeOldSurface) {
/*

		int i, j;
		unsigned char *dst = (unsigned char*) surf->pixels;
		for(j = 0; j < src->h; j++) {
			for(i = 0; i < src->w; i++) {
				Uint32 c = *((Uint32*)SDL_getPixel(src, i, j));
				*((Uint32*)&dst[i+j*surf->pitch]) = c;
			}
		}

		*/

		src = SDL_DisplayFormatAlpha(src);
/*
		SDL_Surface* surf = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32,
			0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
		MYASSERT(surf);
		SDL_Rect rect;
		rect.x = left;
		rect.y = top;
		rect.w = width;
		rect.h = height;
		//May fail to copy alpha info. test it.
		SDL_SetAlpha(surf, 0, 0x0);
		SDL_BlitSurface(src, &rect, surf, NULL);
*/
//		if(freeOldSurface) {
//			SDL_FreeSurface(src);
//		}

		return src;
	}
	#endif	//0

	SDL_Surface* Syscall::loadImage(MemStream& s) {
		int size;
		TEST(s.length(size));
		SDL_RWops* rwops = SDL_RWFromConstMem(s.ptr(), size);
		//SDL_Surface* surf = IMG_LoadPNG_RW(rwops);
		//if(!surf) IMG_LoadJPG_RW(rwops);
		SDL_Surface* surf = IMG_Load_RW(rwops, 0);
		MYASSERT(surf, SDLERR_IMAGE_LOAD_FAILED);
		surf = SDL_DisplayFormatAlpha(surf);
		SDL_FreeRW(rwops);

		return surf;
	}

	SDL_Surface* Syscall::loadSprite(SDL_Surface* surface, ushort left, ushort top, ushort width, ushort height, ushort cx, ushort cy) {
		SDL_Surface* surf = SDL_CreateRGBSurface(SDL_SWSURFACE, surface->w, surface->h, surface->format->BitsPerPixel,
			surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);
		MYASSERT(surf, SDLERR_SPRITE_LOAD_FAILED);
		SDL_Rect rect;
		rect.x = left;
		rect.y = top;
		rect.w = width;
		rect.h = height;
		//May fail to copy alpha info. test it.
		SDL_SetAlpha(surf, 0, 0x0);
		SDL_BlitSurface(surface, &rect, surf, NULL);
		surf = SDL_DisplayFormatAlpha(surf);
		MYASSERT(surf, SDLERR_SPRITE_LOAD_FAILED);
		return surf;
	}

	//***************************************************************************
	// SDL Streams
	//***************************************************************************
	/** Seek to 'offset' relative to whence, one of stdio's whence values:
	SEEK_SET, SEEK_CUR, SEEK_END
	Returns the final offset in the data source, or -1 on error.
	*/
	static int SDLCALL Stream_seek(struct SDL_RWops *context, int offset, int whence) {
		Stream* s = (Stream*)context->hidden.unknown.data1;
		switch(whence) {
		case SEEK_END:
			int len;
			TRM1(s->length(len));
			offset = len + offset;
			whence = SEEK_SET;
		case SEEK_SET:
		case SEEK_CUR:
			(s->seek(whence == SEEK_SET ? Seek::Start : Seek::Current, offset));
			break;
		default:
			TRM1(false);
		}
		int pos;
		TRM1(s->tell(pos));
		return pos;
	}

	/** Read up to 'num' objects each of size 'objsize' from the data
	source to the area pointed at by 'ptr'.
	Returns the number of objects read, or -1 if the read failed.
	*/
	static int SDLCALL Stream_read(struct SDL_RWops *context, void *ptr, int size,
																	 int maxnum)
	{
		Stream* s = (Stream*)context->hidden.unknown.data1;
		int length, pos;
		TRM1(s->length(length));
		TRM1(s->tell(pos));
		int bytesLeft = (length - pos);
		int maxNumLeft = bytesLeft / size;
		int minNum = MIN(maxNumLeft, maxnum);
		TRM1(s->read(ptr, size * minNum));
		return minNum;
	}

	/** Write exactly 'num' objects each of size 'objsize' from the area
	pointed at by 'ptr' to data source.
	Returns 'num', or -1 if the write failed.
	*/
	static int SDLCALL Stream_write(struct SDL_RWops *context, const void *ptr, int size,
																		int num)
	{
		Stream* s = (Stream*)context->hidden.unknown.data1;
		TRM1(s->write(ptr, size * num));
		return num;
	}

	/** Close and free an allocated SDL_RWops structure.
	Returns 0.
	*/
	static int SDLCALL Stream_close(struct SDL_RWops *context) {
		if(context) {
			SDL_FreeRW(context);
		}
		return 0;
	}

	// Niklas: non static because I need it in SDL audio engine.
	SDL_RWops* SDL_RWFromStream(Stream* stream) {
		TEST(stream);
		TEST(stream->isOpen());
		SDL_RWops* rwops = SDL_AllocRW();
		TEST(rwops);
		rwops->seek = Stream_seek;
		rwops->read = Stream_read;
		rwops->write = Stream_write;
		rwops->close = Stream_close;
		rwops->hidden.unknown.data1 = stream;
		return rwops;
	}


	//***************************************************************************
	// Helpers
	//***************************************************************************
	static void fillPanicReport(MAPanicReport& pr, int reportType) {
		pr.runtimeId = RUNTIME_MORE;
		pr.reportType = reportType;
		pr.time = maTime();
		pr.ip = Base::getRuntimeIp();
	}

	static void MoSyncMessageBox(const char* msg, const char* title) {
		if(!gShowScreen)
			return;
#ifdef WIN32
		SDL_SysWMinfo info;
		SDL_VERSION(&info.version);
		if(!SDL_GetWMInfo(&info))
			info.window = NULL;
		MessageBox(info.window, msg, title, MB_ICONERROR);
#elif defined(LINUX)
		GtkWidget* dialog = gtk_message_dialog_new (NULL,
			GTK_DIALOG_DESTROY_WITH_PARENT,
			GTK_MESSAGE_ERROR,
			GTK_BUTTONS_CLOSE,
			"%s\n\n%s", title, msg);
		gtk_dialog_run (GTK_DIALOG (dialog));
		gtk_widget_destroy (dialog);
#elif defined(DARWIN)
		MacMessageBox(msg, title);
#else
#error Unsupported platform!
#endif
	}

	void pixelDoubledBlit(int x, int y, SDL_Surface *dstSurface, SDL_Surface *srcSurface, SDL_Rect srcRect, int multiplier) {
		if(!gShowScreen)
			return;
		//stretch the backbuffer onto the screen
		DEBUG_ASRTZERO(SDL_LockSurface(dstSurface));
		DEBUG_ASRTZERO(SDL_LockSurface(srcSurface));
		DEBUG_ASRTINT(srcSurface->format->BytesPerPixel, 4);
		int dstOffset = x+(dstSurface->pitch>>2)*y;
		int srcOffset = srcRect.x+(srcSurface->pitch>>2)*srcRect.y;
		ASRTINT(dstSurface->format->BytesPerPixel, 4, SDLERR_SCREEN_NOT_32BIT);
		for(int line=0; line < srcRect.h; line++) {
			int* dst = dstOffset + (int*)((char*)dstSurface->pixels + dstSurface->pitch * (line * multiplier));
			int* src = srcOffset + (int*)((char*)srcSurface->pixels + srcSurface->pitch * line);
			for(int j=0; j<multiplier; j++) {
				for(int column=0; column < srcRect.w ; column++) {
					for(int i=0; i< multiplier; i++) {
						dst[column * multiplier + i] = src[column];
					}
				}
				dst = (int*)(((char*)dst) + dstSurface->pitch);
			}
		}
		SDL_UnlockSurface(srcSurface);
		SDL_UnlockSurface(dstSurface);
		SDL_UpdateRect(dstSurface, x, y, srcSurface->w*multiplier, srcSurface->h*multiplier);
	}


	static void MAUpdateScreen() {
#ifndef MOBILEAUTHOR
		if(sSkin) {
			sSkin->drawScreen();
			sSkin->drawMultiTouchSimulation();

			SDL_UpdateRect(gScreen, 0, 0, 0, 0);
		} else {
			SDL_BlitSurface(gBackBuffer, NULL, gScreen, NULL);
			SDL_UpdateRect(gScreen, 0, 0, 0, 0);
		}
#endif
	}

	int currentKeyState = 0;

	static int MAConvertKey(int sdlkey)
	{
		switch(sdlkey)
		{
	#define CASE_MULTI_KEY(mak, sdlk) case SDLK_##sdlk: return MAK_##mak;
	#define CASE_DIRECT_KEY(k) CASE_MULTI_KEY(k, k)
	#define CASE_NUMBER_KEY(k) CASE_MULTI_KEY(k, KP##k)
			NUMBER_KEYS(CASE_NUMBER_KEY);
			DIRECT_KEYS(CASE_DIRECT_KEY);
			MULTI_KEYS(CASE_MULTI_KEY);
			default: return sdlkey;
		}
	}

	static int MAConvertKeyBitMAK(int mak) {
#define CASE_MAK(x) case MAK_##x: return MAKB_##x
		switch(mak) {
			CASE_MAK(LEFT);
			CASE_MAK(UP);
			CASE_MAK(RIGHT);
			CASE_MAK(DOWN);
			CASE_MAK(SOFTLEFT);
			CASE_MAK(SOFTRIGHT);
			CASE_MAK(FIRE);
			CASE_MAK(0);
			CASE_MAK(1);
			CASE_MAK(2);
			CASE_MAK(3);
			CASE_MAK(4);
			CASE_MAK(5);
			CASE_MAK(6);
			CASE_MAK(7);
			CASE_MAK(8);
			CASE_MAK(9);
			CASE_MAK(STAR);
			CASE_MAK(HASH);
			CASE_MAK(CLEAR);
		};

		return 0;
	}

	static void MASendPointerEvent(int x, int y, int touchId, int type) {
			if(!gEventOverflow) {
				if(gEventFifo.count() + 2 == EVENT_BUFFER_SIZE) {	//leave space for Close event
					gEventOverflow = true;
					gEventFifo.clear();
					LOG("EventBuffer overflow!\n");
				}
				MAEvent event;
				event.type = type;
				event.point.x = x;
				event.point.y = y;
				event.touchId = touchId;
				gEventFifo.put(event);
			}
	}

	static void MAHandleKeyEventMAK(int mak, bool pressed, int nativeKey) {
		if(!gEventOverflow) {
			if(gEventFifo.count() + 2 == EVENT_BUFFER_SIZE) {	//leave space for Close event
				gEventOverflow = true;
				gEventFifo.clear();
				LOG("EventBuffer overflow!\n");
			}
			MAEvent event;
			event.type = pressed ? EVENT_TYPE_KEY_PRESSED : EVENT_TYPE_KEY_RELEASED;

			int keyBit = MAConvertKeyBitMAK(mak);
			if(pressed) {
				currentKeyState |= keyBit;
			} else {
				currentKeyState &= ~keyBit;
			}

			event.key = mak;
			event.nativeKey = nativeKey;
			gEventFifo.put(event);
		}
		if(sSkin)
		{
			if(pressed) sSkin->keyPressed(mak);
			else sSkin->keyReleased(mak);
		}
	}

	static void MAHandleKeyEvent(int sdlk, bool pressed) {
		LOGDT("MAHandleKeyEvent %i %i", sdlk, pressed);
		int mak = MAConvertKey(sdlk);
		MAHandleKeyEventMAK(mak, pressed, sdlk);
	}

	static void MAHandleCharEvent(uint unicode) {
		LOGDT("MAHandleCharEvent 0x%x", unicode);
		if(unicode == 0)
			return;
		MAEvent event;
		event.type = EVENT_TYPE_CHAR;
		event.character = unicode;
		gEventFifo.put(event);
	}

	static Uint32 GCCATTRIB(noreturn) SDLCALL ExitCallback(Uint32 interval, void*) {
		LOG("ExitCallback %i\n", interval);

		{	//dump panic report
			MAPanicReport pr;
			fillPanicReport(pr, REPORT_TIMEOUT);
			WriteFileStream file("panic.report");
			file.write(&pr, sizeof(pr));
		}

#ifdef WIN32
		SDL_SysWMinfo info;
		SDL_VERSION(&info.version);
		HANDLE hThread = NULL;
		if(SDL_GetWMInfo(&info)) {
			DWORD processId;
			DWORD threadId = GetWindowThreadProcessId(info.window, &processId);
			hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, threadId);
		}
		SuspendThread(hThread);
		MoSyncMessageBox(
			"The application failed to respond to the Close Event and will be terminated.",
			"MoSync Panic");
		TerminateProcess(GetCurrentProcess(), 0);
		exit(0);	//unreachable, but the compiler doesn't know.
#else
		MoSyncMessageBox(
			"The application failed to respond to the Close Event and will be terminated.",
			"Big Phat Error");
		exit(0);
#endif
	}

	static void MASetClose() {
		//fix up the event queue
		gEventOverflow = gClosing = true;
		gReload = false;
		MAEvent event;
		event.type = EVENT_TYPE_CLOSE;
		gEventFifo.put(event);
		gExitTimer = SDL_AddTimer(EVENT_CLOSE_TIMEOUT, ExitCallback, NULL);
		DEBUG_ASSERT(NULL != gExitTimer);
	}

	static void MARotateScreen() {
		// swap w/h
		int h = screenWidth;
		int w = screenHeight;
		screenWidth = w;
		screenHeight = h;
		gStartupSettings.profile.mScreenWidth = w;
		gStartupSettings.profile.mScreenHeight = h;

		// rebuild screen
		SDL_FreeSurface(gBackBuffer);
		DEBUG_ASSERT(setupScreen(gStartupSettings));
		gDrawSurface = gBackBuffer;

		// send event
		MAEvent e;
		e.type = EVENT_TYPE_SCREEN_CHANGED;
		gEventFifo.put(e);
	}

	//returns true iff maWait should return.
	//must be called only from the main thread!
	bool MAProcessEvents() {
		static bool running = false;
		if(running)
			return 0;
		running = true;

		int PollEventResult = 0;
		SDL_Event event;
		bool ret = false;

#ifdef SUPPORT_OPENGL_ES
		if(sOpenGLMode) {
			Base::openGLProcessEvents(sSubView);
		}
#endif

		while((PollEventResult = FE_PollEvent(&event)) > 0) {
			switch(event.type) {
			case SDL_ACTIVEEVENT:
				LOGDT("SDL_ACTIVEEVENT");
				if (
					//(event.active.state & SDL_APPMOUSEFOCUS) ||
					(event.active.state & SDL_APPINPUTFOCUS) ||
					(event.active.state & SDL_APPACTIVE)
					) {
						MAEvent e;
						if (event.active.gain) {
							e.type = EVENT_TYPE_FOCUS_GAINED;
						} else {
							e.type = EVENT_TYPE_FOCUS_LOST;
						}
						gEventFifo.put(e);
				}
				break;
#ifndef MOBILEAUTHOR
			case SDL_MOUSEMOTION:
				if(event.motion.state&SDL_BUTTON(1))
				{
					if(sSkin)
						sSkin->mouseDragged(event.button.x, event.button.y);
					else
						MASendPointerEvent(event.button.x, event.button.y, 0,EVENT_TYPE_POINTER_DRAGGED);
				}
				else if(event.motion.state&SDL_BUTTON(3))
				{
					if(sSkin)
						sSkin->mouseMultiDragged(event.button.x, event.button.y);
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if(event.button.button == 1)
				{
					if(sSkin)
						sSkin->mousePressed(event.button.x, event.button.y);
					else
						MASendPointerEvent(event.button.x, event.button.y, 0, EVENT_TYPE_POINTER_PRESSED);
				}
				else if(event.button.button == 3)
				{
					if(sSkin)
						sSkin->mouseMultiPressed(event.button.x, event.button.y);
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if(event.button.button == 1)
				{
					if(sSkin)
						sSkin->mouseReleased(event.button.x, event.button.y);
					else
						MASendPointerEvent(event.button.x, event.button.y, 0, EVENT_TYPE_POINTER_RELEASED);
				}
				else if(event.button.button == 3)
				{
					if(sSkin)
						sSkin->mouseMultiReleased(event.button.x, event.button.y);
				}
				break;
#endif	//MOBILEAUTHOR

			case SDL_KEYDOWN:
				if(event.key.keysym.sym == SDLK_ESCAPE) {
					LOGT("SDLK_ESCAPE");
					MASetClose();
					break;
				}
				if(event.key.keysym.sym == SDLK_F4) {
					LOGT("SDLK_F4");
					MARotateScreen();
					break;
				}
				MAHandleKeyEvent(event.key.keysym.sym, true);
				MAHandleCharEvent(event.key.keysym.unicode);
				break;
			case SDL_KEYUP:
				if(	event.key.keysym.sym == SDLK_PAGEUP||
					event.key.keysym.sym == SDLK_PAGEDOWN||
					event.key.keysym.sym == SDLK_ESCAPE) {
						break;
				}

				MAHandleKeyEvent(event.key.keysym.sym, false);
				break;
			case SDL_QUIT:
				LOGT("SDL_QUIT");
				MASetClose();
				break;
			case SDL_VIDEOEXPOSE:
				MAUpdateScreen();
				break;
			case FE_ADD_EVENT:
				{
					LOGDT("FE_ADD_EVENT");
					MAEvent* pe = (MAEvent*)event.user.data1;
					gEventFifo.put(*pe);
					delete pe;
				}
				break;
			case FE_DEFLUX_BINARY:
				LOGDT("FE_DEFLUX_BINARY");
				SYSCALL_THIS->resources.extract_RT_FLUX(event.user.code);
				ROOM(SYSCALL_THIS->resources.add_RT_BINARY(event.user.code,
					(Stream*)event.user.data1));
				break;
			case FE_TIMER:
				LOGDT("Timer event handled: %i %i", gTimerSequence, event.user.code);
				if(gTimerSequence == event.user.code)
					ret = true;
				break;
			case FE_INTERRUPT:
				LOGDT("FE_INTERRUPT");
				ret = true;
				break;
			case FE_CAMERA_VIEWFINDER_UPDATE:
				LOGDT("FE_CAMERA_VIEWFINDER_UPDATE");
				cameraViewFinderUpdate();
				break;
			default:
				LOG("Unhandled event, type %i\n", event.type);
			}
		}
		if(PollEventResult < 0) {
			LOG("SDL_PollEvent returned %i!\n", PollEventResult);
		}
		running = false;
		return ret;
	}

	static Uint32 SDLCALL MATimerCallback(Uint32 interval, void* sequence) {
		LOGD("MATimerCallback %i...", (int)sequence);
		DEBUG_ASRTZERO(SDL_LockMutex(gTimerMutex));
		{
			SDL_UserEvent event = { FE_TIMER, (int)(size_t)sequence, NULL, NULL };
			FE_PushEvent((SDL_Event*)&event);
		}
		DEBUG_ASRTZERO(SDL_UnlockMutex(gTimerMutex));
		LOGD("MATimerCallback %i done\n", (int)sequence);
		return (Uint32)-1;
	}

	static void BtWaitTrigger() {
		LOGD("BtWaitTrigger\n");
		MAEvent* ep = new MAEvent;
		ep->type = EVENT_TYPE_BT;
		ep->state = Bluetooth::maBtDiscoveryState();
		SDL_UserEvent event = { FE_ADD_EVENT, 0, ep, NULL };
		FE_PushEvent((SDL_Event*)&event);
	}
}	//namespace Base

	//***************************************************************************
	// Proper syscalls
	//***************************************************************************
	SYSCALL(int, maGetKeys()) {
		if(gClosing)
			return 0;
		MAProcessEvents();
		return currentKeyState;
	}

	SYSCALL(void, maSetClipRect(int left, int top, int width, int height))
	{
		gDrawSurface->clip_rect.x = left;
		gDrawSurface->clip_rect.y = top;
		gDrawSurface->clip_rect.w = width;
		gDrawSurface->clip_rect.h = height;
	}

	SYSCALL(void, maGetClipRect(MARect *rect))
	{
		SYSCALL_THIS->ValidateMemRange(rect, sizeof(MARect));
		rect->left = gDrawSurface->clip_rect.x;
		rect->top = gDrawSurface->clip_rect.y;
		rect->width = gDrawSurface->clip_rect.w;
		rect->height = gDrawSurface->clip_rect.h;
	}

	SYSCALL(int, maSetColor(int argb)) {
		int oldColor = gCurrentUnconvertedColor;
		gCurrentUnconvertedColor = argb;	//16-bit colors => Problematic.
		gCurrentConvertedColor = SDL_MapRGBA(gBackBuffer->format,
			(argb >> 16) & 0xff, (argb >> 8) & 0xff, argb & 0xff, (argb >> 24) & 0xff);
		return oldColor;
	}
	SYSCALL(void, maPlot(int posX, int posY)) {
		SDL_putPixel(gDrawSurface, posX, posY, gCurrentConvertedColor);
	}
	SYSCALL(void, maLine(int startX, int startY, int endX, int endY)) {
		SDL_drawLine(gDrawSurface, startX, startY, endX, endY, gCurrentConvertedColor);
	}
	SYSCALL(void, maFillRect(int left, int top, int width, int height)) {
		SDL_Rect rect = { (Sint16)left, (Sint16)top, (Uint16)width, (Uint16)height };
		DEBUG_ASRTZERO(SDL_FillRect(gDrawSurface, &rect, gCurrentConvertedColor));
	}

	SYSCALL(void, maFillTriangleStrip(const MAPoint2d* points, int count)) {
		SYSCALL_THIS->ValidateMemRange(points, sizeof(MAPoint2d) * count);
		CHECK_INT_ALIGNMENT(points);
		MYASSERT(count >= 3, ERR_POLYGON_TOO_FEW_POINTS);
		for(int i = 2; i < count; i++) {
			SDL_fillTriangle(gDrawSurface,
				points[i-2].x,
				points[i-2].y,
				points[i-1].x,
				points[i-1].y,
				points[i].x,
				points[i].y,
				gCurrentConvertedColor);
		}
		LOGG("fp color 0x%08x %i:", gCurrentConvertedColor, count);
		for(int i=0; i<count; i++) {
			LOGG(" %ix%i", points[i].x, points[i].y);
		}
		LOGG("\n");
	}

	SYSCALL(void, maFillTriangleFan(const MAPoint2d* points, int count)) {
		SYSCALL_THIS->ValidateMemRange(points, sizeof(MAPoint2d) * count);
		CHECK_INT_ALIGNMENT(points);
		MYASSERT(count >= 3, ERR_POLYGON_TOO_FEW_POINTS);
		for(int i = 2; i < count; i++) {
			SDL_fillTriangle(gDrawSurface,
				points[0].x,
				points[0].y,
				points[i-1].x,
				points[i-1].y,
				points[i].x,
				points[i].y,
				gCurrentConvertedColor);
		}
		LOGG("fp color 0x%08x %i:", gCurrentConvertedColor, count);
		for(int i=0; i<count; i++) {
			LOGG(" %ix%i", points[i].x, points[i].y);
		}
		LOGG("\n");
	}

	template<class Tchar, int SDLCALL sizeFunc(TTF_Font*, const Tchar*, int*, int*)>
	static MAExtent getTextSize(const Tchar* str) {
		if(*str == 0) {
			return 0;
		}
		int x,y;
		DEBUG_ASSERT(gFont != NULL);
		if(sizeFunc(gFont, str, &x, &y) != 0) {
			BIG_PHAT_ERROR(SDLERR_TEXT_SIZE_FAILED);
		}
		return EXTENT(x, y);
	}

	SYSCALL(MAExtent, maGetTextSize(const char* str)) {
		return getTextSize<char, TTF_SizeText>(str);
	}
	SYSCALL(MAExtent, maGetTextSizeW(const wchar* str)) {
		return getTextSize<wchar, TTF_SizeUNICODE>(str);
	}

	template<class Tchar, SDL_Surface* SDLCALL renderFunc(TTF_Font*, const Tchar*, SDL_Color)>
	static void drawText(int left, int top, const Tchar* str) {
		if(*str == 0) {
			return;
		}
		int argb = gCurrentUnconvertedColor;
		SDL_Color color = { (Uint8)(argb >> 16), (Uint8)(argb >> 8), (Uint8)argb, 0 };
		SDL_Surface* text_surface = renderFunc(gFont, str, color);
		if(!text_surface) {
			BIG_PHAT_ERROR(SDLERR_TEXT_RENDER_FAILED);
		}
		SDL_Rect rect = { (Sint16)left, (Sint16)top, 0, 0 };
		SDL_BlitSurface(text_surface, NULL, gDrawSurface, &rect);
		SDL_FreeSurface(text_surface);
	}

	SYSCALL(void, maDrawText(int left, int top, const char* str)) {
		drawText<char, TTF_RenderText_Solid>(left, top, str);
	}
	SYSCALL(void, maDrawTextW(int left, int top, const wchar* str)) {
		drawText<wchar, TTF_RenderUNICODE_Solid>(left, top, str);
	}

	SYSCALL(void, maUpdateScreen()) {
		LOGG("maUpdateScreen()\n");
		if(gClosing)
			return;
		MAUpdateScreen();
		MAProcessEvents();

#ifdef SUPPORT_OPENGL_ES
		if(sOpenGLMode)
				Base::openGLSwap(sSubView);
#endif
	}
	SYSCALL(void, maResetBacklight()) {
	}
	SYSCALL(MAExtent, maGetScrSize()) {
		return EXTENT(screenWidth, screenHeight);
	}

	SYSCALL(void, maDrawImage(MAHandle image, int left, int top)) {
		SDL_Surface* surf = gSyscall->resources.get_RT_IMAGE(image);
		SDL_Rect rect = { (Sint16)left, (Sint16)top, 0, 0 };
		SDL_BlitSurface(surf, NULL, gDrawSurface, &rect);
	}

	SYSCALL(void, maDrawRGB(const MAPoint2d* dstPoint, const void* src,
		const MARect* srcRect, int scanlength))
	{
		SYSCALL_THIS->ValidateMemRange(dstPoint, sizeof(MAPoint2d));
		SYSCALL_THIS->ValidateMemRange(srcRect, sizeof(MARect));

		//Niklas: duno about this validation, sdl does clipping?
		//Fredrik: all memory access must be validated. clipping is irrelevant.
		SYSCALL_THIS->ValidateMemRange(src, sizeof(int)*srcRect->height*scanlength);

		SDL_Rect srcSurfaceRect = { (Sint16)srcRect->left, (Sint16)srcRect->top,
			(Uint16)srcRect->width, (Uint16)srcRect->height};
		SDL_Rect dstSurfaceRect = { (Sint16)dstPoint->x, (Sint16)dstPoint->y,
			(Uint16)srcRect->width, (Uint16)srcRect->height};
		SDL_Surface* srcSurface = SDL_CreateRGBSurfaceFrom((void*)src,
			srcSurfaceRect.w, srcSurfaceRect.h, 32, scanlength<<2,
			0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

		SDL_SetAlpha(srcSurface, SDL_SRCALPHA, 0x0);
		SDL_BlitSurface(srcSurface, &srcSurfaceRect, gDrawSurface, &dstSurfaceRect);

		SDL_FreeSurface(srcSurface);
	}

	SYSCALL(void, maDrawImageRegion(MAHandle image, const MARect* src, const MAPoint2d* dstTopLeft, int transformMode)) {
		//LOG("Entering DrawImageRegion start\n");

		SDL_Surface* surf = gSyscall->resources.get_RT_IMAGE(image);
		gSyscall->ValidateMemRange(src, sizeof(MARect));
		gSyscall->ValidateMemRange(dstTopLeft, sizeof(MAPoint2d));

		unsigned int* srcPixels = (unsigned int*) surf->pixels;
		unsigned int* destPixels = (unsigned int*) gDrawSurface->pixels;
		int dstPitchY = gDrawSurface->pitch>>2;
		int srcPitchX, srcPitchY;
		int transTopLeftX, transTopLeftY;
		int transWidth, transHeight;
		int bpp = 4;
		int width = src->width;
		int height = src->height;
		int u = src->left;
		int v = src->top;
		int left = dstTopLeft->x;
		int top = dstTopLeft->y;

		if( width <= 0 || height <= 0) return;

		if (u > surf->clip_rect.x + surf->clip_rect.w)
			return;
		else if(u < surf->clip_rect.x) {
			left += surf->clip_rect.x - u;
			u = surf->clip_rect.x;
		}
		if (v > surf->clip_rect.y + surf->clip_rect.h)
			return;
		else if(v < surf->clip_rect.y) {
			top += surf->clip_rect.y - v;
			v = surf->clip_rect.y;
		}
		if(u + surf->clip_rect.w < surf->clip_rect.x)
			return;
		else if(u + width > surf->clip_rect.x + surf->clip_rect.w)
			width -= (u + width) - (surf->clip_rect.x + surf->clip_rect.w);
		if(v + height< surf->clip_rect.y)
			return;
		else if(v + height > surf->clip_rect.y + surf->clip_rect.h)
			height -= (v + height) - (surf->clip_rect.y + surf->clip_rect.h);

		if( width <= 0 || height <= 0) return;

		switch(transformMode) {
			case TRANS_NONE:
				srcPitchX = bpp;
				srcPitchY = surf->pitch;
				transTopLeftX = u;
				transTopLeftY = v;
				transWidth = width;
				transHeight = height;
				break;
			case TRANS_ROT90:
				srcPitchX = -surf->pitch;
				srcPitchY = bpp;
				transTopLeftX = u;
				transTopLeftY = v+height-1;
				transWidth = height;
				transHeight = width;
				break;
			case TRANS_ROT180:
				srcPitchX = -bpp;
				srcPitchY = -surf->pitch;
				transTopLeftX = u + width - 1;
				transTopLeftY = v + height - 1;
				transWidth = width;
				transHeight = height;
				break;
			case TRANS_ROT270:
				srcPitchX = surf->pitch;
				srcPitchY = -bpp;
				transTopLeftX = u + width - 1;
				transTopLeftY = v;
				transWidth = height;
				transHeight = width;
				break;
			case TRANS_MIRROR:
				srcPitchX = -bpp;
				srcPitchY = surf->pitch;
				transTopLeftX = u + width - 1;
				transTopLeftY = v;
				transWidth = width;
				transHeight = height;
				break;
			case TRANS_MIRROR_ROT90:
				srcPitchX = -surf->pitch;
				srcPitchY = -bpp;
				transTopLeftX = u+width-1;
				transTopLeftY = v+height-1;
				transWidth = height;
				transHeight = width;
				break;
			case TRANS_MIRROR_ROT180:
				srcPitchX = bpp;
				srcPitchY = -surf->pitch;
				transTopLeftX = u;
				transTopLeftY = v + height - 1;
				transWidth = width;
				transHeight = height;
				break;
			case TRANS_MIRROR_ROT270:
				srcPitchX = surf->pitch;
				srcPitchY = bpp;
				transTopLeftX = u;
				transTopLeftY = v;
				transWidth = height;
				transHeight = width;
				break;
			default:
				BIG_PHAT_ERROR(ERR_IMAGE_TRANSFORM_INVALID);
		}

		srcPitchY>>=2;
		srcPitchX>>=2;

		int destY = top*dstPitchY;
		int srcY = transTopLeftY*(surf->pitch>>2);
		int y = top;

		unsigned int dstRedMask = gDrawSurface->format->Rmask;
		unsigned int dstRedShift = gDrawSurface->format->Rshift;
		unsigned int dstGreenMask = gDrawSurface->format->Gmask;
		unsigned int dstGreenShift = gDrawSurface->format->Gshift;
		unsigned int dstBlueMask = gDrawSurface->format->Bmask;
		unsigned int dstBlueShift = gDrawSurface->format->Bshift;
		unsigned int dstAlphaMask = gDrawSurface->format->Amask;
		unsigned int srcRedMask = surf->format->Rmask;
		unsigned int srcRedShift = surf->format->Rshift;
		unsigned int srcGreenMask = surf->format->Gmask;
		unsigned int srcGreenShift = surf->format->Gshift;
		unsigned int srcBlueMask = surf->format->Bmask;
		unsigned int srcBlueShift = surf->format->Bshift;
		unsigned int srcAlphaMask = surf->format->Amask;
		unsigned int srcAlphaShift = surf->format->Ashift;

		//LOG("Entering DrawImageRegion Loop\n");

		if(surf->flags&SDL_SRCALPHA) {
			while(transHeight) {
				int destX = left;
				int srcX = transTopLeftX;
				width = transWidth;

				if(	y >= gDrawSurface->clip_rect.y &&
					y < gDrawSurface->clip_rect.y + gDrawSurface->clip_rect.h) {

						while(width) {
							if( destX >= gDrawSurface->clip_rect.x &&
								destX < gDrawSurface->clip_rect.x + gDrawSurface->clip_rect.w )
							{
								int d = destPixels[destX + destY];
								int s = srcPixels[srcX + srcY];
								int a = (srcPixels[srcX + srcY]&srcAlphaMask)>>srcAlphaShift;
								int sr = (((s)&srcRedMask)>>srcRedShift);
								int sg = (((s)&srcGreenMask)>>srcGreenShift);
								int sb = (((s)&srcBlueMask)>>srcBlueShift);
								int dr = (((d)&dstRedMask)>>dstRedShift);
								int dg = (((d)&dstGreenMask)>>dstGreenShift);
								int db = (((d)&dstBlueMask)>>dstBlueShift);

								/* Do alpha blitting */
								destPixels[destX + destY] =
									(((dr + (((sr-dr)*(a))>>8)) << dstRedShift)  &dstRedMask) |
									(((dg + (((sg-dg)*(a))>>8)) << dstGreenShift)&dstGreenMask) |
									(((db + (((sb-db)*(a))>>8)) << dstBlueShift) &dstBlueMask);

							}
							srcX+=srcPitchX;
							destX++;
							width--;
						}
				}
				srcY+=srcPitchY;
				destY+=dstPitchY;
				transHeight--;
				y++;
			}
		} else {
			while(transHeight) {
				int destX = left;
				int srcX = transTopLeftX;
				width = transWidth;

				if(	y >= gDrawSurface->clip_rect.y &&
					y < gDrawSurface->clip_rect.y + gDrawSurface->clip_rect.h) {

						while(width) {
							if( destX >= gDrawSurface->clip_rect.x &&
								destX < gDrawSurface->clip_rect.x + gDrawSurface->clip_rect.w )
							{
								/* Do blitting without alpha */
								destPixels[destX + destY] = (destPixels[destX + destY] & dstAlphaMask) |
									(srcPixels[srcX + srcY] & (srcRedMask | srcGreenMask | srcBlueMask));
							}
							srcX+=srcPitchX;
							destX++;
							width--;
						}
				}
				srcY+=srcPitchY;
				destY+=dstPitchY;
				transHeight--;
				y++;
			}
		}
	}


	SYSCALL(MAExtent, maGetImageSize(MAHandle image)) {
		SDL_Surface* surf = gSyscall->resources.get_RT_IMAGE(image);
		return EXTENT(surf->w, surf->h);
	}

	SYSCALL(void, maGetImageData(MAHandle image, void* dst, const MARect* src, int scanlength)) {
		SDL_Surface* surf = gSyscall->resources.get_RT_IMAGE(image);
		gSyscall->ValidateMemRange(src, sizeof(MARect));
		CHECK_INT_ALIGNMENT(src);
		SDL_Rect srcRect = { (Sint16)src->left, (Sint16)src->top, (Uint16)src->width, (Uint16)src->height};
		SDL_Rect dstRect = { 0, 0, (Uint16)src->width, (Uint16)src->height};

		gSyscall->ValidateMemRange(dst, sizeof(int)*scanlength*src->height);
		CHECK_INT_ALIGNMENT(dst);

		MYASSERT(scanlength >= src->width, ERR_IMAGE_OOB);
		MYASSERT(src->left >= 0 && src->top >= 0 &&
			src->left + src->width <= surf->w &&
			src->top + src->height <= surf->h, ERR_IMAGE_OOB);

		/*
		unsigned char *dst_ptr = (unsigned char*)dst;
		unsigned char *src_ptr = (unsigned char*)surf->pixels;
		src_ptr+=((src->left<<2)+src->top*surf->pitch);
		for(int j = 0; j < src->height; j++) {
			memcpy(dst_ptr, src_ptr, src->width<<2);
			dst_ptr+=scanlength<<2;
			src_ptr+=surf->pitch;
		}
		*/

		SDL_Surface* dstSurface = SDL_CreateRGBSurfaceFrom(dst,
			src->width, src->height, 32, scanlength<<2,
			0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

		bool srcAlpha = false;
		if(surf->flags&SDL_SRCALPHA) {
			SDL_SetAlpha(surf, 0, 0);
			srcAlpha = true;
		}

		SDL_BlitSurface(surf, &srcRect, dstSurface, &dstRect);

		if(srcAlpha) {
			SDL_SetAlpha(surf, SDL_SRCALPHA, 0);
		}

		SDL_FreeSurface(dstSurface);
	}

	SYSCALL(MAHandle, maSetDrawTarget(MAHandle handle)) {
		MAHandle temp = gDrawTargetHandle;
		if(gDrawTargetHandle != HANDLE_SCREEN) {
			SYSCALL_THIS->resources.extract_RT_FLUX(gDrawTargetHandle);
			ROOM(SYSCALL_THIS->resources.add_RT_IMAGE(gDrawTargetHandle, gDrawSurface));
			gDrawTargetHandle = HANDLE_SCREEN;
		}
		if(handle == HANDLE_SCREEN) {
			gDrawSurface = gBackBuffer;
		} else {
			SDL_Surface* img = SYSCALL_THIS->resources.extract_RT_IMAGE(handle);
			gDrawSurface = img;
#ifdef RESOURCE_MEMORY_LIMIT
			void* o = (void*)size_RT_IMAGE(img);
#else
			void* o = NULL;
#endif
			ROOM(SYSCALL_THIS->resources.add_RT_FLUX(handle, o));
		}
		gDrawTargetHandle = handle;
		return temp;
	}

	SYSCALL(int, maCreateImageFromData(MAHandle placeholder, MAHandle resource, int offset, int size)) {
		Stream *src = gSyscall->resources.get_RT_BINARY(resource);
		MYASSERT(src->seek(Seek::Start, offset), ERR_DATA_OOB);
		Smartie<Stream> copy(src->createLimitedCopy(size));
		MYASSERT(copy, ERR_DATA_OOB);
		SDL_RWops* rwops = SDL_RWFromStream(copy());
		if(!rwops)
		{
			LOG("%s\n", SDL_GetError());
			DEBIG_PHAT_ERROR;
		}
		SDL_Surface* surf = IMG_LoadPNG_RW(rwops);
		if(!surf) surf = IMG_LoadJPG_RW(rwops);
		SDL_FreeRW(rwops);

		if(!surf)
			return RES_BAD_INPUT;

		surf = SDL_DisplayFormatAlpha(surf);

		return gSyscall->resources.add_RT_IMAGE(placeholder, surf);
	}

	SYSCALL(int, maCreateImageRaw(MAHandle placeholder, const void* src, MAExtent size, int alpha)) {
		SYSCALL_THIS->ValidateMemRange(src, sizeof(int)*EXTENT_X(size)*EXTENT_Y(size));
		CHECK_INT_ALIGNMENT(src);

		SDL_Surface* dst = SDL_CreateRGBSurface(SDL_SWSURFACE|(alpha?SDL_SRCALPHA:0), EXTENT_X(size), EXTENT_Y(size), 32,
			0x00ff0000, 0x0000ff00, 0x000000ff, (alpha?0xff000000:0));
		if(dst==0) return RES_OUT_OF_MEMORY;

/*
		if(alpha) {
			SDL_SetAlpha(dst, SDL_SRCALPHA, 0);
		} else {
			SDL_SetAlpha(dst, 0, 0);
		}
		*/

		int width = EXTENT_X(size);
		int height = dst->h;
		unsigned char *dptr = (unsigned char*)dst->pixels;
		unsigned char *sptr = (unsigned char*)src;

		while(height--) {
			memcpy(dptr, sptr, width<<2);
			sptr+=width<<2;
			dptr+=dst->pitch;
		}

		return gSyscall->resources.add_RT_IMAGE(placeholder, dst);
	}

	SYSCALL(int, maCreateDrawableImage(MAHandle placeholder, int width, int height)) {
		MYASSERT(width > 0 && height > 0, ERR_IMAGE_SIZE_INVALID);
		const SDL_PixelFormat* fmt = gBackBuffer->format;
		SDL_Surface* surf = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, fmt->BitsPerPixel,
			fmt->Rmask, fmt->Gmask, fmt->Bmask, fmt->Amask);
		if(surf==0) return RES_OUT_OF_MEMORY;

		return gSyscall->resources.add_RT_IMAGE(placeholder, surf);
	}

	SYSCALL(int, maGetEvent(MAEvent* dst)) {
		CHECK_INT_ALIGNMENT(dst);
		gSyscall->ValidateMemRange(dst, sizeof(MAEvent));
		MAProcessEvents();
		if(!gClosing)
			gEventOverflow = false;
		if(gEventFifo.count() == 0) {
			return 0;
		}
		*dst = gEventFifo.get();
		return 1;
	}

	SYSCALL(void, maWait(int timeout)) {
		LOGD("maWait %i\n", timeout);
		if(gClosing)
			return;

		if(gEventFifo.count() != 0)
			return;

		DEBUG_ASSERT(gTimerId == NULL);
		if(timeout > 0) {
			//LOGD("Setting timer sequence %i\n", gTimerSequence);
			gTimerId = SDL_AddTimer(timeout, MATimerCallback, (void*)gTimerSequence);
			DEBUG_ASSERT(gTimerId != NULL);
		}
		while(true) {
			bool ret = MAProcessEvents();
			if(ret) {
				break;
			}
			if(gEventFifo.count() != 0)
				break;
			if(FE_WaitEvent(NULL) != 1) {
				LOGT("FE_WaitEvent failed");
				DEBIG_PHAT_ERROR;
			}
		}

		DEBUG_ASRTZERO(SDL_LockMutex(gTimerMutex));
		{
			if(gTimerId != NULL) {
				DEBUG_ASSERT(SDL_RemoveTimer(gTimerId));
				gTimerId = NULL;
			}
			gTimerSequence++;
		}
		DEBUG_ASRTZERO(SDL_UnlockMutex(gTimerMutex));
	}

	SYSCALL(int, maTime()) {
		return (int)time(NULL);
	}
	SYSCALL(int, maLocalTime()) {
#ifdef WIN32
		TIME_ZONE_INFORMATION tzi;
		DWORD res = GetTimeZoneInformation(&tzi);
		MYASSERT(res != TIME_ZONE_ID_INVALID, WINERR_TIMEZONE);
		int bias = tzi.Bias;
		if(res == TIME_ZONE_ID_DAYLIGHT)
			bias += tzi.DaylightBias;
		if(res == TIME_ZONE_ID_STANDARD)
			bias += tzi.StandardBias;
		return (int)(time(NULL) - (bias * 60));
#else
		time_t t = time(NULL);
		tm* lt = localtime(&t);
		return t + lt->tm_gmtoff;
#endif
	}

	SYSCALL(int, maGetMilliSecondCount()) {
		return SDL_GetTicks();
	}

#ifdef RESOURCE_MEMORY_LIMIT
	uint Base::size_RT_IMAGE(SDL_Surface* r) {
		return sizeof(SDL_Surface) + r->pitch * r->h;
	}
	SYSCALL(int, maFreeObjectMemory()) {
		return maTotalObjectMemory() - gSyscall->resources.getResmem();
	}
	SYSCALL(int, maTotalObjectMemory()) {
		int i = gSyscall->resources.getResmemMax();
		if(i < 0)
			return INT_MAX;
		else
			return i;
	}
#else
	SYSCALL(int, maFreeObjectMemory()) {
		return INT_MAX;
	}
	SYSCALL(int, maTotalObjectMemory()) {
		return INT_MAX;
	}
#endif

	SYSCALL(int, maVibrate(int)) {
		return IOCTL_UNAVAILABLE;
	}

	SYSCALL(int, maSoundPlay(MAHandle sound_res, int offset, int size)) {
		int chan = 0;
		Stream *src = gSyscall->resources.get_RT_BINARY(sound_res);
		MYASSERT(src->seek(Seek::Start, offset), ERR_DATA_ACCESS_FAILED);

		//read the MIME type
		char mime[1024];
		size_t i=0;
		byte b;
		do {
			if(!src->readByte(b) || i >= sizeof(mime)) {
				BIG_PHAT_ERROR(ERR_MIME_READ_FAILED);
			}
			mime[i++] = b;
		} while(b);

		int pos;
		src->tell(pos);
		int encodedSize = size - pos + offset;
		LOGA("encodedSize: %i\n", encodedSize);
		Stream* copy = src->createLimitedCopy(encodedSize);

		AudioChannel *chnl = AudioEngine::getChannel(chan);
		AudioSource *audioSource = chnl->getAudioSource();
		if(audioSource!=NULL) {
			audioSource->close();
			delete audioSource;
		}

		audioSource = AudioEngine::getAudioSource(mime, copy);
		MYASSERT(audioSource, SDLERR_SOUND_LOAD_FAILED);

		chnl->setAudioSource(audioSource);
		chnl->setActive(true);
		//startChannel(chan);

		return 0;
	}

	SYSCALL(void, maSoundStop()) {
		//stopChannel(0);
		//rewindChannel(0);
		AudioChannel *chnl = AudioEngine::getChannel(0);
		chnl->setActive(false);
	}

	SYSCALL(int, maSoundIsPlaying()) {
//		return soundIsPlaying(0);
		AudioChannel *chnl = AudioEngine::getChannel(0);
		return (int) chnl->isActive();
	}

	SYSCALL(void, maSoundSetVolume(int vol)) {
		//setVolume(0, vol);
		AudioChannel *chnl = AudioEngine::getChannel(0);
		chnl->setVolume((vol*0xffff)/100);
	}

	SYSCALL(int, maSoundGetVolume()) {
		AudioChannel *chnl = AudioEngine::getChannel(0);
		return (chnl->getVolume()*100)/0xffff;
	}

	static int maFrameBufferGetInfo(MAFrameBufferInfo *info) {
		info->bitsPerPixel = gBackBuffer->format->BitsPerPixel;
		info->bytesPerPixel = gBackBuffer->format->BytesPerPixel;
		info->redMask = gBackBuffer->format->Rmask;
		info->greenMask = gBackBuffer->format->Gmask;
		info->blueMask = gBackBuffer->format->Bmask;
		info->sizeInBytes = gBackBuffer->pitch*gBackBuffer->h;
		info->width = gBackBuffer->w;
		info->height = gBackBuffer->h;
		info->pitch = gBackBuffer->pitch;
		info->redShift = gBackBuffer->format->Rshift;
		info->greenShift = gBackBuffer->format->Gshift;
		info->blueShift = gBackBuffer->format->Bshift;
		info->redBits = 8-gBackBuffer->format->Rloss;
		info->greenBits = 8-gBackBuffer->format->Gloss;
		info->blueBits = 8-gBackBuffer->format->Bloss;
		info->supportsGfxSyscalls = 1;
		return 1;
	}

	static SDL_Surface *internalBackBuffer = NULL;
	static int maFrameBufferInit(void *data) {
		if(internalBackBuffer!=NULL) return 0;
		internalBackBuffer = gBackBuffer;
		gBackBuffer = SDL_CreateRGBSurfaceFrom(data, gBackBuffer->w, gBackBuffer->h, gBackBuffer->format->BitsPerPixel, gBackBuffer->pitch, gBackBuffer->format->Rmask, gBackBuffer->format->Gmask, gBackBuffer->format->Bmask, gBackBuffer->format->Amask);
		if(gBackBuffer == NULL) return 0;
		gDrawSurface = gBackBuffer;
		return 1;
	}

	static int maFrameBufferClose() {
		if(internalBackBuffer==NULL) return 0;
		SDL_FreeSurface(gBackBuffer);
		gBackBuffer = internalBackBuffer;
		internalBackBuffer = NULL;
		gDrawSurface = gBackBuffer;
		return 1;
	}

	static void fillBufferCallback() {
		MAEvent* ep = new MAEvent;
		ep->type = EVENT_TYPE_AUDIOBUFFER_FILL;
		ep->state = 1;
		SDL_UserEvent event = { FE_ADD_EVENT, 0, ep, NULL };
		FE_PushEvent((SDL_Event*)&event);
	}


	static int maAudioBufferInit(const MAAudioBufferInfo *ainfo) {
		AudioSource *src = AudioEngine::getChannel(1)->getAudioSource();
		if(src!=NULL) {
			src->close(); // todo: do a safe delete of the source here.. now it leaks memory,
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
			src->close(); // todo: do a safe delete of the source here.. now it leaks memory,
		}

		AudioEngine::getChannel(1)->setAudioSource(NULL);
		return 1;
	}


#ifdef MA_PROF_SUPPORT_VIDEO_STREAMING
	RtspConnection *rtspConnection;
	SYSCALL(int, maStreamVideoStart(const char* url)) {
		//int res;
		//if((res=rtspCreateConnection(url, rtspConnection))<0) return res;

		return -1;
	}

	SYSCALL(int, maStreamClose(MAHandle stream)) {
		return -1;
	}

	SYSCALL(int, maStreamPause(MAHandle stream)) {
		return -1;
	}

	SYSCALL(int, maStreamResume(MAHandle stream)) {
		return -1;
	}


	SYSCALL(int, maStreamVideoSize(MAHandle stream)) {
		return -1;
	}

	SYSCALL(int, maStreamVideoSetFrame(MAHandle stream, const MARect* rect)) {
		return -1;
	}

	SYSCALL(int, maStreamLength(MAHandle stream)) {
		return -1;
	}

	SYSCALL(int, maStreamPos(MAHandle stream)) {
		return -1;
	}

	SYSCALL(int, maStreamSetPos(MAHandle stream, int pos)) {
		return -1;
	}
#endif

	static int maSendTextSMS(const char* dst, const char* msg);
	//static int maStartVideoStream(const char* url);

#ifdef _MSC_VER
	static double atanh(double x) {
		double d = (1+x) / (1-x);
		d = 0.5 * log(d);
		return d;
	}
#endif

#ifdef SUPPORT_OPENGL_ES
	int maOpenGLInitFullscreen() {
		TEST_Z(Base::subViewOpen(sSkin->getScreenLeft(), sSkin->getScreenTop(), sSkin->getScreenWidth(), sSkin->getScreenHeight(), sSubView));
		TEST_Z(Base::openGLInit(sSubView));
		sOpenGLMode = true;

		return 0;
	}

	int maOpenGLCloseFullscreen() {
		TEST_Z(Base::openGLClose(sSubView));
		TEST_Z(Base::subViewClose(sSubView));
		return 0;
	}

	int maOpenGLTexImage2D(MAHandle image) {
		SDL_Surface* surface = gSyscall->resources.get_RT_IMAGE(image);

        // get the number of channels in the SDL surface
        GLint nOfColors = surface->format->BytesPerPixel;
		GLenum texture_format = 0;
        GLint flipColors = 0;

		int w = nextPowerOf2(1, surface->w);
		int h = nextPowerOf2(1, surface->h);
		bool createdPowerOfTwoTexture = false;
		if(w!=surface->w || h!=surface->h) {

			SDL_Surface* oldSurface =  surface;
			surface = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, surface->format->BitsPerPixel,
				surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);

			byte* src = (byte*)oldSurface->pixels;
			byte* dst = (byte*)surface->pixels;
			for(int y = 0; y < oldSurface->h; y++) {
				memcpy(dst, src, oldSurface->w*surface->format->BytesPerPixel);
				src+=oldSurface->pitch;
				dst+=surface->pitch;
			}

			createdPowerOfTwoTexture = true;
		}

		if (nOfColors == 4)     // contains an alpha channel
        {
                if (surface->format->Bmask == 0xff000000)
                        texture_format = GL_RGBA;
				else {
					texture_format = GL_RGBA;
					flipColors = 1; //texture_format = GL_BGR;
				}
		}
		else if (nOfColors == 3)     // no alpha channel
        {
                if (surface->format->Bmask == 0xff0000)
                        texture_format = GL_RGB;
				else {
					texture_format = GL_RGB;
					flipColors = 1; //texture_format = GL_BGR;
				}
        }

		if(texture_format == 0) {
			return MA_GL_TEX_IMAGE_2D_INVALID_IMAGE;
		}

		byte* data = (byte*)surface->pixels;
		if(flipColors) {
			int numBytes = surface->w * surface->h * surface->format->BytesPerPixel;
			byte* copy = new byte[numBytes];

			for(int i = 0; i < numBytes; i += nOfColors) {
				int rIndex = surface->format->Rshift/8;
				int gIndex = surface->format->Gshift/8;
				int bIndex = surface->format->Bshift/8;
				int aIndex = surface->format->Ashift/8;
				copy[i+0] = data[i+rIndex];
				copy[i+1] = data[i+gIndex];
				copy[i+2] = data[i+bIndex];
				if(nOfColors == 4)
					copy[i+3] = data[i+aIndex];
			}

			data = copy;
		}

		// Edit the texture object's image data using the information SDL_Surface gives us
		glTexImage2D( GL_TEXTURE_2D, 0, texture_format, surface->w, surface->h, 0,
						  texture_format, GL_UNSIGNED_BYTE, data);

		if(flipColors) {
			delete data;
		}

		if(createdPowerOfTwoTexture)
			SDL_FreeSurface(surface);

		return MA_GL_TEX_IMAGE_2D_OK;
	}

	int maOpenGLTexSubImage2D(MAHandle image) {
		SDL_Surface* surface = gSyscall->resources.get_RT_IMAGE(image);

        GLint nOfColors = surface->format->BytesPerPixel;
		GLenum texture_format = 0;
        GLint flipColors = 0;

		if (nOfColors == 4)     // contains an alpha channel
        {
                if (surface->format->Bmask == 0xff000000)
                        texture_format = GL_RGBA;
				else {
					texture_format = GL_RGBA;
					flipColors = 1; //texture_format = GL_BGR;
				}
		}
		else if (nOfColors == 3)     // no alpha channel
        {
                if (surface->format->Bmask == 0xff0000)
                        texture_format = GL_RGB;
				else {
					texture_format = GL_RGB;
					flipColors = 1; //texture_format = GL_BGR;
				}
        }

		byte* data = (byte*)surface->pixels;
		if(flipColors) {
			int numBytes = surface->w * surface->h * surface->format->BytesPerPixel;
			byte* copy = new byte[numBytes];

			for(int i = 0; i < numBytes; i += nOfColors) {
				int rIndex = surface->format->Rshift/8;
				int gIndex = surface->format->Gshift/8;
				int bIndex = surface->format->Bshift/8;
				int aIndex = surface->format->Ashift/8;
				copy[i+0] = data[i+rIndex];
				copy[i+1] = data[i+gIndex];
				copy[i+2] = data[i+bIndex];
				if(nOfColors == 4)
					copy[i+3] = data[i+aIndex];
			}

			data = copy;
		}

		// Edit the texture object's image data using the information SDL_Surface gives us
		glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, surface->w, surface->h, texture_format
						, GL_UNSIGNED_BYTE, data);

		if(flipColors) {
			delete data;
		}

		return MA_GL_TEX_IMAGE_2D_OK;
	}

#endif // SUPPORT_OPENGL_ES

	SYSCALL(longlong, maIOCtl(int function, int a, int b, int c, ...)) {
		va_list argptr;
		va_start(argptr, c);
		switch(function) {

#ifdef FAKE_CALL_STACK
		case maIOCtl_maReportCallStack:
			reportCallStack();
			return 0;

			maIOCtl_case(maDumpCallStackEx);
#endif

#ifdef MEMORY_PROTECTION
		case maIOCtl_maProtectMemory:
			SYSCALL_THIS->protectMemory(a, b);
			return 0;
		case maIOCtl_maUnprotectMemory:
			SYSCALL_THIS->unprotectMemory(a, b);
			return 0;
		case maIOCtl_maSetMemoryProtection:
			SYSCALL_THIS->setMemoryProtection(a);
			return 0;
		case maIOCtl_maGetMemoryProtection:
			return SYSCALL_THIS->getMemoryProtection();
#endif

#ifdef LOGGING_ENABLED
		case maIOCtl_maWriteLog:
			{
				const char* ptr = (const char*)gSyscall->GetValidatedMemRange(a, b);
				LogBin(ptr, b);
				if(ptr[b-1] == '\n')	//hack to get rid of EOL
					b--;
				report(REPORT_STRING, ptr, b);
			}
			return 0;
#endif	//LOGGING_ENABLED

#ifdef SUPPORT_OPENGL_ES
		maIOCtl_IX_OPENGL_ES_caselist;
#endif	//SUPPORT_OPENGL_ES

			maIOCtl_sinh_case(::sinh);
			maIOCtl_cosh_case(::cosh);
			maIOCtl_case(atanh);

			maIOCtl_case(maAccept);

		case maIOCtl_maBtStartDeviceDiscovery:
			return BLUETOOTH(maBtStartDeviceDiscovery)(BtWaitTrigger, a != 0);

		case maIOCtl_maBtStartServiceDiscovery:
			return BLUETOOTH(maBtStartServiceDiscovery)(GVMRA(MABtAddr), GVMR(b, MAUUID), BtWaitTrigger);

			maIOCtl_syscall_case(maBtGetNewDevice);
			maIOCtl_syscall_case(maBtGetNewService);
			maIOCtl_maBtGetNextServiceSize_case(BLUETOOTH(maBtGetNextServiceSize));
			maIOCtl_maBtCancelDiscovery_case(BLUETOOTH(maBtCancelDiscovery));

		case maIOCtl_maPlatformRequest:
			{
#ifdef WIN32
				const char* url = SYSCALL_THIS->GetValidatedStr(a);
				if(sstrcmp(url, "http://") == 0 || sstrcmp(url, "https://") == 0) {
					int result = (int)ShellExecute(NULL, NULL, url, NULL, NULL, SW_SHOW);
					if(result <= 32) {
						LOG("ShellExecute(%s) error %i\n", url, result);
						return CONNERR_GENERIC;
					}
					return 0;
				} else
#endif
				{
					return CONNERR_UNAVAILABLE;
				}
			}

			maIOCtl_case(maSendTextSMS);

			//maIOCtl_case(maStartVideoStream);

			maIOCtl_case(maSendToBackground);
			maIOCtl_case(maBringToForeground);

			maIOCtl_case(maFrameBufferGetInfo);
		case maIOCtl_maFrameBufferInit:
			return maFrameBufferInit(SYSCALL_THIS->GetValidatedMemRange(a,
				gBackBuffer->pitch*gBackBuffer->h));
			maIOCtl_case(maFrameBufferClose);

			maIOCtl_case(maAudioBufferInit);
			maIOCtl_case(maAudioBufferReady);
			maIOCtl_case(maAudioBufferClose);

#ifdef MA_PROF_SUPPORT_VIDEO_STREAMING
			maIOCtl_case(maStreamVideoStart);
			maIOCtl_case(maStreamClose);
			maIOCtl_case(maStreamPause);
			maIOCtl_case(maStreamResume);
			maIOCtl_case(maStreamVideoSize);
			maIOCtl_case(maStreamVideoSetFrame);
			maIOCtl_case(maStreamLength);
			maIOCtl_case(maStreamPos);
			maIOCtl_case(maStreamSetPos);
#endif	//MA_PROF_SUPPORT_VIDEO_STREAMING

			maIOCtl_syscall_case(maFileOpen);
			maIOCtl_syscall_case(maFileExists);
			maIOCtl_syscall_case(maFileClose);
			maIOCtl_syscall_case(maFileCreate);
			maIOCtl_syscall_case(maFileDelete);
			maIOCtl_syscall_case(maFileSize);
			maIOCtl_syscall_case(maFileAvailableSpace);
			maIOCtl_syscall_case(maFileTotalSpace);
			maIOCtl_syscall_case(maFileDate);
			maIOCtl_syscall_case(maFileRename);
			maIOCtl_syscall_case(maFileTruncate);

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

			maIOCtl_case(maCameraFormatNumber);
			maIOCtl_case(maCameraFormat);
			maIOCtl_case(maCameraStart);
			maIOCtl_case(maCameraStop);
			maIOCtl_case(maCameraSnapshot);

			maIOCtl_case(maDBOpen);
			maIOCtl_case(maDBClose);
			maIOCtl_case(maDBExecSQL);
			maIOCtl_case(maDBExecSQLParams);
			maIOCtl_case(maDBCursorDestroy);
			maIOCtl_case(maDBCursorNext);
			maIOCtl_case(maDBCursorGetColumnData);
			maIOCtl_case(maDBCursorGetColumnText);
			maIOCtl_case(maDBCursorGetColumnInt);
			maIOCtl_case(maDBCursorGetColumnDouble);
#ifdef EMULATOR
		maIOCtl_syscall_case(maPimListOpen);
		maIOCtl_syscall_case(maPimListNext);
		maIOCtl_syscall_case(maPimListClose);
		maIOCtl_syscall_case(maPimItemCount);
		maIOCtl_syscall_case(maPimItemGetField);
		maIOCtl_syscall_case(maPimItemFieldCount);
		maIOCtl_syscall_case(maPimItemGetAttributes);
		maIOCtl_syscall_case(maPimFieldType);
		maIOCtl_syscall_case(maPimItemGetValue);
		maIOCtl_syscall_case(maPimItemSetValue);
		maIOCtl_syscall_case(maPimItemAddValue);
		maIOCtl_syscall_case(maPimItemRemoveValue);
		maIOCtl_syscall_case(maPimItemClose);
		maIOCtl_syscall_case(maPimItemCreate);
		maIOCtl_syscall_case(maPimItemRemove);
#endif	//EMULATOR

		case maIOCtl_maGetSystemProperty:
			return maGetSystemProperty(SYSCALL_THIS->GetValidatedStr(a),
				(char*)SYSCALL_THIS->GetValidatedMemRange(b, c), c);

#ifdef WIN32
			maIOCtl_case(maTextBox);
#endif

		case maIOCtl_maSyscallPanicsEnable:
			LOG("maSyscallPanicsEnable\n");
			gSyscall->mPanicOnProgrammerError = true;
			return RES_OK;
		case maIOCtl_maSyscallPanicsDisable:
			LOG("maSyscallPanicsDisable\n");
			gSyscall->mPanicOnProgrammerError = false;
			return RES_OK;

		default:
			LOGD("maIOCtl(%i) unimplemented.\n", function);
			return IOCTL_UNAVAILABLE;
		}
	}	//maIOCtl

	//****************************************************************************
	// maTextBox
	//****************************************************************************
#ifdef WIN32
	static HWND sMainWnd = NULL, sEditBox = NULL, sTextBox = NULL;
	static wchar_t* sTextBoxOutBuf;
	static int sTextBoxOutSize;
	static const wchar_t* sTextBoxInBuf;

	static INT_PTR CALLBACK TextBoxProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		LOGD("TextBoxProc 0x%p, 0x%04x, 0x%x, 0x%x\n", hwnd, uMsg, wParam, (uint)lParam);
		switch(uMsg) {
		case WM_INITDIALOG:
			sEditBox = GetDlgItem(hwnd, IDC_EDIT1);

			// We want a font that supports as much Unicode as possible.
			// Let's use the same font we use for maDrawText().
			if(gWindowsUnifont)
				SendMessage(sEditBox, WM_SETFONT, (WPARAM)gWindowsUnifont, FALSE);

			{
				// fix EOL (add 0x0D bytes)
				std::wstring in;
				in.reserve(wcslen(sTextBoxInBuf));
				const wchar_t* src = sTextBoxInBuf;
				while(*src) {
					if(*src == 0x0A)
						in += (wchar_t)0x0D;
					in += *src;
					src++;
				}
				SetWindowTextW(sEditBox, in.c_str());
			}
			SendMessage(sEditBox, EM_SETLIMITTEXT, sTextBoxOutSize - 1, 0);
			SetFocus(sEditBox);
			break;
		case WM_COMMAND:
			{
				WORD id = LOWORD(wParam);
				if(id == IDOK || id == IDCANCEL) {
					// get text
					int length = GetWindowTextW(sEditBox, sTextBoxOutBuf, sTextBoxOutSize);
					// fix EOL (remove 0x0D bytes)
					const wchar_t* src = sTextBoxOutBuf;
					wchar_t* dst = sTextBoxOutBuf;
					length = 0;
					while(*src) {
						if(*src != 0x0D) {
							*dst = *src;
							dst++;
							length++;
						}
						src++;
					}
					*dst = 0;

					// send event
					MAEvent e;
					e.type = EVENT_TYPE_TEXTBOX;
					e.textboxResult = (id == IDOK) ? MA_TB_RES_OK : MA_TB_RES_CANCEL;
					e.textboxLength = length;
					gEventFifo.put(e);

					// time to close
					LOG("DestroyWindow\n");
					//GLE(DestroyWindow(hwnd));
					EndDialog(hwnd, id);
					sEditBox = sTextBox = NULL;
				}
			}
		}
		return FALSE;
	}

	static int Base::maTextBox(const wchar* title, const wchar* inText, wchar* outText,
		int maxSize, int constraints)
	{
		MYASSERT(sTextBox == NULL, ERR_TEXTBOX_ACTIVE);
		SDL_SysWMinfo info;
		SDL_VERSION(&info.version);
		DEBUG_ASSERT(SDL_GetWMInfo(&info));
		sMainWnd = info.window;

		sTextBoxInBuf = (wchar_t*)inText;
		sTextBoxOutSize = maxSize;
		sTextBoxOutBuf = (wchar_t*)outText;

#if 0	// direct edit-box in window. No worky.
		// Setting window style WS_CLIPCHILDREN prevents drawing on top of the EditBox.
		LONG windowStyle = GetWindowLong(sMainWnd, GWL_STYLE);
		SetWindowLong(sMainWnd, GWL_STYLE, windowStyle | WS_CLIPCHILDREN);

		sEditBox = CreateWindowW(L"EDIT",	// predefined class
			(LPCWSTR)inText,
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
			sMainWnd,	// parent window
			NULL, GetModuleHandle(NULL), NULL);
		GLE(sEditBox);
		GLE(SetWindowPos(sEditBox, NULL,
			sSkin->getScreenLeft(), sSkin->getScreenTop(),
			sSkin->getScreenWidth(), sSkin->getScreenHeight(), 0));
		SetFocus(sEditBox);
#else
#if 0	// modeless dialogs don't recieve key events
		sTextBox = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_TEXTBOX), sMainWnd,
			TextBoxProc);
		GLE(sTextBox);
		ShowWindow(sTextBox, SW_SHOW);
#else	// so we go with modal, for now.
		int res = DialogBoxW(GetModuleHandle(NULL), MAKEINTRESOURCEW(IDD_TEXTBOX), sMainWnd,
			TextBoxProc);
		GLECUSTOM(res <= 0);

		// hack to fix issue 843.
		if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(1))	// if left button is pressed
			PostMessage(sMainWnd, WM_LBUTTONUP, 0, 0);
#endif
#endif
		return 0;
	}
#endif	//WIN32

	//****************************************************************************
	// camera
	//****************************************************************************

	// vga
#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 480
#define CAMERA_BOX_RADIUS_OUTER 50
#define CAMERA_BOX_RADIUS_INNER 25
#define CAMERA_FPS 20
#define CAMERA_SPEED 4	// in pixels per frame

	static int Base::maCameraFormatNumber() {
		return 1;
	}

	static int Base::maCameraFormat(int index, MA_CAMERA_FORMAT* fmt) {
		MYASSERT(index == 0, ERR_INVALID_CAMERA_FORMAT_INDEX);
		fmt->width = CAMERA_WIDTH;
		fmt->height = CAMERA_HEIGHT;
		return 0;
	}

	static Uint32 cameraViewFinderCallback(Uint32 interval, void*) {
		SDL_UserEvent event = { FE_CAMERA_VIEWFINDER_UPDATE, 0, NULL, NULL };
		FE_PushEvent((SDL_Event*)&event);
		return interval;
	}

	static void drawCameraViewFinderBox(int radius, Uint32 color) {
		SDL_Rect rect;
		rect.x = gCameraViewFinderPoint.x - radius;
		rect.y = gCameraViewFinderPoint.y - radius;
		rect.w = radius * 2;
		rect.h = radius * 2;
		DEBUG_ASRTZERO(SDL_FillRect(gBackBuffer, &rect, color));
	}

	static void bouncingBoxCoordUpdate(int& coord, int& direction, int radius, int limit) {
		coord += direction;
		if(coord + radius > limit) {
			coord = limit - radius;
			direction *= -1;
		}
		if(coord - radius < 0) {
			coord = radius;
			direction *= -1;
		}
	}

	static void Base::cameraViewFinderUpdate() {
		if(!gCameraViewFinderActive)
			return;

		// draw background
		SDL_Rect rect = { 0, 0, (Uint16)gBackBuffer->w, (Uint16)gBackBuffer->h };
		DEBUG_ASRTZERO(SDL_FillRect(gBackBuffer, &rect,
			SDL_MapRGB(gBackBuffer->format, 0x80, 0x80, 0x80)));

		// draw box
		drawCameraViewFinderBox(CAMERA_BOX_RADIUS_OUTER,
			SDL_MapRGB(gBackBuffer->format, 0, 0, 0));
		drawCameraViewFinderBox(CAMERA_BOX_RADIUS_INNER,
			SDL_MapRGB(gBackBuffer->format, 0x80, 0x80, 0x80));

		// update coordinates
		bouncingBoxCoordUpdate(gCameraViewFinderPoint.x, gCameraViewFinderDirection.x,
			CAMERA_BOX_RADIUS_OUTER, gBackBuffer->w);
		bouncingBoxCoordUpdate(gCameraViewFinderPoint.y, gCameraViewFinderDirection.y,
			CAMERA_BOX_RADIUS_OUTER, gBackBuffer->h);

		MAUpdateScreen();
	}

	static int Base::maCameraStart() {
		if(gCameraViewFinderActive)
			return 0;
		gCameraViewFinderActive = true;
		gCameraViewFinderPoint.x = CAMERA_BOX_RADIUS_OUTER;
		gCameraViewFinderPoint.y = CAMERA_BOX_RADIUS_OUTER;
		gCameraViewFinderDirection.x = CAMERA_SPEED;
		gCameraViewFinderDirection.y = CAMERA_SPEED;
		gCameraViewFinderTimer = SDL_AddTimer(1000 / CAMERA_FPS, cameraViewFinderCallback, NULL);
		DEBUG_ASSERT(gCameraViewFinderTimer);
		return 1;
	}

	static int Base::maCameraStop() {
		if(!gCameraViewFinderActive)
			return 0;
		gCameraViewFinderActive = false;
		DEBUG_ASSERT(SDL_RemoveTimer(gCameraViewFinderTimer));
		return 1;
	}

	static void fiomf(FREE_IMAGE_FORMAT fif, const char *msg) {
		LOG("fiomf: %i, '%s'\n", fif, msg);
	}

	static int Base::maCameraSnapshot(int formatIndex, MAHandle placeholder) {
		LOGD("maCameraSnapshot(%i, %i)\n", formatIndex, placeholder);
		if(!gCameraViewFinderActive)
			return -2;

		FreeImage_SetOutputMessage(fiomf);

		// copy backbuffer to FIB
		FIBITMAP* dib = FreeImage_Allocate(CAMERA_WIDTH, CAMERA_HEIGHT, 24,
			0xff0000, 0x00ff00, 0x0000ff);
		DEBUG_ASSERT(dib);
		SDL_Surface* dib_surface = SDL_CreateRGBSurfaceFrom(FreeImage_GetBits(dib),
			CAMERA_WIDTH, CAMERA_HEIGHT, 24, CAMERA_WIDTH*3, 0xff0000, 0x00ff00, 0x0000ff, 0);
		DEBUG_ASSERT(dib_surface);

		// can't blit all at once; result is upside-down.
		for(int i=0, dy=CAMERA_HEIGHT-1; i<gBackBuffer->h; i++, dy--) {
			SDL_Rect src = { 0,(Sint16)i, (Uint16)gBackBuffer->w, 1 };
			SDL_Rect dst = { 0,(Sint16)dy, (Uint16)gBackBuffer->w, 1 };
			DEBUG_ASRTZERO(SDL_BlitSurface(gBackBuffer, &src, dib_surface, &dst));
		}

		// open a memory stream
		FIMEMORY *hmem = FreeImage_OpenMemory();
		// encode and save the image to the memory
		DEBUG_ASSERT(FreeImage_SaveToMemory(FIF_JPEG, dib, hmem, 0));
		//DEBUG_ASSERT(FreeImage_Save(FIF_JPEG, dib, "dump.jpg"));

		FreeImage_Unload(dib);

#ifdef LOGGING_ENABLED
		// at this point, hmem contains the entire data in memory stored in fif format.
		// the amount of space used by the memory is equal to file_size
#ifdef DEBUGGING_MODE
		long file_size = FreeImage_TellMemory(hmem);
		LOG("File size : %ld\n", file_size);
#endif
#endif

		DWORD data_size;
		BYTE* data;
		DEBUG_ASSERT(FreeImage_AcquireMemory(hmem, &data, &data_size));

		MemStream* ms = new MemStream(data_size);
		DEBUG_ASSERT(ms->write(data, data_size));
		SYSCALL_THIS->resources.add_RT_BINARY(placeholder, ms);

		// make sure to close the stream since FreeImage_SaveToMemory
		// will cause internal memory allocations and this is the only
		// way to free this allocated memory
		FreeImage_CloseMemory(hmem);
		return 0;
	}

	//****************************************************************************
	// videoStream
	//****************************************************************************

#if 0
	int maStartVideoStream(const char* url) {
		int res;
		SDL_ffmpegFile* file = SDL_ffmpegOpen("C:/test.mpg");
		LOG("SDL_ffmpegOpen 0x%x\n", file);
		DEBUG_ASSERT(file != NULL);

		/* print some info on detected stream to output */
		int s;
		SDL_ffmpegStream *str;

		for(s = 0; s<file->AStreams; s++) {
			str = SDL_ffmpegGetAudioStream(file, s);

			LOG("Info on audiostream #%i:\n", s);
			LOG("\tChannels: %i\n",      str->channels);
			if(strlen(str->language)) LOG("\tLanguage: %s\n",      str->language);
			LOG("\tSampleRate: %i\n",    str->sampleRate);
		}

		for(s = 0; s<file->VStreams; s++) {
			str = SDL_ffmpegGetVideoStream(file, s);

			LOG("Info on videostream #%i:\n", s);
			if(strlen(str->language)) LOG("\tLanguage: %s\n",      str->language);
			LOG("\tFrame: %ix%i\n",  str->width, str->height);
			LOG("\tFrameRate: %.2ffps\n",  1.0 / (str->frameRate[0] / str->frameRate[1]));
		}

    res = SDL_ffmpegSelectVideoStream(file, 0);
		LOG("SDL_ffmpegSelectVideoStream %i\n", res);

    int w,h;
    /* we get the size from our active video stream, if no active video stream */
    /* exists, width and height are set to default values (320x240) */
    res = SDL_ffmpegGetVideoSize(file, &w, &h);
		LOG("SDL_ffmpegGetVideoSize %i, %ix%i\n", res, w, h);

		res = SDL_ffmpegStartDecoding(file);
		LOG("SDL_ffmpegStartDecoding %i\n", res);

    int done = 0;

		while( !done ) {
			/* just some standard SDL event stuff */
			SDL_Event event;
			while(SDL_PollEvent(&event)) {

				if(event.type == SDL_QUIT) {
					done = 1;
					break;
				}
			}
			/* we retrieve the current image from the file */
			/* we get 0 if no file could be retrieved */
			/* important! please note this call should be paired with SDL_ffmpegReleaseVideo */
			SDL_ffmpegVideoFrame* frame = SDL_ffmpegGetVideoFrame(file);
			LOG("SDL_ffmpegGetVideoFrame 0x%x\n", frame);

			if(frame) {

				/* we got a frame, so we better show this one */
				SDL_BlitSurface(frame->buffer, 0, gScreen, 0);

				/* After releasing this frame, you can no longer use it. */
				/* you should call this function every time you get a frame! */
				SDL_ffmpegReleaseVideo(file, frame);

				/* we flip the double buffered screen so we might actually see something */
				SDL_Flip(gScreen);
			}

			/* we wish not to kill our poor cpu, so we give it some timeoff */
			SDL_Delay(5);
		}

    /* after all is said and done, we should call this */
    SDL_ffmpegFree(file);

		return 0;
	}
#endif	//0

	SYSCALL(void, maPanic(int result, const char* message)) {
		LOG("maPanic %i, \"%s\"\n", result, message);
		std::string msg(message);
		std::string s = "User Panic: \""+msg+"\"";
		report(REPORT_EXIT_STRING, s.c_str(), s.length());

		{	//dump panic report
			int buflen = sizeof(MAPanicReport) + msg.length();
			Smartie<char> buffer(new char[buflen]);
			MAPanicReport& pr(*(MAPanicReport*)buffer());
			fillPanicReport(pr, REPORT_USER_PANIC);
			pr.code = result;
			strcpy(pr.string, msg.c_str());
			DEBUG_ASSERT(strlen(pr.string) == msg.length());
			WriteFileStream file("panic.report");
			file.write(buffer(), buflen);
		}

		if(!gReload) {
#ifdef __USE_FULLSCREEN__
			SDL_Quit( );
#endif
			MoSyncMessageBox(message, "User Panic");
		}
		DEBUG_BREAK;
		MoSyncExit(result);
	}

	int maSendTextSMS(const char* dst, const char* msg) {
		char buffer[64];
		int i=0;
		do {
			sprintf(buffer, "sms%i.txt", i);
			FileStream file(buffer);
			if(!file.isOpen())
				break;
			i++;
		} while(true);
		WriteFileStream file(buffer);
		std::string str;
		str += "To: ";
		str += dst;
		str += "\n\n";
		str += msg;
		str += "\n";
		bool res = file.write(str.c_str(), str.size());
		return res ? 0 : CONNERR_GENERIC;
	}

	static int Base::maSendToBackground() {
#ifdef WIN32
		SDL_SysWMinfo info;
		SDL_VERSION(&info.version);
		if(SDL_GetWMInfo(&info)) {
			return ShowWindow(info.window, SW_MINIMIZE);
		} else {
			return -2;
		}
#elif defined(LINUX) || defined(DARWIN)
		//BIG_PHAT_ERROR(ERR_FUNCTION_UNSUPPORTED);
		return IOCTL_UNAVAILABLE;
#else
#error Unknown platform!
#endif
	}

	static int Base::maBringToForeground() {
#ifdef WIN32
		SDL_SysWMinfo info;
		SDL_VERSION(&info.version);
		if(SDL_GetWMInfo(&info)) {
			return ShowWindow(info.window, SW_RESTORE);
		} else {
			return -2;
		}
#elif defined(LINUX) || defined(DARWIN)
		//BIG_PHAT_ERROR(ERR_FUNCTION_UNSUPPORTED);
		return IOCTL_UNAVAILABLE;
#else
#error Unknown platform!
#endif
	}

	static int Base::maGetSystemProperty(const char* key, char* buf, int size) {
#ifdef WIN32
		if(strcmp(key, "mosync.iso-639-1") == 0) {
			LCID lcid = GetUserDefaultLCID();
			int res = GetLocaleInfo(lcid, LOCALE_SISO639LANGNAME, buf, size);
			GLE(res);
			return res;
		}
#endif
		if(strcmp(key, "mosync.device") == 0) {
			static const char model[] = "MoSync Emulator";
			if(size >= (int)sizeof(model)) {
				memcpy(buf, model, sizeof(model));
			}
			return sizeof(model);
		}
		return -2;
	}

void MoSyncExit(int r) {
	reportIp(r, "Exit");
	if(gReload) {
		gReload = false;

		//reset all syscall structures.

		//freeChannel(0);
		AudioEngine::close();

		MANetworkReset();

		//there is no Bluetooth cancel function, so we'll just ignore that for now.
		//chalk one up for Known Issues.

		gDrawTargetHandle = HANDLE_SCREEN;
		gCurrentUnconvertedColor = gCurrentConvertedColor = 0;
		gDrawSurface = gBackBuffer;

		//reset the clip rect
		gBackBuffer->clip_rect.x = 0;
		gBackBuffer->clip_rect.y = 0;
		gBackBuffer->clip_rect.w = gBackBuffer->w;
		gBackBuffer->clip_rect.h = gBackBuffer->h;

		maFillRect(0, 0, gBackBuffer->w, gBackBuffer->h);

		reloadProgram();
	} else {
		if(gExitTimer) {
			SDL_bool res = SDL_RemoveTimer(gExitTimer);
			DEBUG_ASSERT(res);
		}
#ifdef USE_MALIBQUIT
		MALibQuit();	//disabled, hack to allow static destructors
#endif
		AudioEngine::close();
		MoSyncDBClose();

		reportClose();
		exit(r);
	}
}

void MoSyncErrorExit(int errorCode) {
	LOG("ErrorExit %i\n", errorCode);
	char buffer[256];
	char repBuf[256];
#ifdef TRANSLATE_PANICS
	char* ptr = buffer + sprintf(buffer, "MoSync Panic %i\n\n", errorCode);
	char* repPtr = repBuf + sprintf(repBuf, "MoSync Panic %i. ", errorCode);
	if(!isBaseError(errorCode)) {
		ptr += sprintf(ptr, "The %s subsystem reports: ", subSystemString(errorCode));
		repPtr += sprintf(repPtr, "The %s subsystem reports: ", subSystemString(errorCode));
	}
	ptr += sprintf(ptr, "\"%s\"\n", errorDescription(errorCode));
	repPtr += sprintf(repPtr, "\"%s\"", errorDescription(errorCode));
	addRuntimeSpecificPanicInfo(ptr, true);
	addRuntimeSpecificPanicInfo(repPtr, false);
#endif
	LOG("%s", buffer);

	{	//dump panic report
		MAPanicReport pr;
		fillPanicReport(pr, REPORT_PANIC);
		pr.code = errorCode;
		WriteFileStream file("panic.report");
		file.write(&pr, sizeof(pr));
	}

	report(REPORT_EXIT_STRING, repBuf, strlen(repBuf));

	if(!gReload) {
#ifdef __USE_FULLSCREEN__
		SDL_Quit( );
#endif
		MoSyncMessageBox(buffer, "MoSync Panic");
	}

	DEBUG_BREAK;
	MoSyncExit(errorCode);
}
