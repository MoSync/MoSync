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

#include <queue>

#include "config.h"
#include "helpers/log.h"
#include "net/net.h"

#include "StubConnLow.h"
#include "remoteReadThread.h"
#include "StubConnection.h"
#include "helpers.h"

using namespace std;

//******************************************************************************
// defines
//******************************************************************************
#define TEST_CONN(call) { int _res = (call); if(_res < 0) { connError(_res);\
	return; } }

#define MULTI_ACK 0

static const int CHECKSUM_LEN = 2;
static const int MAX_NACKS = 5;	//arbitrary

//******************************************************************************
// static declarations
//******************************************************************************
static TcpConnection* sConn = NULL;
static bool sIsConnected = false;
static PacketCallback sPacketExpector = NULL;
#if MULTI_ACK
static queue<AckCallback> sAckCallbacks;
#else
static AckCallback sAckCallback = NULL;
#endif

static const int BUFSIZE = 1024 * 1024;
static char sBuffer[BUFSIZE];
static int sBufFill = 0;

static void connError(int code);
static bool recvHandler2();
static void remove(int len);
static void handleAck();
static void handleNack();
static void handlePacket(const char* data, int len);

//******************************************************************************
// connect
//******************************************************************************

bool StubConnLow::connect(const std::string& hostname, u16 port) {
	if(sIsConnected) {
		error("Already connected");
		return false;
	}
	sConn = new TcpConnection(hostname, port);
	int res = sConn->connect();
	if(res < 0) {
		connError(res);
		return false;
	}
	sIsConnected = true;
	remoteRecv(sConn, sBuffer, BUFSIZE - 1);
	return true;
}

//******************************************************************************
// error
//******************************************************************************

static void close() {
	if(sConn) {
		sIsConnected = false;
		delete sConn;
		sConn = NULL;
	}
}

static void connError(int code) {
#if MULTI_ACK
	while(!sAckCallbacks.empty())
		sAckCallbacks.pop();
#else
	sAckCallback = NULL;
#endif
	char buf[128];
	sprintf(buf, "Connection failure, code %i", code);
	error(buf);
}

//******************************************************************************
// isIdle
//******************************************************************************

bool StubConnLow::isIdle() {
	return sAckCallback == NULL;
}

//******************************************************************************
// send
//******************************************************************************

void StubConnLow::sendPacket(const char* str, AckCallback ac) {
	//ensureConnection();
	if(!sIsConnected) {
		error("Not connected");
		return;
	}
#if MULTI_ACK
	sAckCallbacks.push(ac);
#else
	//musn't send anything before we've recieved the ack of our last transmission.
	_ASSERT(sAckCallback == NULL);
	sAckCallback = ac;
#endif

	char checkBuf[CHECKSUM_LEN + 1];
	byte checksum = 0;
	int len = 0;
	while(str[len]) {
		checksum += (byte)str[len];
		len++;
	}
	sprintf(checkBuf, "%02X", checksum);

	LOG("Send: $%s#%s\n", str, checkBuf);
	TEST_CONN(sConn->write("$", 1));
	TEST_CONN(sConn->write(str, len));
	TEST_CONN(sConn->write("#", 1));
	TEST_CONN(sConn->write(checkBuf, CHECKSUM_LEN));
}

static void sendAck() {
	TEST_CONN(sConn->write("+", 1));
	LOG("Send: +\n");
}

static void sendNack() {
	TEST_CONN(sConn->write("-", 1));
	LOG("Send: -\n");
}

void StubConnLow::sendInterrupt() {
	TEST_CONN(sConn->write("\x03", 1));
	LOG("Send interrupt\n");
}

//******************************************************************************
// expect
//******************************************************************************

void StubConnLow::expectPacket(PacketCallback pc) {
	_ASSERT(sPacketExpector == NULL);
	sPacketExpector = pc;
}

//******************************************************************************
// mid-level recv handlers
//******************************************************************************

static void handleAck() {
	LOG("Recv Ack\n");
#if MULTI_ACK
	if(!sAckCallbacks.empty()) {
		sAckCallbacks.front()();
		sAckCallbacks.pop();
	}
#else
	AckCallback temp = sAckCallback;
	sAckCallback = NULL;
	temp();
#endif
}
static void handleNack() {
	LOG("Recv Nack\n");
	close();
	error("Negative ack! Can't handle, bugging out.");
}
static void handlePacket(const char* data, int len) {
	LOG("Recv packet(%i): '%s'\n", len, data);
	sendAck();
	if(sPacketExpector) {
		if(sPacketExpector(data, len)) {
			sPacketExpector = NULL;
			return;
		}
	}
	//if we're here, the packet was not expected.
	StubConnection::handleUnexpectedPacket(data, len);
}

//******************************************************************************
// low-level recv handlers
//******************************************************************************

void StubConnLow::recvHandler(int result) {
	if(result < 0) {
		LOG("Recv error %i\n", result);
		connError(result);
		return;
	}
	sBufFill += result;
	sBuffer[sBufFill] = 0;
	//LOG("direct Recv: %s\n", sBuffer);

	while(recvHandler2()) ;

	//LOG("%i bytes remain after recv handling.\n", sBufFill);
	int max = BUFSIZE - 1 - sBufFill;
	if(max == 0) {
		error("Incoming packet won't fit in buffer");
		return;
	}
	remoteRecv(sConn, sBuffer + sBufFill, max);
}

//returns true if it's useful to call this function again.
static bool recvHandler2() {
	if(sBufFill == 0)
		return false;
	if(sBuffer[0] == '+') {
		handleAck();
		remove(1);
		return true;
	}
	if(sBuffer[0] == '-') {
		handleNack();
		remove(1);
		return true;
	}
	if(sBuffer[0] != '$') {
		error("Incoming packet syntax error");
		const char* dollar = strchr(sBuffer, '$');
		if(dollar)
			remove(dollar - sBuffer);
		else
			sBufFill = 0;
		sendNack();
		return true;
	}
	//we now have the beginning of a packet.
	char* packetData = sBuffer + 1;
	const char* pound = strchr(packetData, '#');
	if(pound == NULL)
		return false;
	int packetDataLen = int(pound - packetData);
	int packetTotalLen = packetDataLen + 2 + CHECKSUM_LEN;
	if(sBufFill < packetTotalLen)
		return false;

	//LOG("Raw read: %s\n", sBuffer);

	uint theirChecksum;
	if(sscanf(pound + 1, "%x", &theirChecksum) != 1) {
		error("Checksum syntax error");
		remove(packetTotalLen);
		sendNack();
		return true;
	}

	byte myChecksum = 0;
	for(int i=0; i<packetDataLen; i++) {
		myChecksum += (byte)packetData[i];
	}

	if(myChecksum != theirChecksum) {
		LOG("mine: 0x%02x theirs: 0x%02x\n", myChecksum, theirChecksum);
		error("Checksum mismatch");
		remove(packetTotalLen);
		sendNack();
		return true;
	}

	//now we have a complete packet
	packetData[packetDataLen] = 0;
	handlePacket(packetData, packetDataLen);

	remove(packetTotalLen);
	return true;
}

static void remove(int len) {
	_ASSERT(len <= sBufFill);
	if(len == sBufFill) {
		sBufFill = 0;
		return;
	}
	memmove(sBuffer, sBuffer + len, sBufFill - len);
	sBufFill -= len;
	sBuffer[sBufFill] = 0;
}
