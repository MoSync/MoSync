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
#include <maheap.h>
#include <MAUtil/assert.h>
#include <MAUtil/httpget.h>
#include "shared/print.h"
#include "shared/misc.h"

int ConnReadAtLeast(Handle sock, int min, int max, char* dst);
int AnimatedConnWait();

//#define ANIMATED


extern "C" int MAMain() {
	const char* purl = "http://wap.mopix.se/";
	char buffer[45000] = {0};
	Handle sock;
	int size, result;
	println("HTTP GET test");
	println(purl);

//	maConnect("socket://www.mopix.se:80", &size); //crash test dummy
	sock = maConnect(purl);
	printlnf("sock %i", sock);
	if(sock <= 0) {
		goto exit;
	}
	result = AnimatedConnWait();
	printlnf("result %i", result);
	if(result <= 0) {
		goto exit;
	}
	size = MAUtil::getHttpConLen(sock);
	printlnf("size %i", size);
	if(size <= 0) {
		goto exit;
	}

	{
		result = ConnReadAtLeast(sock, size, sizeof(buffer), buffer);
		printlnf("read %i", result);
		if(result <= 0) {
			goto exit;
		}
		maConnClose(sock);

		buffer[result] = 0;
		printlnf(buffer);
	}
exit:
	println("Yeah, we're done.");
	FREEZE;
}

extern int gPos;
extern int gTextHeight;
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
				maExit(666);
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


#include "shared/print.c"
#include "shared/misc.c"
