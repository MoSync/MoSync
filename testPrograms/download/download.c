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
