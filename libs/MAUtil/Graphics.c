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

#include "Graphics.h"
#include <maassert.h>

#include "GraphicsOpenGL.h"
#include "GraphicsSoftware.h"


#define MA_CLIP_STACK_DEPTH 128
#define MA_TRANSFORM_STACK_DEPTH 128

static MARect sClipStack[MA_CLIP_STACK_DEPTH];
static int sClipStackPtr = -1;
//static MAPoint2d sTransformStack[MA_TRANSFORM_STACK_DEPTH];
//static int sTransformStackPtr = -1;
//static MAPoint2d sCurrentOffset = {0, 0};

#define PUSH_EMPTY_CLIPRECT Gfx_pushClipRect(0,0,0,0)
#define false 0
#define true 1

void dummy_setup(int x, int y, int w, int h);
void dummy_setClipRect(int x, int y, int w, int h);
void dummy_clearMatrix(void);
void dummy_pushMatrix(void);
void dummy_popMatrix(void);
void dummy_translate(int x, int y);
MAPoint2d dummy_getTranslation(void);
void dummy_plot(int x, int y);
void dummy_line(int x1, int y1, int x2, int y2);
void dummy_fillRect(int left, int top, int width, int height);
void dummy_drawText(int left, int top, const char* text);
void dummy_drawTextW(int left, int top, const wchar_t* text);
void dummy_drawImage(MAHandle image, int left, int top);
void dummy_drawRGB(const MAPoint2d *dstPoint, const void *src, const MARect *srcRect, int scanlength);
void dummy_drawImageRegion(MAHandle image, const MARect *srcRect, const MAPoint2d *dstPoint, int transformMode);
void dummy_notifyImageUpdated(MAHandle image);

static void _Gfx_init(void);

static MAGraphicsDriver sDummy = {
	&dummy_setup,
	&dummy_setClipRect,
	&dummy_clearMatrix,
	&dummy_pushMatrix,
	&dummy_popMatrix,
	&dummy_translate,
	&dummy_getTranslation,
	&dummy_plot,
	&dummy_line,
	&dummy_fillRect,
	&dummy_drawText,
	&dummy_drawTextW,
	&dummy_drawImage,
	&dummy_drawRGB,
	&dummy_drawImageRegion,
	&dummy_notifyImageUpdated,	
};

static MAGraphicsDriver* graphicsDriver = &sDummy;
static MAGraphicsDriverType sDefaultDriverType = MA_GRAPHICS_DRIVER_SOFTWARE;


void dummy_setup(int x, int y, int w, int h) {
	Gfx_useDriver(sDefaultDriverType);
	graphicsDriver->setup(x, y, w, h);
}

void dummy_setClipRect(int x, int y, int w, int h)  {
	Gfx_useDriver(sDefaultDriverType);
	graphicsDriver->setClipRect(x, y, w, h);
}

void dummy_clearMatrix(void)  {
	Gfx_useDriver(sDefaultDriverType);
	graphicsDriver->clearMatrix();
}

void dummy_pushMatrix(void)  {
	Gfx_useDriver(sDefaultDriverType);
	graphicsDriver->pushMatrix();
}

void dummy_popMatrix(void)  {
	Gfx_useDriver(sDefaultDriverType);
	graphicsDriver->popMatrix();
}

void dummy_translate(int x, int y)  {
	Gfx_useDriver(sDefaultDriverType);
	graphicsDriver->translate(x, y);
}

MAPoint2d dummy_getTranslation(void)  {
	Gfx_useDriver(sDefaultDriverType);
	return graphicsDriver->getTranslation();
}

void dummy_plot(int x, int y)  {
	Gfx_useDriver(sDefaultDriverType);
	graphicsDriver->plot(x, y);
}

void dummy_line(int x1, int y1, int x2, int y2)  {
	Gfx_useDriver(sDefaultDriverType);
	graphicsDriver->line(x1, y1, x2, y1);
}

void dummy_fillRect(int left, int top, int width, int height)  {
	Gfx_useDriver(sDefaultDriverType);
	graphicsDriver->fillRect(left, top, width, height);
}

void dummy_drawText(int left, int top, const char* text)  {
	Gfx_useDriver(sDefaultDriverType);
	graphicsDriver->drawText(left, top, text);
}

