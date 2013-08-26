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
#include <MAUtil/FileLister.h>

#define TEST(func) do { int _res = (func); if(_res < 0) { printf("Fail %i @ %i\n", _res, __LINE__); return; } } while(0)

static void checkSpace(const char* path) {
	printf("%s\n", path);
	MAHandle file = maFileOpen(path, MA_ACCESS_READ);
	TEST(file);
	int total = maFileTotalSpace(file);
	int avail = maFileAvailableSpace(file);
	printf("%i / %i\n", avail, total);
}

static void checkSpaces() {
	FileLister fl;
	fl.start("");
	MAUtil::String name;
	int res;
	while((res = fl.next(name)) > 0) {
		checkSpace(name.c_str());
	}
	if(res < 0) {
		printf("FileList error %i\n", res);
	}
}

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;
	printf("Hello World!\n");

	checkSpaces();

	FREEZE;
}
