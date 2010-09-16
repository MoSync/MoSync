/* Copyright (C) 2010 Mobile Sorcery AB

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

#include "config_platform.h"

#include <stdio.h>

#include <helpers/log.h>
#include <helpers/helpers.h>

#ifdef LOGGING_ENABLED

static const char* sFilename = "log.txt";

void InitLog(const char* filenameOverride) {
/*	static bool done = false;
	if(done)
		return;
	done = true;

	if(filenameOverride != NULL) {
		sFilename = filenameOverride;
	}

	FILE* file = fopen(sFilename, "w");
	if(!file)
		MoSyncErrorExit(MoSyncError::ERR_INTERNAL);
	fclose(file);
*/
/*	
	InitializeCriticalSection(&gLogCS);

	DEBUG_ASSERT(QueryPerformanceFrequency(&sFrequency));
	Log("PerformanceFrequency: %I64i\n", sFrequency.QuadPart);
	DEBUG_ASSERT(QueryPerformanceCounter(&sStartCount));
	Log("StartCount: %I64i\n", sStartCount.QuadPart);
*/	
}

void LogV(const char* fmt, va_list args) {
/*
	int res;
	InitLog();
//	EnterCriticalSection(&gLogCS);
	{
		FILE* file = fopen(sFilename, "a");
		if(!file) {
			res = -1;
		} else {
			res = vfprintf(file, fmt, args);
			fclose(file);
		}
	}
//	LeaveCriticalSection(&gLogCS);

	if(res < 0)
		MoSyncErrorExit(MoSyncError::ERR_INTERNAL);
*/
}

#endif
