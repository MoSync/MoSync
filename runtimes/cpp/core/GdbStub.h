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
	
	/**
	 * Terminates the program (relatively) gracefully.
	 *
	 * Note:
	 * This function must try to get the main thread out of the
	 * kernel, e.g. if it is waiting idefinitley for a semaphore, 
	 * otherwise MoRE will not receive any signals and will hang 
	 * on Unix like systems if mdb dies.
	 *
	 * This function does not call exit directly, but implicitly
	 * makes the other threads stop. It relies on the VM loop to
	 * exit when waitForRemote returns false.
	 */
	bool quit();

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
