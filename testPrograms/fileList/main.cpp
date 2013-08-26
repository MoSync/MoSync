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
#include <maassert.h>
#include <conprint.h>
#include <MAUtil/String.h>

#if 0	//log to file
#define LOG(str) lprintfln(str.c_str())
#define LPRINTFLN lprintfln
#else
#if 1	//log to screen
#define LOG(str) puts((str + "\n").c_str())
#define LPRINTFLN(fmt, a...) printf(fmt "\n", a)
#else	//don't log anything (useful for timing the list operation)
#define LOG(str)
#define LPRINTFLN(fmt, a...)
#endif
#endif

static void checkEvents() {
	MAEvent event;
	while(maGetEvent(&event)) {
		if(event.type == EVENT_TYPE_CLOSE ||
			(event.type == EVENT_TYPE_KEY_PRESSED && event.key == MAK_0))
		{
			maExit(1);
		}
	}
}

static char buffer[256];

static bool dumpFileList(const char* path) {
	MAHandle list = maFileListStart(path, "*", 0);
	if(list < 0) {
		LPRINTFLN("FLS error %i", list);
		return false;
	}
	bool empty = true;
	while(maFileListNext(list, buffer, sizeof(buffer)) > 0) {
		checkEvents();
		MAUtil::String p2(path);
		p2 += buffer;
		LOG(p2);
		if(p2[p2.size()-1] == '/')
			dumpFileList(p2.c_str());
		empty = false;
	}
	maFileListClose(list);
	//printf("%s: %s\n", empty ? "Empty" : "Done", path);
	return !empty;
}

extern "C" int MAMain() GCCATTRIB(noreturn);
extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;
	printf("dumping File List...\n");
	int startTime = maGetMilliSecondCount();
	dumpFileList("");
	int endTime = maGetMilliSecondCount();
	printf("Done in %i ms\n", endTime - startTime);

	FREEZE;
}
