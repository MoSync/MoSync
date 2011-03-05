/*
	pixdemo - demo of put/get pixel for SDL.
	Copyright (C) 1999, 2000 Neil McGill

	This game is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2 of the License, or (at your option) any later version.

	This game is distributed in the hope that it will be fun,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this game; if not, write to the Free
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

	Neil McGill

	$Id: SDL_Pixel.h,v 1.1.1.1 2001/04/25 17:08:31 moorman Exp $
*/

#ifndef _SDL_LocPixel_GFX_H_
#define _SDL_LocPixel_GFX_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Public interface. Clips to surface and performs locking. Slow.
 */
int SDL_PutPixel(SDL_Surface *dst, Sint16 x, Sint16 y, Uint32 rgb);

/*
 * Semi public interface. No clipping. Assumed locked surface.
 * Faster, but needs to look upp bpp depth and surface width.
 * To avoid lookups, use the cache routines retn'd from the first invocation.
 * (see following functions)
 */
void SDL_LowerPutPixel(SDL_Surface *dst, Sint16 x, Sint16 y, Uint32 rgb);

/*
 * Public interface. Clips to surface and performs locking. Slow.
 */
int SDL_GetPixel(SDL_Surface *dst, Sint16 x, Sint16 y, Uint32 * rgb);

/*
 * Semi public interface. No clipping. Assumed locked surface.
 * Faster, but needs to look upp bpp depth and surface width.
 * To avoid lookups, use the cache routines retn'd from the first invocation.
 * (see following functions)
 */
void SDL_LowerGetPixel(SDL_Surface *dst, Sint16 x, Sint16 y, Uint32 * rgb);

/*
 * Quick row-pixel bit-shift lookups for certain common pixel screen widths:
 *
 * <<1 	2
 * <<2 	4
 * <<3 	8
 * <<4 	16
 * <<5 	32
 * <<6 	64
 * <<7 	128
 * <<8 	256
 * <<9 	512
 * <<10	1024
 */

#define SDL_Pixel_16w(y)			((y)<<4)
#define SDL_Pixel_24w(y)			((y)<<4)+((y)<<3)
#define SDL_Pixel_32w(y)			((y)<<5)
#define SDL_Pixel_48w(y)			((y)<<5)+((y)<<4)
#define SDL_Pixel_64w(y)			((y)<<6)
#define SDL_Pixel_128w(y)			((y)<<7)
#define SDL_Pixel_256w(y)			((y)<<8)
#define SDL_Pixel_320w(y)			(((y)<<8)+((y)<<6))
#define SDL_Pixel_512w(y)			((y)<<9)
#define SDL_Pixel_640w(y)			(((y)<<9)+((y)<<7))
#define SDL_Pixel_800w(y)			(((y)<<9)+((y)<<8)+((y)<<5))
#define SDL_Pixel_1024w(y)			(((y)<<10))
#define SDL_Pixel_ANYw(s,y)			((y)*(s->pitch/s->format->BytesPerPixel))

/*
 * 8 bpp macros:
 */
#define SDL_Pixel_16x8bpp(s,x,y)	((Uint8*)s->pixels+(x)+SDL_Pixel_16w(y))
#define SDL_Pixel_24x8bpp(s,x,y)	((Uint8*)s->pixels+(x)+SDL_Pixel_24w(y))
#define SDL_Pixel_32x8bpp(s,x,y)	((Uint8*)s->pixels+(x)+SDL_Pixel_32w(y))
#define SDL_Pixel_48x8bpp(s,x,y)	((Uint8*)s->pixels+(x)+SDL_Pixel_48w(y))
#define SDL_Pixel_64x8bpp(s,x,y)	((Uint8*)s->pixels+(x)+SDL_Pixel_64w(y))
#define SDL_Pixel_128x8bpp(s,x,y)	((Uint8*)s->pixels+(x)+SDL_Pixel_128w(y))
#define SDL_Pixel_256x8bpp(s,x,y)	((Uint8*)s->pixels+(x)+SDL_Pixel_256w(y))
#define SDL_Pixel_320x8bpp(s,x,y)	((Uint8*)s->pixels+(x)+SDL_Pixel_320w(y))
#define SDL_Pixel_640x8bpp(s,x,y)	((Uint8*)s->pixels+(x)+SDL_Pixel_640w(y))
#define SDL_Pixel_800x8bpp(s,x,y)	((Uint8*)s->pixels+(x)+SDL_Pixel_800w(y))
#define SDL_Pixel_1024x8bpp(s,x,y)	((Uint8*)s->pixels+(x)+SDL_Pixel_1024w(y))
#define SDL_Pixel_8bpp(s,x,y)		((Uint8*)s->pixels+(x)+SDL_Pixel_ANYw(s,y))

