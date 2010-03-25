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

#ifndef _FRAMEBUFFER_H_
#define _FRAMEBUFFER_H_

#include <ma.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
	FrameBuffer API for using with palletized surfaces. Common usage area: porting legacy games that used 4/8-bit palletized surfaces.
	Handles pixel format convertion (from 4/8 bpp to native), scaling (half/direct/double) and orientation change (90-degrees).
 */

#define FLAG_4BPP 1
#define FLAG_RGB666 2

#define ORIENTATION_0 0
#define ORIENTATION_90 1
#define ORIENTATION_180 2
#define ORIENTATION_270 3

/**
 * Initializes the FrameBuffer system. This system sets up the native MoSync framebuffer, so make sure that
 * it isn't in use while using this system.
 * \param w The width in pixels of the framebuffer.
 * \param h The height in pixels of the framebuffer.
 * \param orientation The orientation of the the screen.
 * \param flags Flags for setting bits per pixel.
 */
void FrameBuffer_init(int w, int h, int orientation, int flags);

/**
 * Sets the orientation of the framebuffer.
 * \param orientation The new orientation of the framebuffer.
 */
void FrameBuffer_setOrientation(int orientation);

/**
 * Gets the orientation of the framebuffer.
 * \return The current orientation.
 */
int FrameBuffer_getOrientation(void);

/**
 * Closes the framebuffer.
 */
void FrameBuffer_close(void);

/**
 * Sets a part of the palette.
 * \param startDst Where to start in the destination palette (offset in color elements).
 * \param startSrc Where to start in the src palette (offset in bytes);
 * \param num How many colors should be set.
 * \param src The source palette on the format, in bytes, RGBRGB... (note that if FLAG_RGB666 is set then each color component is only 6 bits).
 * \param flags If FLAG_RGB666 then 6 bits will be used per component.
 */
void FrameBuffer_setPalette(int startDst, int startSrc, int num, const byte *src, int flags);

/**
 * Sets a single palette entry.
 * \param i The index of the color to be set.
 * \param r The red color component.
 * \param g The green color component.
 * \param b The blue color component.
  * \param flags If FLAG_RGB666 then 6 bits will be used per component.
 */
void FrameBuffer_setPaletteEntry(int i, int r, int g, int b, int flags);

/**
 * Gets a single palette entry.
 * \param i The index of the color to be recieved.
 * \param dst A pointer to the destination memory. (3 bytes)
 * \param flags If FLAG_RGB666 then 6 bits will be used per component.
 */
void FrameBuffer_getPaletteEntry(int i, byte *dst, int flags);


/**
 * Blits a part of the the framebuffer to the screen.
 * \param x The topleft x-coordinate of the rectangle to be copied to screen.
 * \param y The topleft y-coordinate of the rectangle to be copied to screen.
 * \param w The width of the rectangle to be copied to screen.
 * \param h The height of the rectangle to be copied to screen.
 * \param dstx The topleft x-coordinate of the destination (in the same coordinate system as x and y).
 * \param dsty The topleft y-coordinate of the destination (in the same coordinate system as x and y).
 * \param buf Pointer to the source framebuffer.
 * \param pitch The pitch of the source framebuffer.
 */
void FrameBuffer_copyRect(short x, short y, short w, short h, int dstx, int dsty, const byte *buf, int pitch);


/**
 * Function to rotate the arrow keys depending on the orientation.
 * \param mak The mak keycode of the arrow key.
 */
int FrameBuffer_getArrowKeyForOrientation(int mak);
int FrameBuffer_getArrowKeyForOrientationInitial(int mak, int initial);

#ifdef __cplusplus
}
#endif

#endif
