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

#include "Image.h"
#include <stdlib.h>
#include <config_platform.h>
#include <helpers/helpers.h>
#include <helpers/cpp_defs.h>

//#include <windows.h>

#include <base_errors.h>
using namespace MoSyncError;

#define SWAP(x, y, temp) {temp=x;x=y;y=temp;}

inline long fp_ceil(long x) {
	x += 0xffff;
	return x >> 16;
}

inline long fp_div32(long a, long b) {
	return (long)((((long long)a)<<16)/((long long)b));
}

inline long fp_mul32(long a, long b) {
	return (long)(((long long)a * (long long)b)>>16);
}  

struct Point {
	int x, y;
};

#ifndef SYMBIAN
static Point clippedPoints[2][16];
static int numPoints[2];
static int currentList = 0;

enum ClipResult  {
	BOTH_IN = 0,
	BOTH_OUT = 1,
	A_IN = 2,
	B_IN = 3,
};

ClipResult clipTopLine(Point &a, Point &b, Point& out, int top) {
	//bool shouldAddThisPoint = false;
	//bool shouldAddClippedPoint = false;
	if(a.y<top && b.y<top) return BOTH_OUT;
	ClipResult clipResult;
	if((a.y>=top && b.y<top)) {
		clipResult = A_IN;
	} else if((a.y<top && b.y>=top)) {
		clipResult = B_IN;
	} else {
		return BOTH_IN;
	}

	if(abs(b.y-a.y)<0xffff) return BOTH_OUT;
	int mul = fp_mul32((top-a.y),(b.x-a.x));
	int div = fp_div32(mul, (b.y-a.y));			
	out.x = a.x + div;
	out.y = top;
	return clipResult;
}

ClipResult clipBottomLine(Point &a, Point &b, Point& out, int bottom) {
	//bool shouldAddThisPoint = false;
	//bool shouldAddClippedPoint = false;
	if(a.y>bottom && b.y>bottom) return BOTH_OUT;
	ClipResult clipResult;
	if((a.y<=bottom && b.y>bottom)) {
		clipResult = A_IN;
	} else if((a.y>bottom && b.y<=bottom)) {
		clipResult = B_IN;
	} else {
		return BOTH_IN;
	}

	if(abs(b.y-a.y)<0xffff) return BOTH_OUT;
	int mul = fp_mul32((bottom-a.y),(b.x-a.x));
	int div = fp_div32(mul, (b.y-a.y));			
	out.x = a.x + div;
	out.y = bottom;
	return clipResult;
}

ClipResult clipLeftLine(Point &a, Point &b, Point& out, int left) {
	//bool shouldAddThisPoint = false;
	//bool shouldAddClippedPoint = false;
	if(a.x<left && b.x<left) return BOTH_OUT;
	ClipResult clipResult;
	if((a.x>=left && b.x<left)) {
		clipResult = A_IN;
	} else if((a.x<left && b.x>=left)) {
		clipResult = B_IN;
	} else {
		return BOTH_IN;
	}

	if(abs(b.x-a.x)<0xffff) return BOTH_OUT;
	int mul = fp_mul32((left-a.x),(b.y-a.y));
	int div = fp_div32(mul, (b.x-a.x));			
	out.x = left;
	out.y =  a.y + div;
	return clipResult;
}

ClipResult clipRightLine(Point &a, Point &b, Point& out, int right) {
	//bool shouldAddThisPoint = false;
	//bool shouldAddClippedPoint = false;
	if(a.x>right && b.x>right) return BOTH_OUT;
	ClipResult clipResult;
	if((a.x<=right && b.x>right)) {
		clipResult = A_IN;
	} else if((a.x>right && b.x<=right)) {
		clipResult = B_IN;
	} else {
		return BOTH_IN;
	}

	if(abs(b.x-a.x)<0xffff) return BOTH_OUT;
	int mul = fp_mul32((right-a.x),(b.y-a.y));
	int div = fp_div32(mul, (b.x-a.x));			
	out.x = right;
	out.y =  a.y + div;
	return clipResult;
}
#endif	//SYMBIAN

