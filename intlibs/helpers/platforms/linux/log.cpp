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

#include <helpers/log.h>
#include <helpers/helpers.h>
#include <helpers/CriticalSection.h>
using namespace MoSyncError;

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
	res = fwrite(data, 1, size, file);
	fclose(file);
	if(res != size)
		MoSyncErrorExit(ERR_INTERNAL);
}

void LogV(const char* fmt, va_list args) {
	int res;
	InitLog();
	CriticalSectionHandler csh(&gLogCS);
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

void InitializeCriticalSection(CRITICAL_SECTION* cs) {
	pthread_mutexattr_t mutexattr;
	// Set the mutex as a recursive mutex
	pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE);
	// create the mutex with the attributes set
	pthread_mutex_init(cs, &mutexattr);
	//After initializing the mutex, the thread attribute can be destroyed
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


void failFunction() {
#ifdef __IPHONE__
__builtin_trap()
#endif
}
