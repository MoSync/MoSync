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

#ifndef CRITICAL_SECTION_H
#define CRITICAL_SECTION_H

#if defined (WIN32) || defined(_WIN32_WCE)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

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

#else	//linux

#include <pthread.h>

typedef pthread_mutex_t CRITICAL_SECTION;

void InitializeCriticalSection(CRITICAL_SECTION* cs);
void DeleteCriticalSection(CRITICAL_SECTION* cs);

class CriticalSectionHandler {
private:
	CRITICAL_SECTION* mCS;
public:
	CriticalSectionHandler(CRITICAL_SECTION* cs) : mCS(cs) {
		pthread_mutex_lock(mCS);
	}
	~CriticalSectionHandler() {
		pthread_mutex_unlock(mCS);
	}
};

#endif	//WIN32

#endif	//CRITICAL_SECTION_H
