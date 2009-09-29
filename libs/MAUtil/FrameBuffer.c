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

#include <maheap.h>
#include <mastdlib.h>
#include <maheap.h>
#include <mastdlib.h>
#include <maassert.h>

#include "FrameBuffer.h"

static MAPoint2d scalerSizes[10];
static byte *frameBuffer;
static MAFrameBufferInfo frameBufferInfo;
static int gOrientation;
static int gFlags;
static int gXOffset;
static int gYOffset;
static int gScalerIndex;

typedef struct Color_t { byte r, g, b; } Color;
static Color realPal[256];
static int pal[256];
static int weighted[16*16]; // only used in 4 bpp mode.

typedef void (*ScalerFunc)(unsigned char *pixel, int pitchx, int pitchy, short x, short y, short w, short h, const byte *buf, int pitch);
ScalerFunc gScalerFunc = NULL;

void chooseScaler();

void FrameBuffer_init(int w, int h, int orientation, int flags) {
	scalerSizes[0].y = w>>1;
	scalerSizes[0].x = h>>1;
	scalerSizes[1].y = w;
	scalerSizes[1].x = h;
	scalerSizes[2].y = w<<1;
	scalerSizes[2].x = h<<1;
	scalerSizes[3].y = 0;
	scalerSizes[3].x = 0;
	gOrientation = orientation;
	gFlags = flags;

	if(!frameBuffer) {
		int res = maFrameBufferGetInfo(&frameBufferInfo);
		if(res < 0)
			maPanic(res, "Framebuffer info failure!");
		frameBuffer = (byte*)malloc(frameBufferInfo.sizeInBytes);
		res = maFrameBufferInit(frameBuffer);
		if(res < 0)
			maPanic(res, "Framebuffer init failure!");
	}

	memset(frameBuffer, 0, frameBufferInfo.sizeInBytes);

	chooseScaler();
}

void FrameBuffer_setOrientation(int orientation) {
	orientation&=0x3;
	gOrientation = orientation;
	chooseScaler();

}

int FrameBuffer_getOrientation() {
	return gOrientation;
}

void FrameBuffer_close() {
	maFrameBufferClose();
	if(frameBuffer) free(frameBuffer);
	frameBuffer = 0;
}

void FrameBuffer_setPalette(int startDst, int startSrc, int num, const byte *src, int flags) {
	int i, j;
	int sIndex = startSrc;
	for (i = startDst; i < startDst + num; ++i) {
		/*
		byte c[3];
		for (j = 0; j < 3; ++j) {
			byte col = src[i * 3 + j];
			c[j] =  (col << 2) | (col & 3);
		}
		*/
		if(flags & FLAG_RGB666) {
			realPal[i].r = (src[sIndex+0]<<2)|(src[sIndex+0]&0x3); //c[0];
			realPal[i].g = (src[sIndex+1]<<2)|(src[sIndex+1]&0x3); //c[1];
			realPal[i].b = (src[sIndex+2]<<2)|(src[sIndex+2]&0x3); //c[2];
		} else {
			realPal[i].r = src[sIndex+0]; //c[0];
			realPal[i].g = src[sIndex+1]; //c[1];
			realPal[i].b = src[sIndex+2]; //c[2];
		}
		sIndex+=3;

		pal[i] =
		(((int)realPal[i].r>>(8-frameBufferInfo.redBits))<<frameBufferInfo.redShift) |
		(((int)realPal[i].g>>(8-frameBufferInfo.greenBits))<<frameBufferInfo.greenShift) |
		(((int)realPal[i].b>>(8-frameBufferInfo.blueBits))<<frameBufferInfo.blueShift);
	}
	
		// build weighted palette.
	if(gFlags&FLAG_4BPP) {
		for (i = startDst; i < startDst + num; ++i) {
			for(j = i; j < startDst+num; j++) {
				weighted[(i<<4)|j] =
				(((int)((realPal[i].r+realPal[j].r)>>1)>>(8-frameBufferInfo.redBits))<<frameBufferInfo.redShift) |
				(((int)((realPal[i].g+realPal[j].g)>>1)>>(8-frameBufferInfo.greenBits))<<frameBufferInfo.greenShift)|
				(((int)((realPal[i].b+realPal[j].b)>>1)>>(8-frameBufferInfo.blueBits))<<frameBufferInfo.blueShift);
				weighted[(j<<4)|i] = weighted[(i<<4)|j];
			}
		}
	}	
}

