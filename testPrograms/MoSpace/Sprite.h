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

#ifndef _MOBRIS_SPRITE_H_
#define _MOBRIS_SPRITE_H_

#include "types.h"

typedef struct Sprite_t {
	Handle* images; /* pointer to the start of the image pointer array */
	Integer numImages; 
	Integer animSpeed;
} Sprite;


#define SPRITE_WIDTH(sPtr) EXTENT_X(maGetImageSize(sPtr->images[0]))
#define SPRITE_HEIGHT(sPtr) EXTENT_Y(maGetImageSize(sPtr->images[0]))





#endif
