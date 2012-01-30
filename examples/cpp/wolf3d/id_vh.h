#ifndef __ID_VH_H__
#define __ID_VH_H__

#define WHITE			15
#define BLACK			0

typedef struct
{
	int width, height;
} pictabletype;

extern pictabletype pictable[NUMPICS];

extern byte fontcolor, backcolor;
extern int fontnumber;
extern int px, py;

#define SETFONTCOLOR(f, b) { fontcolor = f; backcolor = b; }

void VW_UpdateScreen();

void VWB_DrawTile8(int x, int y, int tile);
void VWB_DrawPic(int x, int y, int chunknum);

extern boolean screenfaded;

#define VW_Hlin(x,z,y,c)	VL_Hlin(x,y,(z)-(x)+1,c)
#define VW_Vlin(y,z,x,c)	VL_Vlin(x,y,(z)-(y)+1,c)
#define VW_WaitVBL		VL_WaitVBL
#define VW_FadeIn()		VL_FadeIn(0,255,gamepal,30);
#define VW_FadeOut()		VL_FadeOut(0,255,0,0,0,30);
void	VW_MeasurePropString(const char *string, word *width, word *height);

void VW_DrawPropString(const char *string);

void VL_FadeOut(int start, int end, int red, int green, int blue, int steps);
void VL_FadeIn(int start, int end, const byte *palette, int steps);

void VL_CacheScreen(int chunk);

void VW_Bar(int x, int y, int width, int height, int color);

void VW_Startup();
void VW_Shutdown();

void VL_FillPalette(int red, int green, int blue);
void VW_Plot(int x, int y, int color);
void VL_Hlin(unsigned x, unsigned y, unsigned width, unsigned color);
void VL_Vlin(int x, int y, int height, int color);
void VL_Bar(int x, int y, int width, int height, int color);

void VL_MemToScreen(const byte *source, int width, int height, int x, int y);

void VL_DeModeXize(byte *buf, int width, int height);

#endif
