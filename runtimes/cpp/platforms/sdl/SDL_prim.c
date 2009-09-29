/*
* SDL_prim.c
*
* Copyright (C) 2003-2005 Ryan McGuigan <ryan@tempestgames.com>
*
* See the included 'COPYING' file for license information.
*
* Simple SDL Primitive C functions.  My goal while writing this
* was to keep everything as simple as possible, both in the design
* of these functions and in the usage of these functions.
*
* I hope someone finds this useful.
*
* Adapted to compile without warning on g++ -W -Wall -O2 by tdd@insia.org
* (20050413)
*
*/

#ifdef _MSC_VER
#pragma warning(disable:4244)
#endif

#include "SDL_prim.h"


/*
* simple pixel drawing function.
*
* tests to be sure pixel is within bounds of the surface, so you
* can draw off the surface to your hearts content
*
* it's probably not good practice to be drawing off the surface though...
*
*/
INLINE
void SDL_putPixel(SDL_Surface *surf, int x, int y, Uint32 clr)
{
	int Bpp = surf->format->BytesPerPixel;
	Uint8 *p;

	if (! ((x < surf->clip_rect.x) || 
		   (x >= (surf->clip_rect.x + surf->clip_rect.w)) ||
		   (y < surf->clip_rect.y) || 
		   (y >= (surf->clip_rect.y + surf->clip_rect.h))
		   ))
	{
		p = (Uint8*)surf->pixels + y * surf->pitch + x * Bpp;

		switch(Bpp) {
		case 1:
			*p = clr;
			break;

		case 2:
			*(Uint16*)p = clr;
			break;

		case 3:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
				p[0] = (clr >> 16) & 0xff;
				p[1] = (clr >> 8) & 0xff;
				p[2] = clr & 0xff;
			} else {
				p[0] = clr & 0xff;
				p[1] = (clr >> 8) & 0xff;
				p[2] = (clr >> 16) & 0xff;
			}
			break;

		case 4:
			*(Uint32*)p = clr;
			break;
		}
	}
}


/*
* returns pointer to pixel at x, y
*/
INLINE
Uint8* SDL_getPixel(SDL_Surface *surf, int x, int y)
{
	if (! ((x < surf->clip_rect.x) || 
		   (x >= (surf->clip_rect.x + surf->clip_rect.w)) ||
		   (y < surf->clip_rect.y) || 
		   (y >= (surf->clip_rect.y + surf->clip_rect.h))
		   ))
		return (Uint8*)surf->pixels + y * surf->pitch + x * surf->format->BytesPerPixel;
	else
		return 0;
}


/*
* slow pixel blending function
*
* called from SDL_blendPixel for 8 bit surfaces.
* may be useful for something else, I dunno.
*
*/
INLINE
void __slow_SDL_blendPixel( SDL_Surface *surf, int x, int y, Uint32 clr,
						   Uint8 alpha)
{
	Uint8 *p;
	Uint8 r, g, b, r2, g2, b2;

	if ( (p = SDL_getPixel(surf, x, y)) ) {
		SDL_GetRGB(clr, surf->format, &r, &g, &b);
		SDL_GetRGB(*p, surf->format, &r2, &g2, &b2);

		/* The 3 following lines use explicit cast now - tdd */
		r = r2 + (int) (( r - r2 ) * ( alpha / 255.0 ));
		g = g2 + (int) (( g - g2 ) * ( alpha / 255.0 ));
		b = b2 + (int) (( b - b2 ) * ( alpha / 255.0 ));

		clr = SDL_MapRGB(surf->format, r, g, b);

		SDL_putPixel(surf, x, y, clr);
	}
}


