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

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include "ThreadPoolImpl.h"

class Runnable {
public:
	virtual ~Runnable();
	virtual void run() = 0;
};

class WorkerThread;

class ThreadPool {
public:
	~ThreadPool();

	/// In a separate thread: calls Runnable::run(), then deletes \a r.
	void execute(Runnable* r);

	/// Waits until all Runnables passed to execute() has completed.
	void close();
private:
	std::vector<WorkerThread*> mThreads;
};

#endif	//THREADPOOL_H
