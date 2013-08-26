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
#include "iphone_helpers.h"

#include <base_errors.h>
using namespace MoSyncError;

#include <errno.h>


//*****************************************************************************
//MoSyncThread
//*****************************************************************************
static void* wrapper(void *arg) {
	MoSyncInternalThread* thread = (MoSyncInternalThread*)arg;
	return (void*)thread->func(thread->arg);
}

void MoSyncThread::start(int (*func)(void*), void* arg) {
	mThread.func = func;
	mThread.arg = arg;
	if(pthread_create(&mThread.thread, NULL, wrapper, &mThread)!=0) {
		DEBIG_PHAT_ERROR;
	}
}

int MoSyncThread::join() {
	void *ret;
	if(pthread_join(mThread.thread, &ret)!=0) {
		DEBIG_PHAT_ERROR;
	}
	return (int)ret;
}

void MoSyncThread::sleep ( unsigned int ms ) {
    sleepMillis(ms);
}

//*****************************************************************************
//MoSyncSemaphore
//*****************************************************************************

MoSyncSemaphore::MoSyncSemaphore() {
//	sem_init(&mSem, 0, 0);
	semaphore_create(current_task(), &mSem, SYNC_POLICY_FIFO, 0);
}

MoSyncSemaphore::~MoSyncSemaphore() {
//	sem_destroy(&mSem);
	semaphore_destroy(current_task(), mSem);
}

void MoSyncSemaphore::wait() {
//	sem_wait(&mSem);
   semaphore_wait(mSem);
}

void MoSyncSemaphore::post() {
//	sem_post(&mSem);
    semaphore_signal(mSem);
}
