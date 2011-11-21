/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

#include <mavsprintf.h>
#include "common.h"
#include "MAHeaders.h"

int maCreateImageRawTest();
int maDrawRGBTest();
int maDrawRGBTest2();
int maCreateImageFromDataTest();
int maGetImageDataTest();

void addAdvGfxTests(TestSuite* suite);

/**
 * @brief Adds the advanced graphics tests to the test suite
 *
 * @param suite		The test suite to which these tests shoulc be added
 */
void addAdvGfxTests(TestSuite* suite)
{
	suite->addTestCase(new TemplateCase<maCreateImageRawTest>("maCreateImageRaw test"));
	suite->addTestCase(new TemplateCase<maDrawRGBTest>("maDrawRGB test"));
	suite->addTestCase(new TemplateCase<maDrawRGBTest2>("maDrawRGB test2"));
	suite->addTestCase(new TemplateCase<maCreateImageFromDataTest>("maCreateImageFromData test"));
	suite->addTestCase(new TemplateCase<maGetImageDataTest>("maGetImageData test"));
}

#define GRADIENT_SEGMENT_HEIGHT 16
#define SIDE_HEIGHT 40

/**
 * @brief Tests the maCreateImageRaw syscall
 *
 * The tests generates a gradient with alpha to memory and then
 * creates a drawable resource with the maCreateImageRaw syscall.
 * The test then draws a chessboard pattern and draws the gradient
 * over it.
 */
int maCreateImageRawTest()
{
	MAExtent e = maGetScrSize();
	Dimensions screen;
	screen.width = EXTENT_X(e);
	screen.height = EXTENT_Y(e);
	maSetClipRect(0, 0, screen.width, screen.height);

	int *src = new int[screen.width*GRADIENT_SEGMENT_HEIGHT];
	if(NULL == src)
		return FUNC_OUT_OF_MEMORY_ALLOC;

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
	if(RES_OK != ::maCreateImageRaw(h, src, EXTENT(screen.width,GRADIENT_SEGMENT_HEIGHT), TRUE))
	{
		delete []src;
		return FUNC_OUT_OF_MEMORY_RESOURCE;
	}
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

	return FUNC_OK;
}

/**
 * @brief Tests the maCreateImageFromData syscall
 *
 * The tests creates a drawable resource from a PNG file
 * using the maCreateImageFromData syscall, and the
 * renders the result to the screen.
 */
int maCreateImageFromDataTest()
{
	MAExtent e = maGetScrSize();
	Dimensions screen;
	screen.width = EXTENT_X(e);
	screen.height = EXTENT_Y(e);
	maSetClipRect(0, 0, screen.width, screen.height);

	maSetColor(BLACK);
	maFillRect(0, 0, screen.width, screen.height);

	MAHandle h = maCreatePlaceholder();
	int returnCode = maCreateImageFromData(h, CLIENT_PNG_TEST, 0,
											maGetDataSize(CLIENT_PNG_TEST));
	if(RES_OK != returnCode)
		return FUNC_OUT_OF_MEMORY_RESOURCE;


	maDrawImage(h, (screen.width>>1)-(EXTENT_X(maGetImageSize(h))>>1), (screen.height>>1)-(EXTENT_Y(maGetImageSize(h))>>1));
	maDestroyObject(h);

	maSetColor(0xff0000);
	const char *title = "maCreateImageFromData:";
	maDrawText(4, 0, title);
	maDrawText(4, EXTENT_Y(maGetTextSize(title))+2, "png image");

	maUpdateScreen();

	return FUNC_OK;
}

/**
 * @brief Tests the maDrawRGB syscall
 *
 * The tests generates a gradient with alpha and store that in memory.
 * It then draws a chessboard to the screen and draws the gradient
 * with the maDrawRGB syscall.
 */
int maDrawRGBTest()
{
	MAExtent e = maGetScrSize();
	Dimensions screen;
	screen.width = EXTENT_X(e);
	screen.height = EXTENT_Y(e);
	maSetClipRect(0, 0, screen.width, screen.height);

	int *src = new int[screen.width*GRADIENT_SEGMENT_HEIGHT];
	if(NULL == src)
		return FUNC_OUT_OF_MEMORY_ALLOC;

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

	return FUNC_OK;
}

