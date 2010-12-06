/* Copyright (C) 2010 MoSync AB

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

#include <ma.h>
#include <maheap.h>
#include <conprint.h>

#include "common.h"
#include "Image.h"

int framebufferTest();

#define CC CONVERT_TO_NATIVE_COLOR_FROM_ARGB
#define CONVERT_TO_NATIVE_COLOR_FROM_ARGB(col) \
	((((((col)&0x00ff0000)>>16)>>(8-fi.redBits))<<fi.redShift)&fi.redMask) |\
	((((((col)&0x0000ff00)>>8)>>(8-fi.greenBits))<<fi.greenShift)&fi.greenMask) |\
	((((((col)&0x000000ff)>>0)>>(8-fi.blueBits))<<fi.blueShift)&fi.blueMask)

void framebufferMallocHandler(int size);
void addFramebufferTests(TestSuite* suite);

/**
 * @brief Adds the framebuffer test to the current test suite
 *
 * @param suite 	The test suite in which this should be added to
 */
void addFramebufferTests(TestSuite* suite)
{
	suite->addTestCase(new TemplateCase<framebufferTest>("Framebuffer test"));
}

/**
 * @brief Tests the framebuffer api
 *
 * The test generates an image with 8 colored blocks and
 * renders it to the screen with the framebuffer api.
 * When the frame buffer is allocated a malloc handler is
 * used so that we won't get a maPanic.
 *
 */
int framebufferTest()
{
	MAFrameBufferInfo fi;

	int returnCode = maFrameBufferGetInfo(&fi);
	if(0 > returnCode)
		return FUNC_SYSCALL_NOT_SUPPORTED;

	malloc_handler oldMallocHandler = set_malloc_handler(framebufferMallocHandler);
	int* framebuffer = (int*)malloc(fi.sizeInBytes);
	set_malloc_handler(oldMallocHandler);
	if(NULL == framebuffer)
	{
		return FUNC_OUT_OF_MEMORY_ALLOC;
	}

	returnCode = maFrameBufferInit(framebuffer);
	if( returnCode <= 0)
	{
		free(framebuffer);
		return FUNC_SYSCALL_ERROR;
	}

	//draw color rects, plot some pixels over them
	Image::PixelFormat pf;

	switch(fi.bitsPerPixel) {
		case 12: pf = Image::PIXELFORMAT_RGB444; break;
		case 15: pf = Image::PIXELFORMAT_RGB555; break;
		case 16: pf = Image::PIXELFORMAT_RGB565; break;
		case 24: pf = Image::PIXELFORMAT_RGB888; break;
		case 32: pf = Image::PIXELFORMAT_ARGB8888; break;
		default: maPanic(1, "Unsupported pixel format!");
	}

	Image img((byte*)framebuffer, NULL, fi.width, fi.height, fi.pitch, pf);

	img.bytesPerPixel = fi.bytesPerPixel;
	img.bitsPerPixel = fi.bitsPerPixel;

	img.redMask = fi.redMask;
	img.redShift = fi.redShift;
	img.redBits = fi.redBits;
	img.greenMask = fi.greenMask;
	img.greenShift = fi.greenShift;
	img.greenBits = fi.greenBits;
	img.blueMask = fi.blueMask;
	img.blueShift = fi.blueShift;
	img.blueBits = fi.blueBits;

	int midX = fi.width >> 1;
	int midY = fi.height >> 1;

	int rectWidth = midX - 2;
	int rectHeight = midY / 2 - 2;

	img.drawFilledRect(0, 0, fi.width, fi.height, CC(0x800080));	//deep purple

	img.drawFilledRect(1, 1, rectWidth, rectHeight, CC(RGB(255, 0, 0)));	//RED
	img.drawFilledRect(1+rectWidth+2, 1, rectWidth, rectHeight, CC(RGB(255, 255, 0)));	//YELLA

	img.drawFilledRect(1, 1+rectHeight+2, rectWidth, rectHeight, CC(RGB(0, 255, 0)));	//GREEN
	img.drawFilledRect(1+rectWidth+2, 1+rectHeight+2, rectWidth, rectHeight, CC(RGB(0, 255, 255)));	//TORQUOISE

	img.drawFilledRect(1, 1+(rectHeight+2)*2, rectWidth, rectHeight, CC(RGB(0, 0, 255)));	//BLUE
	img.drawFilledRect(1+rectWidth+2, 1+(rectHeight+2)*2, rectWidth, rectHeight, CC(RGB(255, 0, 255)));	//PURPLE

	img.drawFilledRect(1, 1+(rectHeight+2)*3, rectWidth, rectHeight, CC(RGB(255, 255, 255)));	//WHITE
	img.drawFilledRect(1+rectWidth+2, 1+(rectHeight+2)*3, rectWidth, rectHeight, CC(BLACK));	//BLACK

	//draw points in the corners of the screen
	img.drawPoint(0, 0, 0xFFFFFF);	//WHITE
	img.drawPoint(0, fi.height - 1, 0xFFFFFF);	//WHITE
	img.drawPoint(fi.width - 1, 0, 0xFFFFFF);	//WHITE
	img.drawPoint(fi.width - 1, fi.height - 1, 0xFFFFFF);	//WHITE

	maUpdateScreen();

	returnCode = maFrameBufferClose();
	if(returnCode <= 0)
	{
		return FUNC_SYSCALL_ERROR;
	}
	return FUNC_OK;
}

/**
 * @brief Function which handles malloc errors
 *
 * The standard behavior when a malloc isn't possible
 * is that a maPanic is triggered. By changing the malloc
 * handler the maPanic is not triggered and a printf
 * is just shown to inform the user that it didn't succeed.
 */
void framebufferMallocHandler(int size) {
	printf("malloc(%i) failed\n", size);
}
