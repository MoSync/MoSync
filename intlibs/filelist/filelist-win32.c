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

#include <windows.h>

#include "filelist.h"

int scanDirectory(const char* path, FileListCallback cb) {
	WIN32_FIND_DATA wfd;
	DWORD err;
	HANDLE h = FindFirstFile(path, &wfd);
	if(h == INVALID_HANDLE_VALUE) {
		return -1;
	}

	do {
		cb(wfd.cFileName);
	} while(FindNextFile(h, &wfd));

	err = GetLastError();
	if(!FindClose(h)) {
		return -2;
	}
	if(err != ERROR_NO_MORE_FILES) {
		return -4;
	}
	return 0;
}

int isDirectory(const char* filename) {
	DWORD res = GetFileAttributes(filename);
	if(res == INVALID_FILE_ATTRIBUTES)
		return -1;
	return (res & FILE_ATTRIBUTE_DIRECTORY) ? 1 : 0;
}
