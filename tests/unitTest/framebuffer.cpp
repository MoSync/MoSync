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

#include <ma.h>
#include <maheap.h>
#include <conprint.h>

#include "common.h"
#include "Image.h"

class FramebufferTest : public KeyBaseCase {
public:
	FramebufferTest() : KeyBaseCase("Framebuffer") {}

	void start();

	virtual void open() {
		KeyBaseCase::open();
	}
	virtual void close() {
		KeyBaseCase::close();
	}

	//KeyListener
	virtual void keyPressEvent(int keyCode) {
	}
	virtual void keyReleaseEvent(int keyCode) {
		checkYesNo(keyCode);
	}
};

void framebuffer_malloc_handler(int size);

#define CC CONVERT_TO_NATIVE_COLOR_FROM_ARGB
#define CONVERT_TO_NATIVE_COLOR_FROM_ARGB(col) \
	((((col)&0x00ff0000)>>(16-(fi.redShift-(8-fi.redBits))))&fi.redMask) | \
	((((col)&0x0000ff00)>>(8-(fi.greenShift-(8-fi.greenBits))))&fi.greenMask) | \
	((((col)&0x000000ff)>>(0-(fi.blueShift-(8-fi.blueBits))))&fi.blueMask)

void addFramebufferTests(TestSuite* suite);
void addFramebufferTests(TestSuite* suite) {
	suite->addTestCase(new FramebufferTest);
}

void FramebufferTest::start() {
	MAFrameBufferInfo fi;
	assert("maFrameBufferGetInfo", maFrameBufferGetInfo(&fi) >= 0);
	malloc_handler old_malloc_handler = set_malloc_handler(framebuffer_malloc_handler);
	int* fb = (int*)malloc(fi.sizeInBytes);
	set_malloc_handler(old_malloc_handler);
	if(fb == NULL) {
		assert("malloc", false);
		return;
	}

	int fbRet = maFrameBufferInit(fb);
	if( fbRet <= 0)
	{
		assert("maFrameBufferInit", false);
		free(fb);
		return;
	}

	//draw
#if 0
	for(int i=0; i<fi.sizeInBytes / sizeof(int); i++) {
		fb[i] = rand();
	}
#endif
	//draw color rects, plot some pixels over them

	Image img((byte*)fb, NULL, fi.width, fi.height, fi.pitch, Image::PIXELFORMAT_RGB888);

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
	assert("maFrameBufferClose", maFrameBufferClose() > 0);
}

void framebuffer_malloc_handler(int size) {
	printf("malloc(%i) failed\n", size);
}
