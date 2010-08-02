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
#include <IX_FILE.h>

static bool tryToWrite(const MAUtil::String& dir);

static bool writeAFile(const MAUtil::String& dir) {
	// find a root path
	printf("Dir: '%s'\n", dir.c_str());
	FileLister fl;
	int res = fl.start(dir.c_str());
	if(res < 0) {
		printf("Error %i\n", res);
		return false;
	}
	MAUtil::String file;
	while(1) {
		res = fl.next(file);
		printf("%i: '%s'\n", res, file.c_str());
		if(res < 0) {
			printf("Error %i\n", res);
			return false;
		}
		if(res == 0) {
			printf("Dir '%s' ends.\n", dir.c_str());
			return false;
		}
		if(file[file.size()-1] == '/') {
			if(tryToWrite(dir + file))
				return true;
			if(writeAFile(dir + file))
				return true;
		}
	}
}

static bool deleteIfExist(const MAUtil::String& path) {
	MAHandle file = maFileOpen(path.c_str(), MA_ACCESS_READ_WRITE);
	if(file < 0) {
		printf("Error %i\n", file);
		return false;
	}
	int res = maFileExists(file);
	MAASSERT(res >= 0);
	if(res) {
		printf("File exists.\n");
		res = maFileDelete(file);
		if(res < 0) {
			printf("Error %i\n", res);
			return false;
		}
		printf("Deleted.\n");
	} else {
		printf("File doesn't exist.\n");
	}
	return true;
}

static bool tryToWrite(const MAUtil::String& dir) {
	MAUtil::String filename = dir + "test.txt";
	printf("Open '%s'\n", filename.c_str());
	MAHandle file = maFileOpen(filename.c_str(), MA_ACCESS_READ_WRITE);
	if(file < 0) {
		printf("Error %i\n", file);
		return false;
	}
	int res = maFileExists(file);
	MAASSERT(res >= 0);
	if(res) {
		printf("File exists.\n");
	} else {
		printf("Creating file...\n");
		res = maFileCreate(file);
		if(res < 0) {
			printf("Error %i\n", res);
			return false;
		}
	}
	static const char data[] = "asfihu89ph4nma98fjioan9phadf89h239hdad9h89p\n";
	printf("Writing %lu bytes...\n", sizeof(data));
	res = maFileWrite(file, data, sizeof(data));
	if(res != 0) {
		printf("Write error %i\n", res);
		return false;
	}

	printf("Checking for old file...\n");
	if(!deleteIfExist(dir + "test2.txt"))
		return false;

	printf("Renaming...\n");
	res = maFileRename(file, "test2.txt");
	if(res != 0) {
		printf("Rename error %i\n", res);
		return false;
	}
	
	printf("Closing...\n");
	res = maFileClose(file);
	MAASSERT(res == 0);
	return true;
}

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;
	printf("Hello World!\n");
	bool res = writeAFile("");
	printf("Done, %i. Press 0 to exit.\n", res);
	FREEZE;
}
