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
#include "iphone_helpers.h"

#include <base_errors.h>
using namespace MoSyncError;


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
	sem_init(&mSem, 0, 0);
}

MoSyncSemaphore::~MoSyncSemaphore() {
	sem_destroy(&mSem);
}

void MoSyncSemaphore::wait() {
	sem_wait(&mSem);
}

void MoSyncSemaphore::post() {
	sem_post(&mSem);
}
