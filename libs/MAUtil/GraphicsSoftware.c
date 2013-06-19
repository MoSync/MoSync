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

#include "GraphicsSoftware.h"
#include <maassert.h>

#define MA_CLIP_STACK_DEPTH 128
#define MA_TRANSFORM_STACK_DEPTH 128

static MAPoint2d sTransformStack[MA_TRANSFORM_STACK_DEPTH];
static int sTransformStackPtr = -1;

static MAPoint2d sCurrentOffset = {0, 0};

#define PUSH_EMPTY_CLIPRECT Gfx_pushClipRect(0,0,0,0)
#define false 0
#define true 1

// driver setup
static void soft_setup(int x, int y, int w, int h);
static void soft_setClipRect(int x, int y, int w, int h);
static void soft_clearMatrix(void);
static void soft_init(void);
static void soft_pushMatrix(void);
static void soft_popMatrix(void);
static void soft_translate(int x, int y);
static MAPoint2d soft_getTranslation(void);
static void soft_scale(MAFixed x, MAFixed y);
static void soft_plot(int x, int y);
static void soft_line(int x1, int y1, int x2, int y2);
static void soft_fillRect(int left, int top, int width, int height);
static void soft_drawText(int left, int top, const char* text);
static void soft_drawTextW(int left, int top, const wchar_t* text);
static void soft_drawImage(MAHandle image, int left, int top);
static void soft_drawRGB(const MAPoint2d *dstPoint, const void *src, const MARect *srcRect, int scanlength);
static void soft_drawImageRegion(MAHandle image, const MARect *srcRect, const MAPoint2d *dstPoint, int transformMode);
static void soft_notifyImageUpdated(MAHandle image);
static void soft_beginRendering(void);
static void soft_updateScreen(void);
static void soft_setClearColor(int r, int g, int b);
static void soft_setColor(int r, int g, int b);
static void soft_setAlpha(int a);

static MAGraphicsDriver sSoftwareGraphicsDriver = {
	&soft_setup,
	&soft_setClipRect,
	&soft_clearMatrix,
	&soft_pushMatrix,
	&soft_popMatrix,
	&soft_translate,
	&soft_getTranslation,
	&soft_scale,
	&soft_plot,
	&soft_line,
	&soft_fillRect,
	&soft_drawText,
	&soft_drawTextW,
	&soft_drawImage,
	&soft_drawRGB,
	&soft_drawImageRegion,
	&soft_notifyImageUpdated,
	&soft_beginRendering,
	&soft_updateScreen,
	&soft_setClearColor,
	&soft_setColor,
	&soft_setAlpha

};

//MAGraphicsDriver* Gfx_getDriverSoftware(void) {
void Gfx_useDriverSoftware(void) {
	Gfx_useDriver(&sSoftwareGraphicsDriver);
}


static void soft_setup(int x, int y, int w, int h) {
}

static void soft_setClipRect(int x, int y, int w, int h) {
	maSetClipRect(x, y, w, h);
}

static void soft_clearMatrix(void) {
	sTransformStackPtr = 0;
	sCurrentOffset.x = 0;
	sCurrentOffset.y = 0;
	sTransformStack[0].x = 0;
	sTransformStack[0].y = 0;
}

static void soft_init(void) {
	if(sTransformStackPtr < 0)
		soft_clearMatrix();
}


static void soft_pushMatrix(void) {
	soft_init();
	if(sTransformStackPtr >= MA_TRANSFORM_STACK_DEPTH-1) {
		PANIC_MESSAGE("Transform stack overflow");
		return;
	}

	sTransformStackPtr++;
	sTransformStack[sTransformStackPtr] = sCurrentOffset;
}

static void soft_popMatrix(void) {
	soft_init();
	if(sTransformStackPtr < 0) {
		PANIC_MESSAGE("Transform stack underflow");
		return;
	}
	sCurrentOffset = sTransformStack[sTransformStackPtr];
	sTransformStackPtr--;
}

static void soft_translate(int x, int y) {
	sCurrentOffset.x += x;
	sCurrentOffset.y += y;
}

static MAPoint2d soft_getTranslation(void) {
	return sCurrentOffset;
}

static void soft_scale(MAFixed x, MAFixed y) {

}

static void soft_plot(int x, int y) {
	maPlot(sCurrentOffset.x + x, sCurrentOffset.y + y);
}

static void soft_line(int x1, int y1, int x2, int y2) {
	maLine(sCurrentOffset.x + x1, sCurrentOffset.y + y1, sCurrentOffset.x + x2, sCurrentOffset.y + y2);
}

static void soft_fillRect(int left, int top, int width, int height) {
	maFillRect(
		sCurrentOffset.x + left,
		sCurrentOffset.y + top,
		width,
		height
		);
}

static void soft_drawText(int left, int top, const char* text) {
	maDrawText(sCurrentOffset.x + left, sCurrentOffset.y + top, text);
}

static void soft_drawTextW(int left, int top, const wchar_t* text) {
	maDrawTextW(sCurrentOffset.x + left, sCurrentOffset.y + top, (wchar*)text);
}

static void soft_drawImage(MAHandle image, int left, int top) {
	maDrawImage(image, sCurrentOffset.x + left, sCurrentOffset.y + top);
}

static void soft_drawRGB(const MAPoint2d *dstPoint, const void *src, const MARect *srcRect, int scanlength) {
	MAPoint2d p = *dstPoint;
	p.x += sCurrentOffset.x;
	p.y += sCurrentOffset.y;

	maDrawRGB(&p, src, srcRect, scanlength);
}

static void soft_drawImageRegion(MAHandle image, const MARect *srcRect, const MAPoint2d *dstPoint, int transformMode) {
	MAPoint2d p = *dstPoint;
	p.x += sCurrentOffset.x;
	p.y += sCurrentOffset.y;
	maDrawImageRegion(image, srcRect, &p, transformMode);
}

static void soft_notifyImageUpdated(MAHandle image) {
}

static void soft_beginRendering(void) {
}

static void soft_updateScreen(void) {
	maUpdateScreen();
}

static void soft_setClearColor(int r, int g, int b) {
}

static void soft_setColor(int r, int g, int b) {
	maSetColor(r<<16 | g<<8 | b);
}

static void soft_setAlpha(int a) {
}
