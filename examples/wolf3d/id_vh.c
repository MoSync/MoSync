#include "wl_def.h"

pictabletype pictable[NUMPICS];

int px, py;
byte fontcolor, backcolor;
int fontnumber;

boolean	screenfaded;

static byte palette1[256][3], palette2[256][3];

int xfrac, yfrac;

/* ======================================================================== */

/*
#define SKIPFADE
*/

void VL_FillPalette(int red, int green, int blue)
{
	byte pal[768];
	int i;
	
	for (i = 0; i < 256; i++) {
		pal[i*3+0] = red;
		pal[i*3+1] = green;
		pal[i*3+2] = blue;
	}
	VL_SetPalette(pal);
}

/*
=================
=
= VL_FadeOut
=
= Fades the current palette to the given color in the given number of steps
=
=================
*/

void VL_FadeOut(int start, int end, int red, int green, int blue, int steps)
{
#ifdef SKIPFADE
	VL_FillPalette(red, green, blue);
#else
	int i,j,orig,delta;
	byte *origptr, *newptr;

	VL_GetPalette(&palette1[0][0]);
	memcpy(palette2, palette1, 768);

/* fade through intermediate frames */
	for (i = 0; i < steps; i++)
	{
		origptr = &palette1[start][0];
		newptr = &palette2[start][0];
		for (j=start;j<=end;j++)
		{
			orig = *origptr++;
			delta = red-orig;
			*newptr++ = orig + delta * i / steps;
			orig = *origptr++;
			delta = green-orig;
			*newptr++ = orig + delta * i / steps;
			orig = *origptr++;
			delta = blue-orig;
			*newptr++ = orig + delta * i / steps;
		}

		VL_SetPalette(&palette2[0][0]);
	}

/* final color */
	VL_FillPalette(red, green, blue);
#endif

	screenfaded = true;
}

/*
=================
=
= VL_FadeIn
=
=================
*/

void VL_FadeIn(int start, int end, const byte *palette, int steps)
{
#ifdef SKIPFADE
	VL_SetPalette(palette);
#else
	int i, j, delta;

	VL_GetPalette(&palette1[0][0]);
	memcpy(&palette2[0][0],&palette1[0][0],sizeof(palette1));

	start *= 3;
	end = end*3+2;

/* fade through intermediate frames */
	for (i = 0; i < steps; i++)
	{
		for (j = start;j <= end; j++)
		{
			delta = palette[j]-palette1[0][j];
			palette2[0][j] = palette1[0][j] + delta * i / steps;
		}

		VL_SetPalette(&palette2[0][0]);
	}

/* final color */
	VL_SetPalette(palette);
#endif
	screenfaded = false;
}

void VL_CacheScreen(int chunk)
{
	CA_CacheGrChunk(chunk);
	VL_MemToScreen(grsegs[chunk], 320, 200, 0, 0);
	CA_UnCacheGrChunk(chunk);
}

void VL_DeModeXize(byte *buf, int width, int height)
{
	byte *mem, *ptr, *destline;
	int plane, x, y;
	
	if (width & 3) {
		printf("Not divisible by 4?\n");
		return;
	}
	
	MM_GetPtr((memptr)&mem, width * height);
	
	ptr = buf;

	for (plane = 0; plane < 4; plane++) {
		destline = mem;
		for (y = 0; y < height; y++) {
			for (x = 0; x < width / 4; x++)
				*(destline + x*4 + plane) = *ptr++;
			destline += width;
		}
	}

	memcpy(buf, mem, width * height);
	
	MM_FreePtr((memptr)&mem);
}

/*
=================
=
= VL_Plot
=
=================
*/

static void VL_Plot(int x, int y, int color)
{
	int xend, yend, xs, ys;
	
	xend = x + 1;
	yend = y + 1;
	
	x *= xfrac;
	y *= yfrac;
	xend *= xfrac;
	yend *= yfrac;
	
	x >>= 16;
	y >>= 16;
	xend >>= 16;
	yend >>= 16;
	
	for (xs = x; xs < xend; xs++)
		for (ys = y; ys < yend; ys++)
			*(gfxbuf + ys * vwidth + xs) = color;
}

void VW_Plot(int x, int y, int color)
{
	VL_Plot(x, y, color);
}

