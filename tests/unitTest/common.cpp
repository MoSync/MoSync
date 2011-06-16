/* Copyright (C) 2010 MoSync AB

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
	}
}

void KeyBaseCase::showErrorScreen(int errorCode)
{
	MAExtent e = maGetScrSize();
	Dimensions screen;
	screen.width = EXTENT_X(e);
	screen.height = EXTENT_Y(e);
	maSetClipRect(0, 0, screen.width, screen.height);

	maSetColor(BLACK);
	maFillRect(0, 0, screen.width, screen.height);

	int oldCol = maSetColor(0xff0000);
	const char* testName = name.c_str();
	maDrawText(4, 0, testName);

	int testNameHeight = EXTENT_Y(maGetTextSize(testName));

	if(FUNC_OUT_OF_MEMORY_ALLOC == errorCode)
		maDrawText(4, testNameHeight+2, "Out of Memory");
	else if(FUNC_OUT_OF_MEMORY_RESOURCE == errorCode)
		maDrawText(4, testNameHeight+2, "Couldn't create resource");
	else if(FUNC_SYSCALL_ERROR == errorCode)
		maDrawText(4, testNameHeight+2, "A syscall wasn't called correctly");
	else if(FUNC_SYSCALL_NOT_SUPPORTED == errorCode)
		maDrawText(4, testNameHeight+2, "Unsupported feature");
	else
		maDrawText(4, testNameHeight+2, "Unknown error!");

	maDrawText(4, testNameHeight*2+4, "press key or screen to continue");

	maUpdateScreen();
	maSetColor(oldCol);	
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
