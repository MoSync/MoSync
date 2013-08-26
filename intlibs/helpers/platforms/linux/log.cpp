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

	FILE* file = fopen(sFilename, "wb");
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

#endif // _android

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

#if !defined(_android)

void failFunction() {
#ifdef __IPHONE__
__builtin_trap()
#endif
}

#endif // _android
