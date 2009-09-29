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

static const int N_GPR = 128;

struct Registers {
	u32 gpr[N_GPR];
	u32 pc;
};

//handles errors internally.
namespace StubConnection {
	typedef void (*AckCallback)();
	typedef void (*RegCallback)(const Registers& r);

	void addContinueListener(AckCallback);

	bool connect(const std::string& hostname, u16 port);	//synchronous

	void handleUnexpectedPacket(const char* data, int len);

	void execContinue(AckCallback);
	void step(AckCallback);
	void interrupt();

	void getRegisters(RegCallback);

	void readMemory(void* dst, int src, int len, AckCallback);

	//src may be freed after this function returns
	void writeMemory(int dst, const void* src, int len, AckCallback);

	bool isRunning();
	bool isIdle();

	//external callbacks
	void breakpointHit();
	void interruptHit();
	void stepHit();
	void exitHit(int code);
}

#endif	//STUBCONNECTION_H