/*
* blends pixel onto surface according to alpha
*/
INLINE
void SDL_blendPixel( SDL_Surface *surf, int x, int y, Uint32 clr,
					Uint8 alpha)
{
	Uint8 *p;
	Uint32 R, G, B;

	if ( (p = SDL_getPixel(surf, x, y)) ) {
		switch(surf->format->BytesPerPixel) {
		case 1:  /* 8 bpp */
			__slow_SDL_blendPixel(surf, x, y, clr, alpha);
			break;

		case 2:  /* 16 bpp */
			R = ( (*(Uint16*)p & surf->format->Rmask) + ( ((clr & surf->format->Rmask) - ((*(Uint16*)p) & surf->format->Rmask)) * alpha >> 8) ) & surf->format->Rmask;
			G = ( (*(Uint16*)p & surf->format->Gmask) + ( ((clr & surf->format->Gmask) - ((*(Uint16*)p) & surf->format->Gmask)) * alpha >> 8) ) & surf->format->Gmask;
			B = ( (*(Uint16*)p & surf->format->Bmask) + ( ((clr & surf->format->Bmask) - ((*(Uint16*)p) & surf->format->Bmask)) * alpha >> 8) ) & surf->format->Bmask;
			*(Uint16*)p = R | G | B;
			break;

		case 3:  /* 24 bpp */
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
				p[0] = ( p[0] + ((((clr >> 16) & 0xff) - p[0]) * alpha >> 8) ) & 0xff;
				p[1] = ( p[1] + ((((clr >> 8) & 0xff) - p[1]) * alpha >> 8) ) & 0xff;
				p[2] = ( p[2] + (((clr & 0xff) - p[2]) * alpha >> 8) ) & 0xff;
			} else {
				p[0] = ( p[0] + (((clr & 0xff) - p[0]) * alpha >> 8) ) & 0xff;
				p[1] = ( p[1] + ((((clr >> 8) & 0xff) - p[1]) * alpha >> 8) ) & 0xff;
				p[2] = ( p[2] + ((((clr >> 16) & 0xff) - p[2]) * alpha >> 8) ) & 0xff;
			}
			break;

		case 4:  /* 32 bpp */
			R = ( (*(Uint32*)p & surf->format->Rmask) + ( ((clr & surf->format->Rmask) - (*(Uint32*)p & surf->format->Rmask)) * alpha >> 8) ) & surf->format->Rmask;
			G = ( (*(Uint32*)p & surf->format->Gmask) + ( ((clr & surf->format->Gmask) - (*(Uint32*)p & surf->format->Gmask)) * alpha >> 8) ) & surf->format->Gmask;
			B = ( (*(Uint32*)p & surf->format->Bmask) + ( ((clr & surf->format->Bmask) - (*(Uint32*)p & surf->format->Bmask)) * alpha >> 8) ) & surf->format->Bmask;
			*(Uint32*)p = R | G | B;
			break;
		}
	}
}


/*
* Self explanatory.  draws lines, supports anti-aliasing and
* alpha blending
*
*/
void SDL_drawLine_TG( SDL_Surface *surf, int x, int y, int x2, int y2,
					 Uint32 clr, Uint8 alpha, Uint8 flags )
{
	int xaa, yaa, *a, *b, *a2, *b2, da, xd, yd;
	float aa, db;
	float realb;

	/*
	* for the sake of my sanity when i look at this again...
	*
	* xaa:   the x aa offset value, either 1 or 0
	* yaa:   the y aa offset value, either 1 or 0
	* *a:    reference to either x or y, depending on which way we are
	*        drawing.
	* *b:    reference to either x or y, depending on which way we are
	*        drawing.
	* *a2:   reference to either x2 or y2, depending on which way we are
	*        drawing.
	* *b2:   reference to either x2 or y2, depending on which way we are
	*        drawing.
	* x:     starting x value, current x value
	* y:     starting y value, current y value
	* x2:    ending x value
	* y2:    ending y value
	* xd:    distance from x to x2
	* yd:    distance from y to y2
	* da:    change in a for each iteration, either 1 or -1
	* db:    change in b with respect to a
	* realb: real b value, as b is an int, and db is going to be a
	*        fraction
	* aa:    anti-aliasing value, fraction part of realb
	*
	*/

	/* find x and y distances */
	xd = x2 - x;
	yd = y2 - y;

	if (abs(xd) >= abs(yd)) {
		/* draw left/right to left/right */
		xaa = 0;
		yaa = 1;
		a = &x;
		b = &y;
		a2 = &x2;
		b2 = &y2;
		db = (float)yd / xd;
	} else {
		/* draw top/bottom to top/bottom */
		xaa = 1;
		yaa = 0;
		a = &y;
		b = &x;
		a2 = &y2;
		b2 = &x2;
		db = (float)xd / yd;
	}

	if (!(alpha < 255 && (flags & SDL_TG_ALPHA)))
		alpha = 255;

	/* left or right? */
	da = (*a <= *a2) ? 1 : -1;
	/* up or down? */
	db *= da;
	/* init realb */
	realb = (float)*b;
	/* we're stopping when we hit *a2, so we have to add +/-1 to it */
	*a2 += da;

	if (flags & SDL_TG_LOCK)
		__SDL_PRIM_LOCKSURFACE(surf)

		for (; *a != *a2; *a += da) {
			/* Explicit cast now - tdd */
			*b = (int) floor(realb + 0.5);

			if (flags & SDL_TG_ANTIALIAS) {
				aa = (realb + 0.5 - *b) * alpha;
				SDL_blendPixel(surf, x + xaa, y + yaa, clr, (int)aa);
				SDL_blendPixel(surf, x - xaa, y - yaa, clr, alpha - (int)aa);
			}

			if (alpha < 255)
				SDL_blendPixel(surf, x, y, clr, alpha);
			else
				SDL_putPixel(surf, x, y, clr);

			realb += db;
		}

		if (flags & SDL_TG_LOCK)
			__SDL_PRIM_UNLOCKSURFACE(surf)
}



