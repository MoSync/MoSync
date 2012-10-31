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
#include <stdio.h>
#include <stdlib.h>
#include "filelist.h"

#ifdef _WIN32_WCE
	#define PATH_TYPE WCHAR*
#else
	#define PATH_TYPE char*
#endif

static void getFileTime(const PATH_TYPE path, FILETIME* ft);


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

int compareTime(const char* file1, const char* file2) {
#ifdef _WIN32_WCE
	WCHAR tfn1[MAX_PATH];
	WCHAR tfn2[MAX_PATH];
	convertAsciiToUnicode(tfn1, MAX_PATH, file1);
	convertAsciiToUnicode(tfn2, MAX_PATH, file2);
#else
	const char* tfn1 = file1;
	const char* tfn2 = file2;
#endif
	FILETIME ft1, ft2;
	int hDiff, lDiff;

	getFileTime(tfn1, &ft1);
	getFileTime(tfn2, &ft2);

	hDiff = ft1.dwHighDateTime - ft2.dwHighDateTime;
	lDiff = ft1.dwLowDateTime - ft2.dwLowDateTime;
	if (hDiff) {
		return hDiff;
	} else {
		return lDiff;
	}
}

static void getFileTime(const PATH_TYPE path, FILETIME* ft) {
	HANDLE handle;
	handle = CreateFile(path, 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (handle == INVALID_HANDLE_VALUE || !GetFileTime(handle, NULL, NULL, ft)) {
		ft->dwLowDateTime = 0;
		ft->dwHighDateTime = 0;
    }
	CloseHandle(handle);
}

#ifndef _WIN32_WCE
char* fullpath(const char* name) {
	return _fullpath(NULL, name, 0);
}
#endif
