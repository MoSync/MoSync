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

#ifndef _SE_MSAB_MOSYNC_FONT_H_
#define _SE_MSAB_MOSYNC_FONT_H_

#include <maapi.h>

typedef struct MAFont_t {   //  Size   | Acc. size
	int charWidth;			//     4   |     4
	int charHeight;			//     4   |     8
	int charsetWidth;		//     4   |    12
	int charsetHeight;		//     4   |    16
	int asciiStart;			//     4   |    20
	int charsPerLine;		//	   4   |    24
	Handle handle;			//     4   |    28
} MAFont;

Handle maInitFont(Handle resource, Extent charSize, Extent charsetSize, int asciiStart);
void maDrawChar(Handle font, char c, int x, int y);
void maDrawString(Handle font, const char* str, int x, int y);
Extent maGetStringExtents(Handle font, const char *str);


#endif
