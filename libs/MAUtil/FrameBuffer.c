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

static MAPoint2d sScalerSizes[10];
static byte *sFrameBuffer;
static MAFrameBufferInfo sFrameBufferInfo;
static int sOrientation;
static int sFlags;
static int sXOffset;
static int sYOffset;
static int sScalerIndex;

typedef struct Color_t { byte r, g, b; } Color;
static Color sRealPal[256];
static int sPal[256];
static int sWeighted[16*16]; // only used in 4 bpp mode.

typedef void (*ScalerFunc)(unsigned char *pixel, int pitchx, int pitchy, short x, short y, short w, short h, const byte *buf, int pitch);
ScalerFunc sScalerFunc = NULL;

void chooseScaler();

void FrameBuffer_init(int w, int h, int orientation, int flags) {
	sScalerSizes[0].x = w>>1;
	sScalerSizes[0].y = h>>1;
	sScalerSizes[1].x = w;
	sScalerSizes[1].y = h;
	sScalerSizes[2].x = w<<1;
	sScalerSizes[2].y = h<<1;
	sScalerSizes[3].x = 0;
	sScalerSizes[3].y = 0;
	sOrientation = orientation;
	sFlags = flags;

	if(!sFrameBuffer) {
		int res = maFrameBufferGetInfo(&sFrameBufferInfo);
		if(res < 0)
			maPanic(res, "Framebuffer info failure!");
		sFrameBuffer = (byte*)malloc(sFrameBufferInfo.sizeInBytes);
		res = maFrameBufferInit(sFrameBuffer);
		if(res < 0)
			maPanic(res, "Framebuffer init failure!");
	}

	memset(sFrameBuffer, 0, sFrameBufferInfo.sizeInBytes);

	chooseScaler();
}

void FrameBuffer_setOrientation(int orientation) {
	orientation&=0x3;
	sOrientation = orientation;
	chooseScaler();

}

int FrameBuffer_getOrientation() {
	return sOrientation;
}

