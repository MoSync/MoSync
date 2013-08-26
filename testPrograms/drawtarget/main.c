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
