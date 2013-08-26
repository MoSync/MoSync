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

#include "config_platform.h"

#include <stdio.h>

#include <helpers/log.h>
#include <helpers/helpers.h>

#if 1//def LOGGING_ENABLED

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
