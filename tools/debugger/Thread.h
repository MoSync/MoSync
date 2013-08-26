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
	 * Same as wait but with a timeout. 
	 *
	 * @param timeout The number of miliseconds to wait for the semaphore.
	 * @return True if the semaphore was acquired in timeout miliseconds,
	 *         false otherwise.
	 */
	bool tryWait(int timeout);

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
