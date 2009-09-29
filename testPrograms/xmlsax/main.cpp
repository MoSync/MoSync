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

#include <MAUtil/MAStream.h>
#include <MAUtil/MAXML.h>
#include "MAHeaders.h"
#include <conprint.h>
#include <maheap.h>

#define FREEZE for(;;) { maWait(WAIT_KEY); }

BOOL MyStart(const char* tag, const AttrMap* argv);
BOOL MyEnd(const char* tag);
BOOL MyCharData(const char* data);

int doTestData(Handle data) {
	int res;
	MAStream* stream = MAStream_create_data(data);
	if(!stream) {
		FREEZE;
	}
	res = MAXML_parseEx(stream, MyStart, MyEnd, MyCharData, 128, 16, 1024, 16, TRUE);
	if(res)
		printf("XML result %i\n", res);
	free(stream);
	return res;
}

int doTestMem(Handle data) {
	int res;
	MAStream* stream;
	{
		static char buffer[10*1024];
		int size = maGetDataSize(data);
		if(size > (int)sizeof(buffer)) {
			printf("%i > %i!\n", size, (int)sizeof(buffer));
			FREEZE;
		}
		maReadData(data, buffer, 0, size);
		stream = MAStream_create_memory(buffer, size);
	}
	if(!stream) {
		FREEZE;
	}
	res = MAXML_parseEx(stream, MyStart, MyEnd, MyCharData, 128, 16, 1024, 16, TRUE);
	if(res)
		printf("XML error %i\n", res);
	free(stream);
	return res;
}

int doTestSocket(Handle sock) {
	int res;
	MAStream* stream = MAStream_create_conn(sock);
	if(!stream) {
		FREEZE;
	}
	res = MAXML_parseEx(stream, MyStart, MyEnd, MyCharData, 128, 16, 1024, 16, TRUE);
	if(res)
		printf("XML result %i\n", res);
	free(stream);
	return res;
}

void doTest(Handle data) {
	int startTime;

	if(maGetDataSize(data) < 1024) {
		startTime = maGetMilliSecondCount();
		if(doTestMem(data) != 0) {
			FREEZE;
		}
		printf("MemMs: %i\n", maGetMilliSecondCount() - startTime);
	}

	startTime = maGetMilliSecondCount();
	if(doTestData(data) != 0) {
		FREEZE;
	}
	printf("DataMs: %i\n", maGetMilliSecondCount() - startTime);
}


extern "C" int MAMain() {
	//doTest(R_XML0);
	doTest(R_XML1);
	//doTest(R_XML2);
	//doTest(R_XML3);
	{
		Handle conn;
		int dummy, startTime;
		maConnect("http://rss.cnn.com/rss/cnn_topstories.rss", &dummy);
		maWait(WAIT_CONN);
		conn = maConnState();
		if(conn < 0) {
			printf("conn %i\n", conn);
			FREEZE;
		}
		startTime = maGetMilliSecondCount();
		doTestSocket(conn);
		printf("SockMs: %i\n", maGetMilliSecondCount() - startTime);
	}
	FREEZE;
}


BOOL MyStart(const char* tag, const AttrMap* argv) {
	int i;
	printf("Start %i:%s\n", MAXML_getNAttrs(argv), tag);
	for(i=0; i<MAXML_getNAttrs(argv); i++) {
		printf(" %s=\"%s\"\n", MAXML_getAttrName(argv, i), MAXML_getAttrValue(argv, i));
	}
	return TRUE;
}
BOOL MyEnd(const char* tag) {
	printf("End %s\n", tag);
	return TRUE;
}
BOOL MyCharData(const char* data) {
	printf("Data %s\n", data);
	return TRUE;
}

#include <MAUtil/MAXML.cpp>
#include <MAUtil/MAStream.cpp>