/*
* Circle drawing/filling function.  Uses a more optimized algorithm
* when drawing non-anti-aliased circles, uses a mathematically correct algo
* when drawing anti-aliased circles.  After much trial and error, I have
* removed 100% of pixel overlapping, no more spots in anti-aliased circles.
*
*/
void SDL_drawCircle_TG( SDL_Surface *surf, int x1, int y1, int r, Uint32 clr,
					   Uint8 alpha, Uint8 flags )
{
	int x = 0, tmp, dy = 0, y = r, r2, aa = 0, fy = 0;
	int r2p = r * r;
	float aatmp;

#	define __CIRCLE_OUTAA \
	{ \
	if (x) { \
	SDL_blendPixel(surf, x1 + x, y1 + y + 1, clr, aa); \
	SDL_blendPixel(surf, x1 + x, y1 - y - 1, clr, aa); \
	} \
	SDL_blendPixel(surf, x1 - x, y1 + y + 1, clr, aa); \
	SDL_blendPixel(surf, x1 - x, y1 - y - 1, clr, aa); \
	}

#	define __CIRCLE_INAA \
	{ \
	if (x) { \
	SDL_blendPixel(surf, x1 + x, y1 + y - 1, clr, alpha - aa); \
	SDL_blendPixel(surf, x1 + x, y1 - y + 1, clr, alpha - aa); \
	} \
	SDL_blendPixel(surf, x1 - x, y1 + y - 1, clr, alpha - aa); \
	SDL_blendPixel(surf, x1 - x, y1 - y + 1, clr, alpha - aa); \
	}

#	define __CIRCLE_OUTAA_ROT90 \
	{ \
	if (x) { \
	SDL_blendPixel(surf, x1 + y + 1, y1 + x, clr, aa); \
	SDL_blendPixel(surf, x1 - y - 1, y1 - x, clr, aa); \
	} \
	SDL_blendPixel(surf, x1 - y - 1, y1 + x, clr, aa); \
	SDL_blendPixel(surf, x1 + y + 1, y1 - x, clr, aa); \
	}

#	define __CIRCLE_INAA_ROT90 \
	{ \
	if (x) { \
	SDL_blendPixel(surf, x1 + y - 1, y1 + x, clr, alpha - aa); \
	SDL_blendPixel(surf, x1 - y + 1, y1 - x, clr, alpha - aa); \
	} \
	SDL_blendPixel(surf, x1 - y + 1, y1 + x, clr, alpha - aa); \
	SDL_blendPixel(surf, x1 + y - 1, y1 - x, clr, alpha - aa); \
	}

#	define __CIRCLE_BLEND \
	{ \
	if (x) { \
	SDL_blendPixel(surf, x1 + x, y1 + y, clr, alpha); \
	SDL_blendPixel(surf, x1 + x, y1 - y, clr, alpha); \
	} \
	SDL_blendPixel(surf, x1 - x, y1 + y, clr, alpha); \
	SDL_blendPixel(surf, x1 - x, y1 - y, clr, alpha); \
	}

#	define __CIRCLE_BLEND_ROT90 \
	{ \
	if (x) { \
	SDL_blendPixel(surf, x1 - y, y1 + x, clr, alpha); \
	SDL_blendPixel(surf, x1 + y, y1 + x, clr, alpha); \
	} \
	SDL_blendPixel(surf, x1 + y, y1 - x, clr, alpha); \
	SDL_blendPixel(surf, x1 - y, y1 - x, clr, alpha); \
	}

#	define __CIRCLE_DRAW \
	{ \
	if (x) { \
	SDL_putPixel(surf, x1 + x, y1 + y, clr); \
	SDL_putPixel(surf, x1 + x, y1 - y, clr); \
	} \
	SDL_putPixel(surf, x1 - x, y1 + y, clr); \
	SDL_putPixel(surf, x1 - x, y1 - y, clr); \
	}

#	define __CIRCLE_DRAW_ROT90 \
	{ \
	if (x) { \
	SDL_putPixel(surf, x1 - y, y1 + x, clr); \
	SDL_putPixel(surf, x1 + y, y1 + x, clr); \
	} \
	SDL_putPixel(surf, x1 + y, y1 - x, clr); \
	SDL_putPixel(surf, x1 - y, y1 - x, clr); \
	}

	if (!(alpha < 255 && (flags & SDL_TG_ALPHA)))
		alpha = 255;

	if (r < 6)
		r2 = 0;
	else
		r2 = r / 2;

	if (flags & SDL_TG_LOCK)
		__SDL_PRIM_LOCKSURFACE(surf)

		if (flags & SDL_TG_FILL) {
			if (alpha < 255)
				SDL_blendPixel(surf, x1, y1, clr, alpha);
			else
				SDL_putPixel(surf, x1, y1, clr);
		}

		do {
			if (alpha < 255) {
				__CIRCLE_BLEND
					__CIRCLE_BLEND_ROT90
			} else {
				__CIRCLE_DRAW
					__CIRCLE_DRAW_ROT90
			}

			if (flags & SDL_TG_FILL) {
				/* Fill the circle... */
				if (alpha < 255) {
					for (fy = y - 1; fy >= (x ? x : 1); fy--) {
						if (x) {
							SDL_blendPixel(surf, x1 - x, y1 + fy, clr, alpha);
							SDL_blendPixel(surf, x1 - x, y1 - fy, clr, alpha);
						}
						SDL_blendPixel(surf, x1 + x, y1 + fy, clr, alpha);
						SDL_blendPixel(surf, x1 + x, y1 - fy, clr, alpha);

						if (fy > x) {
							if (x) {
								SDL_blendPixel(surf, x1 - fy, y1 + x, clr, alpha);
								SDL_blendPixel(surf, x1 + fy, y1 + x, clr, alpha);
							}
							SDL_blendPixel(surf, x1 - fy, y1 - x, clr, alpha);
							SDL_blendPixel(surf, x1 + fy, y1 - x, clr, alpha);
						}
					}
				} else {
					for (fy = y - 1; fy >= (x ? x : 1); fy--) {
						if (x) {
							SDL_putPixel(surf, x1 - x, y1 + fy, clr);
							SDL_putPixel(surf, x1 - x, y1 - fy, clr);
						}
						SDL_putPixel(surf, x1 + x, y1 + fy, clr);
						SDL_putPixel(surf, x1 + x, y1 - fy, clr);

						if (fy > x) {
							if (x) {
								SDL_putPixel(surf, x1 - fy, y1 + x, clr);
								SDL_putPixel(surf, x1 + fy, y1 + x, clr);
							}
							SDL_putPixel(surf, x1 - fy, y1 - x, clr);
							SDL_putPixel(surf, x1 + fy, y1 - x, clr);
						}
					}
				}
			}

			if (flags & SDL_TG_ANTIALIAS) {
				/*
				* anti-aliased circle algorithm.  this is
				* mathematically correct, but fairly expensive.
				* looks extremely good though.
			 *
			 * this is also very simple, we simply find the y
			 * value with respect to each x value, using a circle
			 * equation.  the anti-alias values are based on the
			 * real y coordinates.
			 */
				__CIRCLE_OUTAA
					__CIRCLE_OUTAA_ROT90
					if (!(flags & SDL_TG_FILL)) {
						__CIRCLE_INAA
							if (x < y-1)
								__CIRCLE_INAA_ROT90
					}

					aatmp = sqrt((float)r2p - x*x);
					/* Explicit cast now - tdd */
					y = (int) aatmp;
					aatmp = aatmp - (int)aatmp;
					/* Explicit cast now - tdd */
					aa = (int) (aatmp * alpha);
					x++;
			} else {
				/*
				* i came up with this circle drawing algorithm on my
				* own.  i don't know how it compares to other circle
				* drawing algos as far as optimization, but this seems
				* to work pretty well.  it's also pretty damn
				* simple, which is what i'm all about.
			 */

				/* move along x axis */
				x++;

				/*
				* adjust y if we're moving too far out of the circle.
				* the r2 is simply the radius over 2, unless the
				* radius is less than a particular value(above).
				* this simply makes the circle look more accurate,
				* as we're working with integers so it's never exact,
				* and we don't want to change the y value unless we're
				* going off by a particular amount.  this also more
				* closely approximates the above, mathematically
				* correct, algo.
			 */
				if (dy)
					y += dy;
				tmp = (r2p - x*x - y*y + r2);
				if (tmp < 0)
					dy = -1;
				else if (dy)
					dy = 0;
			}
		} while (x < y);

		if (x == y) {
			if (alpha < 255)
				__CIRCLE_BLEND
			else
			__CIRCLE_DRAW

			if (flags & SDL_TG_ANTIALIAS) {
				__CIRCLE_OUTAA
					__CIRCLE_OUTAA_ROT90
			}
		}
		else if (flags & SDL_TG_ANTIALIAS)
			__CIRCLE_OUTAA

			if (flags & SDL_TG_LOCK)
				__SDL_PRIM_UNLOCKSURFACE(surf)
}


