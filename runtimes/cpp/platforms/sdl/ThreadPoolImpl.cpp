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
