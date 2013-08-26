/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
