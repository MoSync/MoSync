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

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

//hack
#define CONFIG_H
#define LOGGING_ENABLED

#define PTHREAD_ATTR_INIT_ERROR_STR  "pthread_mutexattr_init failed"
#define PTHREAD_ATTR_SET_ERROR_STR   "pthread_mutexattr_settype failed"
#define PTHREAD_MUTEX_INIT_ERROR_STR "pthread_mutex_init failed"

#define PTHREAD_ERROR(errorMessage, errorCode) { \
	char buffer[256]; \
	sprintf(buffer, "Critical section failed with message \"%s\" and error code: %d", errorMessage, errorCode); \
	logWithNSLog(buffer, strlen(buffer)); \
	pthread_exit(NULL); \
};

#include <helpers/log.h>
#include <helpers/helpers.h>
#include <helpers/CriticalSection.h>
#include "iphone_helpers.h"

using namespace MoSyncError;

#if !defined(_android)

static const char* sFilename = "log.txt";
timeval sStartTime;

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

	gettimeofday(&sStartTime, NULL);
}

void LogBin(const void* data, int size) {
	int res;
	InitLog();
	CriticalSectionHandler csh(&gLogCS);
	FILE* file = fopen(sFilename, "ab");	
	logWithNSLog((const char*)data, size);	
	res = fwrite(data, 1, size, file);
	fclose(file);
	if(res != size)
		MoSyncErrorExit(ERR_INTERNAL);
}

void LogV(const char* fmt, va_list args) {
	int res;
	InitLog();
	CriticalSectionHandler csh(&gLogCS);
	
	char tempString[1024];
	res = vsprintf(tempString, fmt, args);
	logWithNSLog(tempString, res);
	
	FILE* file = fopen(sFilename, "a");
	res = vfprintf(file, fmt, args);
	fclose(file);
	if(res < 0)
		MoSyncErrorExit(ERR_INTERNAL);
}

void LogTime() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	Log(" @ %li\n", ((tv.tv_sec - sStartTime.tv_sec) * 1000) + ((tv.tv_usec - sStartTime.tv_usec) / 1000));
}

void LogTime(const char* fmt, ...) {
	InitLog();
	CriticalSectionHandler csh(&gLogCS);
	va_list argptr;
	va_start(argptr, fmt);
	LogV(fmt, argptr);
	LogTime();
}

#endif // _android

void InitializeCriticalSection(CRITICAL_SECTION* cs) {
	pthread_mutexattr_t mutexattr;
	int returnCode;

	// Init mutex attribute
	returnCode = pthread_mutexattr_init(&mutexattr);
	if ( returnCode )
		PTHREAD_ERROR(PTHREAD_ATTR_INIT_ERROR_STR, returnCode);

	// Set the mutex as a recursive mutex
	returnCode = pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE);
	if ( returnCode )
		PTHREAD_ERROR(PTHREAD_ATTR_SET_ERROR_STR, returnCode);

	// Create the mutex with the attributes set
	returnCode = pthread_mutex_init(cs, &mutexattr);
	if ( returnCode )
		PTHREAD_ERROR(PTHREAD_MUTEX_INIT_ERROR_STR, returnCode);

	// After initializing the mutex, the thread attribute can be destroyed
	pthread_mutexattr_destroy(&mutexattr);
}

void DeleteCriticalSection(CRITICAL_SECTION* cs) {
	pthread_mutex_destroy(cs);
}

void EnterCriticalSection(CRITICAL_SECTION* cs) {
	pthread_mutex_lock(cs);
}

void LeaveCriticalSection(CRITICAL_SECTION* cs) {
	pthread_mutex_unlock(cs);
}

#if !defined(_android)

void failFunction() {
#ifdef __IPHONE__
__builtin_trap()
#endif
}

#endif // _android
