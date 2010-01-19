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
#include <IX_FILE.h>

char buffer[256];

bool dumpFileList(const char* path) {
	MAHandle list = maFileListStart(path, "*");
	if(list < 0) {
		printf("Error %i\n", list);
		return false;
	}
	bool empty = true;
	while(maFileListNext(list, buffer, sizeof(buffer)) > 0) {
		MAUtil::String p2(path);
		p2 += buffer;
		MAUtil::String p3 = p2 + "\n";
		PrintConsole(p3.c_str());
		dumpFileList(p2.c_str());
		empty = false;
	}
	maFileListClose(list);
	printf("%s: %s\n", empty ? "Empty" : "Done", path);
	return !empty;
}

/**
* This object encapsulates a file list handle.
* It can be reused for multiple listings.
* \see maFileListStart()
*/
class FileLister {
public:
	FileLister() : mList(-1) {}
	~FileLister() { close(); }

	/**
	* \see maFileListStart()
	* Closes the active list, if any.
	*/
	int start(const char* dir, const char* filter = "*");

	/**
	* Writes the name of the next file in the list to \a dst.
	* \a dst's old contents are overwritten.
	* Returns the length of the name, or \< 0 on error.
	* On error, \a dst is not modified.
	*/
	int next(MAUtil::String& dst);

	/** \see maFileListClose() */
	void close();
private:
	MAHandle mList;
};

int FileLister::start(const char* dir, const char* filter) {
	close();
	return mList = maFileListStart(dir, filter);
}

int FileLister::next(MAUtil::String& dst) {
	int len = maFileListNext(mList, NULL, 0);
	if(len < 0)
		return len;
	dst.resize(len);
	len = maFileListNext(mList, dst.pointer(), len+1);
	MAASSERT(len > 0);
	return len;
}

void FileLister::close() {
	if(mList > 0) {
		int res = maFileListClose(mList);
		MAASSERT(res == 0);
		mList = -1;
	}
}

static void writeAFile() {
	// find a root path
	FileLister fl;
	int res = fl.start("");
	if(res < 0) {
		printf("Error %i\n", res);
		return;
	}
	MAUtil::String root;
	res = fl.next(root);
	if(res < 0) {
		printf("Error %i\n", res);
		return;
	}
	printf("Root: '%s'\n", root.c_str());
	
	MAUtil::String filename = root + "test.txt";
	printf("Open '%s'\n", filename.c_str());
	MAHandle file = maFileOpen(filename.c_str(), MA_ACCESS_READ_WRITE);
	if(file < 0) {
		printf("Error %i\n", file);
		return;
	}
	res = maFileExists(file);
	MAASSERT(res >= 0);
	if(res) {
		printf("File exists.\n");
	} else {
		printf("Creating file...\n");
		res = maFileCreate(file);
		if(res < 0) {
			printf("Error %i\n", res);
			return;
		}
	}
	static const char data[] = "asfihu89ph4nma98fjioan9phadf89h239hdad9h89p\n";
	printf("Writing %lu bytes...\n", sizeof(data));
	res = maFileWrite(file, data, sizeof(data));
	MAASSERT(res == 0);
	printf("Closing...\n");
	res = maFileClose(file);
	MAASSERT(res == 0);
	printf("Done.\n");
}

extern "C" int MAMain() {
	printf("Hello World!\n");
	writeAFile();
	FREEZE;
}
