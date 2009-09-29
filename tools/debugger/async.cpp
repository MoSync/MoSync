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

#include <queue>

#include "async.h"
#include "Thread.h"
#include "helpers.h"

using namespace std;

static MoSyncSemaphore sWait, sAccess;
static queue<DebuggerEvent*> sQueue;
static bool sIsWaiting = false;

void initEventSystem() {
	sAccess.post();
}

void getEvent(DebuggerEvent** pde) {
	sAccess.wait();
	_ASSERT(!sIsWaiting);
	while(sQueue.empty()) {
		sIsWaiting = true;
		sAccess.post();
		sWait.wait();
		sAccess.wait();
	}
	sIsWaiting = false;
	*pde = sQueue.front();
	sQueue.pop();
	sAccess.post();
}

void putEvent(DebuggerEvent* de) {
	sAccess.wait();
	sQueue.push(de);
	if(sIsWaiting)
		sWait.post();
	sAccess.post();
}
