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

#include <mavsprintf.h>
#include "common.h"
#include "MAHeaders.h"

void maCreateImageRawTest();
void maDrawRGBTest();
void maCreateImageFromDataTest();
void maGetImageDataTest();

void addAdvGfxTests(TestSuite* suite);
void addAdvGfxTests(TestSuite* suite) {
	suite->addTestCase(new TemplateCase<maCreateImageRawTest>("maCreateImageRaw test"));
	suite->addTestCase(new TemplateCase<maDrawRGBTest>("maDrawRGB test"));
	suite->addTestCase(new TemplateCase<maCreateImageFromDataTest>("maCreateImageFromData test"));
	suite->addTestCase(new TemplateCase<maGetImageDataTest>("maGetImageData test"));
}



#define GRADIENT_SEGMENT_HEIGHT 16
void maCreateImageRawTest()
{
	MAExtent e = maGetScrSize();
	Dimensions screen;
	screen.width = EXTENT_X(e);
	screen.height = EXTENT_Y(e);
	maSetClipRect(0, 0, screen.width, screen.height);

	int *src = new int[screen.width*GRADIENT_SEGMENT_HEIGHT];
	int delta = (0xff<<16)/screen.width;
	int ofs = 0;
	for(int j = 0; j < GRADIENT_SEGMENT_HEIGHT; j++)
	{
		int c = 0;
		for(int i = 0; i < screen.width; i++)
		{
			src[i+ofs] = (c>>16)*0x01010101;
			c+=delta;
		}
		ofs+=screen.width;
	}

	MAHandle h = maCreatePlaceholder();
	::maCreateImageRaw(h, src, EXTENT(screen.width,GRADIENT_SEGMENT_HEIGHT), TRUE);
	delete []src;

	int incx = (screen.width>>3);
	int incy = (screen.height>>4);
	int x = 0;
	for(int j = 0; j < screen.height; j+=incy)
	{
		for(int i = 0; i < screen.width; i+=incx)
		{
			maSetColor((x&0x1)*0xffffffff);
			maFillRect(i, j, incx, incy);
			x++;
		}
		x++;
	}

	for(int j = 0; j < screen.height; j+=GRADIENT_SEGMENT_HEIGHT)
	{
		maDrawImage(h, 0, j);
	}

	maSetColor(0xff0000);
	const char *title = "maCreateImageRaw:";
	maDrawText(4, 0, title);
	maDrawText(4, EXTENT_Y(maGetTextSize(title))+2, "alpha gradient on chessboard");

	maUpdateScreen();
}

void maCreateImageFromDataTest()
{
	MAExtent e = maGetScrSize();
	Dimensions screen;
	screen.width = EXTENT_X(e);
	screen.height = EXTENT_Y(e);
	maSetClipRect(0, 0, screen.width, screen.height);

	maSetColor(BLACK);
	maFillRect(0, 0, screen.width, screen.height);

	MAHandle h = maCreatePlaceholder();
	maCreateImageFromData(h, CLIENT_PNG_TEST, 0, maGetDataSize(CLIENT_PNG_TEST));
	maDrawImage(h, (screen.width>>1)-(EXTENT_X(maGetImageSize(h))>>1), (screen.height>>1)-(EXTENT_Y(maGetImageSize(h))>>1));
	maDestroyObject(h);

	maSetColor(0xff0000);
	const char *title = "maCreateImageFromData:";
	maDrawText(4, 0, title);
	maDrawText(4, EXTENT_Y(maGetTextSize(title))+2, "png image");

	maUpdateScreen();
}

void maDrawRGBTest()
{
	MAExtent e = maGetScrSize();
	Dimensions screen;
	screen.width = EXTENT_X(e);
	screen.height = EXTENT_Y(e);
	maSetClipRect(0, 0, screen.width, screen.height);

	int *src = new int[screen.width*GRADIENT_SEGMENT_HEIGHT];
	int delta = (0xff<<16)/screen.width;
	int ofs = 0;
	for(int j = 0; j < GRADIENT_SEGMENT_HEIGHT; j++)
	{
		int c = 0xffffff;
		for(int i = 0; i < screen.width; i++)
		{
			src[i+ofs] = (c>>16)*0x01010101;
			c-=delta;
		}
		ofs+=screen.width;
	}

	int incx = (screen.width>>3);
	int incy = (screen.height>>4);
	int x = 1;
	for(int j = 0; j < screen.height; j+=incy)
	{
		for(int i = 0; i < screen.width; i+=incx)
		{
			maSetColor((x&0x1)*0xffffffff);
			maFillRect(i, j, incx, incy);
			x++;
		}
		x++;
	}

	MAPoint2d dst = {0, 0};
	MARect srcRect = {0, 0, screen.width, GRADIENT_SEGMENT_HEIGHT};
	for(int j = 0; j < screen.height; j+=GRADIENT_SEGMENT_HEIGHT)
	{
		maDrawRGB(&dst, src, &srcRect, screen.width);
		dst.y+=GRADIENT_SEGMENT_HEIGHT;
	}
	delete []src;

	maSetColor(0xff0000);
	const char *title = "maDrawRGB:";
	maDrawText(4, 0, title);
	maDrawText(4, EXTENT_Y(maGetTextSize(title))+2, "alpha gradient on chessboard");

	maUpdateScreen();
}

void maGetImageDataTest()
{
	MAExtent e = maGetScrSize();
	Dimensions screen;
	screen.width = EXTENT_X(e);
	screen.height = EXTENT_Y(e);
	maSetClipRect(0, 0, screen.width, screen.height);

	maSetColor(BLACK);
	maFillRect(0, 0, screen.width, screen.height);

	MAHandle h = maCreatePlaceholder();
	maCreateImageFromData(h, CLIENT_PNG_TEST, 0,  maGetDataSize(CLIENT_PNG_TEST));

	MAExtent size = maGetImageSize(h);
	MARect srcRect = {0, 0, EXTENT_X(size), EXTENT_Y(size)};
	int *img = new int[EXTENT_X(size)*EXTENT_Y(size)];
	maGetImageData(h, img, &srcRect, EXTENT_X(size));

	int imageWidth = EXTENT_X(maGetImageSize(h));
	int imageHeight = EXTENT_Y(maGetImageSize(h));


	maDestroyObject(h);
	::maCreateImageRaw(h, img, size, TRUE);
	delete []img;
//	maDrawImage(h, screen.width>>1, screen.height>>1);
	maDrawImage(h, (screen.width>>1)-(imageWidth>>1), (screen.height>>1)-(imageHeight>>1));

	maSetColor(0xff0000);
	const char *title = "maGetImageData:";
	maDrawText(4, 0, title);
	maDrawText(4, EXTENT_Y(maGetTextSize(title))+2, "png image");

	maUpdateScreen();
}
