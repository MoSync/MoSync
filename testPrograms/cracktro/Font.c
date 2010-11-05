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

#include "Font.h"

MAFont fonts[16];
int maxFontIndex = 0;

void calcCharPos(MAFont *f, char c, int *x, int *y) {
	int idx = (int)c - f->asciiStart;
	int row = idx / f->charsPerLine;
	int col = idx % f->charsPerLine;
	*x = col * f->charWidth;
	*y = row * f->charHeight;
}

MAHandle maInitFont(MAHandle resource, MAExtent charSize, MAExtent charsetSize, int asciiStart) {
	//int i;
	MAFont *f;
	//int charsPerLine = 0;

	if(maxFontIndex >= 15) return (MAHandle)0;
	f = &fonts[maxFontIndex];

	f->charWidth = EXTENT_X(charSize);
	f->charHeight = EXTENT_Y(charSize);
	f->charsetWidth = EXTENT_X(charsetSize);
	f->charsetHeight = EXTENT_Y(charsetSize);
	f->asciiStart = asciiStart;
	f->handle = resource;
	f->charsPerLine = f->charsetWidth/f->charWidth;

	maxFontIndex++;
	return maxFontIndex-1;
}

void maDrawChar(MAHandle font, char c, int x, int y) {

	MAFont* f = &fonts[font];
	MARect srcRect;
	MAPoint2d destPoint = {x,y};
	
	int fx,fy;
	
	calcCharPos(f, c, &fx, &fy);
	
	srcRect.left =  fx;
	srcRect.top = fy;
	srcRect.width = f->charWidth;
	srcRect.height = f->charHeight;

	maDrawImageRegion(f->handle, &srcRect, &destPoint, 0);
}

void maDrawString(MAHandle font, const char* str, int x, int y) {

	MAFont* f = &fonts[font];
	//int charsPerLine = f->charsetWidth/f->charWidth; //5
	MARect srcRect = {0, 0, f->charWidth, f->charHeight};
	MAPoint2d destPoint = {x,y};

	while(*str) {
		calcCharPos(f, *str, &srcRect.left, &srcRect.top);

		maDrawImageRegion(f->handle, &srcRect, &destPoint, 0);
		destPoint.x += f->charWidth;
		str++;
	}
}

MAExtent maGetStringExtents(MAHandle font, const char *str) {
	MAFont* f = &fonts[font];
	return EXTENT(f->charWidth*strlen(str), f->charHeight);
}
