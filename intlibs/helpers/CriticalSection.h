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

#ifndef CRITICAL_SECTION_H
#define CRITICAL_SECTION_H

#if defined (WIN32) || defined(_WIN32_WCE)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#else	//linux

#include <pthread.h>

typedef pthread_mutex_t CRITICAL_SECTION;

void InitializeCriticalSection(CRITICAL_SECTION* cs);
void DeleteCriticalSection(CRITICAL_SECTION* cs);
void EnterCriticalSection(CRITICAL_SECTION* cs);
void LeaveCriticalSection(CRITICAL_SECTION* cs);

#endif	//WIN32

class CriticalSectionHandler {
public:
	CriticalSectionHandler(CRITICAL_SECTION* cs) : mCS(cs) {
		EnterCriticalSection(mCS);
	}
	~CriticalSectionHandler() {
		LeaveCriticalSection(mCS);
	}
private:
	CRITICAL_SECTION* mCS;
};

#endif	//CRITICAL_SECTION_H
