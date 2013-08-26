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
