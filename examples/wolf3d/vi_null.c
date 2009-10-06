/* input/video output "/dev/null" support */

/* this file does nothing but sit there and look pretty dumb */

#include "wl_def.h"

byte *gfxbuf = NULL;
static unsigned char pal[768];
int gOrientation;

#include <MAUtil/FrameBuffer.h>

extern void keyboard_handler(int code, int press);
extern boolean InternalKeyboard[NumCodes];

#include <IX_AUDIOBUFFER.h>

/*
==========================
=
= Quit
=
==========================
*/

void Quit(const char *error)
{
	memptr screen = NULL;

	if (!error || !*error) {
		CA_CacheGrChunk(ORDERSCREEN);
		screen = grsegs[ORDERSCREEN];
		WriteConfig();
	} else if (error) {
		CA_CacheGrChunk(ERRORSCREEN);
		screen = grsegs[ERRORSCREEN];
	}

	ShutdownId();

	if (screen) {
		/* printf("spiffy ansi screen goes here..\n"); */
	}

	if (error && *error) {
		//fprintf(stderr, "Quit: %s\n", error);
		//exit(EXIT_FAILURE);
		maPanic(EXIT_FAILURE, error);
 	}
	exit(EXIT_SUCCESS);
}

void VL_WaitVBL(int vbls)
{
	unsigned long last = get_TimeCount() + vbls;
	while (last > get_TimeCount()) {
		INL_Update();
	}
}

void VW_UpdateScreen()
{
// do palettized gfxbuf > frameBuffer conversion here..

	FrameBuffer_copyRect(0, 0, 320, 200, 0, 0, gfxbuf, 320);
	maResetBacklight();
	maUpdateScreen();
	//INL_Update();
}

/*
=======================
=
= VL_Startup
=
=======================
*/

void VL_Startup()
{
	if (gfxbuf == NULL) {
		gfxbuf = malloc(320 * 200 * 1);
		FrameBuffer_init(320, 200, ORIENTATION_0, 0);
	}
}

/*
=======================
=
= VL_Shutdown
=
=======================
*/

void VL_Shutdown()
{
	if (gfxbuf != NULL) {
		free(gfxbuf);
		gfxbuf = NULL;
		FrameBuffer_close();
	}
}

//===========================================================================

/*
=================
=
= VL_SetPalette
=
=================
*/

void VL_SetPalette(const byte *palette)
{
	memcpy(pal, palette, 768);
	FrameBuffer_setPalette(0, 0, 256, palette, FLAG_RGB666);
}

/*
=================
=
= VL_GetPalette
=
=================
*/

void VL_GetPalette(byte *palette)
{
	memcpy(palette, pal, 768);
}

/*
void INL_Update()
{
}
*/

static int XKeysymToScancode(unsigned int keysym)
{
	if(keysym == MAK_UP || keysym == MAK_DOWN || keysym == MAK_LEFT || keysym == MAK_RIGHT) {
		keysym = FrameBuffer_getArrowKeyForOrientation(keysym);
	}

	switch (keysym) {
//		case SDLK_KP_ENTER:
//		case SDLK_RETURN:
		case MAK_SOFTRIGHT:
			return sc_Enter;
//		case SDLK_ESCAPE:
		case MAK_0:
			return sc_Escape;
		//case SDLK_SPACE:
		case MAK_SOFTLEFT:
			return sc_Space;
//		case SDLK_BACKSPACE:
//			return sc_BackSpace;
//		case SDLK_TAB:
//			return sc_Tab;
//		case SDLK_LALT:
//		case SDLK_RALT:
//			return sc_Alt;
//		case SDLK_LCTRL:
//		case SDLK_RCTRL:
		case MAK_FIRE:
			return sc_Control;
/*
		case SDLK_CAPSLOCK:
			return sc_CapsLock;
		case SDLK_LSHIFT:
			return sc_LShift;
		case SDLK_RSHIFT:
			return sc_RShift;
*/
//		case SDLK_UP:
//		case SDLK_KP8:
		case MAK_UP:
			return sc_UpArrow;
//		case SDLK_DOWN:
//		case SDLK_KP2:
		case MAK_DOWN:
			return sc_DownArrow;
//		case SDLK_LEFT:
//		case SDLK_KP4:
		case MAK_LEFT:
			return sc_LeftArrow;
//		case SDLK_RIGHT:
//		case SDLK_KP6:
		case MAK_RIGHT:
			return sc_RightArrow;
/*
		case SDLK_HOME:
			return sc_Home;
		case SDLK_END:
			return sc_End;
		case SDLK_PAGEUP:
			return sc_PgUp;
		case SDLK_PAGEDOWN:
			return sc_PgDn;
		case SDLK_INSERT:
			return sc_Insert;
		case SDLK_DELETE:
			return sc_Delete;
		case SDLK_F1:
			return sc_F1;
		case SDLK_F2:
			return sc_F2;
		case SDLK_F3:
			return sc_F3;
		case SDLK_F4:
			return sc_F4;
		case SDLK_F5:
			return sc_F5;
		case SDLK_F6:
			return sc_F6;
		case SDLK_F7:
			return sc_F7;
		case SDLK_F8:
			return sc_F8;
		case SDLK_F9:
			return sc_F9;
		case SDLK_F10:
			return sc_F10;
		case SDLK_F11:
			return sc_F11;
		case SDLK_F12:
			return sc_F12;

		case SDLK_1:
			return sc_1;
		case SDLK_2:
			return sc_2;
		case SDLK_3:
			return sc_3;
		case SDLK_4:
			return sc_4;
		case SDLK_5:
			return sc_5;
		case SDLK_6:
			return sc_6;
		case SDLK_7:
			return sc_7;
		case SDLK_8:
			return sc_8;
		case SDLK_9:
			return sc_9;
		case SDLK_0:
			return sc_0;

		case SDLK_a:
			return sc_A;
		case SDLK_b:
			return sc_B;
		case SDLK_c:
			return sc_C;
		case SDLK_d:
			return sc_D;
		case SDLK_e:
			return sc_E;
		case SDLK_f:
			return sc_F;
		case SDLK_g:
			return sc_G;
		case SDLK_h:
			return sc_H;
		case SDLK_i:
			return sc_I;
		case SDLK_j:
			return sc_J;
		case SDLK_k:
			return sc_K;
		case SDLK_l:
			return sc_L;
		case SDLK_m:
			return sc_M;
		case SDLK_n:
			return sc_N;
		case SDLK_o:
			return sc_O;
		case SDLK_p:
			return sc_P;
		case SDLK_q:
			return sc_Q;
		case SDLK_r:
			return sc_R;
		case SDLK_s:
			return sc_S;
		case SDLK_t:
			return sc_T;
		case SDLK_u:
			return sc_U;
		case SDLK_v:
			return sc_V;
		case SDLK_w:
			return sc_W;
		case SDLK_x:
			return sc_X;
		case SDLK_y:
			return sc_Y;
		case SDLK_z:
			return sc_Z;
		case SDLK_PAUSE:
			return 0xE1;
		*/
		default:
			return sc_None;
	}
}

