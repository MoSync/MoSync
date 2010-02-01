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

/******************************************************************************/
/* MoSync GDB Stub                                                            */
/* (c) 2007 Mobile Sorcery                                                    */
/* This is used to compile the GDB debug version of the mosync runtime.       */
/******************************************************************************/

#include <stdexcept>
#include "config_platform.h"
#include <helpers/helpers.h>
#include <helpers/attribute.h>
#include <base/base_errors.h>

using namespace MoSyncError;

#ifdef GDB_DEBUG

#include "config_platform.h"
#include "GdbStub.h"
#include "fastevents.h"
#include "sdl_syscall.h"


#define CORE mCore

// Map from integer to a hexadecimal ASCII character.
char GdbStub::hexChars[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};


GdbStub::GdbStub(Core::VMCore *core)
{
	outputBuffer.resize(1024);
	curOutputBuffer = outputBuffer.begin();
	mInputBuffer.resize(1024);
    mCore = core;
    mWaitingForAck = false;
    mQuit = false;
    mExitPacketSent = false;
    mMessageMutex.post();	//leave it open so exactly one thread can get in
}

int GdbStub::hexToNum(char c) {
	if(c>='0'&&c<='9') return c-'0';
	else if(c>='a'&&c<='f') return c+10-'a';
	else if(c>='A'&&c<='F') return c+10-'A';
	else return -1;
}

void GdbStub::checkAndResize(int len) {
	unsigned long curIndex = (unsigned long)(curOutputBuffer-outputBuffer.begin());
	unsigned long newSize = (len+curIndex)+1; // +1 for the null terminator...
	if(newSize>(outputBuffer.size())) {
		//BIG_PHAT_ERROR(ERR_INTERNAL);
		outputBuffer.resize(newSize);
		curOutputBuffer = outputBuffer.begin()+curIndex;
	}	
}

void GdbStub::appendOut(const char *what) {
	checkAndResize(strlen(what));
	curOutputBuffer += sprintf(curOutputBuffer, "%s", what);	
}

void GdbStub::appendOut(char what) {
	checkAndResize(1);
	*curOutputBuffer++ = what;
	*curOutputBuffer = 0;
}

void GdbStub::clearOutputBuffer() {
	curOutputBuffer = outputBuffer.begin();
	checkAndResize(1024);
	appendOut('$');
	
}

void GdbStub::setupDebugConnection() {
	connection = NULL;
	int res = server.open(50000);
	MYASSERT(res > 0, ERR_GDB_SERVER_OPEN);

	res = server.accept(connection);
	DEBUG_ASSERT(res > 0);

	mControlThread.start(sRunControl, this);
	mReadThread.start(sRunRead, this);
}

int GdbStub::sRunControl(void* arg) {
	GdbStub* s = (GdbStub*)arg;
	s->runControl();
	return 0;
}
void GdbStub::runControl() {
	LOG("runControl()\n");
	while(!mQuit) {
		try {
			handleMessage();
		} catch(std::exception& e) {
			LOG("Caught exception: %s\n", e.what());
			mQuit = true;
		}
	}
	mExecSem.post();
}

void GdbStub::handleMessage() {
	MESSAGE m = getMessage();	//blocking
	switch(m.type) {
	case eInput:
		//LOG("MESSAGE input\n");
		handleInput();
		mReadSem.post();
		break;
	case eException:
		LOG("MESSAGE exception\n");
		sendExceptionPacket(m.code);
		break;
	case eExit:
		LOG("MESSAGE exit\n");
		sendExitPacket(m.code);
		break;
	case eTerminate:
		LOG("MESSAGE terminate\n");
		if(!mExitPacketSent)
			sendTerminationPacket(m.code);
		mQuit = true;
		break;
	default:
		DEBIG_PHAT_ERROR;
	}
}

GdbStub::MESSAGE GdbStub::getMessage() {
	MESSAGE m;
	mMessageSem.wait();	//wait for a message to arrive in the queue
	mMessageMutex.wait();	//get the lock on the queue
	{
		m = mMessageQueue.pop();
	}
	mMessageMutex.post();	//release the lock on the queue
	return m;
}

