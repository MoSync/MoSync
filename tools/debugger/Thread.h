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

#ifndef THREAD_H
#define THREAD_H

#include <SDL/SDL.h>

/**
 * Defines a simple thread class.
 */
class MoSyncThread {
public:
	/**
	 * Makes a thread of the given function and starts it.
	 *
	 * @param func A function that will be executed as a thread.
	 * @param arg Arguments passed to the function.
	 */
	void start(int (SDLCALL * func)(void*), void* arg);

	/**
	 * Sleep for the given number of milliseconds.
	 *
	 * @param ms Number of milliseconds to sleep.
	 */
	static void sleep(unsigned int ms);

	/**
	 * Waits for the given thread to stop.
	 *
	 * @return The returned value of the thread function.
	 */
	int join();
private:

	/**
	 * The underlying thread implementation.
	 */
	SDL_Thread* mThread;
};

class MoSyncSemaphore {
public:
	/**
	 * Creates a semaphore with an initial value of 0.
	 */
	MoSyncSemaphore();

	/**
	 * Destroys the underlying semaphore.
	 */
	~MoSyncSemaphore();

	/**
	 * Waits (blocking) until the semaphore has a value of at least 1, and then
	 * reduces it by 1.
	 */
	void wait();

	/**
	 * Increases the value of the semaphore by 1.
	 */
	void post();
private:

	/**
	 * The underlying semaphore implementation.
	 */
	SDL_sem* mSem;
};

class MoSyncMutex {
public:
	MoSyncMutex();
	~MoSyncMutex();

	/**
	 * Initializes the mutex, must be called before any call to lock or
	 * unlock.
	 */
	void init();

	/**
	 * Locks the mutex.
	 */
	void lock();

	/**
	 * Unlocks the mutex.
	 */
	void unlock();

	/**
	 * Closes the mutex, must be called before destruction.
	 */
	void close();

private:
	/**
	 * 
	 */
	SDL_mutex* mMutex;
};

/**
 * Defines a handler for a mutex that locks the mutex at construction and 
 * unlocks the mutex at destruction.
 */
class MoSyncMutexHandler {
public:
	/**
	 * Locks the given mutex.
	 *
	 * @param mutex Mutex to handle.
	 */
	MoSyncMutexHandler(MoSyncMutex* mutex) : mMutex(mutex) {
		mMutex->lock();
	}

	/**
	 * Unlocks the mutex.
	 */
	~MoSyncMutexHandler() {
		mMutex->unlock();
	}

private:
	/**
	 * Mutex that will is handled.
	 */
	MoSyncMutex* mMutex;
};

#endif	//THREAD_H
