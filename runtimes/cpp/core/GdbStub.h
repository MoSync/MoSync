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

#ifndef _GDB_STUB_H_
#define _GDB_STUB_H_

#include "Core.h"
#include <net/net.h>
#include "ThreadPoolImpl.h"
#include <mostl/vector.h>

/* used when compiling the GDB stub */
#define DBG_STAB ENDOP

class GdbStub {
public:
	GdbStub(Core::VMCore *core);
	void setupDebugConnection();
	void closeDebugConnection();

	// we arrive here when a trap occurs in the mosync program
	void exceptionHandler(int exception);
	void exitHandler(int exception);
	bool waitForRemote();	//returns true if stub has quit.

private:
	static char hexChars[];
        
	// Connections from and to the GDB host.
	TcpServer server;
	TcpConnection *connection;

	void runControl();
	static int sRunControl(void*);
	bool mQuit, mExitPacketSent;
	MoSyncThread mControlThread;

	//waitForRemote()
	MoSyncSemaphore mExecSem;

	// Unprotected queue
	enum MessageType {
		eInput, eException, eExit, eTerminate
	};
	struct MESSAGE {
		MessageType type;
		int code;
	};
	class MessageQueue {
	private:
		static const int QUEUE_SIZE = 4;
		MESSAGE mArray[QUEUE_SIZE];
		int mWP, mRP;
	public:
		MessageQueue();
		MESSAGE pop();
		void push(MESSAGE);
	} mMessageQueue;

	// Internal messages
	MoSyncSemaphore mMessageSem;
	MoSyncSemaphore mMessageMutex;

	MESSAGE getMessage();
	void putMessage(MESSAGE);
	void handleMessage();

	// handlers
	void sendExceptionPacket(int code);
	void sendExitPacket(int code);
	void sendTerminationPacket(int code);

	void runRead();
	static int sRunRead(void*);
	MoSyncThread mReadThread;
	MoSyncSemaphore mReadSem;
	MoSyncSemaphore mReadThreadSem;

	void handleInput();
	void handleAck();
	void handleNack();
	int handlePacket(int pos);	//returns new pos, or <0 if packet was incomplete

	// Used to store the input data and output data.
	//char mInputBuffer[1024*10];	//todo:make variable length
	mostd::vector<char> mInputBuffer;
	int mInputPos;
	char* mInputPtr;	//legacy

	mostd::vector<char> outputBuffer;
	char *curOutputBuffer;

	// Acks
	char getAck();

	bool mWaitingForAck;

	// Reference to the core, used to retrieve and write memory/registers and such.
	Core::VMCore *mCore;

	char tempBuffer[1024];

	template<typename type>
	const char* convertDataTypeToString(type b) {
		char* ret = tempBuffer;
		for(int i = (sizeof(type)<<3)-4; i >= 0; i-=4) {
			*ret++ = (hexChars[((b>>i)&0xf)]);
		}
		*ret++ = 0;
		return tempBuffer;
	}	

	int hexToNum(char c);

	template<typename type>
	type getDataTypeFromString(char*& b) {
		type ret = 0;
		for(int i = (sizeof(type)*2)-1; i >= 0; i-=1) {
			ret |= ((type)(hexToNum(*b)))<<(i*4);
			b++;
		}
		return ret;
	}	

	template<typename type>
	type getBoundedDataTypeFromInput(char bound) {
		const char *start = mInputPtr;
		while(*mInputPtr!=bound) mInputPtr++;
		const char *end = mInputPtr;
		mInputPtr++;
		type ret = 0;
		type n = 0;
		while(start!=end && (n=hexToNum(*start))!=-1) {
			ret<<=4;
			ret|=n;
			start++;
		}
		return ret;
	}	


	void appendOut(const char *what);
	void appendOut(char what);
	void checkAndResize(int len);

	template<typename type>
	void appendDataTypeToOutput(type d) {
		appendOut(convertDataTypeToString<type>(d));
	}

	template<typename type>
	type getDataTypeFromInput() {
		return getDataTypeFromString<type>(mInputPtr);
		//mInputPtr += ((sizeof(type))-1)<<1;
	}

	void clearOutputBuffer();

	void putDebugChar(char c);
	void putDebugChars(char *c, int len);

	void transmissionNAK();
	void transmissionACK();

	// Required commands follows:
	bool readRegisters();
	bool writeRegisters();
	bool readMemory();
	bool writeMemory();
	bool continueExec();
	bool stepExec();

	// Optional commands follows:
	bool lastSignal();
	bool readRegister();
	bool writeRegister();
	bool killRequest();
	bool toggleDebug();
	bool reset();
	bool search();
	bool generalSet();
	bool generalQuery();
	bool sectionOffsetsQuery();
	bool consoleOutput();
	bool defaultResponse();

	// Select and execute command:
	bool executeCommand();
	void putPacket();

	// Read and execute command and send reply.
	bool doPacket();
};


#endif
