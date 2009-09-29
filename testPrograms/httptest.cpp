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