#define SDL_PutPixel_16x8bpp(s,x,y,rgb)		*SDL_Pixel_16x8bpp(s,x,y) = rgb
#define SDL_PutPixel_24x8bpp(s,x,y,rgb)		*SDL_Pixel_24x8bpp(s,x,y) = rgb
#define SDL_PutPixel_32x8bpp(s,x,y,rgb)		*SDL_Pixel_32x8bpp(s,x,y) = rgb
#define SDL_PutPixel_48x8bpp(s,x,y,rgb)		*SDL_Pixel_48x8bpp(s,x,y) = rgb
#define SDL_PutPixel_64x8bpp(s,x,y,rgb)		*SDL_Pixel_64x8bpp(s,x,y) = rgb
#define SDL_PutPixel_128x8bpp(s,x,y,rgb)	*SDL_Pixel_128x8bpp(s,x,y) = rgb
#define SDL_PutPixel_256x8bpp(s,x,y,rgb)	*SDL_Pixel_256x8bpp(s,x,y) = rgb
#define SDL_PutPixel_320x8bpp(s,x,y,rgb)	*SDL_Pixel_320x8bpp(s,x,y) = rgb
#define SDL_PutPixel_640x8bpp(s,x,y,rgb)	*SDL_Pixel_640x8bpp(s,x,y) = rgb
#define SDL_PutPixel_800x8bpp(s,x,y,rgb)	*SDL_Pixel_800x8bpp(s,x,y) = rgb
#define SDL_PutPixel_1024x8bpp(s,x,y,rgb)	*SDL_Pixel_1024x8bpp(s,x,y) = rgb
#define SDL_PutPixel_8bpp(s,x,y,rgb)		*SDL_Pixel_8bpp(s,x,y) = rgb

#define SDL_GetPixel_16x8bpp(s,x,y,rgb)		*rgb = *SDL_Pixel_16x8bpp(s,x,y)
#define SDL_GetPixel_24x8bpp(s,x,y,rgb)		*rgb = *SDL_Pixel_24x8bpp(s,x,y)
#define SDL_GetPixel_32x8bpp(s,x,y,rgb)		*rgb = *SDL_Pixel_32x8bpp(s,x,y)
#define SDL_GetPixel_48x8bpp(s,x,y,rgb)		*rgb = *SDL_Pixel_48x8bpp(s,x,y)
#define SDL_GetPixel_64x8bpp(s,x,y,rgb)		*rgb = *SDL_Pixel_64x8bpp(s,x,y)
#define SDL_GetPixel_128x8bpp(s,x,y,rgb)	*rgb = *SDL_Pixel_128x8bpp(s,x,y)
#define SDL_GetPixel_256x8bpp(s,x,y,rgb)	*rgb = *SDL_Pixel_256x8bpp(s,x,y)
#define SDL_GetPixel_320x8bpp(s,x,y,rgb)	*rgb = *SDL_Pixel_320x8bpp(s,x,y)
#define SDL_GetPixel_640x8bpp(s,x,y,rgb)	*rgb = *SDL_Pixel_640x8bpp(s,x,y)
#define SDL_GetPixel_800x8bpp(s,x,y,rgb)	*rgb = *SDL_Pixel_800x8bpp(s,x,y)
#define SDL_GetPixel_1024x8bpp(s,x,y,rgb)	*rgb = *SDL_Pixel_1024x8bpp(s,x,y)
#define SDL_GetPixel_8bpp(s,x,y,rgb)		*rgb = *SDL_Pixel_8bpp(s,x,y)

/*
 * 16 bpp macros:
 */