#ifdef SYMBIAN
unsigned char* initMulTable() {
	unsigned char* mulTable = new unsigned char[256*256];
#else
static unsigned char mulTable[256*256];
void initMulTable() {
#endif
	for(int i = 0; i < 256; i++) {
		for(int j = 0; j < 256; j++) {
			mulTable[i+j*256] = (i*j)/255;
		}
	}
#ifdef SYMBIAN
	return mulTable;
#endif
}

Image::Image() {
}

bool Image::hasData() {if(data==NULL) return false; return true;}

bool Image::hasAlpha() {if(alpha==NULL) return false; return true;}

void Image::calculateConstants() {
	
	alphaMask	= 0x0;
	alphaShift	= 0;
	
	switch(pixelFormat) {
		case PIXELFORMAT_RGB444:
			redMask		= 0x0f00;
			greenMask   = 0x00f0;
			blueMask	= 0x000f;
			redShift	= 8;
			greenShift	= 4;
			blueShift	= 0;
			redBits		= 4;
			greenBits	= 4;
			blueBits	= 4;
			bytesPerPixel = 2;
			bitsPerPixel = 12;
		break;

		case PIXELFORMAT_RGB555:
			redMask		= 0x7c00;
			greenMask   = 0x03e0;
			blueMask	= 0x001f;
			redShift	= 10;
			greenShift	= 5;
			blueShift	= 0;
			redBits		= 5;
			greenBits	= 5;
			blueBits	= 5;
			bytesPerPixel = 2;
			bitsPerPixel = 15;
		break;

		case PIXELFORMAT_RGB565:
			redMask		= 0xf800;
			greenMask   = 0x07e0;
			blueMask	= 0x001f;
			redShift	= 11;
			greenShift	= 5;
			blueShift	= 0;
			redBits		= 5;
			greenBits	= 6;
			blueBits	= 5;	
			bytesPerPixel = 2;
			bitsPerPixel = 16;
		break;

		case PIXELFORMAT_RGB888:
			redMask		= 0x00ff0000;
			greenMask   = 0x0000ff00;
			blueMask	= 0x000000ff;
			redShift	= 16;
			greenShift	= 8;
			blueShift	= 0;
			redBits		= 8;
			greenBits	= 8;
			blueBits	= 8;		
			bytesPerPixel = 4;
			bitsPerPixel = 24;
		break;

		case PIXELFORMAT_ARGB8888:
			redMask		= 0x00ff0000;
			greenMask   = 0x0000ff00;
			blueMask	= 0x000000ff;
			alphaMask	= 0xff000000;
			alphaShift	= 24;
			redShift	= 16;
			greenShift	= 8;
			blueShift	= 0;
			alphaBits	= 8;
			redBits		= 8;
			greenBits	= 8;
			blueBits	= 8;		
			bytesPerPixel = 4;
			bitsPerPixel = 32;
		break;
	}
}

void Image::init(unsigned char *data, unsigned char *alpha, bool makeCopy) {
	if(data) {
		if(makeCopy) {
			this->data = new unsigned char[pitch*height];
			if(this->data == NULL) if(this->data == 0) return;
			memcpy(this->data, data, pitch*height);		
			if(alpha) {
				this->alpha = new unsigned char[width*height];
				if(this->alpha == NULL)
				{
					delete this->data;
					this->data = NULL;
					return;
				}
				memcpy(this->alpha, alpha, width*height);
			}
		} else {
			this->data = data;
			this->alpha = alpha;
		}
	} else {
		//
		int size = pitch * height;
		this->data = new unsigned char[size];
		if(this->data == 0) return;
	}

	clipRect.x = 0;
	clipRect.y = 0;
	clipRect.width = width;
	clipRect.height = height;
}

Image::Image(unsigned char *data, unsigned char *alpha, const ImageInitParams &params, bool makeCopy, bool shouldFreeData) :
	data(NULL),
	alpha(NULL),
	width(params.width),
	height(params.height),
	pitch(params.pitch),
	bytesPerPixel(params.bytesPerPixel),
	bitsPerPixel(params.bitsPerPixel),

	alphaMask(params.alphaMask),
	alphaShift(params.alphaShift),
	alphaBits(params.alphaBits),

	redMask(params.redMask),
	redShift(params.redShift),
	redBits(params.redBits),

	greenMask(params.greenMask),
	greenShift(params.greenShift),
	greenBits(params.greenBits),

	blueMask(params.blueMask),
	blueShift(params.blueShift),
	blueBits(params.blueBits),
	shouldFreeData(shouldFreeData)
{
	if(height>65536 || pitch>65536) return;
	init(data, alpha, makeCopy);
}

Image::Image(int width, int height, int pitch, PixelFormat pixelFormat) :
	pixelFormat(pixelFormat),
	data(NULL),
	alpha(NULL),
	width(width),
	height(height),
	pitch(pitch),
	shouldFreeData(true)
{
	if(height>65536 || pitch>65536) return;
	init(NULL, NULL, false);
	calculateConstants();
}

Image::Image(unsigned char *data, unsigned char *alpha, int width, int height, int pitch, PixelFormat pixelFormat, bool makeCopy, bool shouldFreeData) :
	pixelFormat(pixelFormat),
	data(NULL),
	alpha(NULL),
	width(width),
	height(height),
	pitch(pitch),
	shouldFreeData(shouldFreeData)
{
	if(height>65536 || pitch>65536) return;
	init(data, alpha, makeCopy);
	calculateConstants();
}
	
Image::~Image() {
	if(shouldFreeData) {
		if(data) {
			delete data;
			data = 0;
		}
		if(alpha) {
			delete alpha;
			alpha = 0;
		}
	}
}

#ifndef SYMBIAN
void Image::drawImage(int left, int top, Image *img) {
	ClipRect srcRect = {0, 0, img->width, img->height};
	drawImageRegion(left, top, &srcRect, img, TRANS_NONE);
}
#endif

#define BIG_PHAT_SOURCE_RECT_ERROR { BIG_PHAT_ERROR(ERR_SOURCE_RECT_OOB);}

void Image::drawImageRegion(int left, int top, ClipRect *srcRect, Image *img, int transformMode) {
	int width = srcRect->width,
		height = srcRect->height,
		u = srcRect->x,
		v = srcRect->y;
	int imgWidth = img->width;

	int bpp = img->bytesPerPixel,
		//dstPitchY = pitch,
		srcPitchX, 
		srcPitchY,
		transTopLeftX, 
		transTopLeftY, 
		transBottomRightX, 
		transBottomRightY,
		transWidth, 
		transHeight;

	int dirHorizontalX = 0,
		dirHorizontalY = 0,
		dirVerticalX = 0,
		dirVerticalY = 0;

	switch(transformMode) {
			case TRANS_NONE:
				srcPitchX = bpp;
				srcPitchY = img->pitch;
				transTopLeftX = u;
				transTopLeftY = v;
				transBottomRightX = u + width - 1;
				transBottomRightY = v + height - 1;
				transWidth = width;
				transHeight = height;
				dirHorizontalX = 1;
				dirVerticalY = 1;
				break;
			case TRANS_ROT90:
				srcPitchX = -img->pitch;
				srcPitchY = bpp;
				transTopLeftX = u;
				transTopLeftY = v+height-1;
				transBottomRightX = u + width - 1;
				transBottomRightY = v;
				transWidth = height;
				transHeight = width;
				dirHorizontalY = -1;
				dirVerticalX = 1;
				break;
			case TRANS_ROT180:
				srcPitchX = -bpp;
				srcPitchY = -img->pitch;
				transTopLeftX = u + width - 1;
				transTopLeftY = v + height - 1;
				transBottomRightX = u;
				transBottomRightY = v;
				transWidth = width;
				transHeight = height;
				dirHorizontalX = -1;
				dirVerticalY = -1;
				break;
			case TRANS_ROT270:
				srcPitchX = img->pitch;
				srcPitchY = -bpp;
				transTopLeftX = u + width - 1;
				transTopLeftY = v;
				transBottomRightX = u;
				transBottomRightY = v + height - 1;
				transWidth = height;
				transHeight = width;
				dirHorizontalY = 1;
				dirVerticalX = -1;
				break;
			case TRANS_MIRROR:
				srcPitchX = -bpp;
				srcPitchY = img->pitch;
				transTopLeftX = u + width - 1;
				transTopLeftY = v;
				transBottomRightX = u;
				transBottomRightY = v + height - 1;
				transWidth = width;
				transHeight = height;
				dirHorizontalX = -1;
				dirVerticalY = 1;
				break;
			case TRANS_MIRROR_ROT90:
				srcPitchX = -img->pitch;
				srcPitchY = -bpp;
				transTopLeftX = u+width-1;
				transTopLeftY = v+height-1;
				transBottomRightX = u;
				transBottomRightY = v;
				transWidth = height;
				transHeight = width;
				dirHorizontalY = 1;
				dirVerticalX = 1;
				break;
			case TRANS_MIRROR_ROT180:
				srcPitchX = bpp;
				srcPitchY = -img->pitch;
				transTopLeftX = u;
				transTopLeftY = v + height - 1;
				transBottomRightX = u + width - 1;
				transBottomRightY = v;
				transWidth = width;
				transHeight = height;
				dirHorizontalX = 1;
				dirVerticalY = -1;
				break;
			case TRANS_MIRROR_ROT270:
				srcPitchX = img->pitch;
				srcPitchY = bpp;
				transTopLeftX = u;
				transTopLeftY = v;
				transBottomRightX = u + width - 1;
				transBottomRightY = v + height - 1;
				transWidth = height;
				transHeight = width;
				dirHorizontalY = -1;
				dirVerticalX = -1;
				break;
			default:
				DEBIG_PHAT_ERROR;
	}

	if( transWidth <= 0 || transHeight <= 0) return;
	
	if (transTopLeftX >= img->width) {BIG_PHAT_SOURCE_RECT_ERROR;}
	else if(transTopLeftX < 0) {BIG_PHAT_SOURCE_RECT_ERROR;}
	if (transTopLeftY >= img->height) {BIG_PHAT_SOURCE_RECT_ERROR;}
	else if(transTopLeftY < 0) {BIG_PHAT_SOURCE_RECT_ERROR;}
	if(transBottomRightX < 0) {BIG_PHAT_SOURCE_RECT_ERROR;}
	else if(transBottomRightX >= img->width) {BIG_PHAT_SOURCE_RECT_ERROR;}
	if(transBottomRightY < 0) {BIG_PHAT_SOURCE_RECT_ERROR;}
	else if(transBottomRightY >= img->height) {BIG_PHAT_SOURCE_RECT_ERROR;}

	if( transWidth <= 0 || transHeight <= 0) return;

	if (left >= clipRect.x + clipRect.width) 
		return;
	else if(left < clipRect.x) {
		transTopLeftX += (clipRect.x - left)*dirHorizontalX;
		transTopLeftY += (clipRect.x - left)*dirHorizontalY;
		transWidth -= clipRect.x - left;
		left = clipRect.x;
	}
	if (top >= clipRect.y + clipRect.height) 
		return;
	else if(top < clipRect.y) {
		transTopLeftX += (clipRect.y - top)*dirVerticalX;		
		transTopLeftY += (clipRect.y - top)*dirVerticalY;		
		transHeight -= clipRect.y - top;
		top = clipRect.y;
	}
	if(left + transWidth < clipRect.x) 
		return;
	else if(left + transWidth >= clipRect.x + clipRect.width)
		transWidth -= (left + transWidth) - (clipRect.x + clipRect.width);
	if(top + height < clipRect.y) 
		return;
	else if(top + transHeight >= clipRect.y + clipRect.height)
		transHeight -= (top + transHeight) - (clipRect.y + clipRect.height);
	
	if( transWidth <= 0 || transHeight<= 0) return;

	unsigned char *dst = &data[left*bytesPerPixel + top*pitch];
	unsigned char *src = &img->data[transTopLeftX*img->bytesPerPixel + transTopLeftY*img->pitch];

	if(img->alpha) {
		switch(bpp) {
				case 2:
					{
						srcPitchX>>=1;
						unsigned char *salpha = &img->alpha[transTopLeftX + transTopLeftY*(img->width)];
						unsigned char *ascan;
						unsigned short *src_scan;
						unsigned short *dst_scan;

						while(transHeight--) {
							src_scan = (unsigned short*)src;
							dst_scan = (unsigned short*)dst;
							ascan = salpha;
							int x = transWidth;
							while(x--) {
								int sr = (((*src_scan)&img->redMask)>>img->redShift);
								int sg = (((*src_scan)&img->greenMask)>>img->greenShift);
								int sb = (((*src_scan)&img->blueMask)>>img->blueShift);
								int dr = (((*dst_scan)&redMask)>>redShift);
								int dg = (((*dst_scan)&greenMask)>>greenShift);
								int db = (((*dst_scan)&blueMask)>>blueShift);

								if(*ascan == 255) {
									*dst_scan = (((sr)<< redShift)&redMask) |
										(((sg)<< greenShift)&greenMask) |
										(((sb)<< blueShift)&blueMask);
								} else if(*ascan == 0) {
									*dst_scan = (((dr)<< redShift)&redMask) |
										(((dg)<< greenShift)&greenMask) |
										(((db)<< blueShift)&blueMask);									
								} else {
									*dst_scan = 
										(((dr + (((sr-dr)*(*ascan))>>8)) << redShift)&redMask) |
										(((dg + (((sg-dg)*(*ascan))>>8)) << greenShift)&greenMask) |
										(((db + (((sb-db)*(*ascan))>>8)) << blueShift)&blueMask);
								}
/*
								*dst_scan = 
									(((mulTable[dr][255-*ascan] + mulTable[sr][*ascan]) << redShift)&redMask) |
									(((mulTable[dg][255-*ascan] + mulTable[sg][*ascan]) << greenShift)&greenMask) |
									(((mulTable[db][255-*ascan] + mulTable[sb][*ascan]) << blueShift)&blueMask);
*/
								src_scan+=srcPitchX;
								dst_scan++;
								ascan+=srcPitchX;
							}
							src += srcPitchY;
							dst += pitch;
							salpha += imgWidth;
						}	
					}
					break;
				default:
					BIG_PHAT_ERROR(ERR_UNSUPPORTED_BPP);
		}
	}
	else
	{
		if(img->alphaMask) {
			switch(bpp) {
				case 4:
				{
					srcPitchX>>=2;
					unsigned int *src_scan;
					unsigned int *dst_scan;
					
					while(transHeight--) {
						src_scan = (unsigned int*)src;
						dst_scan = (unsigned int*)dst;
						int x = transWidth;
						while(x--) {
							int sr = (((*src_scan)&img->redMask)>>img->redShift);
							int sg = (((*src_scan)&img->greenMask)>>img->greenShift);
							int sb = (((*src_scan)&img->blueMask)>>img->blueShift);
							int sa = (((*src_scan)&img->alphaMask)>>img->alphaShift);
							
							int dr = (((*dst_scan)&redMask)>>redShift);
							int dg = (((*dst_scan)&greenMask)>>greenShift);
							int db = (((*dst_scan)&blueMask)>>blueShift);
							
							if(sa == 255) {
								*dst_scan = (((sr)<< redShift)&redMask) |
								(((sg)<< greenShift)&greenMask) |
								(((sb)<< blueShift)&blueMask);
							} else if(sa == 0) {
								*dst_scan = (((dr)<< redShift)&redMask) |
								(((dg)<< greenShift)&greenMask) |
								(((db)<< blueShift)&blueMask);									
							} else {
								*dst_scan = 
								(((dr + (((sr-dr)*(sa))>>8)) << redShift)&redMask) |
								(((dg + (((sg-dg)*(sa))>>8)) << greenShift)&greenMask) |
								(((db + (((sb-db)*(sa))>>8)) << blueShift)&blueMask);
							}
							src_scan+=srcPitchX;
							dst_scan++;
						}
						src += srcPitchY;
						dst += pitch;
					}	
				}
					break;
				default:
					BIG_PHAT_ERROR(ERR_UNSUPPORTED_BPP);
			}			
		} else {
			int dstOffsetY = -transWidth*bytesPerPixel + pitch;
			int srcOffsetY = -srcPitchX*transWidth + srcPitchY;
			while(transHeight--) {
				width = transWidth;
				while(width--) {
					memcpy(dst, src, bytesPerPixel);
					src+=srcPitchX;
					dst+=bytesPerPixel;
				}
				dst+=dstOffsetY;
				src+=srcOffsetY;
			}	
		}
	}
}

#ifndef SYMBIAN
void Image::drawPoint(int posX, int posY, int color) {
	if( posX < clipRect.x || 
		posX >= clipRect.x + clipRect.width || 
		posY < clipRect.y ||
		posY >= clipRect.y + clipRect.height) return;
	switch(bytesPerPixel) {
		case 2:
			*((unsigned short*)&data[posX*bytesPerPixel +
				posY*pitch])= color&0xffff;
			break;
		case 4:
			*((unsigned int*)&data[posX*bytesPerPixel +
				posY*pitch])= color;
			break;
		default:
			BIG_PHAT_ERROR(ERR_UNSUPPORTED_BPP);
	}
}

bool clipLine(Point &a, Point &b, int left, int right, int top, int bottom) {
	left<<=16;
	right<<=16;
	top<<=16;
	bottom<<=16;

	Point clippedPoint;
	ClipResult res;
	res = clipLeftLine(a, b, clippedPoint, left);
	switch(res) {
		case A_IN: b = clippedPoint; break;
		case B_IN: a = clippedPoint; break;
		case BOTH_OUT: return false; break;
		case BOTH_IN: break;
	}
	res = clipRightLine(a, b, clippedPoint, right);
	switch(res) {
		case A_IN: b = clippedPoint; break;
		case B_IN: a = clippedPoint; break;
		case BOTH_OUT: return false; break;
		case BOTH_IN: break;
	}

	res = clipTopLine(a, b, clippedPoint, top);
	switch(res) {
		case A_IN: b = clippedPoint; break;
		case B_IN: a = clippedPoint; break;
		case BOTH_OUT: return false; break;
		case BOTH_IN: break;
	}

	res = clipBottomLine(a, b, clippedPoint, bottom);
	switch(res) {
		case A_IN: b = clippedPoint; break;
		case B_IN: a = clippedPoint; break;
		case BOTH_OUT: return false; break;
		case BOTH_IN: break;
	}

	return true;
}

void Image::drawLine(int x0, int y0, int x1, int y1, int realColor) {
		int left = clipRect.x;
		int right = clipRect.x + clipRect.width - 1;
		int top = clipRect.y;
		int bottom = clipRect.y + clipRect.height - 1;

		int bpp = bytesPerPixel;
		unsigned char *pixel = data;
		unsigned int color = realColor;
		//int flags = 0;

		Point a = {x0<<16, y0<<16};
		Point b = {x1<<16, y1<<16};
		if(!clipLine(a, b, left, right, top, bottom)) return;
		x0 = fp_ceil(a.x);
		x1 = fp_ceil(b.x);
		y0 = fp_ceil(a.y);
		y1 = fp_ceil(b.y);

		int dy = y1 - y0;
		int dx = x1 - x0;

		if(dx == 0) {
			if(dy == 0) return;
			if(y0 > y1) { dx = y0; y0 = y1; y1 = dx; }
			pixel+=x0*bpp+y0*pitch;
			switch(bpp) {
				case 2:
				{
					while(y0<=y1) {
						*((unsigned short*)pixel) = color;			
						y0++;
						pixel+=pitch;
					}
				}
				break;
				case 4:
				{
					while(y0<=y1) {
						*((unsigned int*)pixel) = color;			
						y0++;
						pixel+=pitch;
					}
				}
				break;
				default:
					BIG_PHAT_ERROR(ERR_UNSUPPORTED_BPP);
			}

			return;
		}
		else if(dy == 0) {
			if(x0 > x1) { dx = x0; x0 = x1; x1 = dx; }
			pixel+=x0*bpp+y0*pitch;
			switch(bpp) {
				case 2:
				{
					while(x0<=x1) {
						*((unsigned short*)pixel) = color;			
						x0++;
						pixel+=bpp;
					}
				}
				break;
				case 4:
				{
					while(x0<=x1) {
						*((unsigned int*)pixel) = color;			
						x0++;
						pixel+=bpp;
					}
				}
				break;
				default:
					BIG_PHAT_ERROR(ERR_UNSUPPORTED_BPP);
			}

			return;
		}

		int temp;
		if(abs(dy)>abs(dx)) {
			if(y1<y0) {SWAP(x1, x0, temp); SWAP(y1, y0, temp); }
			int dxdy = (dx<<16)/dy;
			x0<<=16;
			switch(bpp) {
				case 2:
					for(int y = y0; y <= y1; y++) {
						//drawPoint(fp_ceil(x0), y, color);
						(((unsigned short*)&pixel[y*pitch])[fp_ceil(x0)]) = color;		
						x0+=dxdy;
					}
					break;
				case 4:
					for(int y = y0; y <= y1; y++) {
						//drawPoint(fp_ceil(x0), y, color);
						(((unsigned int*)&pixel[y*pitch])[fp_ceil(x0)]) = color;	
						x0+=dxdy;
					}
					break;
			}

		} else {
			if(x1<x0) {SWAP(x1, x0, temp); SWAP(y1, y0, temp); }
			int dydx = (dy<<16)/dx;
			y0<<=16;
			/*
			for(int x = x0; x <= x1; x++) {
				drawPoint(x, fp_ceil(y0), color);
				y0+=dydx;
			}
			*/

			switch(bpp) {
				case 2:
					for(int x = x0; x <= x1; x++) {
						//drawPoint(x, fp_ceil(y0), color);
						(((unsigned short*)&pixel[fp_ceil(y0)*pitch])[x]) = color;		
						y0+=dydx;
					}
					break;
				case 4:
					for(int x = x0; x <= x1; x++) {
						//drawPoint(fp_ceil(x0), y, color);
						(((unsigned int*)&pixel[fp_ceil(y0)*pitch])[x]) = color;	
						y0+=dydx;
					}
					break;
			}
		}

		/*
		dy = y1 - y0;
		dx = x1 - x0;
		// Get sign of direction
		int sdx = sgn(dx);
		int sdy = sgn(dy);
		int ac;
		// Get positive slope values absolute magentude
		if(dx<0) dx = -dx;
		if(dy<0) dy = -dy;
		int n;
		//main loop stuff
		if (dx >= dy)
		{
			ac = dx>>1;
			for (n=0;n<dx;n++)
			{
				// line math
				ac += dy;
				if (ac > dx)
				{
					ac -= dx;
					y0  += sdy;
				}
				drawPoint(x0, y0, color);
				//memcpy(&pixel[x0*bpp+y0*pitch], &color, bpp); 
				x0 += sdx;
			}
		}
		else
		{
			ac = dy>>1;
			for (n=0;n<dy;n++)
			{      
				// line math
				ac += dx;
				if (ac > dy)
				{
					ac -= dy;
					x0  += sdx;
				}
				drawPoint(x0, y0, color);
				//memcpy(&pixel[x0*bpp+y0*pitch], &color, bpp); 
				y0 += sdy;
			}
		}
		*/
}

void Image::drawFilledRect(int x, int y, int rectWidth, int rectHeight, int realColor) {
		/* clip it ! */
	if (x > clipRect.x + clipRect.width) 
			return;
		else if(x < clipRect.x) {
			rectWidth-=clipRect.x-x;
			x = clipRect.x;
		}
		if (y > clipRect.y + clipRect.height) 
			return;
		else if(y < clipRect.y) {
			rectHeight-=clipRect.y-y;
			y = clipRect.y;
		}
		if(x + rectWidth < clipRect.x) 
			return;
		else if(x + rectWidth > clipRect.x + clipRect.width)
			rectWidth -= (x + rectWidth) - (clipRect.x + clipRect.width);
		if(y + rectHeight< clipRect.y) 
			return;
		else if(y + rectHeight > clipRect.y + clipRect.height)
			rectHeight -= (y + rectHeight) - (clipRect.y + clipRect.height);

		unsigned char *dst = &data[x*bytesPerPixel + y*pitch];
		unsigned int w;

		switch(bytesPerPixel) {
			case 2:
				{
					unsigned short *scan;
					unsigned short color = realColor&0xffff;
					while(rectHeight--) {
						w = rectWidth;
						scan = (unsigned short*) dst;
						while(w--) {
							*scan++ = color;
						}
						dst+=pitch;
					}
				}
				break;
			case 4:
				{
					unsigned int *scan;
					unsigned int color = realColor;
					while(rectHeight--) {
						w = rectWidth;
						scan = (unsigned int*) dst;
						while(w--) {
							*scan++ = color;
						}
						dst+=pitch;
					}
				}
				break;
			default:
				BIG_PHAT_ERROR(ERR_UNSUPPORTED_BPP);
		}
}

void Image::clipPolygonTop(int src, int dst) {
	int top = clipRect.y<<16;
	numPoints[dst] = 0;
	for(int i = 0; i < numPoints[src]; i++) {
		int next = (i+1)%numPoints[src];

		Point clippedPoint;
		ClipResult clipResult =
			clipTopLine(clippedPoints[src][i], clippedPoints[src][next], clippedPoint, top);
		switch(clipResult) {
			case A_IN:
				clippedPoints[dst][numPoints[dst]++] = clippedPoints[src][i];
				clippedPoints[dst][numPoints[dst]++] = clippedPoint;
			case B_IN:
				clippedPoints[dst][numPoints[dst]++] = clippedPoint;
				break;
			case BOTH_IN:
				clippedPoints[dst][numPoints[dst]++] = clippedPoints[src][i];
				break;
			case BOTH_OUT:
				break;
		}

		/*
		bool shouldAddThisPoint = false;
		bool shouldAddClippedPoint = false;

		if(clippedPoints[src][i].y<top && clippedPoints[src][next].y<top) continue;

		if((clippedPoints[src][i].y>=top && clippedPoints[src][next].y<top)) {
			shouldAddThisPoint = true;
			shouldAddClippedPoint = true;

		} else if((clippedPoints[src][i].y<top && clippedPoints[src][next].y>=top)) {
			shouldAddClippedPoint = true;
		} else {
			shouldAddThisPoint = true;
		}

		if(shouldAddThisPoint) {
			clippedPoints[dst][numPoints[dst]] = clippedPoints[src][i];
			numPoints[dst]++;	
		}

		if(shouldAddClippedPoint) {
			if(abs(clippedPoints[src][next].y-clippedPoints[src][i].y)<0xffff) continue;
			int mul = fp_mul32((top-clippedPoints[src][i].y),(clippedPoints[src][next].x-clippedPoints[src][i].x));
			int div = fp_div32(mul, (clippedPoints[src][next].y-clippedPoints[src][i].y));			
			clippedPoints[dst][numPoints[dst]].x = clippedPoints[src][i].x + div;
			clippedPoints[dst][numPoints[dst]].y = top;
			numPoints[dst]++;
		}
		*/
	}
	currentList = dst;
}

void Image::clipPolygonLeft(int src, int dst) {
	int left = clipRect.x<<16;
	numPoints[dst] = 0;
	for(int i = 0; i < numPoints[src]; i++) {
		int next = (i+1)%numPoints[src];

		Point clippedPoint;
		ClipResult clipResult =
			clipLeftLine(clippedPoints[src][i], clippedPoints[src][next], clippedPoint, left);
		switch(clipResult) {
			case A_IN:
				clippedPoints[dst][numPoints[dst]++] = clippedPoints[src][i];
				clippedPoints[dst][numPoints[dst]++] = clippedPoint;
			case B_IN:
				clippedPoints[dst][numPoints[dst]++] = clippedPoint;
				break;
			case BOTH_IN:
				clippedPoints[dst][numPoints[dst]++] = clippedPoints[src][i];
				break;
			case BOTH_OUT:
				break;
		}


		/*
		bool shouldAddThisPoint = false;
		bool shouldAddClippedPoint = false;

		if(clippedPoints[src][i].x<left && clippedPoints[src][next].x<left) continue;

		if((clippedPoints[src][i].x>=left && clippedPoints[src][next].x<left)) {
			shouldAddThisPoint = true;
			shouldAddClippedPoint = true;

		} else if((clippedPoints[src][i].x<left && clippedPoints[src][next].x>=left)) {
			shouldAddClippedPoint = true;
		} else {
			shouldAddThisPoint = true;
		}

		if(shouldAddThisPoint) {
			clippedPoints[dst][numPoints[dst]] = clippedPoints[src][i];
			numPoints[dst]++;	
		}

		if(shouldAddClippedPoint) {
			if(abs(clippedPoints[src][next].x-clippedPoints[src][i].x)<0xffff) continue;
			int mul = fp_mul32((left-clippedPoints[src][i].x),(clippedPoints[src][next].y-clippedPoints[src][i].y));
			int div = fp_div32(mul, (clippedPoints[src][next].x-clippedPoints[src][i].x));
			clippedPoints[dst][numPoints[dst]].x = left;
			clippedPoints[dst][numPoints[dst]].y = clippedPoints[src][i].y + div;
			numPoints[dst]++;
		}
		*/
	}

	currentList = dst;
}

void Image::clipPolygonRight(int src, int dst) {
	int right = (clipRect.x+clipRect.width-1)<<16;
	numPoints[dst] = 0;
	for(int i = 0; i < numPoints[src]; i++) {
		int next = (i+1)%numPoints[src];

		Point clippedPoint;
		ClipResult clipResult =
			clipRightLine(clippedPoints[src][i], clippedPoints[src][next], clippedPoint, right);
		switch(clipResult) {
			case A_IN:
				clippedPoints[dst][numPoints[dst]++] = clippedPoints[src][i];
				clippedPoints[dst][numPoints[dst]++] = clippedPoint;
			case B_IN:
				clippedPoints[dst][numPoints[dst]++] = clippedPoint;
				break;
			case BOTH_IN:
				clippedPoints[dst][numPoints[dst]++] = clippedPoints[src][i];
				break;
			case BOTH_OUT:
				break;
		}


		/*
		bool shouldAddThisPoint = false;
		bool shouldAddClippedPoint = false;

		if(clippedPoints[src][i].x>right && clippedPoints[src][next].x>right) continue;

		if((clippedPoints[src][i].x<=right && clippedPoints[src][next].x>right)) {
			shouldAddThisPoint = true;
			shouldAddClippedPoint = true;

		} else if((clippedPoints[src][i].x>right && clippedPoints[src][next].x<=right)) {
			shouldAddClippedPoint = true;
		} else {
			shouldAddThisPoint = true;
		}

		if(shouldAddThisPoint) {
			clippedPoints[dst][numPoints[dst]] = clippedPoints[src][i];
			numPoints[dst]++;	
		}

		if(shouldAddClippedPoint) {
			if(abs(clippedPoints[src][next].x-clippedPoints[src][i].x)<0xffff) continue;
			int mul = fp_mul32((right-clippedPoints[src][i].x),(clippedPoints[src][next].y-clippedPoints[src][i].y));
			int div = fp_div32(mul, (clippedPoints[src][next].x-clippedPoints[src][i].x));
			clippedPoints[dst][numPoints[dst]].x = right;
			clippedPoints[dst][numPoints[dst]].y = clippedPoints[src][i].y + div;
			numPoints[dst]++;
		}
		*/
	}

	currentList = dst;
}

void Image::clipPolygonBottom(int src, int dst) {
	int bottom = (clipRect.y+clipRect.height-1)<<16;
	numPoints[dst] = 0;
	for(int i = 0; i < numPoints[src]; i++) {
		int next = (i+1)%numPoints[src];

		Point clippedPoint;
		ClipResult clipResult =
			clipBottomLine(clippedPoints[src][i], clippedPoints[src][next], clippedPoint, bottom);
		switch(clipResult) {
			case A_IN:
				clippedPoints[dst][numPoints[dst]++] = clippedPoints[src][i];
				clippedPoints[dst][numPoints[dst]++] = clippedPoint;
			case B_IN:
				clippedPoints[dst][numPoints[dst]++] = clippedPoint;
				break;
			case BOTH_IN:
				clippedPoints[dst][numPoints[dst]++] = clippedPoints[src][i];
				break;
			case BOTH_OUT:
				break;
		}

		/*
		bool shouldAddThisPoint = false;
		bool shouldAddClippedPoint = false;

		if(clippedPoints[src][i].y>bottom && clippedPoints[src][next].y>bottom) continue;

		if((clippedPoints[src][i].y<=bottom && clippedPoints[src][next].y>bottom)) {
			shouldAddThisPoint = true;
			shouldAddClippedPoint = true;

		} else if((clippedPoints[src][i].y>bottom && clippedPoints[src][next].y<=bottom)) {
			shouldAddClippedPoint = true;
		} else {
			shouldAddThisPoint = true;
		}

		if(shouldAddThisPoint) {
			clippedPoints[dst][numPoints[dst]] = clippedPoints[src][i];
			numPoints[dst]++;	
		}

		if(shouldAddClippedPoint) {
			if(abs(clippedPoints[src][next].y-clippedPoints[src][i].y)<0xffff) continue;
			int mul = fp_mul32((bottom-clippedPoints[src][i].y),(clippedPoints[src][next].x-clippedPoints[src][i].x));
			int div = fp_div32(mul, (clippedPoints[src][next].y-clippedPoints[src][i].y));			
			clippedPoints[dst][numPoints[dst]].x = clippedPoints[src][i].x + div;
			clippedPoints[dst][numPoints[dst]].y = bottom;
			numPoints[dst]++;
		}
		*/
	}

	currentList = dst;
}

bool Image::clipPolygon() {
	clipPolygonTop(0, 1);
	if(numPoints[currentList]<3) return false;
	clipPolygonLeft(1, 0);
	if(numPoints[currentList]<3) return false;
	clipPolygonRight(0, 1);
	if(numPoints[currentList]<3) return false;
	clipPolygonBottom(1, 0);
	return numPoints[currentList]>=3;
}


// change this when screen gets bigger than 1024 ;) (4 kb shouldn't be too big..)
#define RECIP_LUT_SIZE 1024
static int recipLut[RECIP_LUT_SIZE];

void initRecipLut() {
	recipLut[0] = 0xffff;
	for(int i = 1; i < RECIP_LUT_SIZE; i++) {
		recipLut[i] = 0xffff/i;
	}
}

void Image::drawTriangleWithoutClipping(int x1, int y1, int x2, int y2, int x3, int y3, int color) {
	int temp,
		longest,
		height,
		x_left,
		x_right,
		x_mid_left,
		x_mid_right,
		dxdy_left1,
		dxdy_right1,
		dxdy_left2,
		dxdy_right2;

	if(y1>y2) { SWAP(x1, x2, temp); SWAP(y1, y2, temp); }
	if(y1>y3) { SWAP(x1, x3, temp); SWAP(y1, y3, temp); }
	if(y2>y3) { SWAP(x2, x3, temp); SWAP(y2, y3, temp); }	
	
	/*
	dx = x2-x1;
	if(dx==0) {
		// if vertical line take delta of lower side instead
		dx = x2-x3;
		if(dx==0) 
			return;
	}*/
    
	height = y3 - y1;
    if(height == 0)
        return;
	temp = fp_mul32(((y2 - y1) << 16), recipLut[height]);
    longest = temp * (x3 - x1) + ((x1 - x2) << 16);
    if(longest == 0)
        return;

	dxdy_left1	= 0;
	dxdy_right1	= 0;
	dxdy_left2	= 0;
	dxdy_right2 = 0;
	x_left		= x1<<16,
	x_right		= x1<<16;

	if(longest<0) {
		// mid is on right side
		if(y3-y1) {
			dxdy_left1 = fp_mul32(((x3-x1)<<16), recipLut[y3-y1]);
			dxdy_left2 = dxdy_left1;
			if(y2-y1) {
				dxdy_right1 = fp_mul32(((x2-x1)<<16), recipLut[y2-y1]);
			} 
			if(y3-y2) {
				dxdy_right2 = fp_mul32(((x3-x2)<<16), recipLut[y3-y2]);
			}
		} else {
			return;
		}
		x_mid_left = x_left + dxdy_left1*(y2-y1);
		x_mid_right = x2<<16;
	} else {
		// mid is on left side
		if(y3-y1) {
			dxdy_right1 = fp_mul32(((x3-x1)<<16), recipLut[y3-y1]);
			dxdy_right2 = dxdy_right1;
			if(y2-y1) {
				dxdy_left1 = fp_mul32(((x2-x1)<<16), recipLut[y2-y1]);
			}
			if(y3-y2) {
				dxdy_left2 = fp_mul32(((x3-x2)<<16), recipLut[y3-y2]);
			}		
		} else {
			return;
		}
		x_mid_left = x2<<16;
		x_mid_right = x_right + dxdy_right1*(y2-y1);
	}

	unsigned char *dst = &data[y1*pitch];
	switch(bytesPerPixel) {
		case 2:
			for(int y = y1; y < y2; y++) {
				int x_start = fp_ceil(x_left);
				int w = (fp_ceil(x_right)-x_start);
				if(w>0) {
					short *scan = (short*)dst;
					scan+=x_start;
					while(w--) *scan++=color;
				}
				dst+=pitch;
				x_left+=dxdy_left1;
				x_right+=dxdy_right1;
			}
			x_left = x_mid_left;
			x_right = x_mid_right;
			dst = &data[y2*pitch];
			for(int y = y2; y < y3; y++) {
				int x_start = fp_ceil(x_left);
				int w = (fp_ceil(x_right)-x_start);
				if(w>0) {
					short *scan = (short*)dst;
					scan+=x_start;
					while(w--) *scan++=color;
				}
				dst+=pitch;
				x_left+=dxdy_left2;
				x_right+=dxdy_right2;
			}
			break;
		case 4:
			for(int y = y1; y < y2; y++) {
				int x_start = fp_ceil(x_left);
				int w = (fp_ceil(x_right)-x_start);
				if(w>0) {
					int *scan = (int*)dst;
					scan+=x_start;
					while(w--) *scan++=color;
				}
				dst+=pitch;
				x_left+=dxdy_left1;
				x_right+=dxdy_right1;
			}
			x_left = x_mid_left;
			x_right = x_mid_right;
			dst = &data[y2*pitch];
			for(int y = y2; y < y3; y++) {
				int x_start = fp_ceil(x_left);
				int w = (fp_ceil(x_right)-x_start);
				if(w>0) {
					int *scan = (int*)dst;
					scan+=x_start;
					while(w--) *scan++=color;
				}	
				dst+=pitch;
				x_left+=dxdy_left2;
				x_right+=dxdy_right2;
			}
		break;
	}
}

void Image::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int color) {	
	clippedPoints[0][0].x = x1<<16;
	clippedPoints[0][0].y = y1<<16;
	clippedPoints[0][1].x = x2<<16;
	clippedPoints[0][1].y = y2<<16;
	clippedPoints[0][2].x = x3<<16;
	clippedPoints[0][2].y = y3<<16;
	currentList = 0;
	numPoints[0] = 3;
	
	if(!clipPolygon()) return;

	for(int i = 0; i < numPoints[currentList]-1; i++) {
		drawTriangleWithoutClipping(
			fp_ceil(clippedPoints[currentList][0].x),
			fp_ceil(clippedPoints[currentList][0].y),
			fp_ceil(clippedPoints[currentList][i].x),
			fp_ceil(clippedPoints[currentList][i].y),
			fp_ceil(clippedPoints[currentList][i+1].x),
			fp_ceil(clippedPoints[currentList][i+1].y),
			color);	
	}

	/*
	for(int i = 0; i < numPoints[currentList]-1; i++) {
		drawLine(
			fp_ceil(clippedPoints[currentList][i].x),
			fp_ceil(clippedPoints[currentList][i].y),
			fp_ceil(clippedPoints[currentList][i+1].x),
			fp_ceil(clippedPoints[currentList][i+1].y),
			0xffffff);

		drawLine(
			fp_ceil(clippedPoints[currentList][0].x),
			fp_ceil(clippedPoints[currentList][0].y),
			fp_ceil(clippedPoints[currentList][i].x),
			fp_ceil(clippedPoints[currentList][i].y),
			0xffffff);

		drawLine(
			fp_ceil(clippedPoints[currentList][0].x),
			fp_ceil(clippedPoints[currentList][0].y),
			fp_ceil(clippedPoints[currentList][i+1].x),
			fp_ceil(clippedPoints[currentList][i+1].y),
			0xffffff);
	}
	*/
}
#endif	//SYMBIAN
