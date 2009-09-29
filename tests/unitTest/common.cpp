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

#include "common.h"

void KeyBaseCase::checkYesNo(int keyCode) {
	if(keyCode == TK_YES || keyCode == TK_NO) {
		assert(name, keyCode == TK_YES);
		suite->runNextCase();
	}
}

void clearScreen(int color) {
	MAExtent e = maGetScrSize();
	Dimensions screen;
	screen.width = EXTENT_X(e);
	screen.height = EXTENT_Y(e);

	maSetClipRect(0, 0, screen.width, screen.height);
	int oldCol = maSetColor(color);
	maFillRect(0, 0, screen.width, screen.height);
	maSetColor(oldCol);
}
