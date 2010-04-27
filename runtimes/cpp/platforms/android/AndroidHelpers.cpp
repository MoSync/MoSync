#include "config_platform.h"

#include <stdio.h>

#include <helpers/log.h>
#include <helpers/helpers.h>

static const char* sFilename = "log.txt";

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
		MoSyncErrorExit(MoSyncError::ERR_INTERNAL);
	fclose(file);
/*	
	InitializeCriticalSection(&gLogCS);

	DEBUG_ASSERT(QueryPerformanceFrequency(&sFrequency));
	Log("PerformanceFrequency: %I64i\n", sFrequency.QuadPart);
	DEBUG_ASSERT(QueryPerformanceCounter(&sStartCount));
	Log("StartCount: %I64i\n", sStartCount.QuadPart);
*/	
}

void LogV(const char* fmt, va_list args) {
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
}
