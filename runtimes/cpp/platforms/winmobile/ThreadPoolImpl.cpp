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

#include "config_platform.h"
#include <helpers/helpers.h>

#include "ThreadPoolImpl.h"

#include <base_errors.h>
using namespace MoSyncError;
#include "wce_errors.h"


//*****************************************************************************
//MoSyncThread
//*****************************************************************************

void MoSyncThread::start(int (*func)(void*), void* arg) {
	mThread = CreateThread(NULL, 
		0, // ignored
		reinterpret_cast<LPTHREAD_START_ROUTINE>(func),
		this,
		0,
		NULL);
	GLE(mThread);
}

int MoSyncThread::join() {
	if(mThread) {
		DWORD result;
		//SDL_WaitThread(mThread, &result);
		if(WaitForSingleObject(mThread, INFINITE) == WAIT_FAILED) {
			GLE(0);
		}
		GLE(GetExitCodeThread(mThread, &result));
		return (int)result;
	} else {
		LOG("Joining broken thread!\n");
		return 0;
	}
}

void MoSyncThread::sleep ( unsigned int ms ) {
    Sleep( (DWORD) ms );
}

//*****************************************************************************
//MoSyncSemaphore
//*****************************************************************************

MoSyncSemaphore::MoSyncSemaphore() {
	mSem = CreateSemaphore(
			NULL,
			0, 
			32*1024, 
			NULL);

	GLE(mSem);
}

MoSyncSemaphore::~MoSyncSemaphore() {
	if(mSem) {
		GLE(CloseHandle(mSem));
		mSem = NULL;
	}
}

void MoSyncSemaphore::wait() {
	if(WaitForSingleObject(mSem, INFINITE) == WAIT_FAILED) {
		GLE(0);
	}
}

void MoSyncSemaphore::post() {
	GLE(ReleaseSemaphore(mSem, 1, NULL));
}