void dummy_drawTextW(int left, int top, const wchar_t* text)  {
	Gfx_useDriver(sDefaultDriverType);
	graphicsDriver->drawTextW(left, top, text);
}

void dummy_drawImage(MAHandle image, int left, int top)  {
	Gfx_useDriver(sDefaultDriverType);
	graphicsDriver->drawImage(image, left, top);
}

void dummy_drawRGB(const MAPoint2d *dstPoint, const void *src, const MARect *srcRect, int scanlength)  {
	Gfx_useDriver(sDefaultDriverType);
	graphicsDriver->drawRGB(dstPoint, src, srcRect, scanlength);
}

void dummy_drawImageRegion(MAHandle image, const MARect *srcRect, const MAPoint2d *dstPoint, int transformMode)  {
	Gfx_useDriver(sDefaultDriverType);
	graphicsDriver->drawImageRegion(image, srcRect, dstPoint, transformMode);
}

void dummy_notifyImageUpdated(MAHandle image)  {
	Gfx_useDriver(sDefaultDriverType);
	graphicsDriver->notifyImageUpdated(image);
}

void Gfx_useDriver(MAGraphicsDriverType driver) {
	_Gfx_init();

	if(driver == MA_GRAPHICS_DRIVER_OPENGL) {
		graphicsDriver = Gfx_getDriverOpenGL();	
	}
	else if(driver == MA_GRAPHICS_DRIVER_SOFTWARE) {
		graphicsDriver = Gfx_getDriverSoftware();		
	}
}

void Gfx_setup(MAGraphicsDriverType driver, int x, int y, int w, int h) {
	Gfx_useDriver(driver);	
	graphicsDriver->setup(x, y, w, h);

	Gfx_clearMatrix();
	
	sClipStackPtr = 0;
	graphicsDriver->setClipRect(x, y, w, h);
	
	sClipStack[0].left = x;
	sClipStack[0].top = y;
	sClipStack[0].width = w;
	sClipStack[0].height = h;
}

static void _Gfx_init(void) {

	//if(!graphicsDriver)
	//	Gfx_useDriver(MA_GRAPHICS_DRIVER_SOFTWARE);

	if(sClipStackPtr < 0)
		Gfx_clearClipRect();
	//if(sTransformStackPtr < 0)
	//	Gfx_clearMatrix();
}

/**
 * Clears the clip rect stack.
**/
void Gfx_clearClipRect(void) {
	MAExtent s = maGetScrSize();
	sClipStackPtr = 0;
	//maSetClipRect(0,0,EXTENT_X(s),EXTENT_Y(s));
	graphicsDriver->setClipRect(0,0,EXTENT_X(s),EXTENT_Y(s));
	
	sClipStack[0].left = 0;
	sClipStack[0].top = 0;
	sClipStack[0].width = EXTENT_X(s);
	sClipStack[0].height = EXTENT_Y(s);
}

/** Sets the clip rect to the content of the top of the stack without changing the stack.  
 *  Returns true if the area of the restored clip rect is > 0, otherwise false.
**/
BOOL Gfx_restoreClipRect(void) {
	_Gfx_init();
	//maSetClipRect(
	graphicsDriver->setClipRect(
		sClipStack[sClipStackPtr].left, 
		sClipStack[sClipStackPtr].top, 
		sClipStack[sClipStackPtr].width, 
		sClipStack[sClipStackPtr].height 
		);
	if(sClipStack[sClipStackPtr].width>0 && sClipStack[sClipStackPtr].height>0) return TRUE;
	else return FALSE;
}

/** Pushes the specified clip rect on the stack and sets it as the current.
 *  Returns true if the area of the clip rect is > 0, otherwise false. 
**/
BOOL Gfx_pushClipRect(int left, int top, int width, int height) {
	MAPoint2d currentTranslation;
		
	_Gfx_init();
	if(sClipStackPtr >= MA_CLIP_STACK_DEPTH-1) {
		PANIC_MESSAGE("Clip stack overflow");
	}
	
	//left+=sCurrentOffset.x; top+=sCurrentOffset.y;
	currentTranslation = Gfx_getTranslation();
	left+=currentTranslation.x;
	top+=currentTranslation.y;
	
	//maSetClipRect(left, top, width, height);
	graphicsDriver->setClipRect(left, top, width, height);
		
	sClipStackPtr++;
	sClipStack[sClipStackPtr].left   = left; 
	sClipStack[sClipStackPtr].top    = top; 
	sClipStack[sClipStackPtr].width  = width; 
	sClipStack[sClipStackPtr].height = height; 		
	if(sClipStack[sClipStackPtr].width>0 && sClipStack[sClipStackPtr].height>0) return TRUE;
	else return FALSE;
}

