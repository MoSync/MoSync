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

#include "ThreadPool.h"

using namespace MoSyncError;


class WorkerThread {
public:
	WorkerThread();
	~WorkerThread();

	bool isAvailable();
	void start(Runnable* r);
private:
	MoSyncThread mThread;
	MoSyncSemaphore mSem;
	Runnable* mR;
	bool mQuit;

	void run();
	static int homeRun(void*);
};

Runnable::~Runnable() {}

//*****************************************************************************
//ThreadPool
//*****************************************************************************

void ThreadPool::execute(Runnable* r) {
	WorkerThread* wt = NULL;
	for(uint i=0; i<mThreads.size(); i++) {
		WorkerThread* temp = mThreads[i];
		if(temp->isAvailable()) {
			wt = temp;
			break;
		}
	}
	if(wt == NULL) {	//no available threads; we must make another.
		wt = new WorkerThread();
		mThreads.push_back(wt);
	}
	wt->start(r);
}

//this will wait for all outstanding operations to complete. not so useful.
void ThreadPool::close() {
	LOGD("Closing %i threads.\n", mThreads.size());
	for(uint i=0; i<mThreads.size(); i++) {
		WorkerThread* wt = mThreads[i];
		delete wt;
	}
	mThreads.clear();
}

ThreadPool::~ThreadPool() {
	DEBUG_ASSERT(mThreads.size() == 0);	//make sure it's closed
}

//*****************************************************************************
//WorkerThread
//*****************************************************************************

#ifdef _MSC_VER
#pragma warning(disable:4355)
#endif

WorkerThread::WorkerThread() : mR(NULL), mQuit(false) {
	mThread.start(homeRun, this);
}

bool WorkerThread::isAvailable() {
	return mR == NULL;
}

void WorkerThread::start(Runnable* r) {
	//LOG("wtStart\n");
	DEBUG_ASSERT(isAvailable());
	mR = r;
	mSem.post();
}

WorkerThread::~WorkerThread() {
	mQuit = true;
	mSem.post();
	mThread.join();
}

int WorkerThread::homeRun(void* data) {
	WorkerThread* wt = (WorkerThread*)data;
	//LOGT("homeRun. Quit: 0x%x  mr:0x%x", wt->mQuit, wt->mR);
	wt->run();
	return 0;
}

void WorkerThread::run() {
	while(!mQuit) {
		do {
			mSem.wait();
			if(mQuit)
				return;
		} while(mR == NULL);

		LOGD("WTrun\n");
		mR->run();
		LOGD("WTend\n");
		delete mR;
		mR = NULL;
	}
}
