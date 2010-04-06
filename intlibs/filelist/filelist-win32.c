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

#ifdef _WIN32_WCE
#include <helpers/smartie.h>
#include "wce_helpers.h"
#endif

#include <windows.h>
#include "filelist.h"

int scanDirectory(const char* path, FileListCallback cb) {
#ifdef _WIN32_WCE
	WCHAR tpath[MAX_PATH];
	convertAsciiToUnicode(tpath, MAX_PATH, path);
#else
	const char* tpath = path;
#endif

	WIN32_FIND_DATA wfd;
	DWORD err;
	HANDLE h = FindFirstFile(tpath, &wfd);
	if(h == INVALID_HANDLE_VALUE) {
		return -1;
	}

	do {
#ifdef _WIN32_WCE
		char tfn[MAX_PATH];
		convertUnicodeToAscii(tfn, MAX_PATH, wfd.cFileName);
#else
		const char* tfn = wfd.cFileName;
#endif
		cb(tfn);
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
#ifdef _WIN32_WCE
	WCHAR tfn[MAX_PATH];
	convertAsciiToUnicode(tfn, MAX_PATH, filename);
#else
	const char* tfn = filename;
#endif

	DWORD res = GetFileAttributes(tfn);
	if(res == (DWORD)-1)
		return -1;
	return (res & FILE_ATTRIBUTE_DIRECTORY) ? 1 : 0;
}
