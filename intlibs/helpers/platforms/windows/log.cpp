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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
//#include <fstream>

//hack
#define CONFIG_H
#define LOGGING_ENABLED

#include <helpers/log.h>
#include <helpers/helpers.h>
using namespace MoSyncError;

static const char* sFilename = "log.txt";
static LARGE_INTEGER sFrequency;
static LARGE_INTEGER sStartCount;

CRITICAL_SECTION gLogCS;

void InitLog(const char* filenameOverride) {
	static bool done = false;
	if(done)
		return;
	done = true;

	if(filenameOverride != NULL) {
		sFilename = filenameOverride;
	}

	FILE* file = fopen(sFilename, "w");
	if(!file)
		MoSyncErrorExit(ERR_INTERNAL);
	fclose(file);
	InitializeCriticalSection(&gLogCS);

	DEBUG_ASSERT(QueryPerformanceFrequency(&sFrequency));
	Log("PerformanceFrequency: %I64i\n", sFrequency.QuadPart);
	DEBUG_ASSERT(QueryPerformanceCounter(&sStartCount));
	Log("StartCount: %I64i\n", sStartCount.QuadPart);
}

void LogBin(const void* data, int size) {
	int res;
	InitLog();
	EnterCriticalSection(&gLogCS);
	{
#ifdef _WIN32_WCE
		FILE* file = fopen(sFilename, "ab");
		res = fwrite(data, 1, size, file);
		fclose(file);
#else
#if 0
		std::ofstream file(sFilename, std::ios_base::out | std::ios_base::app | std::ios_base::binary);
		file.write((const char*)data, size);
		if(file.good()) {
			res = size;
		} else {
			res = -1;
		}
#endif
		res = -1;
		HANDLE hFile = CreateFile(sFilename, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL, NULL);
		if(hFile != INVALID_HANDLE_VALUE) {
			DWORD dRes;
			dRes = SetFilePointer(hFile, 0, NULL, FILE_END);
			if(dRes != INVALID_SET_FILE_POINTER) {
				DWORD writ;
				dRes = WriteFile(hFile, data, size, &writ, NULL);
				if(dRes != 0 && (int)writ == size) {
					res = size;
				}
			}
			CloseHandle(hFile);
		}
#endif
	}
	LeaveCriticalSection(&gLogCS);

	if(res != size)
		MoSyncErrorExit(ERR_INTERNAL);
}

void LogV(const char* fmt, va_list args) {
	int res;
	InitLog();
	EnterCriticalSection(&gLogCS);
	{
		FILE* file = fopen(sFilename, "a");
		if(!file) {
			res = -1;
		} else {
			res = vfprintf(file, fmt, args);
			fclose(file);
		}
	}
	LeaveCriticalSection(&gLogCS);

	if(res < 0)
		MoSyncErrorExit(ERR_INTERNAL);
}

void LogTime() {
	LARGE_INTEGER counter;
	DEBUG_ASSERT(QueryPerformanceCounter(&counter));
	double delta = double(counter.QuadPart - sStartCount.QuadPart);
	Log(" @ %i\n", int((delta * 1000) / sFrequency.QuadPart));
}

void LogTime(const char* fmt, ...) {
	InitLog();
	EnterCriticalSection(&gLogCS);
	{
		va_list argptr;
		va_start(argptr, fmt);
		LogV(fmt, argptr);
		LogTime();
	}
	LeaveCriticalSection(&gLogCS);
}

void failFunction() {
}