void GdbStub::putMessage(MESSAGE m) {
	//LOG("putMessage %i\n", m.type);
	mMessageMutex.wait();	//get the lock on the queue
	{
		mMessageQueue.push(m);
	}
	mMessageMutex.post();	//release the lock on the queue
	mMessageSem.post();	//signal getMessage()
}


GdbStub::MessageQueue::MessageQueue() {
	mWP = mRP = 0;
}
GdbStub::MESSAGE GdbStub::MessageQueue::pop() {
	DEBUG_ASSERT(mWP != mRP);
	MESSAGE m = mArray[mRP];
	mRP++;
	if(mRP == QUEUE_SIZE)
		mRP = 0;
	return m;
}
void GdbStub::MessageQueue::push(MESSAGE m) {
	int oldWP = mWP;
	mWP++;
	if(mWP == QUEUE_SIZE)
		mWP = 0;
	DEBUG_ASSERT(mWP != mRP);
	mArray[oldWP] = m;
}


//wait until the remote has requested that execution be started/resumed.
bool GdbStub::waitForRemote() {
	if(!mQuit) {
		if(!mControlThread.isCurrent()) {
			mExecSem.wait();
		}
	}
	return mQuit;
}

// we arrive here when a trap occurs in the mosync program
void GdbStub::exceptionHandler(int code) {
	MESSAGE m;
	m.type = eException;
	m.code = code;
	putMessage(m);
}
void GdbStub::sendExceptionPacket(int code) {
	clearOutputBuffer();
	appendOut('S');
	appendOut(hexChars[(code>>4)&0xf]);
	appendOut(hexChars[(code)&0xf]);
	putPacket();
}

void GdbStub::exitHandler(int code) {
	MESSAGE m;
	m.type = eExit;
	m.code = code;
	putMessage(m);
}
void GdbStub::sendExitPacket(int code) {
	clearOutputBuffer();
	appendOut('W');
	appendOut(hexChars[(code>>4)&0xf]);
	appendOut(hexChars[(code)&0xf]);
	putPacket();
	mExitPacketSent = true;
}

void GdbStub::sendTerminationPacket(int code) {
	clearOutputBuffer();
	appendOut('X');
	appendOut(hexChars[(code>>4)&0xf]);
	appendOut(hexChars[(code)&0xf]);
	putPacket();
}

void GdbStub::closeDebugConnection() {
	{
		MESSAGE m;
		m.type = eTerminate;
		m.code = 0;
		putMessage(m);
	}
	mControlThread.join();

	server.close();
	if(connection) {
		connection->close();
		mReadSem.post();
		mReadThread.join();
	}
}

int GdbStub::sRunRead(void* arg) {
	GdbStub* s = (GdbStub*)arg;
	s->runRead();
	return 0;
}
void GdbStub::runRead() {
	mInputPos = 0;
	while(!mQuit) {
		if(mInputPos>=(int)mInputBuffer.size()) {
			mInputBuffer.resize((mInputBuffer.size()+1)*2);
		}
		int res = connection->read(mInputBuffer.begin() + mInputPos,
			(mInputBuffer.size() - mInputPos)-1);
		//LOG("GDB read %i\n", res);
		if(res <= 0) {
			LOG("connection->read error %i\n", res);
			break;
		}
		mInputPos += res;
		mInputBuffer[mInputPos] = 0;
		MESSAGE m;
		m.type = eInput;
		putMessage(m);
		mReadSem.wait();	//wait until this packet has been handled before reading more
		//LOG("GDB read wait complete\n");
	}
	mExecSem.post();
}

void GdbStub::handleInput() {
	int pos = 0;
	while(pos < mInputPos) {
		switch(mInputBuffer[pos]) {
		case '+':
			handleAck();
			pos++;
			break;
		case '-':
			handleNack();
			pos++;
			break;
		case '$':
			pos = handlePacket(pos + 1);
			break;
		case 0x03:
			LOG("Stub recieved interrupt signal\n");
			mCore->mGdbSignal = eInterrupt;
			{
				SDL_UserEvent event = { FE_INTERRUPT, 0, NULL, NULL };
				FE_PushEvent((SDL_Event*)&event);
			}
			pos++;
			break;
		default:
			DEBIG_PHAT_ERROR;
		}
		if(pos < 0) {
			return;
		}
	}
	//now we've handled everything, and can reset the input buffer.
	mInputPos = 0;
}