/*
* draws triangles
*
* this isn't very good, there is a good amount of pixel overlapping, 
* and there are sometimes pixels dangling off the corners...
*
* bleh, fuck it, i'll fix it later
*
*/
void SDL_drawTriangle_TG(SDL_Surface *surf, int x1, int y1, int x2, int y2,
						 int x3, int y3, Uint32 clr, Uint8 alpha, Uint8 flags)
{
	int x, y, c, xlen, ylen, i, aaoffset;
	float a1, b1, a2, b2, slopea, slopeb, aa, aastep, pb1, pb2;
	float slope1, slope2, slope3;

	/* avoid compiler warnings... */
	aa = 0.0;
	aastep = 0.0;
	aaoffset = 0;

#	define __TRI_DRAW(PB, B, SLOPE) \
	{ \
	if (fabs(SLOPE) > 1.0) { \
	c = (PB) > (B) ? -1 : 1; \
	if (flags & SDL_TG_ANTIALIAS) { \
	aastep = alpha / fabs(SLOPE); \
	aa = alpha; \
	} \
	/* Explicit cast now - tdd */ \
	for (y = (int) (PB); ( c > 0 ? y < floor(B) : y > (B) ); y += c) { \
	if (alpha < 255) \
	SDL_blendPixel(surf, x, y, clr, alpha); \
				else \
				SDL_putPixel(surf, x, y, clr); \
				if (flags & SDL_TG_ANTIALIAS) { \
				SDL_blendPixel(surf, x - 1, y, clr, (int)aa); \
				SDL_blendPixel(surf, x + 1, y, clr, (int)(alpha - aa)); \
				aa -= aastep; \
				} \
	} \
	} else { \
	if (alpha < 255) \
	SDL_blendPixel(surf, x, (int)(B), clr, (int)alpha); \
			else \
			SDL_putPixel(surf, x, (int)(B), clr); \
			if (flags & SDL_TG_ANTIALIAS) { \
			aa = (B) - (int)(B); \
			aa *= alpha; \
			SDL_blendPixel(surf, x, (int)(B) + 1, clr, (int)aa); \
			SDL_blendPixel(surf, x, (int)(B) - 1, clr, (int)(alpha - aa)); \
			} \
	} \
	}

#	define __TRI_FILL_AA(PB, B, B2, SLOPE) \
	{ \
	if (fabs(SLOPE) > 2.0) { \
	c = (SLOPE > 0) ? 1 : -1; \
	if (SLOPE > 0) { \
	if (SLOPE == slope3) { \
	aaoffset = 0; \
	aa = 0; \
	aastep = alpha / SLOPE; \
	} \
				else { \
				aaoffset = -1; \
				aa = alpha; \
				aastep = alpha / SLOPE * -1; \
	} \
	} \
			else { \
			if (SLOPE == slope3) { \
			aaoffset = 0; \
			aa = 0; \
			aastep = alpha / SLOPE * -1; \
			} \
				else { \
				aaoffset = -1; \
				aa = alpha; \
				aastep = alpha / SLOPE; \
	} \
	} \
	/* Explicit cast now - tdd */ \
	for (y = (int) ((PB) + c); ( c > 0 ? y < floor(B) : y > (B) ); y += c) { \
	SDL_blendPixel(surf, x + aaoffset, y, clr, (int)aa); \
	aa += aastep; \
	} \
	} else { \
	if (B > B2) \
	c = 1; \
			else \
			c = -1; \
			aa = fabs( (B) - (int)(B) ); \
			if (c < 0) \
			aa = alpha - aa * alpha; \
			else \
			aa *= alpha; \
			/* Explicit cast now - tdd */ \
			SDL_blendPixel(surf, x, (int) ((B) + c), clr, (int)aa); \
	} \
	}

	if ( !(flags & SDL_TG_ALPHA && alpha < 255) )
		alpha = 255;

	/*
	* sort points by X coordinate so we can split the drawing
	* at a slope change
	*/
	while ( !(x1 <= x2 && x1 <= x3) ) {
		x = x1;
		y = y1;
		x1 = x2;
		y1 = y2;
		x2 = x3;
		y2 = y3;
		x3 = x;
		y3 = y;
	}
	while ( !(x2 <= x3) ) {
		x = x2;
		y = y2;
		x2 = x3;
		y2 = y3;
		x3 = x;
		y3 = y;
	}

	xlen = x3 - x1;
	ylen = y3 - y1;
	slope1 = (float)ylen / xlen;

	xlen = x1 - x2;
	ylen = y1 - y2;
	slope2 = (float)ylen / xlen;

	xlen = x2 - x3;
	ylen = y2 - y3;
	slope3 = (float)ylen / xlen;

	a1 = x1;
	a2 = x2;
	b1 = y1;
	b2 = y1;
	slopea = slope1;
	slopeb = slope2;

	pb1 = b1;
	pb2 = b2;

	if (flags & SDL_TG_LOCK)
		__SDL_PRIM_LOCKSURFACE(surf)

		for (i = 0; i <= 1; i++) {
			if ((a1 == x1 && a1 != a2) || (a1 != x1 && a1 - 1 != a2)) {
				/* Explicit cast now - tdd */
				for (x = (int) a1; x <= a2; x++) {
					if (flags & SDL_TG_FILL) {
						c = ((int)b1 < (int)b2) ? 1 : -1;
						for (y = (int)b1; (c > 0 ? y <= (int)b2 : y >= (int)b2); y += c) {
							if (alpha < 255)
								SDL_blendPixel(surf, x, y, clr, alpha);
							else
								SDL_putPixel(surf, x, y, clr);
						}
						if (flags & SDL_TG_ANTIALIAS) {
							__TRI_FILL_AA(pb1, b1, b2, slopea)
								__TRI_FILL_AA(pb2, b2, b1, slopeb)
						}
					}
					else {
						__TRI_DRAW(pb1, b1, slopea)
							__TRI_DRAW(pb2, b2, slopeb)
					}
					pb1 = b1;
					pb2 = b2;

					b1 += slopea;
					b2 += slopeb;
				}
			} else if (!(flags & SDL_TG_FILL)) {
				if (a1 == x1)
					SDL_drawLine_TG(surf, x1,y1, x2,y2, clr, alpha, flags);
				else
					SDL_drawLine_TG(surf, x2,y2, x3,y3, clr, alpha, flags);
			}

			a1 = x2 + 1;
			a2 = x3;
			b2 = y2;
			slopeb = slope3;
			b2 += slopeb;
		}

		if (flags & SDL_TG_LOCK)
			__SDL_PRIM_UNLOCKSURFACE(surf)
}


