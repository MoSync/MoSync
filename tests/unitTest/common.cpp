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
