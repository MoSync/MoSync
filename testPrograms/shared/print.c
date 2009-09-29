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

#include "misc.h"
#include "print.h"
#include "maarg.h"

void printlnf(const char* fmt, ...) {
	char buf[1024];
	va_list args;
	int n;

	va_start(args, fmt);
	n = vsprintf(buf, fmt, args);
	va_end(args);

	if(n > 0) {
		println(buf);
	}
}

int gPos=0;
int gClearOnNext=1;
int gTextHeight=0;

void ClearScreen() {
	const int scrHeight = EXTENT_Y(maGetScrSize());
	const int scrWidth = EXTENT_X(maGetScrSize());
	int prevColor = maSetColor(0);
	maFillRect(0, 0, scrWidth, scrHeight);
	maSetColor(prevColor);
	gPos = 0;
	gClearOnNext = 1;
}

void println(const char* text) {
	const int scrHeight = EXTENT_Y(maGetScrSize());
	const int scrWidth = EXTENT_X(maGetScrSize());
	if(gClearOnNext) {
		gTextHeight = EXTENT_Y(maGetTextSize(" "));
		maSetColor(0);
		maFillRect(0,0, scrWidth, scrHeight);
		gClearOnNext = 0;
	}
	maSetColor(0x00ff00);
	maDrawText(0, gPos, text);
	gPos += gTextHeight;
	if(gPos + gTextHeight > scrHeight) {	//if gTextHeight can change, this is only an approximation.
		gPos = 0;
		gClearOnNext = 1;
	}
	maUpdateScreen();
}