void FrameBuffer_setPaletteEntry(int i, int r, int g, int b, int flags) {
	byte c[4] = {r, g, b, 0};
	FrameBuffer_setPalette(i, 0, 1, c, flags);
}

void FrameBuffer_getPaletteEntry(int i, byte *dst, int flags) {
	if(flags & FLAG_RGB666) {
		dst[0] = realPal[i].r>>2;
		dst[1] = realPal[i].g>>2;
		dst[2] = realPal[i].b>>2;
	} else {
		dst[0] = realPal[i].r;
		dst[1] = realPal[i].g;
		dst[2] = realPal[i].b;
	}
}

static void pixelHalf_4bpp(unsigned char *pixel, int pitchx, int pitchy, short x, short y, short w, short h, const byte *buf, int pitch) {
	int i;
	w>>=1;
	buf += y * pitch + x;
	if(frameBufferInfo.bitsPerPixel>8 && frameBufferInfo.bitsPerPixel<=16) {
		int mopitch = pitchy>>1;
		h>>=1;
		pitch<<=1;
		while (h--) {
			short *scan = (short *)pixel;
			for (i = 0; i < w; ++i) {
				*scan = weighted[*(buf + i)];
				scan+=mopitch;
			}
			y++;
			pixel += pitchx;
			buf += pitch;
		}
	}
	else if(frameBufferInfo.bitsPerPixel>16 && frameBufferInfo.bitsPerPixel<=32) {
		int mopitch = pitchy>>2;
		h>>=1;
		pitch<<=1;
		while (h--) {
			int *scan = (int *)pixel;
			for (i = 0; i < w; ++i) {
				*scan = weighted[*(buf + i)];
				scan+=mopitch;
			}
			y++;
			pixel += pitchx;
			buf += pitch;
		}
	}
}

static void pixelDirect_4bpp(unsigned char *pixel, int pitchx, int pitchy, short x, short y, short w, short h, const byte *buf, int pitch) {
	int i;
	w>>=1;
	buf += y * pitch + x;
	if(frameBufferInfo.bitsPerPixel>8 && frameBufferInfo.bitsPerPixel<=16) {
		int mopitch = pitchy>>1;
		while (h--) {
			short *scan = (short *)pixel;
			for (i = 0; i < w; ++i) {
				byte p1 = *(buf + i) >> 4;
				byte p2 = *(buf + i) & 0xF;
				*scan = pal[p1];
				scan+=mopitch;
				*scan = pal[p2];
				scan+=mopitch;
			}
			y++;
			pixel += pitchx;
			buf += pitch;
		}
	}
	else if(frameBufferInfo.bitsPerPixel>16 && frameBufferInfo.bitsPerPixel<=32) {
		int mopitch = pitchy>>2;
		while (h--) {
			int *scan = (int *)pixel;
			for (i = 0; i < w; ++i) {
				byte p1 = *(buf + i) >> 4;
				byte p2 = *(buf + i) & 0xF;
				*scan = pal[p1];
				scan+=mopitch;
				*scan = pal[p2];
				scan+=mopitch;
			}
			y++;
			pixel += pitchx;
			buf += pitch;
		}
	}
}

static void pixelDouble_4bpp(unsigned char *pixel, int pitchx, int pitchy, short x, short y, short w, short h, const byte *buf, int pitch) {
	int i;
	w>>=1;
	buf += y * pitch + x;
	if(frameBufferInfo.bitsPerPixel>8 && frameBufferInfo.bitsPerPixel<=16) {
		int mopitch = pitchy>>1;
		while (h--) {
			short *scan_upper = (short *)(pixel);
			short *scan_lower = (short *)(pixel+pitchx);
			for (i = 0; i < w; ++i) {
				byte p1 = *(buf + i) >> 4;
				byte p2 = *(buf + i) & 0xF;

				*scan_upper = *scan_lower = pal[p1];
				scan_upper+=mopitch; scan_lower+=mopitch;
				*scan_upper = *scan_lower = pal[p1];
				scan_upper+=mopitch; scan_lower+=mopitch;
				*scan_upper = *scan_lower = pal[p2];
				scan_upper+=mopitch; scan_lower+=mopitch;
				*scan_upper = *scan_lower = pal[p2];
				scan_upper+=mopitch; scan_lower+=mopitch;
			}
			y++;
			pixel += pitchx << 1;
			buf += pitch;
		}
	}
	else if(frameBufferInfo.bitsPerPixel>16 && frameBufferInfo.bitsPerPixel<=32) {
		int mopitch = pitchy>>2;
		while (h--) {
			int *scan_upper = (int *)(pixel);
			int *scan_lower = (int *)(pixel+pitchx);
			for (i = 0; i < w; ++i) {
				byte p1 = *(buf + i) >> 4;
				byte p2 = *(buf + i) & 0xF;

				*scan_upper = *scan_lower = pal[p1];
				scan_upper+=mopitch; scan_lower+=mopitch;
				*scan_upper = *scan_lower = pal[p1];
				scan_upper+=mopitch; scan_lower+=mopitch;
				*scan_upper = *scan_lower = pal[p2];
				scan_upper+=mopitch; scan_lower+=mopitch;
				*scan_upper = *scan_lower = pal[p2];
				scan_upper+=mopitch; scan_lower+=mopitch;
			}
			y++;
			pixel += pitchx << 1;
			buf += pitch;
		}
	}
}

