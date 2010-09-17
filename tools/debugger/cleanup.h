#ifndef CLEANUP_H
#define CLEANUP_H

#include "Thread.h"

/**
 * This module will try to stop all active threads and close the 
 * event system. The reason why to put it into a separate module
 * is to have the clean up gathered in one place.
 * 
 * This is not a very elegant solution, and is a candidate for
 * refactoring.
 */
namespace cleanup {
	/**
	 * Makes sure the active threads are stopped before exit.
	 *
	 * @param userThread Thread that handles user input.
	 * @param remoteThread Thread that handles communication with MoRE.
	 */
	void init(MoSyncThread *userThread, MoSyncThread *remoteThread);
}


#endif