#define SDL_Pixel_16x16bpp(s,x,y)	((Uint16*)s->pixels+(x)+SDL_Pixel_16w(y))
#define SDL_Pixel_24x16bpp(s,x,y)	((Uint16*)s->pixels+(x)+SDL_Pixel_24w(y))
#define SDL_Pixel_32x16bpp(s,x,y)	((Uint16*)s->pixels+(x)+SDL_Pixel_32w(y))
#define SDL_Pixel_48x16bpp(s,x,y)	((Uint16*)s->pixels+(x)+SDL_Pixel_48w(y))
#define SDL_Pixel_64x16bpp(s,x,y)	((Uint16*)s->pixels+(x)+SDL_Pixel_64w(y))
#define SDL_Pixel_128x16bpp(s,x,y)	((Uint16*)s->pixels+(x)+SDL_Pixel_128w(y))
#define SDL_Pixel_256x16bpp(s,x,y)	((Uint16*)s->pixels+(x)+SDL_Pixel_256w(y))
#define SDL_Pixel_320x16bpp(s,x,y)	((Uint16*)s->pixels+(x)+SDL_Pixel_320w(y))
#define SDL_Pixel_640x16bpp(s,x,y)	((Uint16*)s->pixels+(x)+SDL_Pixel_640w(y))
#define SDL_Pixel_800x16bpp(s,x,y)	((Uint16*)s->pixels+(x)+SDL_Pixel_800w(y))
#define SDL_Pixel_1024x16bpp(s,x,y)	((Uint16*)s->pixels+(x)+SDL_Pixel_1024w(y))
#define SDL_Pixel_16bpp(s,x,y)		((Uint16*)s->pixels+(x)+SDL_Pixel_ANYw(s,y))

#define SDL_PutPixel_16x16bpp(s,x,y,rgb)	*SDL_Pixel_16x16bpp(s,x,y) = rgb
#define SDL_PutPixel_24x16bpp(s,x,y,rgb)	*SDL_Pixel_24x16bpp(s,x,y) = rgb
#define SDL_PutPixel_32x16bpp(s,x,y,rgb)	*SDL_Pixel_32x16bpp(s,x,y) = rgb
#define SDL_PutPixel_48x16bpp(s,x,y,rgb)	*SDL_Pixel_48x16bpp(s,x,y) = rgb
#define SDL_PutPixel_64x16bpp(s,x,y,rgb)	*SDL_Pixel_64x16bpp(s,x,y) = rgb
#define SDL_PutPixel_128x16bpp(s,x,y,rgb)	*SDL_Pixel_128x16bpp(s,x,y) = rgb
#define SDL_PutPixel_256x16bpp(s,x,y,rgb)	*SDL_Pixel_256x16bpp(s,x,y) = rgb
#define SDL_PutPixel_320x16bpp(s,x,y,rgb)	*SDL_Pixel_320x16bpp(s,x,y) = rgb
#define SDL_PutPixel_640x16bpp(s,x,y,rgb)	*SDL_Pixel_640x16bpp(s,x,y) = rgb
#define SDL_PutPixel_800x16bpp(s,x,y,rgb)	*SDL_Pixel_800x16bpp(s,x,y) = rgb
#define SDL_PutPixel_1024x16bpp(s,x,y,rgb)	*SDL_Pixel_1024x16bpp(s,x,y) = rgb
#define SDL_PutPixel_16bpp(s,x,y,rgb)		*SDL_Pixel_16bpp(s,x,y) = rgb

