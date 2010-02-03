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
