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

#ifndef STUBCONNECTION_H
#define STUBCONNECTION_H

#include <string>

#include "helpers/types.h"

/**
 * Number of general purpose registers.
 */
static const int N_GPR = 128;

/**
 * Struct meant to hold the value of different registers.
 */
struct Registers {
	u32 gpr[N_GPR];
	u32 pc;
};

const Registers& getReg();
bool isRegValid();

#define CHECK_REG if(!isRegValid()) { error("Inferior is not ready"); return; }

#define NEED_REG \
	CHECK_REG;\
	const Registers& r(getReg());

#define ASSERT_REG \
	_ASSERT(isRegValid());\
	const Registers& r(getReg());

//handles errors internally.
namespace StubConnection {
	typedef void (*AckCallback)();

	/**
	 * Adds a continue listener that will be called when the program is
	 * stepped or continued.
	 *
	 * @param cb Function that will be called when the program is stepped or
	 *           continued.
	 */
	void addContinueListener(AckCallback cb);

	/**
	 * Makes a TCP connection to the given host on the specified port.
	 *
	 * @param hostname Hostname as a symbolic name or ip address.
	 * @param port The port to connect on.
	 * @param cb Function that will be called when the connection is 
	 *           established.
	 */
	void connect(const std::string& hostname, u16 port, AckCallback cb);

	/**
	 * Handles unexpected packets
	 *
	 * @param data Contains the data of the packet.
	 * @param len Length of the packet.
	 */
	void handleUnexpectedPacket(const char* data, int len);

	/**
	 * Sends a message to MoRE telling it to continue the execution.
	 *
	 * @param cb Function that will be called once the execution is continued.
	 */
	void execContinue(AckCallback cb);

	/**
	 * Sends a message to MoRE telling it to step one line.
	 *
	 * @param cb Will be called once MoRE has executed the step.
	 */
	void step(AckCallback cb);

	/**
	 * Sends a message to MoRE telling it to stop the execution.
	 */
	void interrupt();

	/**
	 * Sends a request to MoRE telling it to read memory at a specific 
	 * address. The callback function is then called when MoRE has provided
	 * the memory values.
	 *
	 * @param dst Buffer to store the memory values.
	 * @param src Start address of the memory to read.
	 * @param len Length of the memory to read in bytes.
	 * @param cb Called when the memory values has been received from MoRE.
	 */
	void readMemory(void* dst, int src, int len, AckCallback cb);

	//src may be freed after this function returns
	/**
	 * Sends a request to MoRE telling it write the code memory at a specific
	 * address.
	 *
	 * @param dst The address in the code memory to write to.
	 * @param src An array of values to write to the memory. 
	 * @param len Length of the array to write in bytes.
	 * @param cb Function called when the values has been written to memory.
	 */
	void writeCodeMemory(int dst, const void* src, int len, AckCallback cb);

	/**
	 * Sends a request to MoRE telling it to write the data memory at a
	 * specific address.
	 *
	 * @param dst The address in the data memory to write to.
	 * @param src An array of values to write to the memory. 
	 * @param len Length of the array to write in bytes.
	 * @param cb Function called when the values has been written to memory.
	 */
	void writeDataMemory(int dst, const void* src, int len, AckCallback cb);

	/**
	 * Returns true if we are currently sending a packet.
	 *
	 * @return 
	 */
	bool isRunning();

	/**
	 * Determines if we are currently sending a packet or waiting for a 
	 * response.
	 *
	 * @return true if we are currently not sending a packet and not waiting
	 *              for a response, false otherwise.
	 */
	bool isIdle();

	/**
	 * Responds to the GDB session that a breakpoint has been reached.
	 */
	void breakpointHit();

	/**
	 * Responds to the GDB session that an interrupt has been performed.
	 */
	void interruptHit();

	/**
	 * Responds to the GDB session that a step has been performed.
	 */
	void stepHit();

	/**
	 * Responds to the GDB session that the program being debugged has exited.
	 */
	void exitHit(int code);
}

#endif	//STUBCONNECTION_H