#define SDL_GetPixel_16x16bpp(s,x,y,rgb)	*rgb = *SDL_Pixel_16x16bpp(s,x,y)
#define SDL_GetPixel_24x16bpp(s,x,y,rgb)	*rgb = *SDL_Pixel_24x16bpp(s,x,y)
#define SDL_GetPixel_32x16bpp(s,x,y,rgb)	*rgb = *SDL_Pixel_32x16bpp(s,x,y)
#define SDL_GetPixel_48x16bpp(s,x,y,rgb)	*rgb = *SDL_Pixel_48x16bpp(s,x,y)
#define SDL_GetPixel_64x16bpp(s,x,y,rgb)	*rgb = *SDL_Pixel_64x16bpp(s,x,y)
#define SDL_GetPixel_128x16bpp(s,x,y,rgb)	*rgb = *SDL_Pixel_128x16bpp(s,x,y)
#define SDL_GetPixel_256x16bpp(s,x,y,rgb)	*rgb = *SDL_Pixel_256x16bpp(s,x,y)
#define SDL_GetPixel_320x16bpp(s,x,y,rgb)	*rgb = *SDL_Pixel_320x16bpp(s,x,y)
#define SDL_GetPixel_640x16bpp(s,x,y,rgb)	*rgb = *SDL_Pixel_640x16bpp(s,x,y)
#define SDL_GetPixel_800x16bpp(s,x,y,rgb)	*rgb = *SDL_Pixel_800x16bpp(s,x,y)
#define SDL_GetPixel_1024x16bpp(s,x,y,rgb)	*rgb = *SDL_Pixel_1024x16bpp(s,x,y)
#define SDL_GetPixel_16bpp(s,x,y,rgb)		*rgb = *SDL_Pixel_16bpp(s,x,y)

/*
 * 24 bpp (sucks for optimisations... Use 32 bpp if possible):
 */
#define SDL_Pixel_16x24bpp(s,x,y)\
		((Uint8*)s->pixels+(((x)+SDL_Pixel_16w(y))*3))
#define SDL_Pixel_24x24bpp(s,x,y)\
		((Uint8*)s->pixels+(((x)+SDL_Pixel_24w(y))*3))
#define SDL_Pixel_32x24bpp(s,x,y)\
		((Uint8*)s->pixels+(((x)+SDL_Pixel_32w(y))*3))
#define SDL_Pixel_48x24bpp(s,x,y)\
		((Uint8*)s->pixels+(((x)+SDL_Pixel_48w(y))*3))
#define SDL_Pixel_64x24bpp(s,x,y)\
		((Uint8*)s->pixels+(((x)+SDL_Pixel_64w(y))*3))
#define SDL_Pixel_128x24bpp(s,x,y)\
		((Uint8*)s->pixels+(((x)+SDL_Pixel_128w(y))*3))
#define SDL_Pixel_256x24bpp(s,x,y)\
		((Uint8*)s->pixels+(((x)+SDL_Pixel_256w(y))*3))
#define SDL_Pixel_320x24bpp(s,x,y)\
		((Uint8*)s->pixels+(((x)+SDL_Pixel_320w(y))*3))
#define SDL_Pixel_640x24bpp(s,x,y)\
		((Uint8*)s->pixels+(((x)+SDL_Pixel_640w(y))*3))
#define SDL_Pixel_800x24bpp(s,x,y)\
		((Uint8*)s->pixels+(((x)+SDL_Pixel_800w(y))*3))
#define SDL_Pixel_1024x24bpp(s,x,y)\
		((Uint8*)s->pixels+(((x)+SDL_Pixel_1024w(y))*3))
#define SDL_Pixel_24bpp(s,x,y)\
		((Uint8*)s->pixels+(((x)+SDL_Pixel_ANYw(s,y))*3))

#define SDL_PutPixel_16x24bpp(s,x,y,rgb)\
		memcpy(SDL_Pixel_16x24bpp(s,x,y), &rgb, 3)
#define SDL_PutPixel_24x24bpp(s,x,y,rgb)\
		memcpy(SDL_Pixel_24x24bpp(s,x,y), &rgb, 3)
#define SDL_PutPixel_32x24bpp(s,x,y,rgb)\
		memcpy(SDL_Pixel_32x24bpp(s,x,y), &rgb, 3)
#define SDL_PutPixel_48x24bpp(s,x,y,rgb)\
		memcpy(SDL_Pixel_48x24bpp(s,x,y), &rgb, 3)
#define SDL_PutPixel_64x24bpp(s,x,y,rgb)\
		memcpy(SDL_Pixel_64x24bpp(s,x,y), &rgb, 3)
#define SDL_PutPixel_128x24bpp(s,x,y,rgb)\
		memcpy(SDL_Pixel_128x24bpp(s,x,y), &rgb, 3)
#define SDL_PutPixel_256x24bpp(s,x,y,rgb)\
		memcpy(SDL_Pixel_256x24bpp(s,x,y), &rgb, 3)