static void pixelHalf_8bpp(unsigned char *pixel, int pitchx, int pitchy, short x, short y, short w, short h, const byte *buf, int pitch) {
	int i;
	w>>=1;
	buf += y * pitch + x;
	if(frameBufferInfo.bitsPerPixel>8 && frameBufferInfo.bitsPerPixel<=16) {
		int mopitch = pitchy>>1;
		h>>=1;
		pitch<<=1;
		while (h--) {
			short *scan = (short *)pixel;
			for (i = 0; i < w; ++i) {
				*scan = pal[*(buf + i*2)];
				scan+=mopitch;
			}
			y++;
			pixel += pitchx;
			buf += pitch;
		}
	}
	else if(frameBufferInfo.bitsPerPixel>16 && frameBufferInfo.bitsPerPixel<=32) {
		int mopitch = pitchy>>2;
		h>>=1;
		pitch<<=1;
		while (h--) {
			int *scan = (int *)pixel;
			for (i = 0; i < w; ++i) {
				*scan = pal[*(buf + i*2)];
				scan+=mopitch;
			}
			y++;
			pixel += pitchx;
			buf += pitch;
		}
	}
}

static void pixelDirect_8bpp(unsigned char *pixel, int pitchx, int pitchy, short x, short y, short w, short h, const byte *buf, int pitch) {
	int i;
	buf += y * pitch + x;
	if(frameBufferInfo.bitsPerPixel>8 && frameBufferInfo.bitsPerPixel<=16) {
		int mopitch = pitchy>>1;
		while (h--) {
			short *scan = (short *)pixel;
			for (i = 0; i < w; ++i) {
				*scan = pal[*(buf + i)];
				scan+=mopitch;
			}
			y++;
			pixel += pitchx;
			buf += pitch;
		}
	}
	else if(frameBufferInfo.bitsPerPixel>16 && frameBufferInfo.bitsPerPixel<=32) {
		int mopitch = pitchy>>2;
		while (h--) {
			int *scan = (int *)pixel;
			for (i = 0; i < w; ++i) {
				*scan = pal[*(buf + i)];
				scan+=mopitch;
			}
			y++;
			pixel += pitchx;
			buf += pitch;
		}
	}
}

