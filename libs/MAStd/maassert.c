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

#include "ma.h"
#include "mastring.h"
#include "conprint.h"
#include "maassert.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

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