void INL_Update()
{
	MAEvent event;
	while (maGetEvent(&event)) {
			switch(event.type) {
				case EVENT_TYPE_POINTER_PRESSED:
					keyboard_handler(sc_Space, 1);
				break;
				case EVENT_TYPE_POINTER_RELEASED:
					keyboard_handler(sc_Space, 0);
				break;

				case EVENT_TYPE_KEY_PRESSED:
					if(event.key == MAK_1) {
						FrameBuffer_setOrientation(gOrientation++);
						VW_UpdateScreen();
						break;
					}
					keyboard_handler(XKeysymToScancode(event.key), 1);
					break;
				case EVENT_TYPE_KEY_RELEASED:
					keyboard_handler(XKeysymToScancode(event.key), 0);
					break;
				case EVENT_TYPE_CLOSE:
					Quit(NULL);
					break;
				case EVENT_TYPE_AUDIOBUFFER_FILL:
					AudioBufferFill();
					break;
				default:
					break;
			}
	}
	VW_UpdateScreen();
}

void IN_GetMouseDelta(int *dx, int *dy)
{
	/*
	*dx = NULL;
	*dy = NULL;
	*/
}

byte IN_MouseButtons()
{
	return 0;
}

/*
===================
=
= IN_JoyButtons
=
===================
*/

byte IN_JoyButtons()
{
	return 0;
}

///////////////////////////////////////////////////////////////////////////
//
//	IN_GetJoyAbs() - Reads the absolute position of the specified joystick
//
///////////////////////////////////////////////////////////////////////////
void IN_GetJoyAbs(word joy,word *xp,word *yp)
{
	*xp = 0;
	*yp = 0;
}

///////////////////////////////////////////////////////////////////////////
//
//	INL_GetJoyDelta() - Returns the relative movement of the specified
//		joystick (from +/-127)
//
///////////////////////////////////////////////////////////////////////////
void INL_GetJoyDelta(word joy,int *dx,int *dy)
{
	*dx = 0;
	*dy = 0;
}

///////////////////////////////////////////////////////////////////////////
//
//	INL_GetJoyButtons() - Returns the button status of the specified
//		joystick
//
///////////////////////////////////////////////////////////////////////////
word INL_GetJoyButtons(word joy)
{
	return 0;
}

///////////////////////////////////////////////////////////////////////////
//
//      IN_SetupJoy() - Sets up thresholding values and calls INL_SetJoyScale()
//              to set up scaling values
//
///////////////////////////////////////////////////////////////////////////
void IN_SetupJoy(word joy,word minx,word maxx,word miny,word maxy)
{
}

/*
int main(int argc, char *argv[])
{
	vwidth = 320;
	vheight = 200;

	return WolfMain(argc, argv);
}
*/
