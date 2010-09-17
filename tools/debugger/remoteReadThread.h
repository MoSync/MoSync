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

#ifndef REMOTEREADTHREAD_H
#define REMOTEREADTHREAD_H

#include "bluetooth/connection.h"
#include "Thread.h"

/**
 * Continuously looks for available packets from MoRE, and notifies the
 * main program via the global event queue.
 *
 * @param arg is not used.
 * @return Should not return, but returns 0 when a connection error 
 *         has occurred.
 */
int SDLCALL remoteReadThreadFunc(void* arg);

/**
 * Notifies the remoteReadThread that there is a packet available from
 * MoRE. Which results in a eRecv event in the global event queue.
 *
 * @param conn Connection where the packet is available.
 * @param dst Buffer to store the packet.
 * @param maxLen Length of the buffer.
 */
void remoteRecv(Connection* conn, void* dst, int maxLen);

/**
 * Stops the read thread.
 */
void stopReadThread();

#endif	//REMOTEREADTHREAD_H
