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
