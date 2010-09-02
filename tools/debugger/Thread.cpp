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

#include "helpers.h"
#include "Thread.h"

//*****************************************************************************
//MoSyncThread
//*****************************************************************************

void MoSyncThread::start(int (SDLCALL * func)(void*), void* arg) {
	mThread = SDL_CreateThread(func, arg);
	_ASSERT(mThread);
}

void MoSyncThread::sleep(unsigned int ms) {
	SDL_Delay(ms);
}

int MoSyncThread::join() {
	_ASSERT(mThread);
	int result;
	SDL_WaitThread(mThread, &result);
	return result;
}

//*****************************************************************************
//MoSyncSemaphore
//*****************************************************************************

MoSyncSemaphore::MoSyncSemaphore() {
	mSem = SDL_CreateSemaphore(0);
	_ASSERT(mSem);
}

MoSyncSemaphore::~MoSyncSemaphore() {
	_ASSERT(mSem);
	SDL_DestroySemaphore(mSem);
}

void MoSyncSemaphore::wait() {
	int res = SDL_SemWait(mSem);
	_ASSERT(res == 0);
}

bool MoSyncSemaphore::tryWait(int timeout) {
	_ASSERT(timeout > 0);
	if(SDL_SemWaitTimeout(mSem, timeout) == 0) {
		return true;
	}
	else {
		return false;
	}

}

void MoSyncSemaphore::post() {
	int res = SDL_SemPost(mSem);
	_ASSERT(res == 0);
}

//*****************************************************************************
//MoSyncMutex
//*****************************************************************************

MoSyncMutex::MoSyncMutex() : mMutex(NULL) {}

void MoSyncMutex::init() {
	mMutex = SDL_CreateMutex();
	_ASSERT(mMutex);
}

MoSyncMutex::~MoSyncMutex() {
	_ASSERT(mMutex == NULL);	//make sure it's closed
}

void MoSyncMutex::close() {
	if(mMutex) {
		SDL_DestroyMutex(mMutex);
		mMutex = NULL;
	}
}

void MoSyncMutex::lock() {
	if(!mMutex) init();
	_ASSERT(SDL_mutexP(mMutex) == 0);
}

void MoSyncMutex::unlock() {
	_ASSERT(mMutex != NULL);
	_ASSERT(SDL_mutexV(mMutex) == 0);
}
