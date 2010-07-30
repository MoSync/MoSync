/* Copyright (C) 2010 Mobile Sorcery AB

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
#include <maassert.h>
#include <conprint.h>
#include <MAUtil/String.h>
#include <MAUtil/FileLister.h>
#include <IX_FILE.h>

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
