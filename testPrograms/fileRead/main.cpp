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

// Reads a log file from a s60v3 debug runtime.
static bool tryToRead() {
	MAUtil::String filename = "C:/Data/msrlogold.txt";
	printf("Open '%s'\n", filename.c_str());
	MAHandle file = maFileOpen(filename.c_str(), MA_ACCESS_READ);
	if(file < 0) {
		printf("Error %i\n", file);
		return false;
	}
	int res = maFileExists(file);
	MAASSERT(res >= 0);
	if(!res) {
		printf("File does not exist.\n");
		return false;
	}
	int size = maFileSize(file);
	printf("Size: %i\n", size);
	MAASSERT(size >= 0);
	static char data[32*1024];
	MAASSERT(size < (int)sizeof(data));
	res = maFileRead(file, data, size);
	MAASSERT(res == 0);
	data[32] = 0;
	printf("%s\n", data);
	printf("Closing...\n");
	res = maFileClose(file);
	MAASSERT(res == 0);
	printf("Done.\n");
	return true;
}

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;
	printf("Hello World!\n");
	tryToRead();
	FREEZE;
}
