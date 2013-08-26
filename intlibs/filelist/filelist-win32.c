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