void FrameBuffer_close() {
	maFrameBufferClose();
	if(sFrameBuffer) free(sFrameBuffer);
	sFrameBuffer = 0;
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
			sRealPal[i].r = (src[sIndex+0]<<2)|(src[sIndex+0]&0x3); //c[0];
			sRealPal[i].g = (src[sIndex+1]<<2)|(src[sIndex+1]&0x3); //c[1];
			sRealPal[i].b = (src[sIndex+2]<<2)|(src[sIndex+2]&0x3); //c[2];
		} else {
			sRealPal[i].r = src[sIndex+0]; //c[0];
			sRealPal[i].g = src[sIndex+1]; //c[1];
			sRealPal[i].b = src[sIndex+2]; //c[2];
		}
		sIndex+=3;

		sPal[i] =
		(((int)sRealPal[i].r>>(8-sFrameBufferInfo.redBits))<<sFrameBufferInfo.redShift) |
		(((int)sRealPal[i].g>>(8-sFrameBufferInfo.greenBits))<<sFrameBufferInfo.greenShift) |
		(((int)sRealPal[i].b>>(8-sFrameBufferInfo.blueBits))<<sFrameBufferInfo.blueShift);
	}
	
		// build sWeighted palette.
	if(sFlags&FLAG_4BPP) {
		for (i = startDst; i < startDst + num; ++i) {
			for(j = i; j < startDst+num; j++) {
				sWeighted[(i<<4)|j] =
				(((int)((sRealPal[i].r+sRealPal[j].r)>>1)>>(8-sFrameBufferInfo.redBits))<<sFrameBufferInfo.redShift) |
				(((int)((sRealPal[i].g+sRealPal[j].g)>>1)>>(8-sFrameBufferInfo.greenBits))<<sFrameBufferInfo.greenShift)|
				(((int)((sRealPal[i].b+sRealPal[j].b)>>1)>>(8-sFrameBufferInfo.blueBits))<<sFrameBufferInfo.blueShift);
				sWeighted[(j<<4)|i] = sWeighted[(i<<4)|j];
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
		dst[0] = sRealPal[i].r>>2;
		dst[1] = sRealPal[i].g>>2;
		dst[2] = sRealPal[i].b>>2;
	} else {
		dst[0] = sRealPal[i].r;
		dst[1] = sRealPal[i].g;
		dst[2] = sRealPal[i].b;
	}
}

static void pixelHalf_4bpp(unsigned char *pixel, int pitchx, int pitchy, short x, short y, short w, short h, const byte *buf, int pitch) {
	int i;
	w>>=1;
	buf += y * pitch + x;
	if(sFrameBufferInfo.bitsPerPixel>8 && sFrameBufferInfo.bitsPerPixel<=16) {
		int mopitch = pitchy>>1;
		h>>=1;
		pitch<<=1;
		while (h--) {
			short *scan = (short *)pixel;
			for (i = 0; i < w; ++i) {
				*scan = sWeighted[*(buf + i)];
				scan+=mopitch;
			}
			y++;
			pixel += pitchx;
			buf += pitch;
		}
	}
	else if(sFrameBufferInfo.bitsPerPixel>16 && sFrameBufferInfo.bitsPerPixel<=32) {
		int mopitch = pitchy>>2;
		h>>=1;
		pitch<<=1;
		while (h--) {
			int *scan = (int *)pixel;
			for (i = 0; i < w; ++i) {
				*scan = sWeighted[*(buf + i)];
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
	if(sFrameBufferInfo.bitsPerPixel>8 && sFrameBufferInfo.bitsPerPixel<=16) {
		int mopitch = pitchy>>1;
		while (h--) {
			short *scan = (short *)pixel;
			for (i = 0; i < w; ++i) {
				byte p1 = *(buf + i) >> 4;
				byte p2 = *(buf + i) & 0xF;
				*scan = sPal[p1];
				scan+=mopitch;
				*scan = sPal[p2];
				scan+=mopitch;
			}
			y++;
			pixel += pitchx;
			buf += pitch;
		}
	}
	else if(sFrameBufferInfo.bitsPerPixel>16 && sFrameBufferInfo.bitsPerPixel<=32) {
		int mopitch = pitchy>>2;
		while (h--) {
			int *scan = (int *)pixel;
			for (i = 0; i < w; ++i) {
				byte p1 = *(buf + i) >> 4;
				byte p2 = *(buf + i) & 0xF;
				*scan = sPal[p1];
				scan+=mopitch;
				*scan = sPal[p2];
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
	if(sFrameBufferInfo.bitsPerPixel>8 && sFrameBufferInfo.bitsPerPixel<=16) {
		int mopitch = pitchy>>1;
		while (h--) {
			short *scan_upper = (short *)(pixel);
			short *scan_lower = (short *)(pixel+pitchx);
			for (i = 0; i < w; ++i) {
				byte p1 = *(buf + i) >> 4;
				byte p2 = *(buf + i) & 0xF;

				*scan_upper = *scan_lower = sPal[p1];
				scan_upper+=mopitch; scan_lower+=mopitch;
				*scan_upper = *scan_lower = sPal[p1];
				scan_upper+=mopitch; scan_lower+=mopitch;
				*scan_upper = *scan_lower = sPal[p2];
				scan_upper+=mopitch; scan_lower+=mopitch;
				*scan_upper = *scan_lower = sPal[p2];
				scan_upper+=mopitch; scan_lower+=mopitch;
			}
			y++;
			pixel += pitchx << 1;
			buf += pitch;
		}
	}
	else if(sFrameBufferInfo.bitsPerPixel>16 && sFrameBufferInfo.bitsPerPixel<=32) {
		int mopitch = pitchy>>2;
		while (h--) {
			int *scan_upper = (int *)(pixel);
			int *scan_lower = (int *)(pixel+pitchx);
			for (i = 0; i < w; ++i) {
				byte p1 = *(buf + i) >> 4;
				byte p2 = *(buf + i) & 0xF;

				*scan_upper = *scan_lower = sPal[p1];
				scan_upper+=mopitch; scan_lower+=mopitch;
				*scan_upper = *scan_lower = sPal[p1];
				scan_upper+=mopitch; scan_lower+=mopitch;
				*scan_upper = *scan_lower = sPal[p2];
				scan_upper+=mopitch; scan_lower+=mopitch;
				*scan_upper = *scan_lower = sPal[p2];
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
	if(sFrameBufferInfo.bitsPerPixel>8 && sFrameBufferInfo.bitsPerPixel<=16) {
		int mopitch = pitchy>>1;
		h>>=1;
		pitch<<=1;
		while (h--) {
			short *scan = (short *)pixel;
			for (i = 0; i < w; ++i) {
				*scan = sPal[*(buf + i*2)];
				scan+=mopitch;
			}
			y++;
			pixel += pitchx;
			buf += pitch;
		}
	}
	else if(sFrameBufferInfo.bitsPerPixel>16 && sFrameBufferInfo.bitsPerPixel<=32) {
		int mopitch = pitchy>>2;
		h>>=1;
		pitch<<=1;
		while (h--) {
			int *scan = (int *)pixel;
			for (i = 0; i < w; ++i) {
				*scan = sPal[*(buf + i*2)];
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
	if(sFrameBufferInfo.bitsPerPixel>8 && sFrameBufferInfo.bitsPerPixel<=16) {
		int mopitch = pitchy>>1;
		while (h--) {
			short *scan = (short *)pixel;
			for (i = 0; i < w; ++i) {
				*scan = sPal[*(buf + i)];
				scan+=mopitch;
			}
			y++;
			pixel += pitchx;
			buf += pitch;
		}
	}
	else if(sFrameBufferInfo.bitsPerPixel>16 && sFrameBufferInfo.bitsPerPixel<=32) {
		int mopitch = pitchy>>2;
		while (h--) {
			int *scan = (int *)pixel;
			for (i = 0; i < w; ++i) {
				*scan = sPal[*(buf + i)];
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
	if(sFrameBufferInfo.bitsPerPixel>8 && sFrameBufferInfo.bitsPerPixel<=16) {
		int mopitch = pitchy>>1;
		while (h--) {
			short *scan_upper = (short *)(pixel);
			short *scan_lower = (short *)(pixel+pitchx);
			for (i = 0; i < w; ++i) {
				short col = sPal[*(buf + i)];
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
	else if(sFrameBufferInfo.bitsPerPixel>16 && sFrameBufferInfo.bitsPerPixel<=32) {
		int mopitch = pitchy>>2;
		while (h--) {
			int *scan_upper = (int *)(pixel);
			int *scan_lower = (int *)(pixel+pitchx);
			for (i = 0; i < w; ++i) {
				int col = sPal[*(buf + i)];
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
	if(sScalerIndex == 0) { dstx>>=1; dsty>>=1; }
	if(sScalerIndex == 2) { dstx<<=1; dsty<<=1; }
	
	switch(sOrientation) {
	case ORIENTATION_0:
		dx = (sXOffset);
		dy = (sYOffset);				
		pitchx = sFrameBufferInfo.pitch;
		pitchy = sFrameBufferInfo.bytesPerPixel;
		dx += dstx;
		dy += dsty;	
		break;
	case ORIENTATION_90:
		dx = (sXOffset+sScalerSizes[sScalerIndex].y-1);
		dy = sYOffset;
		pitchx = -sFrameBufferInfo.bytesPerPixel;
		pitchy = sFrameBufferInfo.pitch;
		dx += -dsty;
		dy += dstx;						
		break;
	case ORIENTATION_180:
		dx = (sXOffset+sScalerSizes[sScalerIndex].x-1);
		dy = (sYOffset+sScalerSizes[sScalerIndex].y-1);
		pitchx = -sFrameBufferInfo.pitch;
		pitchy = -sFrameBufferInfo.bytesPerPixel;				
		dx += -dstx;
		dy += -dsty;				
		break;
	case ORIENTATION_270:	
		dx = (sXOffset);
		dy = (sYOffset+sScalerSizes[sScalerIndex].x-1);
		pitchx = sFrameBufferInfo.bytesPerPixel;
		pitchy = -sFrameBufferInfo.pitch;
		dx += dsty;
		dy += -dstx;					
		break;
	default:
		PANIC_MESSAGE("bad orientation");
	}
	
	pixel = (unsigned char*)&sFrameBuffer[dx*sFrameBufferInfo.bytesPerPixel+dy*sFrameBufferInfo.pitch];

	srand((int)sScalerFunc);;
	sScalerFunc(pixel, pitchx, pitchy, x, y, w, h, buf, pitch);
}

void chooseScaler() {
	int scaler = 0;
	const MAPoint2d* size = sScalerSizes;
	int bestScaler = -1;
	if(sOrientation == ORIENTATION_0 || sOrientation == ORIENTATION_180) {
		while(size->x!=0) {
			if(size->x <= sFrameBufferInfo.width && size->y <= sFrameBufferInfo.height) {
				bestScaler = scaler;
			}
			scaler++;
			size++;
		}
		scaler = bestScaler;
		sXOffset = ((sFrameBufferInfo.width)-sScalerSizes[scaler].x)>>1;
		sYOffset = ((sFrameBufferInfo.height)-sScalerSizes[scaler].y)>>1;
	} else {
		while(size->x!=0) {
			if(size->y <= sFrameBufferInfo.width && size->x <= sFrameBufferInfo.height) {
				bestScaler = scaler;
			}
			scaler++;
			size++;
		}
		scaler = bestScaler;
		sXOffset = ((sFrameBufferInfo.width)-sScalerSizes[scaler].y)>>1;
		sYOffset = ((sFrameBufferInfo.height)-sScalerSizes[scaler].x)>>1;
	}

	if(sFlags & FLAG_4BPP) {
		switch(scaler) {
			case 0: sScalerFunc = pixelHalf_4bpp; break;
			case 1: sScalerFunc = pixelDirect_4bpp; break;
			case 2: sScalerFunc = pixelDouble_4bpp; break;
		}
	} else {
		switch(scaler) {
			case 0: sScalerFunc = pixelHalf_8bpp; break;
			case 1: sScalerFunc = pixelDirect_8bpp; break;
			case 2: sScalerFunc = pixelDouble_8bpp; break;
		}
	}

	sScalerIndex = scaler;
}

int FrameBuffer_getArrowKeyForOrientationInitial(int mak, int initial) {
	int i;
	int mask = 0;
	if(mak == MAK_UP) mask|=1;
	if(mak == MAK_RIGHT) mask|=2;
	if(mak == MAK_DOWN) mask|=4;
	if(mak == MAK_LEFT) mask|=8;
	for(i = initial; i < sOrientation; i++) {
		if(mask&0x1) mask|=1<<4;
		mask>>=1;
	}
	/*
	if(mask&1) return MAK_LEFT;
	if(mask&2) return MAK_UP;
	if(mask&4) return MAK_RIGHT;
	if(mask&8) return MAK_DOWN;
	*/
	if(mask&1) return MAK_UP;
	if(mask&2) return MAK_RIGHT;
	if(mask&4) return MAK_DOWN;
	if(mask&8) return MAK_LEFT;
	else return 0;
}

int FrameBuffer_getArrowKeyForOrientation(int mak) {
	return FrameBuffer_getArrowKeyForOrientationInitial(mak, 0);
}
