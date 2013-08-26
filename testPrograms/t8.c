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

#include <maapi.h>
#include <maassert.h>

#define STDTEST

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define RGB(r, g, b) (((r) << 16) | ((g) << 8) | (b))

int MAMain() {
	const MAExtent scrSize = maGetScrSize();
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
		MAEvent event;
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
