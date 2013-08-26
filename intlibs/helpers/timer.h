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

#ifndef TIMER_H
#define TIMER_H

#include "log.h"

#if defined(_WIN32)
#include <windows.h>
#else
#include <stdint.h>
#include <sys/time.h>
typedef union _LARGE_INTEGER {
	struct {
		uint32_t LowPart;
		int32_t HighPart;
	}   ;
	struct {
		uint32_t LowPart;
		int32_t HighPart;
	}   u;
	int64_t QuadPart;
} LARGE_INTEGER;
#endif

class ProfTime {
private:
	LARGE_INTEGER li;
public:
	static ProfTime now() {
		ProfTime n;
#ifdef _WIN32
		DEBUG_ASSERT(QueryPerformanceCounter(&n.li));
#else
		struct timeval tv;
		DEBUG_ASSERT(!gettimeofday(&tv, NULL));
		n.li.QuadPart = (int64_t) tv.tv_sec * 1000000 + (int64_t) tv.tv_usec;
#endif
		return n;
	}
	ProfTime() {
		li.QuadPart = 0;
	}
	ProfTime(const ProfTime& other) : li(other.li) {}
	ProfTime& operator+=(const ProfTime& other) {
		li.QuadPart += other.li.QuadPart;
		return *this;
	}
	ProfTime operator-(const ProfTime& other) {
		ProfTime t;
		t.li.QuadPart = li.QuadPart - other.li.QuadPart;
		return t;
	}
	double toMilliSeconds() {
#ifdef _WIN32
		LARGE_INTEGER freq;
		DEBUG_ASSERT(QueryPerformanceFrequency(&freq));
		return double(li.QuadPart) * 1000 / double(freq.QuadPart);
#else
		return li.QuadPart / 1000;
#endif
	}
};

// Measures time from construction to destruction, and logs it.
class Timer {
public:
	Timer(const char* name) : mName(name), mStart(ProfTime::now()) {}
	~Timer() {
		ProfTime elapsed = ProfTime::now() - mStart;
		LOG("%s: %f ms\n", mName, elapsed.toMilliSeconds());
	}
private:
	const char* mName;
	ProfTime mStart;
};

#endif	//TIMER_H
