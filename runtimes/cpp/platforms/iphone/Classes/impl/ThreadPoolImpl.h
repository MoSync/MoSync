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

#ifndef THREADPOOLIMPL_H
#define THREADPOOLIMPL_H

#include <pthread.h>
#include <mach/semaphore.h>
#include <mach/task.h>
#include <mach/mach_init.h>

typedef int (*MoSyncThreadFunc)(void*);

struct MoSyncInternalThread {
	MoSyncThreadFunc func;
	void *arg;
	pthread_t thread;
	
};

class MoSyncThread {
public:
	void start(int (*func)(void*), void* arg);
	int join();
        static void sleep ( unsigned int ms );
private:
	MoSyncInternalThread mThread;
};

class MoSyncSemaphore {
public:
	MoSyncSemaphore();
	~MoSyncSemaphore();
	void wait();
	void post();
private:
	
	//sem_t mSem;
	semaphore_t mSem;
	
};

#endif	//THREADPOOLIMPL_H
