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

#include <ma.h>
#include "..\shared\print.h"
#include "..\shared\misc.h"

#define BIGASSBUFFERSIZE 45000

int main() {
	char bigAssBuffer[BIGASSBUFFERSIZE];
	char* hello = "HTTP GET test";
	Extent screenSize, textSize;
	screenSize = maGetScrSize();

	//Draw centered text
	println("");
	textSize = maDrawText(0, 0, hello);
	maSetColor(0);
	maFillRect(0, 0, EXTENT_X(textSize), EXTENT_Y(textSize));  //erase test text
	maDrawText((EXTENT_X(screenSize) - EXTENT_X(textSize)) / 2, 0, hello);
	maUpdateScreen();

	println("Connecting...");
	int res = maHttpGet("link.astando.se"
		"/guido/LvRouting.asmx/GenerateRouteGuido?fromX=100502&fromY=77476&toX=100591&toY=77368",
		//"wap.mopix.se/test/test.comb",
		bigAssBuffer, BIGASSBUFFERSIZE);

	printiln("Got %i bytes", res);
	println(bigAssBuffer);
	println("PAK to quit");

	PAKAndRelease();
	maExit(0);
}

#include "..\shared\print.c"
#include "..\shared\misc.c"
