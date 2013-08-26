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