/** Computes the intersection rectangle of the specified clip rect and the current, pushing the result on the stack.
   *  Returns true if the area of the resulting clip rect is > 0, otherwise false. 
   **/
BOOL Gfx_intersectClipRect(int left, int top, int width, int height) {
	int pLeft;
	int pTop;
	int pWidth;
	int pHeight;
	MAPoint2d currentTranslation;	
	
	_Gfx_init();
	pLeft = sClipStack[sClipStackPtr].left;
	pTop = sClipStack[sClipStackPtr].top;
	pWidth = sClipStack[sClipStackPtr].width;
	pHeight = sClipStack[sClipStackPtr].height;
	
	
	currentTranslation = Gfx_getTranslation();
	left+=currentTranslation.x; top+=currentTranslation.y;

	if(sClipStackPtr >= MA_CLIP_STACK_DEPTH-1) {
		PANIC_MESSAGE("Clip stack overflow");
	}

	if((!pWidth) || (!pHeight)) {
		PUSH_EMPTY_CLIPRECT;
		return 0;
	}

	if(left + width < pLeft)
	{
		PUSH_EMPTY_CLIPRECT;
		return false;
	}

	if(top + height < pTop)
	{
		PUSH_EMPTY_CLIPRECT;
		return false;
	}

	if(left > pLeft + pWidth)
	{
		PUSH_EMPTY_CLIPRECT;
		return false;
	}

	if(top > pTop + pHeight)
	{
		PUSH_EMPTY_CLIPRECT;
		return false;
	}
	
	if(left < pLeft)
	{
		width -= pLeft - left;
		left =  pLeft;
	}

	if(top < pTop)
	{
		height -= pTop - top;
		top =  pTop;
	}

	if(left + width > pLeft + pWidth)
	{
		width -= (left + width) - (pLeft + pWidth);
	}

	if(top + height > pTop + pHeight)
	{
		height -= (top + height) - (pTop + pHeight);
	}

//	maSetClipRect(left, top, width, height);
	graphicsDriver->setClipRect(left, top, width, height);
	
	sClipStackPtr++;
	sClipStack[sClipStackPtr].left   = left; 
	sClipStack[sClipStackPtr].top    = top; 
	sClipStack[sClipStackPtr].width  = width; 
	sClipStack[sClipStackPtr].height = height; 		
	
	if(sClipStack[sClipStackPtr].width>0 && sClipStack[sClipStackPtr].height>0) return TRUE;
	else return FALSE;

}

/**
   * Pops a clip rect off the stack and sets is as the current. 
   * Returns true if the area of the resulting clip rect is > 0,  otherwise false. 
   **/
BOOL Gfx_popClipRect(void) {
	_Gfx_init();
	sClipStackPtr--;
	if(sClipStackPtr <= -1) {
		sClipStackPtr = -1;
		return FALSE;
	} else //maSetClipRect(
		graphicsDriver->setClipRect(
		sClipStack[sClipStackPtr].left, 
		sClipStack[sClipStackPtr].top, 
		sClipStack[sClipStackPtr].width, 
		sClipStack[sClipStackPtr].height);

	if(sClipStack[sClipStackPtr].width>0 && sClipStack[sClipStackPtr].height>0) return TRUE;
	else return FALSE;
}

/** 
  * Clears the transform stack.
  **/
void Gfx_clearMatrix(void) {
	/*
	sTransformStackPtr = 0;
	sCurrentOffset.x = 0;
	sCurrentOffset.y = 0;
	sTransformStack[0].x = 0;
	sTransformStack[0].y = 0;
	*/
	
	graphicsDriver->clearMatrix();
	
}

