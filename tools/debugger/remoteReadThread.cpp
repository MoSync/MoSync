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

#define CONFIG_H	//hack
#include "helpers/helpers.h"

#include "bluetooth/connection.h"

#include "Thread.h"
#include "async.h"
#include "helpers.h"
#include "remoteReadThread.h"

//******************************************************************************
// static declarations
//******************************************************************************

/* Maximum time to wait for the read semaphore */
#define READ_SEM_TIMEOUT 500

static MoSyncSemaphore sRead;
static Connection* sConn;
static void* sDst;
static int sMaxLen;
static bool run = true;

//******************************************************************************
// functions
//******************************************************************************

int SDLCALL remoteReadThreadFunc(void* arg) {
	//TODO: more error management, restart support.
	LOG("remoteReadThreadFunc start\n");
	while(run) {
		/* Wait for a while and test if we should keep running */
		if(!sRead.tryWait(READ_SEM_TIMEOUT)) {
			continue;
		}
		
		DebuggerEvent* de = new DebuggerEvent;
		de->type = DebuggerEvent::eRecv;
		int result = sConn->read(sDst, sMaxLen);
		de->result = result;
		//LOG("Remote read result: %i\n", result);
		putEvent(de);
		if(result < 0)
			break;
	}
	LOG("remoteReadThreadFunc end\n");
	return 0;
}

void remoteRecv(Connection* conn, void* dst, int maxLen) {
	sConn = conn;
	sDst = dst;
	sMaxLen = maxLen;
	sRead.post();
}

void stopReadThread() {
	run = false;
}
