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


namespace TextOutput {
	void drawImage(Image *dst, int x, int y, const unsigned int *src, int w, int h, unsigned int color) {	
		if(x < dst->clipRect.x) return;
		if(x + w > dst->clipRect.x + dst->clipRect.width) return;
		if(y < dst->clipRect.y) return;
		if(y + h > dst->clipRect.y + dst->clipRect.height) return;

		unsigned int bpp = dst->bytesPerPixel;
		const unsigned int *src_ptr = &src[0];

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
				for(int j = 0; j < h; j++) {
					dst_scan = (unsigned short*)dst_ptr;
					for(int i = 0; i < w; i++) {
						int d = *dst_scan;
						unsigned int s = (*src_ptr)&0xff;

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
				}
			}
			break;
		case 4:
			{
				unsigned int *dst_scan;
				for(int j = 0; j < h; j++) {
					dst_scan = (unsigned int*)dst_ptr;
					for(int i = 0; i < w; i++) {
						int d = *dst_scan;
						unsigned int s = (*src_ptr)&0xff;
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
				}
			}
			break;
		default:
			BIG_PHAT_ERROR(WCEERR_UNSUPPORTED_BPP);
			break;
		}
	}

	SIZE getTextSize(const void *str, bool wide) {
		SIZE ret;
		ret.cx = 0;
		ret.cy = 0;

		HANDLE hFont;
		HDC dc = GetDC(NULL);
		HDC cdc = CreateCompatibleDC(dc);
		GLE(cdc);
		ReleaseDC(NULL, dc);

		if(hFont = (HFONT)GetStockObject(SYSTEM_FONT)) {
			SelectObject(cdc, hFont);
		}

		if(wide)
			GetTextExtentPoint32W(cdc, (LPCWSTR)str, wcslen((const wchar_t*)str), &ret);
		else {
			size_t len = strlen((const char*)str);
			WCHAR *unicode = new WCHAR[len];
			convertAsciiToUnicode(unicode, len, (const char*)str);
			GetTextExtentPoint32W(cdc, unicode, len, &ret);
			delete unicode;
		}

		DeleteDC(cdc);

		return ret;
	}

	void drawText(Image *dst, int x, int y, const void *str, unsigned int color, bool wide) {
		DWORD* pBitmapBits;
		BITMAPINFO bmi;

		SIZE size = getTextSize(str, wide);

		ZeroMemory(&bmi.bmiHeader, sizeof(BITMAPINFOHEADER));
		bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth       =  (int)size.cx;
		bmi.bmiHeader.biHeight      = -(int)size.cy;
		bmi.bmiHeader.biPlanes      = 1;
		bmi.bmiHeader.biCompression = BI_RGB;
		bmi.bmiHeader.biBitCount    = 32;


		HANDLE hFont = (HFONT)GetStockObject(SYSTEM_FONT);
		GLE(hFont);

		HDC dc = GetDC(NULL);
		HDC cdc = CreateCompatibleDC(dc);
		GLE(cdc);
		ReleaseDC(NULL, dc);

		HBITMAP hbmBitmap = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS,
			(void**)&pBitmapBits, NULL, 0);
		GLE(hbmBitmap);

		SelectObject(cdc, hFont);
		SelectObject(cdc, hbmBitmap);
		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = size.cx;
		rect.bottom = size.cy;

		SetBkMode(cdc, OPAQUE);
		SetBkColor(cdc, 0);
		SetTextColor(cdc, 0xffffff);

		if(wide) {
			GLE(DrawTextW(cdc, (LPCWSTR)str, -1, &rect, DT_TOP|DT_LEFT|DT_SINGLELINE));
		} else {
			size_t len = strlen((const char*)str);
			WCHAR *unicode = new WCHAR[len];
			convertAsciiToUnicode(unicode, len, (const char*)str);
			GLE(DrawTextW(cdc, unicode, len, &rect, DT_TOP|DT_LEFT|DT_SINGLELINE));
			delete unicode;
		}

		//copy text to framebuffer
		drawImage(dst, x, y, (const unsigned int*)pBitmapBits, rect.right, rect.bottom, color);

		DeleteDC(cdc);
		DeleteObject(hbmBitmap);	
	}

} // namespace TextOutput