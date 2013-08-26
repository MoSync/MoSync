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