/** Pushes the current transform on the stack **/
void Gfx_pushMatrix(void) {
	/*
	_Gfx_init();
	if(sTransformStackPtr >= MA_TRANSFORM_STACK_DEPTH-1) {
		PANIC_MESSAGE("Transform stack overflow");
		return;
	}

	//maSetClipRect(left, top, width, height);
	sTransformStackPtr++;
	sTransformStack[sTransformStackPtr] = sCurrentOffset;
	*/
	graphicsDriver->pushMatrix();

}
/** Pops the previous transform off the stack **/
void Gfx_popMatrix(void) {
	/*
	_Gfx_init();
	if(sTransformStackPtr < 0) {
		PANIC_MESSAGE("Transform stack underflow");
		return;
	}
	sCurrentOffset = sTransformStack[sTransformStackPtr];
	sTransformStackPtr--;
	*/
	graphicsDriver->popMatrix();
}
/** Translates the current transform by x,y **/
void Gfx_translate(int x, int y) {
	/*
	sCurrentOffset.x += x;
	sCurrentOffset.y += y;
	*/
	graphicsDriver->translate(x, y);
}

MAPoint2d Gfx_getTranslation(void) {
	return graphicsDriver->getTranslation(); //sCurrentOffset;
}


BOOL Gfx_pushRect(int x, int y, int width, int height) {
	BOOL res = Gfx_intersectClipRect(x, y, width, height);
	Gfx_pushMatrix();
	Gfx_translate(x, y);
	return res;
}

BOOL Gfx_popRect(void) {
	Gfx_popMatrix();
	return Gfx_popClipRect();
}


/** Plots a pixel with the current color at x, y with respect to the current transform **/
void Gfx_plot(int x, int y) {
	//maPlot(sCurrentOffset.x + x, sCurrentOffset.y + y);
	graphicsDriver->plot(x, y);
	
}
/** Draws a line with the current color from x1,  y1 to x2, y2 with respect to the current transform **/
void Gfx_line(int x1, int y1, int x2, int y2) {
	//maLine(sCurrentOffset.x + x1, sCurrentOffset.y + y1, sCurrentOffset.x + x2, sCurrentOffset.y + y2);
	graphicsDriver->line(x1, y1, x2, y2);
}
/** Draws a line with the current color from x1,  y1 to x2, y2 with respect to the current transform **/
void Gfx_fillRect(int left, int top, int width, int height) {
	/*
	maFillRect(
		sCurrentOffset.x + left,
		sCurrentOffset.y + top,
		width,
		height
	);
	*/
	
	graphicsDriver->fillRect(left, top, width, height);
}

void Gfx_drawText(int left, int top, const char* text) {
	//maDrawText(sCurrentOffset.x + left, sCurrentOffset.y + top, text);
	
	graphicsDriver->drawText(left, top, text);
}

void Gfx_drawTextW(int left, int top, const wchar_t* text) {
	//maDrawTextW(sCurrentOffset.x + left, sCurrentOffset.y + top, text);
	
	graphicsDriver->drawTextW(left, top, text);
}

void Gfx_drawImage(MAHandle image, int left, int top) {
	//maDrawImage(image, sCurrentOffset.x + left, sCurrentOffset.y + top);

	// probably do early out rejection before...	
	graphicsDriver->drawImage(image, left, top);
}

void Gfx_drawRGB(const MAPoint2d *dstPoint, const void *src, const MARect *srcRect, int scanlength) {
	//MAPoint2d p = *dstPoint;
	//p.x += sCurrentOffset.x;
	//p.y += sCurrentOffset.y;
//	maDrawRGB(&p, src, srcRect, scanlength);
	graphicsDriver->drawRGB(dstPoint, src, srcRect, scanlength);

}

void Gfx_drawImageRegion(MAHandle image, const MARect *srcRect, const MAPoint2d *dstPoint, int transformMode) {
	/*
	MAPoint2d p = *dstPoint;
	p.x += sCurrentOffset.x;
	p.y += sCurrentOffset.y;
	maDrawImageRegion(image, srcRect, &p, transformMode);
	
#ifdef GRAPHICS_DEBUGGING
	sNumCalls++;
#endif
*/
	graphicsDriver->drawImageRegion(image, srcRect, dstPoint, transformMode);
}

void Gfx_notifyImageUpdated(MAHandle image) {
	graphicsDriver->notifyImageUpdated(image);
}