static void pixelDouble_8bpp(unsigned char *pixel, int pitchx, int pitchy, short x, short y, short w, short h, const byte *buf, int pitch) {
	int i;
	//w>>=1;
	buf += y * pitch + x;
	if(frameBufferInfo.bitsPerPixel>8 && frameBufferInfo.bitsPerPixel<=16) {
		int mopitch = pitchy>>1;
		while (h--) {
			short *scan_upper = (short *)(pixel);
			short *scan_lower = (short *)(pixel+pitchx);
			for (i = 0; i < w; ++i) {
				short col = pal[*(buf + i)];
				*scan_upper = *scan_lower = col;
				scan_upper+=mopitch; scan_lower+=mopitch;
				*scan_upper = *scan_lower = col;
				scan_upper+=mopitch; scan_lower+=mopitch;
			}
			y++;
			pixel += pitchx << 1;
			buf += pitch;
		}
	}
	else if(frameBufferInfo.bitsPerPixel>16 && frameBufferInfo.bitsPerPixel<=32) {
		int mopitch = pitchy>>2;
		while (h--) {
			int *scan_upper = (int *)(pixel);
			int *scan_lower = (int *)(pixel+pitchx);
			for (i = 0; i < w; ++i) {
				int col = pal[*(buf + i)];
				*scan_upper = *scan_lower = col;
				scan_upper+=mopitch; scan_lower+=mopitch;
				*scan_upper = *scan_lower = col;
				scan_upper+=mopitch; scan_lower+=mopitch;
			}
			y++;
			pixel += pitchx << 1;
			buf += pitch;
		}
	}
}
void FrameBuffer_copyRect(short x, short y, short w, short h, int dstx, int dsty, const byte *buf, int pitch) {
	unsigned char *pixel = 0;
	int pitchx = 0, pitchy = 0;
	int dx, dy;
	if(gScalerIndex == 0) { dstx>>=1; dsty>>=1; }
	if(gScalerIndex == 2) { dstx<<=1; dsty<<=1; }
	
	switch(gOrientation) {
	case ORIENTATION_0:
		dx = (gXOffset+scalerSizes[gScalerIndex].x-1);
		dy = gYOffset;
		pitchx = -frameBufferInfo.bytesPerPixel;
		pitchy = frameBufferInfo.pitch;
		dx += -dsty;
		dy += dstx;						
		break;
	case ORIENTATION_90:
		dx = (gXOffset+scalerSizes[gScalerIndex].y-1);
		dy = (gYOffset+scalerSizes[gScalerIndex].x-1);
		pitchx = -frameBufferInfo.pitch;
		pitchy = -frameBufferInfo.bytesPerPixel;				
		dx += -dstx;
		dy += -dsty;				
		break;
	case ORIENTATION_180:	
		dx = (gXOffset);
		dy = (gYOffset+scalerSizes[gScalerIndex].y-1);
		pitchx = frameBufferInfo.bytesPerPixel;
		pitchy = -frameBufferInfo.pitch;
		dx += dsty;
		dy += -dstx;					
		break;
	case ORIENTATION_270:
		dx = (gXOffset);
		dy = (gYOffset);				
		pitchx = frameBufferInfo.pitch;
		pitchy = frameBufferInfo.bytesPerPixel;
		dx += dstx;
		dy += dsty;	
		break;
	default:
		PANIC_MESSAGE("bad orientation");
	}
	
	pixel = (unsigned char*)&frameBuffer[dx*frameBufferInfo.bytesPerPixel+dy*frameBufferInfo.pitch];

	srand((int)gScalerFunc);;
	gScalerFunc(pixel, pitchx, pitchy, x, y, w, h, buf, pitch);
}

void chooseScaler() {
	int scaler = 0;
	const MAPoint2d* size = scalerSizes;
	int bestScaler = -1;
	if(gOrientation == ORIENTATION_0 || gOrientation == ORIENTATION_180) {
		while(size->x!=0) {
			if(size->x <= frameBufferInfo.width && size->y <= frameBufferInfo.height) {
				bestScaler = scaler;
			}
			scaler++;
			size++;
		}
		scaler = bestScaler;
		gXOffset = ((frameBufferInfo.width)-scalerSizes[scaler].x)>>1;
		gYOffset = ((frameBufferInfo.height)-scalerSizes[scaler].y)>>1;
	} else {
		while(size->x!=0) {
			if(size->y <= frameBufferInfo.width && size->x <= frameBufferInfo.height) {
				bestScaler = scaler;
			}
			scaler++;
			size++;
		}
		scaler = bestScaler;
		gXOffset = ((frameBufferInfo.width)-scalerSizes[scaler].y)>>1;
		gYOffset = ((frameBufferInfo.height)-scalerSizes[scaler].x)>>1;
	}

	if(gFlags & FLAG_4BPP) {
		switch(scaler) {
			case 0: gScalerFunc = pixelHalf_4bpp; break;
			case 1: gScalerFunc = pixelDirect_4bpp; break;
			case 2: gScalerFunc = pixelDouble_4bpp; break;
		}
	} else {
		switch(scaler) {
			case 0: gScalerFunc = pixelHalf_8bpp; break;
			case 1: gScalerFunc = pixelDirect_8bpp; break;
			case 2: gScalerFunc = pixelDouble_8bpp; break;
		}
	}

	gScalerIndex = scaler;
}

int FrameBuffer_getArrowKeyForOrientation(int mak) {
	int i;
	int mask = 0;
	if(mak == MAK_UP) mask|=1;
	if(mak == MAK_RIGHT) mask|=2;
	if(mak == MAK_DOWN) mask|=4;
	if(mak == MAK_LEFT) mask|=8;
	for(i = 0; i < gOrientation; i++) {
		if(mask&0x1) mask|=1<<4;
		mask>>=1;
	}
	if(mask&1) return MAK_LEFT;
	if(mask&2) return MAK_UP;
	if(mask&4) return MAK_RIGHT;
	if(mask&8) return MAK_DOWN;
	else return 0;
}
