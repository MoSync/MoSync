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