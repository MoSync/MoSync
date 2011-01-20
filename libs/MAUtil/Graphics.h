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

/** \file Graphics.h
 * \brief Translation/clipping stack for MoSync graphics.
 *
 * This file contains a set of graphics functions that closely mirror the ones available as MoSync syscalls. However, these
 * functions make use of a transformation stack similar to ones commonly found in other graphics libraries. It simplifies
 * managing transformations, removing the need to manually keep track of the current transformation state.
 * Currently, only translation is supported.
 *
 * \author Patrick Broman
 *
 */

#define _NO_OLDNAMES	//avoid conflicts with y1()
#include <ma.h>
 
#ifndef _SE_MSAB_MAUTIL_GRAPHICS_H_
#define _SE_MSAB_MAUTIL_GRAPHICS_H_

#include "GraphicsOpenGL.h"
#include "GraphicsSoftware.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef int MAFixed; // 16.16 fixed point

typedef void (*SetupFunc)(int x, int y, int w, int h);
typedef void (*SetClipRectFunc)(int x, int y, int w, int h);
typedef void (*ClearMatrixFunc)(void);
typedef void (*PushMatrixFunc)(void);
typedef void (*PopMatrixFunc)(void);
typedef void (*TranslateFunc)(int x, int y);
typedef MAPoint2d (*GetTranslationFunc)(void);
typedef void (*ScaleFunc)(MAFixed x, MAFixed y);
typedef void (*PlotFunc)(int x, int y);
typedef void (*LineFunc)(int x1, int y1, int x2, int y2);
typedef void (*FillRectFunc)(int left, int top, int width, int height);
typedef void (*DrawTextFunc)(int left, int top, const char* text);
typedef void (*DrawTextWFunc)(int left, int top, const wchar_t* text);
typedef void (*DrawImageFunc)(MAHandle image, int left, int top);
typedef void (*DrawRGBFunc)(const MAPoint2d *dstPoint, const void *src, const MARect *srcRect, int scanlength);
typedef void (*DrawImageRegionFunc)(MAHandle image, const MARect *srcRect, const MAPoint2d *dstPoint, int transformMode);
typedef void (*NotifyImageUpdated)(MAHandle image);
typedef void (*BeginRendering)(void);
typedef void (*UpdateScreen)(void);
typedef void (*SetClearColor)(int r, int g, int b);
typedef void (*SetColor)(int r, int g, int b);
typedef void (*SetAlpha)(int a);


typedef struct MAGraphicsDriver_t {
	SetupFunc setup;
	SetClipRectFunc setClipRect;
	ClearMatrixFunc clearMatrix;
	PushMatrixFunc pushMatrix;
	PopMatrixFunc popMatrix;
	TranslateFunc translate;
	GetTranslationFunc getTranslation;
	ScaleFunc scale;
	PlotFunc plot;
	LineFunc line;
	FillRectFunc fillRect;
	DrawTextFunc drawText;
	DrawTextWFunc drawTextW;
	DrawImageFunc drawImage;
	DrawRGBFunc drawRGB;
	DrawImageRegionFunc drawImageRegion;
	NotifyImageUpdated notifyImageUpdated; // not very pretty (for opengl so that it knows that it has to update the texture again)
	BeginRendering beginRendering;
	UpdateScreen updateScreen;
	SetClearColor setClearColor;	
	SetColor setColor;
	SetAlpha setAlpha;
} MAGraphicsDriver;


void Gfx_useDriver(MAGraphicsDriver* driver);

void Gfx_setup(int x, int y, int w, int h);

/** 
  * Clears the clip rect stack.
  **/
void Gfx_clearClipRect(void);

/** Sets the clip rect to the content of the top of the stack without changing the stack.  
    *  Returns true if the area of the restored clip rect is > 0, otherwise false.
    **/
BOOL Gfx_restoreClipRect(void);

/** Pushes the specified clip rect on the stack and sets it as the current.
   *  Returns true if the area of the clip rect is > 0, otherwise false. 
   **/
BOOL Gfx_pushClipRect(int left, int top, int width, int height);

/** Computes the intersection rectangle of the specified clip rect and the current, pushing the result on the stack.
   *  Returns true if the area of the resulting clip rect is > 0, otherwise false. 
   **/
BOOL Gfx_intersectClipRect(int left, int top, int width, int height);

/**
   * Pops a clip rect off the stack and sets is as the current. 
   * Returns true if the area of the resulting clip rect is > 0,  otherwise false. 
   **/
BOOL Gfx_popClipRect(void);

/** 
  * Clears the transform stack.
  **/
void Gfx_clearMatrix(void);
/** Pushes the current transform on the stack **/
void Gfx_pushMatrix(void);
/** Pops the previous transform off the stack **/
void Gfx_popMatrix(void);
/** Translates the current transform by x,y **/
void Gfx_translate(int x, int y);
MAPoint2d Gfx_getTranslation(void);

void Gfx_scale(MAFixed x, MAFixed y);

/**
* Combines clipping and translation.
* \see Gfx_intersectClipRect()
*/
BOOL Gfx_pushRect(int x, int y, int width, int height);

/**
* \see Gfx_popClipRect()
*/
BOOL Gfx_popRect(void);

/** Plots a pixel with the current color at x, y with respect to the current transform **/
void Gfx_plot(int x, int y);
/** Draws a line with the current color from x1,  y1 to x2, y2 with respect to the current transform **/
void Gfx_line(int x1, int y1, int x2, int y2);
/** Draws a line with the current color from x1,  y1 to x2, y2 with respect to the current transform **/
void Gfx_fillRect(int left, int top, int width, int height);
void Gfx_drawText(int left, int top, const char* text);
void Gfx_drawTextW(int left, int top, const wchar_t* text);
void Gfx_drawImage(MAHandle image, int left, int top);
void Gfx_drawRGB(const MAPoint2d *dstPoint, const void *src, const MARect *srcRect, int scanlength);
void Gfx_drawImageRegion(MAHandle image, const MARect *srcRect, const MAPoint2d *dstPoint, int transformMode);

void Gfx_notifyImageUpdated(MAHandle image);

// when in opengl mode, clears the depth and color buffer.
// software do nothing.
void Gfx_beginRendering(void);
void Gfx_updateScreen(void);
void Gfx_setClearColor(int r, int g, int b);
void Gfx_setColor(int r, int g, int b);
void Gfx_setAlpha(int a);

#ifdef __cplusplus
}
#endif

#endif

