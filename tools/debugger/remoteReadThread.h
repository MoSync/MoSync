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