static void drawRgb2(int* buf, int x, int y, int color) {
	for(int j = 0; j < SIDE_HEIGHT; j++) {
		for(int i = 0; i < SIDE_HEIGHT; i++) {
			buf[i+j*SIDE_HEIGHT] = color;
		}
	}

	MAPoint2d dst = {x, SIDE_HEIGHT+y};
	MARect srcRect = {0, 0, SIDE_HEIGHT, SIDE_HEIGHT};
	maDrawRGB(&dst, buf, &srcRect, SIDE_HEIGHT);
}

/**
* @brief Tests the maDrawRGB syscall again.
*
* This test attempts to draw rectangles in pure
* blue, red, and green, respectively.
*/
int maDrawRGBTest2()
{
	MAExtent e = maGetScrSize();
	Dimensions screen;
	screen.width = EXTENT_X(e);
	screen.height = EXTENT_Y(e);
	maSetClipRect(0, 0, screen.width, screen.height);
	maSetColor(0);
	maFillRect(0,0, screen.width, screen.height);

	int *src = new int[SIDE_HEIGHT*SIDE_HEIGHT];
	if(NULL == src)
		return FUNC_OUT_OF_MEMORY_ALLOC;
	drawRgb2(src, 0,0, 0xffff0000);	//red, upper-left
	drawRgb2(src, SIDE_HEIGHT,0, 0xff00ff00);	//green, upper-right
	drawRgb2(src, 0,SIDE_HEIGHT, 0xff0000ff);	//blue, lower-left
	delete []src;

	maSetColor(0xff0000);
	const char *title = "maDrawRGB2:";
	maDrawText(4, 0, title);
	maDrawText(4, EXTENT_Y(maGetTextSize(title))+2, "RG\nB");

	maUpdateScreen();

	return FUNC_OK;
}

/**
 * @brief Tests the maGetImageData syscall
 *
 * The tests generates a drawable resource from a PNG file with the
 * maCreateImageFromData syscall. The actual pixels are then retrived
 * with maGetImageData and a new drawable resource is created with
 * maCreateImageRaw.
 * The resulting resource are then drawn to the scren with maDrawImage.
 */
int maGetImageDataTest()
{
	MAExtent e = maGetScrSize();
	Dimensions screen;
	screen.width = EXTENT_X(e);
	screen.height = EXTENT_Y(e);
	maSetClipRect(0, 0, screen.width, screen.height);

	maSetColor(BLACK);
	maFillRect(0, 0, screen.width, screen.height);

	MAHandle h = maCreatePlaceholder();
	int returnCode = maCreateImageFromData(h, CLIENT_PNG_TEST, 0,
											maGetDataSize(CLIENT_PNG_TEST));
	if(RES_OK != returnCode)
		return FUNC_OUT_OF_MEMORY_RESOURCE;

	MAExtent size = maGetImageSize(h);
	MARect srcRect = {0, 0, EXTENT_X(size), EXTENT_Y(size)};
	int *img = new int[EXTENT_X(size)*EXTENT_Y(size)];
	if(NULL == img)
		return FUNC_OUT_OF_MEMORY_ALLOC;

	maGetImageData(h, img, &srcRect, EXTENT_X(size));

	int imageWidth = EXTENT_X(maGetImageSize(h));
	int imageHeight = EXTENT_Y(maGetImageSize(h));


	maDestroyObject(h);
	returnCode = ::maCreateImageRaw(h, img, size, TRUE);
	if(RES_OK != returnCode)
	{
		delete []img;
		return FUNC_OUT_OF_MEMORY_RESOURCE;
	}

	delete []img;
	maDrawImage(h, (screen.width>>1)-(imageWidth>>1), (screen.height>>1)-(imageHeight>>1));

	maSetColor(0xff0000);
	const char *title = "maGetImageData:";
	maDrawText(4, 0, title);
	maDrawText(4, EXTENT_Y(maGetTextSize(title))+2, "png image");

	maUpdateScreen();

	return FUNC_OK;
}
