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
