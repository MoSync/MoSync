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

#include "maapi.h"
#include "MAassert.h"

//#define RECT_SIZE_TEST

#define RGB(r, g, b) (((r) << 16) | ((g) << 8) | (b))

int MAMain() {
	Extent screenSize;
	int midX, midY;
	int width, height;
	int scrW, scrH;

	screenSize = maGetScrSize();
	scrW = EXTENT_X(screenSize);
	scrH = EXTENT_Y(screenSize);
	midX = scrW >> 1;
	midY = scrH >> 1;
#ifdef RECT_SIZE_TEST
	width = 1;
	height = 1;
#else	//normal color test
	width = midX - 2;
	height = midY / 2 - 2;
#endif

	maSetColor(0x800080);	//deep purple
	maFillRect(0, 0, scrW, scrH);

	maSetColor(RGB(255, 0, 0));				//RED
	maFillRect(1, 1, width, height);
	maSetColor(RGB(255, 255, 0));				//YELLA
	maFillRect(1+width+2, 1, width, height);
	maSetColor(RGB(0, 255, 0));				//GREEN
	maFillRect(1, 1+height+2, width, height);
	maSetColor(RGB(0, 255, 255));				//TORQUOISE
	maFillRect(1+width+2, 1+height+2, width, height);
	maSetColor(RGB(0, 0, 255));				//BLUE
	maFillRect(1, 1+(height+2)*2, width, height);
	maSetColor(RGB(255, 0, 255));				//PURPLE
	maFillRect(1+width+2, 1+(height+2)*2, width, height);
	maSetColor(RGB(255, 255, 255));				//WHITE
	maFillRect(1, 1+(height+2)*3, width, height);
	maSetColor(0);				//BLACK
	maFillRect(1+width+2, 1+(height+2)*3, width, height);

	maUpdateScreen();
	FREEZE;
}
