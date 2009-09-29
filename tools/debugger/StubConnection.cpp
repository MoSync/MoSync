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

#include <string.h>
#include <stdlib.h>
#include <vector>

#include "config.h"
#include "helpers/log.h"
#include "helpers/smartie.h"

#include "GdbCommon.h"

#include "StubConnection.h"
#include "StubConnLow.h"
#include "helpers.h"

#include "memory.h"

using namespace std;

//******************************************************************************
// statics
//******************************************************************************

static StubConnection::RegCallback sGetRegistersCallback;
static StubConnection::AckCallback sReadMemoryCallback;
static int sReadMemoryLen;
static byte* sReadMemoryDst;
static StubConnection::AckCallback sWriteMemoryCallback;
static vector<StubConnection::AckCallback> sContinueListeners;
static vector<StubConnection::AckCallback> sStopListeners;
static bool sRunning = false;
static Registers sCachedReg;
static bool sCachedRegValid = false;
static bool sIdle = true;

static void stepAck();
static void getRegistersAck();
static bool getRegistersPacket(const char* data, int len);
static void readMemoryAck();
static bool readMemoryPacket(const char* data, int len);
static void writeMemoryAck();
static bool writeMemoryPacket(const char* data, int len);

static bool asyncPacket(const char* data, int len);

static void cacheContinue();

static void unIdle();
static void setIdle();

//******************************************************************************
// small fries
//******************************************************************************

void StubConnection::addContinueListener(AckCallback ac) {
	sContinueListeners.push_back(ac);
}

static void callContinueListeners() {
	sRunning = true;
	LOG("sRunning = true;\n");
	for(size_t i=0; i<sContinueListeners.size(); i++) {
		sContinueListeners[i]();
	}
}

bool StubConnection::isRunning() {
	return sRunning || !isIdle();
}

bool StubConnection::connect(const std::string& hostname, u16 port) {
	static bool first = true;
	if(first) {
		addContinueListener(cacheContinue);
		first = false;
	}
	return StubConnLow::connect(hostname, port);
}

void StubConnection::handleUnexpectedPacket(const char* data, int len) {
	//eprintf("Unexpected packet, %i bytes:\n%s\n", len, data);
	if(!asyncPacket(data, len))
		oprintf("@%s\n", data);
}

void StubConnection::execContinue(AckCallback cb) {
	if(sRunning) {
		error("Already running");
		return;
	}
	callContinueListeners();
	StubConnLow::sendPacket("c", cb);
}

void StubConnection::step(AckCallback cb) {
	if(sRunning) {
		error("Already running");
		return;
	}
	callContinueListeners();
	StubConnLow::sendPacket("s", cb);
}

void StubConnection::interrupt() {
	if(!sRunning) {
		error("Not running");
		return;
	}
	unIdle();
	StubConnLow::sendInterrupt();
}

//returns true if this is an error packet. in that case, also report the error.
static bool checkErrorPacket(const char* data, int len) {
	if(len != 4)
		return false;
	if(data[0] != 'E')
		return false;
	if(data[1] != ' ')
		return false;
	char buffer[128];
	sprintf(buffer, "Stub reported error '%s'\n", data + 2);
	error(buffer);
	return true;
}

static void unIdle() {
	if(!StubConnection::isIdle()) {
		error("Connection busy!");
		return;
	}
	sIdle = false;
	LOG("sIdle = false;\n");
}

static void setIdle() {
	_ASSERT(!sIdle);
	sIdle = true;
	LOG("sIdle = true;\n");
}

bool StubConnection::isIdle() {
	return sIdle && StubConnLow::isIdle();
}

//******************************************************************************
// asyncPacket
//******************************************************************************

static bool asyncPacket(const char* data, int len) {
	if(len != 3)
		return false;
	//todo: fix code dupes
	if(strcmp(data, "S01") == 0) {	//breakpoint
		sRunning = false;
		StubConnection::breakpointHit();
		return true;
	}
	if(strcmp(data, "S02") == 0) {	//interrupt
		sRunning = false;
		StubConnection::interruptHit();
		return true;
	}
	if(strcmp(data, "S03") == 0) {	//step
		sRunning = false;
		StubConnection::stepHit();
		return true;
	}
	if(data[0] == 'W') {	//exit
		sRunning = false;
		int code = strtoul(data + 1, NULL, 16);
		StubConnection::exitHit(code);
		return true;
	}
	return false;
}