/* 
font is:
height is a word
256 word offsets from start
256 byte widths
data
*/
void VW_DrawPropString(const char *string)
{
	byte *font;
	int width, step, height, x, xs, y;
	byte *source, *ptrs;
	byte ch;

	font = grsegs[STARTFONT+fontnumber];
	height = font[0] | (font[1] << 8);

	xs = 0;
	
	while ((ch = *string++) != 0) {
		width = step = font[2 + 512 + ch];
		source = font+font[2+ch*2+0]+(font[2+ch*2+1]<<8);
		for (x = 0; x < width; x++) {
			ptrs = source;
			for (y = 0; y < height; y++) {
				if (*ptrs)
					VL_Plot(px+xs, py+y, fontcolor);
				ptrs += step;
			}
			xs++;
			source++;
		}
	}
}

void VW_MeasurePropString(const char *string, word *width, word *height)
{
	int w, mw;
	byte *font = grsegs[STARTFONT+fontnumber];
	
	w = 0;
	mw = 0;
	*height = font[0] | (font[1] << 8);
	for (;*string; string++) {
		if (*string == '\n') {
			if (mw < w)
				mw = w;

			w = 0;
			*height += font[0] | (font[1] << 8);
		} else {
			w += font[2 + 512 + *(byte *)string]; 
		}
	}
	if (mw < w)
		mw = w;
		
	*width = mw;
}

void VWB_DrawTile8(int x, int y, int tile)
{
	VL_MemToScreen(grsegs[STARTTILE8]+(tile*64), 8, 8, x, y);
}

void VWB_DrawPic(int x, int y, int chunknum)
{
	int picnum = chunknum - STARTPICS;
	int width, height;

	width = pictable[picnum].width;
	height = pictable[picnum].height;

	VL_MemToScreen(grsegs[chunknum], width, height, x, y);
}

/*
=================
=
= VL_Hlin
=
=================
*/

void VL_Hlin(unsigned x, unsigned y, unsigned width, unsigned color)
{
/*
	int xend, yend;
	int w, h;
	byte *ptr;
	
	xend = x + width;
	yend = y + 1;
	
	x *= xfrac;
	y *= yfrac;
	xend *= xfrac;
	yend *= yfrac;
	
	w = (xend - x) >> 16;
	h = (yend - y) >> 16;

	ptr = gfxbuf + vwidth * (y >> 16) + (x >> 16);
	
	while (h--) {
		memset(ptr, color, w);
		ptr += vwidth;
	}
*/

	unsigned int w;
	
	for (w = 0; w < width; w++)
		VL_Plot(x+w, y, color);
		
}

/*
=================
=
= VL_Vlin
=
=================
*/

void VL_Vlin(int x, int y, int height, int color)
{
/*
	int xend, yend;
	int w, h;
	byte *ptr;
	
	xend = x + 1;
	yend = y + height;
	
	x *= xfrac;
	y *= yfrac;
	xend *= xfrac;
	yend *= yfrac;
	
	w = (xend - x) >> 16;
	h = (yend - y) >> 16;

	ptr = gfxbuf + vwidth * (y >> 16) + (x >> 16);
	
	while (h--) {
		memset(ptr, color, w);
		ptr += vwidth;
	}
*/
	int h;
	
	for (h = 0; h < height; h++)
		VL_Plot(x, y+h, color);
}

/*
=================
=
= VL_Bar
=
=================
*/

void VW_Bar(int x, int y, int width, int height, int color)
{
	int w, h;
	byte *ptr;
	
	x *= xfrac;
	y *= yfrac;

	w = (width * xfrac) >> 16;
	h = (height * yfrac) >> 16;
	
	ptr = gfxbuf + vwidth * (y >> 16) + (x >> 16);
	
	while (h--) {
		memset(ptr, color, w);
		ptr += vwidth;
	}
}

void VL_Bar(int x, int y, int width, int height, int color)
{
	byte *ptr = gfxbuf + vwidth * y + x;
	while (height--) {
		memset(ptr, color, width);
		ptr += vwidth;
	}
}

/*
=================
=
= VL_MemToScreen
=
= Draws a block of data to the screen.
=
=================
*/

void VL_MemToScreen(const byte *source, int width, int height, int x, int y)
{
	int w, h;
	
	for (w = 0; w < width; w++)
		for (h = 0; h < height; h++)
			VL_Plot(x+w, y+h, source[h*width+w]);
}

void VW_Startup()
{
	VL_Startup();
	
	xfrac = (vwidth << 16) / 320;
	yfrac = (vheight << 16) / 200;
}

void VW_Shutdown()
{
	VL_Shutdown();
}
