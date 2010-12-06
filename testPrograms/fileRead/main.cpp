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
