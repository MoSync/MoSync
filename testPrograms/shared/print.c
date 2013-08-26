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