void GdbStub::handleAck() {
	LOG("GDB ACK was sent from the client.\n");
	if(mWaitingForAck) {
		mWaitingForAck = false;
	} else {
		DEBIG_PHAT_ERROR;
	}
}

char GdbStub::getAck() {
	DEBUG_ASSERT(!mWaitingForAck);
	mWaitingForAck = true;
	while(!mQuit && mWaitingForAck) {
		handleMessage();
	}
	return '+';
}

void GCCATTRIB(noreturn) GdbStub::handleNack() {
	LOG("GDB NACK was sent from the client.\n");
	DEBIG_PHAT_ERROR;
}

int GdbStub::handlePacket(int pos) {
	int begin = pos;
	uint myChecksum = 0;
	while(pos < mInputPos && mInputBuffer[pos] != '#') {
		myChecksum += (byte)mInputBuffer[pos];
		pos++;
	}
	myChecksum &= 0xFF;

	//make sure we got a whole packet
	if(mInputBuffer[pos] != '#')
		return -1;
	//including the checksum
	pos++;
	if(pos + 2 > mInputPos)
		return -1;

	//check the checksum
	uint theirChecksum;
	int res = sscanf(mInputBuffer.begin() + pos, "%x", &theirChecksum);
	pos += 2;
	if(res != 1 || myChecksum != theirChecksum) {
		transmissionNAK();
		return pos;
	}

	//parse the packet
	mInputPtr = mInputBuffer.begin() + begin;
	doPacket();
	return pos;
}


void GdbStub::putDebugChar(char c) {
	//LOG("putDebugChar(%c)\n", c);
	int res = connection->write(&c, 1);
	if(res <= 0) {
		LOG("connection->write error %i\n", res);
		throw std::logic_error("gdbConn write error");
	}
}

void GdbStub::putDebugChars(char *c, int len) {
	int res = connection->write(c, len);
	if(res <= 0) {
		LOG("connection->write error %i\n", res);
		throw std::logic_error("gdbConn write error");
	}
}

void GdbStub::transmissionNAK() {
	LOG("GDB transmission: NAK\n");
	putDebugChar('-');
}

void GdbStub::transmissionACK() {
	LOG("GDB transmission: ACK\n");
	putDebugChar('+');
}

#define NUM_REGS 128
// Required commands follows:
bool GdbStub::readRegisters() {
	for(int i = 0; i < NUM_REGS/*mCore.mRegs.length*/; i++) {
		appendDataTypeToOutput<int>(mCore->regs[i]);
	}
	appendDataTypeToOutput<int>(Core::GetIp(mCore));
	return true;
}

bool GdbStub::writeRegisters() {
	int i;
	for(i = 0; i < NUM_REGS /*mCore.mRegs.length*/; i++) {
		mCore->regs[i] = getDataTypeFromInput<int>();
	}
	Core::SetIp(mCore, getDataTypeFromInput<int>());
	return true;
}

bool GdbStub::readMemory() {
	int address = getBoundedDataTypeFromInput<int>(',');
	int length = getBoundedDataTypeFromInput<int>(0);	

	byte *src;
	int size;
	if(address >= DATA_MEMORY_START && address < INSTRUCTION_MEMORY_START) {
		size = mCore->DATA_SEGMENT_SIZE;
		src = (byte*)mCore->mem_ds;
	} else {
		size = mCore->CODE_SEGMENT_SIZE;
		src = (byte*)mCore->mem_cs;
	}
	address &= ADDRESS_MASK;
	if(address >= size || address + length > size) {
		LOG("bad address: 0x%x + 0x%x\n", address, length);
		return false;
	}
	src += address;

	for(int i = 0; i < length; i++) {
		appendDataTypeToOutput<byte>(src[i]);
	}
	return true;
}

