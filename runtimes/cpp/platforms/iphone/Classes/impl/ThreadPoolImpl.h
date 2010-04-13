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
