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
