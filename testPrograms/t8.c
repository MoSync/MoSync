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

#include <maapi.h>
#include <maassert.h>

#define STDTEST

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define RGB(r, g, b) (((r) << 16) | ((g) << 8) | (b))

int MAMain() {
	const Extent scrSize = maGetScrSize();
	const int scrHeight = EXTENT_Y(scrSize);
	const int scrWidth = EXTENT_X(scrSize);
start:	maSetColor(0x808080);
	maFillRect(0, 0, scrWidth, scrHeight);

	maSetColor(0xffffff);
#ifdef STDTEST
	maLine(50, 50, 100, 100);
	maUpdateScreen();
	maLine(50, -50, 100, -100);
	maUpdateScreen();
	maLine(-50, 50, -100, 100);
	maUpdateScreen();
	maLine(-50, -50, -100, -100);
	maUpdateScreen();
	maLine(-10, 20, 20, -10);	//5
	maUpdateScreen();
	maLine(-1, 1, 1, -1);	//6-7
	maUpdateScreen();
	maLine(-20, 10, 10, -20);	//8
	maUpdateScreen();
	maLine(-40, 40, 50, -100);	//9
	maUpdateScreen();
	maLine(172, 226, 176, 205);	//crash test dummy
	maUpdateScreen();
	maLine(-5, 1, 2, -1);
	maUpdateScreen();
#else	//Line pixel test
	maLine(1, 1, 1, 5);
	maUpdateScreen();
	maSetColor(0x00ffff);
	maLine(1, 2, 3, 2);
	maUpdateScreen();
	maSetColor(0xff00ff);
	maLine(2, 3, 0, 3);
	maUpdateScreen();
	maSetColor(0xff80ff);
	maLine(0, 4, 2, 4);
	maUpdateScreen();
	maSetColor(0xffffff);
	maLine(4, 5, 4, 1);
	maUpdateScreen();
#endif

	//Freeze
	while(1) {
		maWait(0);
		EVENT event;
		if(maGetEvent(&event)) {
			if(event.type == EVENT_TYPE_CLOSE)
				break;
			if(event.type == EVENT_TYPE_KEY_PRESSED) {
				if(event.key == MAK_0)
					break;
			}
		}
		goto start;
	}
	return 0;
}
