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

#include <queue>

#include "async.h"
#include "Thread.h"
#include "helpers.h"

using namespace std;

static MoSyncSemaphore sWait;
static MoSyncMutex sAccess;
static queue<DebuggerEvent*> sQueue;
static bool sIsWaiting = false;

void closeEventSystem() {
	sAccess.close();
}

void initEventSystem() {
	sAccess.init();
}

void getEvent(DebuggerEvent** pde) {
	sAccess.lock();
	_ASSERT(!sIsWaiting);
	while(sQueue.empty()) {
		sIsWaiting = true;
		sAccess.unlock();
		sWait.wait();
		sAccess.lock();

	}
	sIsWaiting = false;
	*pde = sQueue.front();
	sQueue.pop();
	sAccess.unlock();
}

void putEvent(DebuggerEvent* de) {
	sAccess.lock();
	sQueue.push(de);
	if(sIsWaiting)
		sWait.post();
	sAccess.unlock();
}
