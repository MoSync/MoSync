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

#ifndef __IOS__
#include "ma.h"
#include "mastring.h"
#include "conprint.h"
#include "maassert.h"
#else
#include <ma.h>
#include <mastring.h>
#include <conprint.h>
#include <maassert.h>
#endif //#ifndef __IOS__

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#ifdef __cplusplus
extern "C" {
#endif
const char* FileNameFromPath(const char* path) {
	const char* backslash = strrchr(path, '\\');
	const char* frontslash = strrchr(path, '/');
	if(frontslash == NULL && backslash == NULL)
		return path;
	return MAX(frontslash, backslash) + 1;
}

void Freeze(int val) {
	maUpdateScreen();
	for(;;) {
		MAEvent event;
		while(maGetEvent(&event)) {
			if(event.type == EVENT_TYPE_CLOSE ||
				(event.type == EVENT_TYPE_KEY_PRESSED &&
				(event.key == MAK_0 || event.key == MAK_BACK)))
			{
				maExit(val);
			}
		}
		maWait(0);
	}
}

void bfeHandler(int value, const char* filename, int line) {
	char buf[1024];
	sprintf(buf, "BFE %s:%i", FileNameFromPath(filename), line);
	maPanic(value, buf);
}

void panicFunction(const char* msg, const char* function) {
	if(function != NULL) {
		char buf[1024];
		sprintf(buf, "\"%s\" in %s", msg, function);
		maPanic(1, buf);
	} else {
		maPanic(1, msg);
	}
}
#ifdef __cplusplus
}
#endif //extern C
