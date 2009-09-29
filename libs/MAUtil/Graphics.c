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

#define MA_CLIP_STACK_DEPTH 128
#define MA_TRANSFORM_STACK_DEPTH 128

static MARect clipStack[MA_CLIP_STACK_DEPTH];
static int clipStackPtr = -1;
static MAPoint2d transformStack[MA_TRANSFORM_STACK_DEPTH];
static int transformStackPtr = -1;

static MAPoint2d currentOffset = {0, 0};

#define PUSH_EMPTY_CLIPRECT Gfx_pushClipRect(0,0,0,0)
#define false 0
#define true 1

static void _Gfx_init() {
	if(clipStackPtr < 0)
		Gfx_clearClipRect();
	if(transformStackPtr < 0)
		Gfx_clearMatrix();
}

/**
 * Clears the clip rect stack.
**/
void Gfx_clearClipRect() {
	MAExtent s = maGetScrSize();
	clipStackPtr = 0;
	maSetClipRect(0,0,EXTENT_X(s),EXTENT_Y(s));
	clipStack[0].left = 0;
	clipStack[0].top = 0;
	clipStack[0].width = EXTENT_X(s);
	clipStack[0].height = EXTENT_Y(s);
}

/** Sets the clip rect to the content of the top of the stack without changing the stack.  
 *  Returns true if the area of the restored clip rect is > 0, otherwise false.
**/
BOOL Gfx_restoreClipRect() {
	_Gfx_init();
	maSetClipRect(
		clipStack[clipStackPtr].left, 
		clipStack[clipStackPtr].top, 
		clipStack[clipStackPtr].width, 
		clipStack[clipStackPtr].height 
		);
	if(clipStack[clipStackPtr].width>0 && clipStack[clipStackPtr].height>0) return TRUE;
	else return FALSE;
}

/** Pushes the specified clip rect on the stack and sets it as the current.
 *  Returns true if the area of the clip rect is > 0, otherwise false. 
**/
BOOL Gfx_pushClipRect(int left, int top, int width, int height) {
	_Gfx_init();
	if(clipStackPtr >= MA_CLIP_STACK_DEPTH-1) {
		PANIC_MESSAGE("Clip stack overflow");
		return FALSE;
	}
	left+=currentOffset.x; top+=currentOffset.y;
	maSetClipRect(left, top, width, height);
	
	clipStackPtr++;
	clipStack[clipStackPtr].left   = left; 
	clipStack[clipStackPtr].top    = top; 
	clipStack[clipStackPtr].width  = width; 
	clipStack[clipStackPtr].height = height; 		
	if(clipStack[clipStackPtr].width>0 && clipStack[clipStackPtr].height>0) return TRUE;
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
	_Gfx_init();
	pLeft = clipStack[clipStackPtr].left;
	pTop = clipStack[clipStackPtr].top;
	pWidth = clipStack[clipStackPtr].width;
	pHeight = clipStack[clipStackPtr].height;
	
	left+=currentOffset.x; top+=currentOffset.y;

	if(clipStackPtr >= MA_CLIP_STACK_DEPTH-1) {
		PANIC_MESSAGE("Clip stack overflow");
		return FALSE;
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

	maSetClipRect(left, top, width, height);
	
	clipStackPtr++;
	clipStack[clipStackPtr].left   = left; 
	clipStack[clipStackPtr].top    = top; 
	clipStack[clipStackPtr].width  = width; 
	clipStack[clipStackPtr].height = height; 		
	
	if(clipStack[clipStackPtr].width>0 && clipStack[clipStackPtr].height>0) return TRUE;
	else return FALSE;

}

/**
   * Pops a clip rect off the stack and sets is as the current. 
   * Returns true if the area of the resulting clip rect is > 0,  otherwise false. 
   **/
BOOL Gfx_popClipRect() {
	_Gfx_init();
	clipStackPtr--;
	if(clipStackPtr <= -1) {
		clipStackPtr = -1;
		return FALSE;
	} else maSetClipRect(	
		clipStack[clipStackPtr].left, 
		clipStack[clipStackPtr].top, 
		clipStack[clipStackPtr].width, 
		clipStack[clipStackPtr].height);

	if(clipStack[clipStackPtr].width>0 && clipStack[clipStackPtr].height>0) return TRUE;
	else return FALSE;
}

/** 
  * Clears the transform stack.
  **/
void Gfx_clearMatrix() {
	transformStackPtr = 0;
	currentOffset.x = 0;
	currentOffset.y = 0;
	transformStack[0].x = 0;
	transformStack[0].y = 0;
}

/** Pushes the current transform on the stack **/
void Gfx_pushMatrix() {
	_Gfx_init();
	if(transformStackPtr >= MA_TRANSFORM_STACK_DEPTH-1) {
		PANIC_MESSAGE("Transform stack overflow");
		return;
	}

	//maSetClipRect(left, top, width, height);
	transformStackPtr++;
	transformStack[transformStackPtr] = currentOffset;
}
/** Pops the previous transform off the stack **/
void Gfx_popMatrix() {
	_Gfx_init();
	if(transformStackPtr < 0) {
		PANIC_MESSAGE("Transform stack underflow");
		return;
	}
	transformStackPtr--;
	currentOffset = transformStack[transformStackPtr];
}
/** Translates the current transform by x,y **/
void Gfx_translate(int x, int y) {
	currentOffset.x += x;
	currentOffset.y += y;
}

MAPoint2d Gfx_getTranslation() {
	return currentOffset;
}


BOOL Gfx_pushRect(int x, int y, int width, int height) {
	BOOL res = Gfx_intersectClipRect(x, y, width, height);
	Gfx_pushMatrix();
	Gfx_translate(x, y);
	return res;
}

BOOL Gfx_popRect() {
	Gfx_popMatrix();
	return Gfx_popClipRect();
}


/** Plots a pixel with the current color at x, y with respect to the current transform **/
void Gfx_plot(int x, int y) {
	maPlot(currentOffset.x + x, currentOffset.y + y);
}
/** Draws a line with the current color from x1,  y1 to x2, y2 with respect to the current transform **/
void Gfx_line(int x1, int y1, int x2, int y2) {
	maLine(currentOffset.x + x1, currentOffset.y + y1, currentOffset.x + x2, currentOffset.y + y2);
}
/** Draws a line with the current color from x1,  y1 to x2, y2 with respect to the current transform **/
void Gfx_fillRect(int left, int top, int width, int height) {
	maFillRect(
		currentOffset.x + left,
		currentOffset.y + top,
		width,
		height
	);
}

void Gfx_drawText(int left, int top, const char* text) {
	maDrawText(currentOffset.x + left, currentOffset.y + top, text);
}

void Gfx_drawImage(MAHandle image, int left, int top) {
	maDrawImage(image, currentOffset.x + left, currentOffset.y + top);
}

void Gfx_drawRGB(const MAPoint2d *dstPoint, const void *src, const MARect *srcRect, int scanlength) {
	MAPoint2d p = *dstPoint;
	p.x += currentOffset.x;
	p.y += currentOffset.y;
	
	maDrawRGB(&p, src, srcRect, scanlength);
}

void Gfx_drawImageRegion(MAHandle image, const MARect *srcRect, const MAPoint2d *dstPoint, int transformMode) {
	MAPoint2d p = *dstPoint;
	p.x += currentOffset.x;
	p.y += currentOffset.y;
	maDrawImageRegion(image, srcRect, &p, transformMode);
}