#define SDL_PutPixel_320x24bpp(s,x,y,rgb)\
		memcpy(SDL_Pixel_320x24bpp(s,x,y), &rgb, 3)
#define SDL_PutPixel_640x24bpp(s,x,y,rgb)\
		memcpy(SDL_Pixel_640x24bpp(s,x,y), &rgb, 3)
#define SDL_PutPixel_800x24bpp(s,x,y,rgb)\
		memcpy(SDL_Pixel_800x24bpp(s,x,y), &rgb, 3)
#define SDL_PutPixel_1024x24bpp(s,x,y,rgb)\
		memcpy(SDL_Pixel_1024x24bpp(s,x,y), &rgb, 3)
#define SDL_PutPixel_24bpp(s,x,y,rgb)\
		memcpy(SDL_Pixel_24bpp(s,x,y), &rgb, 3)

#define SDL_GetPixel_16x24bpp(s,x,y,rgb)\
		memcpy(rgb, SDL_Pixel_16x24bpp(s,x,y), 3)
#define SDL_GetPixel_24x24bpp(s,x,y,rgb)\
		memcpy(rgb, SDL_Pixel_24x24bpp(s,x,y), 3)
#define SDL_GetPixel_32x24bpp(s,x,y,rgb)\
		memcpy(rgb, SDL_Pixel_32x24bpp(s,x,y), 3)
#define SDL_GetPixel_48x24bpp(s,x,y,rgb)\
		memcpy(rgb, SDL_Pixel_48x24bpp(s,x,y), 3)
#define SDL_GetPixel_64x24bpp(s,x,y,rgb)\
		memcpy(rgb, SDL_Pixel_64x24bpp(s,x,y), 3)
#define SDL_GetPixel_128x24bpp(s,x,y,rgb)\
		memcpy(rgb, SDL_Pixel_128x24bpp(s,x,y), 3)
#define SDL_GetPixel_256x24bpp(s,x,y,rgb)\
		memcpy(rgb, SDL_Pixel_256x24bpp(s,x,y), 3)
#define SDL_GetPixel_320x24bpp(s,x,y,rgb)\
		memcpy(rgb, SDL_Pixel_320x24bpp(s,x,y), 3)
#define SDL_GetPixel_640x24bpp(s,x,y,rgb)\
		memcpy(rgb, SDL_Pixel_640x24bpp(s,x,y), 3)
#define SDL_GetPixel_800x24bpp(s,x,y,rgb)\
		memcpy(rgb, SDL_Pixel_800x24bpp(s,x,y), 3)
#define SDL_GetPixel_1024x24bpp(s,x,y,rgb)\
		memcpy(rgb, SDL_Pixel_1024x24bpp(s,x,y), 3)
#define SDL_GetPixel_24bpp(s,x,y,rgb)\
		memcpy(rgb, SDL_Pixel_24bpp(s,x,y), 3)

/*
 * 32 bpp macros:
 */
#define SDL_Pixel_16x32bpp(s,x,y)	((Uint32*)s->pixels+(x)+SDL_Pixel_16w(y))
#define SDL_Pixel_24x32bpp(s,x,y)	((Uint32*)s->pixels+(x)+SDL_Pixel_24w(y))
#define SDL_Pixel_32x32bpp(s,x,y)	((Uint32*)s->pixels+(x)+SDL_Pixel_32w(y))
#define SDL_Pixel_48x32bpp(s,x,y)	((Uint32*)s->pixels+(x)+SDL_Pixel_48w(y))
#define SDL_Pixel_64x32bpp(s,x,y)	((Uint32*)s->pixels+(x)+SDL_Pixel_64w(y))
#define SDL_Pixel_128x32bpp(s,x,y)	((Uint32*)s->pixels+(x)+SDL_Pixel_128w(y))
#define SDL_Pixel_256x32bpp(s,x,y)	((Uint32*)s->pixels+(x)+SDL_Pixel_256w(y))
#define SDL_Pixel_320x32bpp(s,x,y)	((Uint32*)s->pixels+(x)+SDL_Pixel_320w(y))
#define SDL_Pixel_640x32bpp(s,x,y)	((Uint32*)s->pixels+(x)+SDL_Pixel_640w(y))
#define SDL_Pixel_800x32bpp(s,x,y)	((Uint32*)s->pixels+(x)+SDL_Pixel_800w(y))
#define SDL_Pixel_1024x32bpp(s,x,y)	((Uint32*)s->pixels+(x)+SDL_Pixel_1024w(y))
#define SDL_Pixel_32bpp(s,x,y)		((Uint32*)s->pixels+(x)+SDL_Pixel_ANYw(s,y))

