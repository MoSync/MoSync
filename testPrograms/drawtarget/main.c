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
#include "maheaders.h"

int MAMain() {
	int x=0, y=0, dx=1, dy=1;
	int scrW, scrH;
	int srcW, srcH;
	{
		int srcSize = maGetImageSize(SIMG);
		srcW = EXTENT_X(srcSize);
		srcH = EXTENT_Y(srcSize);
	}
	maCreateDrawableImage(TIMG, srcW,srcH);
	maSetDrawTarget(TIMG);

	maSetColor(0);	//black
	maFillRect(0,0, srcW,srcH);
	maDrawImage(SIMG, 0,0);
	maSetColor(0xff0000);	//red
	maLine(0,0, srcW,srcH);
	maLine(srcW,0, 0,srcH);

	maSetDrawTarget(HANDLE_SCREEN);
	{
		int scrSize = maGetScrSize();
		scrW = EXTENT_X(scrSize);
		scrH = EXTENT_Y(scrSize);
	}
	while(1) {
		//int then = maGetMilliSecondCount();

		maSetColor(0);	//black
		maFillRect(0,0, scrW,scrH);

		maDrawImage(TIMG, x, y);
		if((dx > 0 && x + srcW > scrW) || (dx < 0 && x < 0)) {
			dx = -dx;
		}
		if((dy > 0 && y + srcH > scrH) || (dy < 0 && y < 0)) {
			dy = -dy;
		}
		x += dx;
		y += dy;
		maUpdateScreen();
		/*{
		int now = maGetMilliSecondCount();
		int time_padding = 10 - (now - then);
		if(time_padding > 0)
		maSleep(time_padding);
		}*/
	}
}
