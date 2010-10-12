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