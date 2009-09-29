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

#include "TextOutput.h"
#include <config_platform.h>
#include <helpers/helpers.h>

#include <base_errors.h>
using namespace MoSyncError;

#include "wce_errors.h"
#include "wce_helpers.h"

#define FONT_BITMAP_WIDTH 256
#define FONT_BITMAP_HEIGHT 512

TextOutput::TextOutput() {
	fontBitmap = new unsigned char[FONT_BITMAP_WIDTH*FONT_BITMAP_HEIGHT];
}

TextOutput::~TextOutput() {
	if(fontBitmap) {
		delete fontBitmap;
		fontBitmap = 0;
	}
}

bool TextOutput::init() {
	DWORD*      pBitmapBits;
	BITMAPINFO bmi;
	ZeroMemory( &bmi.bmiHeader, sizeof(BITMAPINFOHEADER) );
	bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth       =  (int)FONT_BITMAP_WIDTH;
	bmi.bmiHeader.biHeight      = -(int)FONT_BITMAP_HEIGHT;
	bmi.bmiHeader.biPlanes      = 1;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biBitCount    = 32;
		
	HDC dc = CreateCompatibleDC(GetDC(NULL));
	HBITMAP hbmBitmap = CreateDIBSection( NULL, &bmi, DIB_RGB_COLORS,
				(void**)&pBitmapBits, NULL, 0 );

	HANDLE hFont;


	if(hFont = (HFONT)GetStockObject(SYSTEM_FONT)) {
		SelectObject(dc, hFont);
	}

	SelectObject(dc, hbmBitmap);

	SIZE size;
	WCHAR str[2] = TEXT("x");
	if(GetTextExtentPoint32( dc, str, 1, &size )==0)
		return false;

	spacing = (int)ceil(size.cy * 0.3f);

	DWORD x = spacing;
	DWORD y = 0;

	SetBkMode(dc, OPAQUE);
	SetBkColor(dc, 0);
	SetTextColor(dc, 0xffffff);
	
	for( char c = 32; c < 127; c++ )
	{
		str[0] = c;

		if(GetTextExtentPoint32( dc, str, 1, &size ) == 0)
			return false;

		if( (DWORD)(x + size.cx + spacing) > FONT_BITMAP_WIDTH )
		{
			x  = spacing;
			y += size.cy + 1;
		}

		if( y + size.cy > FONT_BITMAP_HEIGHT ) {
			break;
		}

		RECT rect;
		rect.left = x;
		rect.top = y;
		rect.right = FONT_BITMAP_WIDTH;
		rect.bottom = FONT_BITMAP_HEIGHT;

		DrawText(dc, str, 1, &rect ,DT_TOP|DT_LEFT|DT_SINGLELINE);

		fontCoords[c-32].x = x;
		fontCoords[c-32].y = y;
		fontCoords[c-32].width = size.cx;
		fontCoords[c-32].height = size.cy;

		x += size.cx + (2 * spacing);
	}

	DeleteDC(dc);

	int index = 0;
	for(int i = 0; i < FONT_BITMAP_HEIGHT; i++) {
		for(int j = 0; j < FONT_BITMAP_WIDTH; j++) {
			fontBitmap[index] = (unsigned char) (pBitmapBits[index]&0xff);
			index++;
		}
	}

	DeleteObject(hbmBitmap);

	return true;
}

void TextOutput::drawChar(Image *dst, int x, int y, char c, unsigned int color) {
	
	Rect fontRect = fontCoords[c-32];
	if(x < dst->clipRect.x) return;
	if(x + fontRect.width > dst->clipRect.x + dst->clipRect.width) return;
	if(y < dst->clipRect.y) return;
	if(y + fontRect.height > dst->clipRect.y + dst->clipRect.height) return;
	
	unsigned int bpp = dst->bytesPerPixel;
	unsigned char *src_ptr = &fontBitmap[fontRect.x + fontRect.y*FONT_BITMAP_WIDTH];

	unsigned int redMask = dst->redMask;
	unsigned int greenMask = dst->greenMask;
	unsigned int blueMask = dst->blueMask;
	unsigned int redShift = dst->redShift;
	unsigned int greenShift = dst->greenShift;
	unsigned int blueShift = dst->blueShift;

	int red = (((color)&redMask)>>redShift);
	int green = (((color)&greenMask)>>greenShift);
	int blue = (((color)&blueMask)>>blueShift);
			
	unsigned char *dst_ptr = &dst->data[dst->pitch*y + x*bpp];

	switch(bpp) {
		case 2:
			{
			unsigned short *dst_scan;
			for(int j = 0; j < fontRect.height; j++) {
				dst_scan = (unsigned short*)dst_ptr;
				for(int i = 0; i < fontRect.width; i++) {
					int d = *dst_scan;
					int s = *src_ptr;

					int dr = (((d)&redMask)>>redShift);
					int dg = (((d)&greenMask)>>greenShift);
					int db = (((d)&blueMask)>>blueShift);
					*dst_scan = 
									(((dr + (((red-dr)*(s))>>8)) << redShift)&redMask) |
									(((dg + (((green-dg)*(s))>>8)) << greenShift)&greenMask) |
									(((db + (((blue-db)*(s))>>8)) << blueShift)&blueMask);

					dst_scan++;
					src_ptr++;
				}
				dst_ptr+=dst->pitch;
				src_ptr+=-fontRect.width + FONT_BITMAP_WIDTH;
			}
			}
			break;
		case 4:
			{
			unsigned int *dst_scan;
			for(int j = 0; j < fontRect.height; j++) {
				dst_scan = (unsigned int*)dst_ptr;
				for(int i = 0; i < fontRect.width; i++) {
					int d = *dst_scan;
					int s = *src_ptr;
					int dr = (((d)&redMask)>>redShift);
					int dg = (((d)&greenMask)>>greenShift);
					int db = (((d)&blueMask)>>blueShift);
					*dst_scan = 
									(((dr + (((red-dr)*(s))>>8)) << redShift)&redMask) |
									(((dg + (((green-dg)*(s))>>8)) << greenShift)&greenMask) |
									(((db + (((blue-db)*(s))>>8)) << blueShift)&blueMask);

					dst_scan++;
					src_ptr++;
				}
				dst_ptr+=dst->pitch;
				src_ptr+=-fontRect.width + FONT_BITMAP_WIDTH;
			}
			}
			break;
		default:
			BIG_PHAT_ERROR(WCEERR_UNSUPPORTED_BPP);
			break;
	}
}

SIZE TextOutput::getTextSize(const char *str) {
	int w = 0, h = 0;
	while(*str) {
		if(fontCoords[(*str)-32].height > h) h = fontCoords[(*str)-32].height;	
		w += fontCoords[(*str)-32].width;
		str++;
	}

	SIZE ret;
	ret.cx = w;
	ret.cy = h;
	return ret;
}

void TextOutput::drawText(Image *dst, int x, int y, const char *str, unsigned int color) {
	while(*str) {
		drawChar(dst, x, y, *str, color);
		x += fontCoords[(*str)-32].width;
		str++;
	}
}