#define SDL_PutPixel_16x32bpp(s,x,y,rgb)	*SDL_Pixel_16x32bpp(s,x,y) = rgb
#define SDL_PutPixel_24x32bpp(s,x,y,rgb)	*SDL_Pixel_24x32bpp(s,x,y) = rgb
#define SDL_PutPixel_32x32bpp(s,x,y,rgb)	*SDL_Pixel_32x32bpp(s,x,y) = rgb
#define SDL_PutPixel_48x32bpp(s,x,y,rgb)	*SDL_Pixel_48x32bpp(s,x,y) = rgb
#define SDL_PutPixel_64x32bpp(s,x,y,rgb)	*SDL_Pixel_64x32bpp(s,x,y) = rgb
#define SDL_PutPixel_128x32bpp(s,x,y,rgb)	*SDL_Pixel_128x32bpp(s,x,y) = rgb
#define SDL_PutPixel_256x32bpp(s,x,y,rgb)	*SDL_Pixel_256x32bpp(s,x,y) = rgb
#define SDL_PutPixel_320x32bpp(s,x,y,rgb)	*SDL_Pixel_320x32bpp(s,x,y) = rgb
#define SDL_PutPixel_640x32bpp(s,x,y,rgb)	*SDL_Pixel_640x32bpp(s,x,y) = rgb
#define SDL_PutPixel_800x32bpp(s,x,y,rgb)	*SDL_Pixel_800x32bpp(s,x,y) = rgb
#define SDL_PutPixel_1024x32bpp(s,x,y,rgb)	*SDL_Pixel_1024x32bpp(s,x,y) = rgb
#define SDL_PutPixel_32bpp(s,x,y,rgb) 		*SDL_Pixel_32bpp(s,x,y) = rgb

#define SDL_GetPixel_16x32bpp(s,x,y,rgb)	*rgb = *SDL_Pixel_16x32bpp(s,x,y)
#define SDL_GetPixel_24x32bpp(s,x,y,rgb)	*rgb = *SDL_Pixel_24x32bpp(s,x,y)
#define SDL_GetPixel_32x32bpp(s,x,y,rgb)	*rgb = *SDL_Pixel_32x32bpp(s,x,y)
#define SDL_GetPixel_48x32bpp(s,x,y,rgb)	*rgb = *SDL_Pixel_48x32bpp(s,x,y)
#define SDL_GetPixel_64x32bpp(s,x,y,rgb)	*rgb = *SDL_Pixel_64x32bpp(s,x,y)
#define SDL_GetPixel_128x32bpp(s,x,y,rgb)	*rgb = *SDL_Pixel_128x32bpp(s,x,y)
#define SDL_GetPixel_256x32bpp(s,x,y,rgb)	*rgb = *SDL_Pixel_256x32bpp(s,x,y)
#define SDL_GetPixel_320x32bpp(s,x,y,rgb)	*rgb = *SDL_Pixel_320x32bpp(s,x,y)
#define SDL_GetPixel_640x32bpp(s,x,y,rgb)	*rgb = *SDL_Pixel_640x32bpp(s,x,y)
#define SDL_GetPixel_800x32bpp(s,x,y,rgb)	*rgb = *SDL_Pixel_800x32bpp(s,x,y)
#define SDL_GetPixel_1024x32bpp(s,x,y,rgb)	*rgb = *SDL_Pixel_1024x32bpp(s,x,y)
#define SDL_GetPixel_32bpp(s,x,y,rgb) 		*rgb = *SDL_Pixel_32bpp(s,x,y)

#ifdef __cplusplus
}
#endif

#else
#error reinclusion
#endif _SDL_Pixel_GFX_H_
