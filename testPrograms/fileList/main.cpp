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
		if(p2[p2.size()-1] == '/')
			dumpFileList(p2.c_str());
		empty = false;
	}
	maFileListClose(list);
	//printf("%s: %s\n", empty ? "Empty" : "Done", path);
	return !empty;
}

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;

	dumpFileList("");
	/*MAUtil::String path;
	do {
		path += buffer;
	} while(dumpFileList(path.c_str()));*/
	
	FREEZE;
}
