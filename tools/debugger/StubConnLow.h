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

#ifndef STUBCONNLOW_H
#define STUBCONNLOW_H

#include <string>
#include "helpers/types.h"

typedef void (*AckCallback)();

//returns true if the packet was of the expected type.
typedef bool (*PacketCallback)(const char* data, int len);

//handles errors internally.
namespace StubConnLow {
	bool connect(const std::string& hostname, u16 port);	//synchronous

	//at most one packet can be sent at a time.
	void sendPacket(const char* str, AckCallback);

	//at most one packet can be expected at a time.
	void expectPacket(PacketCallback);

	void recvHandler(int result);

	//the interrupt message is a single byte that doesn't (afaik) get acknowledged.
	//this function therefore returns immediately.
	//the stub is expected to send a Stopped packet as a result of the interrupt.
	void sendInterrupt();

	bool isIdle();
}

#endif	//STUBCONNLOW_H