//******************************************************************************
// getRegisters
//******************************************************************************

void StubConnection::getRegisters(RegCallback cb) {
	if(sRunning) {
		error("Inferior is running");
		return;
	}
	if(sCachedRegValid) {
		cb(sCachedReg);
		return;
	}
	sGetRegistersCallback = cb;
	unIdle();
	StubConnLow::sendPacket("g", getRegistersAck);
}
static void getRegistersAck() {
	StubConnLow::expectPacket(getRegistersPacket);
}
static bool getRegistersPacket(const char* data, int len) {
	//eprintf("Recieved packet, %i bytes (we want %i): '%s'\n",
		//len, sizeof(Registers) * 2, data);
	if(checkErrorPacket(data, len))
		return true;
	//since there's no identifier,
	//we have to assume that a packet of the right size is our reply.
	if(len != sizeof(Registers) * 2)
		return false;
	//we can also check that there are no non-hex characters
	for(int i=0; i<len; i++) {
		if(!isxdigit(data[i]))
			return false;
	}
	//parse GPRs
	for(int i=0; i<N_GPR; i++) {
		char buf[12];
		memcpy(buf, data + i*8, 8);
		buf[8] = 0;
		sCachedReg.gpr[i] = strtoul(buf, NULL, 16);
	}
	{	//parse PC
		char buf[12];
		memcpy(buf, data + N_GPR*8, 8);
		buf[8] = 0;
		sCachedReg.pc = strtoul(buf, NULL, 16);
	}
	sCachedRegValid = true;
	setIdle();
	sGetRegistersCallback(sCachedReg);
	return true;
}

static void cacheContinue() {
	sCachedRegValid = false;
	clearMemoryCacheBits();
}

//******************************************************************************
// readMemory
//******************************************************************************

void StubConnection::readMemory(void* dst, int src, int len, AckCallback cb) {
	_ASSERT(len > 0);
	_ASSERT(src > 0);
	_ASSERT(src+len <= gMemSize);
	if(isMemoryCached(src, len)) { cb(); return; }

	sReadMemoryCallback = cb;
	sReadMemoryLen = len;
	sReadMemoryDst = (byte*)dst;
	char buffer[64];
	sprintf(buffer, "m%X,%X", src, len);
	unIdle();
	StubConnLow::sendPacket(buffer, readMemoryAck);
}
static void readMemoryAck() {
	StubConnLow::expectPacket(readMemoryPacket);
}
static bool readMemoryPacket(const char* data, int len) {
	//eprintf("Recieved packet, %i bytes (we want %i): '%s'\n",
		//len, sReadMemoryLen * 2, data);
	if(checkErrorPacket(data, len))
		return true;
	if(len != sReadMemoryLen * 2)
		return false;
	//we also check that there are no non-hex characters
	for(int i=0; i<len; i++) {
		if(!isxdigit(data[i]))
			return false;
	}
	//parse data, byte for byte
	for(int i=0; i<sReadMemoryLen; i++) {
		char buf[4];
		memcpy(buf, data + i*2, 2);
		buf[3] = 0;
		sReadMemoryDst[i] = (byte)strtoul(buf, NULL, 16);
	}
	setIdle();
	sReadMemoryCallback();
	return true;
}

//******************************************************************************
// writeMemory
//******************************************************************************

void StubConnection::writeMemory(int dst, const void* src, int len, AckCallback cb) {
	sWriteMemoryCallback = cb;
	Smartie<char> buffer(new char[32 + len * 2]);
	char* ptr = buffer();
	ptr += sprintf(ptr, "M%X,%X:", dst, len);
	for(int i=0; i<len; i++) {
		ptr += sprintf(ptr, "%02X", ((byte*)src)[i]);
	}
	unIdle();
	StubConnLow::sendPacket(buffer(), writeMemoryAck);
}
static void writeMemoryAck() {
	StubConnLow::expectPacket(writeMemoryPacket);
}
static bool writeMemoryPacket(const char* data, int len) {
	if(checkErrorPacket(data, len))
		return true;
	if(len != 2)
		return false;
	if(strcmp(data, "OK") != 0)
		return false;
	setIdle();
	sWriteMemoryCallback();
	return true;
}