bool GdbStub::writeMemory() {
	int address = getBoundedDataTypeFromInput<int>(',');
	int length = getBoundedDataTypeFromInput<int>(':');

	byte *dst;
	int size;
	if(address>=DATA_MEMORY_START&&address<INSTRUCTION_MEMORY_START) {
		size = mCore->DATA_SEGMENT_SIZE;
		dst = (byte*)mCore->mem_ds;
	} else {
		size = mCore->CODE_SEGMENT_SIZE;
		dst = (byte*)mCore->mem_cs;
	}
	address&=ADDRESS_MASK;

	if(address >= size || address + length > size) {
		LOG("bad address: 0x%x + 0x%x\n", address, length);
		return false;
	}

	for(int i = 0; i < length; i++) {
		dst[address+i] = getDataTypeFromInput<byte>();
	}
	appendOut("OK");

	return true;
}

bool GdbStub::continueExec() {
	int address = getBoundedDataTypeFromInput<int>(0);
	if(address) {
		Core::SetIp(mCore, address);
	}
	mCore->mGdbSignal = eNone;
	mExecSem.post();
	return true;
}

bool GdbStub::stepExec() {
	int address = getBoundedDataTypeFromInput<int>(0);
	if(address) {
		Core::SetIp(mCore, address);
	}
	mCore->mGdbSignal = eStep;
	mExecSem.post();
	return true;
}

// Optional commands follows:
bool GdbStub::lastSignal() {
	return false;
}

bool GdbStub::readRegister() {
	return false;
}

bool GdbStub::writeRegister() {
	return false;
}

bool GdbStub::killRequest() {
	return false;
}

bool GdbStub::toggleDebug() {
	return false;
}

bool GdbStub::reset() {
	return false;
}

bool GdbStub::search() {
	return false;
}

bool GdbStub::generalSet() {
	return false;
}

bool GdbStub::generalQuery() {
	return false;
}

bool GdbStub::sectionOffsetsQuery() {
	return false;
}

bool GdbStub::consoleOutput() {
	return false;
}

bool GdbStub::defaultResponse() {
	return false;
}

// Select and execute command:
bool GdbStub::executeCommand() {
	char instruction = *mInputPtr;
	mInputPtr++;

	LOG("GDB instruction '%c' was sent from the client.\n", instruction);

	switch(instruction) {

		// required
		case 'g': return readRegisters();
		case 'G': return writeRegisters();
		case 'm': return readMemory();
		case 'M': return writeMemory();
		case 'c': return continueExec();
		case 's': return stepExec();

			// optional;


#if 0
		case 'p': return readRegister();		
		case 'P': return writeRegister();			
		case '?': return lastSignal();
		case 'k': return killRequest();
		case 'd': return toggleDebug();
		case 'r': return reset();
		case 't': return search();
		case 'q': 
			{
				if((inputBuffer.toString()).startsWith("Offsets") == true) {
					return sectionOffsetsQuery();
				} else {
					return generalQuery();
				}
			}
			case 'Q' return generalSet();
				case 'O' return consoleOutput();
#endif	//0

		default:
			return defaultResponse();
	}
}

void GdbStub::putPacket() {
	DEBUG_ASSERT(!mWaitingForAck);
	int calculatedChecksum = 0;
	const char *cur = outputBuffer.begin() + 1;
	*curOutputBuffer = 0;
	int len = 0;
	while(*cur) {
		len++;
		calculatedChecksum += (byte)(*cur++);
	}
	if(len == 0)
		return;

	appendOut('#');
	appendOut(hexChars[(calculatedChecksum>>4)&0xf]);
	appendOut(hexChars[(calculatedChecksum)&0xf]);
	*curOutputBuffer = 0;

	LOG("GDB transmission: \"%s\"\n", outputBuffer.begin());
	mWaitingForAck = true;
	putDebugChars(outputBuffer.begin(), curOutputBuffer - outputBuffer.begin());
}

// Read and execute command and send reply.
bool GdbStub::doPacket() {
	clearOutputBuffer();

	if(executeCommand() == false) {
		// command arguments or command were wrong somehow.
		transmissionNAK();
		return false;
	}

	transmissionACK();
	putPacket();

	return true;
}

#endif	//GDB_DEBUG
