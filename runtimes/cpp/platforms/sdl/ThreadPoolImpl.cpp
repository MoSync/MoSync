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
using namespace MoSyncError;

//*****************************************************************************
//MoSyncThread
//*****************************************************************************

void MoSyncThread::start(int (SDLCALL * func)(void*), void* arg) {
	//LOGT("Creating thread");
	mThread = SDL_CreateThread(func, arg);
	DEBUG_ASSERT(mThread);
	//LOG("Created thread 0x%08X\n", mThread);
	//int dummy;
	//SDL_WaitThread(mThread, &dummy);
}

int MoSyncThread::join() {
	if(mThread) {
		int result;
		SDL_WaitThread(mThread, &result);
		return result;
	} else {
		LOG("Joining broken thread!\n");
		return 0;
	}
}

void MoSyncThread::sleep ( unsigned int ms ) {
    SDL_Delay( (Uint32) ms );
}

bool MoSyncThread::isCurrent() {
	if(mThread) {
		if(SDL_ThreadID() == SDL_GetThreadID(mThread))
			return true;
	}
	return false;
}

//*****************************************************************************
//MoSyncSemaphore
//*****************************************************************************

MoSyncSemaphore::MoSyncSemaphore() {
	mSem = SDL_CreateSemaphore(0);
	DEBUG_ASSERT(mSem);
	//LOG("Created semaphore 0x%08X\n", mSem);
}

MoSyncSemaphore::~MoSyncSemaphore() {
	if(mSem)
		SDL_DestroySemaphore(mSem);
}

void MoSyncSemaphore::wait() {
	DEBUG_ASRTZERO(SDL_SemWait(mSem));
}

void MoSyncSemaphore::post() {
	DEBUG_ASRTZERO(SDL_SemPost(mSem));
}
