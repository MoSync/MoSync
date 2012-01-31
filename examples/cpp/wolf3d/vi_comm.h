#ifndef __VI_COMM_H__
#define __VI_COMM_H__

/* ======================================================================== */

extern byte *gfxbuf;

void VL_Startup();
void VL_Shutdown();

void VL_WaitVBL(int vbls);
void VW_UpdateScreen();

void VL_SetPalette(const byte *palette);
void VL_GetPalette(byte *palette);

void VL_MemToScreen(const byte *source, int width, int height, int x, int y);

/* ======================================================================== */

#define	MaxPlayers	4
#define	MaxJoys		2
#define	NumCodes	128

typedef	int8_t ScanCode;

#define	sc_None			0
#define	sc_Bad			0xff
#define	sc_Return		0x1c
#define	sc_Enter		sc_Return
#define	sc_Escape		0x01
#define	sc_Space		0x39
#define	sc_BackSpace		0x0e
#define	sc_Tab			0x0f
#define	sc_Alt			0x38
#define	sc_Control		0x1d
#define	sc_CapsLock		0x3a
#define	sc_LShift		0x2a
#define	sc_RShift		0x36

#define	sc_UpArrow		0x48
#define	sc_DownArrow		0x50
#define	sc_LeftArrow		0x4b
#define	sc_RightArrow		0x4d
#define	sc_Home			0x47
#define	sc_End			0x4f
#define	sc_PgUp			0x49
#define	sc_PgDn			0x51

#define	sc_Insert		0x52
#define	sc_Delete		0x53
#define	sc_F1			0x3b
#define	sc_F2			0x3c
#define	sc_F3			0x3d
#define	sc_F4			0x3e
#define	sc_F5			0x3f
#define	sc_F6			0x40
#define	sc_F7			0x41
#define	sc_F8			0x42
#define	sc_F9			0x43
#define	sc_F10			0x44
#define	sc_F11			0x57
#define	sc_F12			0x59

#define	sc_1			0x02
#define	sc_2			0x03
#define	sc_3			0x04
#define	sc_4			0x05
#define	sc_5			0x06
#define	sc_6			0x07
#define	sc_7			0x08
#define	sc_8			0x09
#define	sc_9			0x0a
#define	sc_0			0x0b

#define	sc_A			0x1e
#define	sc_B			0x30
#define	sc_C			0x2e
#define	sc_D			0x20
#define	sc_E			0x12
#define	sc_F			0x21
#define	sc_G			0x22
#define	sc_H			0x23
#define	sc_I			0x17
#define	sc_J			0x24
#define	sc_K			0x25
#define	sc_L			0x26
#define	sc_M			0x32
#define	sc_N			0x31
#define	sc_O			0x18
#define	sc_P			0x19
#define	sc_Q			0x10
#define	sc_R			0x13
#define	sc_S			0x1f
#define	sc_T			0x14
#define	sc_U			0x16
#define	sc_V			0x2f
#define	sc_W			0x11
#define	sc_X			0x2d
#define	sc_Y			0x15
#define	sc_Z			0x2c

#define	key_None		0

typedef	enum {
	ctrl_Keyboard,
	ctrl_Joystick,
	ctrl_Joystick1 = ctrl_Joystick, 
	ctrl_Joystick2,
	ctrl_Mouse
} ControlType;

typedef	enum {
	motion_Left = -1,
	motion_Up = -1,
	motion_None = 0,
	motion_Right = 1,
	motion_Down = 1
} Motion;

typedef	enum {
	dir_North,
	dir_NorthEast,
	dir_East,
	dir_SouthEast,
	dir_South,
	dir_SouthWest,
	dir_West,
	dir_NorthWest,
	dir_None
} Direction;

typedef	struct {
	boolean button0, button1, button2, button3;
	int x, y;
	Motion xaxis, yaxis;
	Direction dir;
} ControlInfo;

typedef	struct {
	ScanCode button0, button1, upleft, up, upright, 
		left, right, downleft, down, downright;
} KeyboardDef;

extern boolean Keyboard[], MousePresent, JoysPresent[];

extern	boolean		Paused;
extern	char		LastASCII;
extern	ScanCode	LastScan;
extern	KeyboardDef	KbdDefs;
extern	ControlType	Controls[MaxPlayers];

#define	IN_KeyDown(code)	(Keyboard[(code)])

extern void INL_Update();

extern void IN_Startup(), IN_Shutdown(), IN_ClearKeysDown(),
		IN_ReadControl(int,ControlInfo *),
		IN_GetJoyAbs(word joy,word *xp,word *yp),
		IN_SetupJoy(word joy,word minx,word maxx,word miny,word maxy),
		IN_Ack();
		
extern boolean IN_UserInput(longword delay);
extern const char *IN_GetScanName(ScanCode);

byte IN_MouseButtons();
byte IN_JoyButtons();
word INL_GetJoyButtons(word joy);

void IN_GetMouseDelta(int *dx, int *dy);
void INL_GetJoyDelta(word joy,int *dx,int *dy);

void IN_StartAck();
boolean IN_CheckAck();

#endif
