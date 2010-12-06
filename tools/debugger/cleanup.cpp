/* Copyright (C) 2010 MoSync AB

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

#include <cstdlib>

#include "cleanup.h"

#include "async.h"
#include "remoteReadThread.h"
#include "userInputThread.h"
#include "Thread.h"

namespace cleanup {
	/**
	 * Stops the running threads and closes the event system.
	 */
	static void cleanup();
	
	
	/**
	 * Threads to stop.
	 */
	static MoSyncThread *s_userThread = NULL;
	static MoSyncThread *s_remoteThread = NULL;
	static bool s_isInitalized = false;
	
	void init(MoSyncThread *userThread, MoSyncThread *remoteThread) {
		if(userThread == NULL || remoteThread == NULL) {
			throw std::invalid_argument("cleanup::init(): userThread or remoteThread is NULL.");
		}
		
		if(s_isInitalized) {
			throw std::logic_error("cleanup::init(): cleanup is already initialized.");
		}
		
		s_isInitalized = true; 
		
		s_userThread = userThread;
		s_remoteThread = remoteThread;
		
		/* Make sure threads are stopped when program is about to exit, and
		 * before the destructors are called. */
		std::atexit(&cleanup);
	}
	
	static void cleanup() {
		stopUserThread();
		stopReadThread();
		s_userThread->join();
		s_remoteThread->join();
		
		/* The threads MUST be stopped before the event system is
		 * closed, since the threads continously access the event 
		 * system. */
		closeEventSystem();
	}
	
	
}