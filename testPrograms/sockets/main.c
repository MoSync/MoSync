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


//#define ANIMATED
#define CONPRINT

#include <ma.h>
#include <maheap.h>

#ifndef CONPRINT
#include "../shared/print.h"
#else
#include <conprint.h>
#define printlnf printf
#define println printf
#endif	//ANIMATED

int ConnReadAtLeast(Handle conn, int min, int max, char* dst);
int AnimatedConnWait();

int MAMain() {
	Handle conn;
	char ping[] = "ping\n";
	char buffer[1024];
	int res;

#ifdef CONPRINT
	InitConsole();
	gConsoleLogging = 1;
#endif

	//println("Socket test");
retry:
	printlnf("Connecting...");
	conn = maConnect("socket://130.237.3.104:6666");	//test DNS resolution
	res = AnimatedConnWait();
	if(res <= 0) {
		printlnf("err %i", res);
		goto exit;
	}
	/*conn = maConnect("socket://217.25.35.146:81");	//test IP address reading
	res = AnimatedConnWait();
	if(res <= 0) {
		printlnf("err %i", res);
		goto exit;
	}*/

	println("Writing...");
	maConnWrite(conn, ping, sizeof(ping));
	res = AnimatedConnWait();
	if(res <= 0) {
		printlnf("err %i", res);
		goto exit;
	}

	println("Reading...");

	res = ConnReadAtLeast(conn, 4, sizeof(buffer), buffer);
	if(res <= 0) {
		printlnf("err %i", res);
		goto exit;
	}
	maConnClose(conn);

	printlnf("Got %i bytes", res);

	buffer[res] = 0;
	println(buffer);
	maUpdateScreen();

exit:
	println("Press a key");
	for(;;) {
		EVENT event;
		while(maGetEvent(&event)) {
			if(event.type == EVENT_TYPE_CLOSE ||
				(event.type == EVENT_TYPE_KEY_PRESSED && event.key == MAK_0))
			{
				maExit(0);
			} else if(event.type == EVENT_TYPE_KEY_PRESSED) {
				goto retry;
			}
		}
		maWait(0);
	}
}

int gPos;
int gTextHeight;
int AnimatedConnWait() {
	int startTime = maGetMilliSecondCount();
	int res;
#ifdef ANIMATED
	int lPos = gPos;
	int i=0;
	int screenWidth = EXTENT_X(maGetScrSize());

	maSetColor(0);
	maFillRect(0, lPos, screenWidth - 1, gTextHeight);
	printlnf("a%i",i++);
#endif	//ANIMATED
	while(1) {
		EVENT event;

#ifdef ANIMATED
		gPos = lPos;
		maSetColor(0);
		maFillRect(0, lPos, screenWidth - 1, gTextHeight);
		printlnf("b%i",i++);
#else
		maWait(0);
#endif	//ANIMATED

		if(maGetEvent(&event)) {
			if(event.type == EVENT_TYPE_CLOSE ||
				(event.type == EVENT_TYPE_KEY_PRESSED && event.key == MAK_0))
			{
				maExit(0);
			} else if(event.type == EVENT_TYPE_CONN) {
				res = event.conn.result;
				break;
			}
		}
	}
	printlnf("w%i", maGetMilliSecondCount() - startTime);
	return res;
}

int ConnReadAtLeast(Handle conn, int min, int max, char* dst) {
	int lPos = gPos;
	int pos = 0;
	while(pos < min) {
		int res;
		maConnRead(conn, dst + pos, max - pos);
		gPos = lPos;
		res = AnimatedConnWait();
		if(res <= 0) {
			return res;
		}
		pos += res;
	}
	return pos;
}

#ifndef CONPRINT
#include "../shared/print.c"
#include "../shared/misc.c"
#endif
