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

#ifndef _TCP_CONNECTION_H_
#define _TCP_CONNECTION_H_

#include <string>
#include <vector>
#include <utility>

#include <bluetooth/connection.h>
#include <net/net.h>

#include "netImpl.h"

#include "ThreadPool.h"

//***************************************************************************
//Connection classes
//***************************************************************************

class RtspConnection : public ProtocolConnection, public Runnable {
public:
	RtspConnection(const std::string& hostname, u16 port, const std::string& path);

	int play();
	int pause();
	int teardown();
	
	void run();

	void cleanup();
protected:
	int setup();

	//ProtocolConnection
	std::string methodString();
	std::string protocolVersion();
	std::string pathString();
	int readResponseCode(const char* line, int len);

	bool setMethod(int method);
	void parseDescribeData(const char *desc);
	void parseTransportData(char *temp, int stream);

	int sendAndVerify(int method);
	int	setupStream(int stream);

private:

	struct Stream {
		enum Type {
			AUDIO = 0,
			VIDEO = 1
		};
			
		enum {
			MAX_PACKET_TYPE_LEN = 64
		};

		Type streamType;
		char packetType[MAX_PACKET_TYPE_LEN];
		int trackID;

		uint sourceAddress;
		int sourcePort[2];
	};

	std::vector<Stream> streams;

	int mMethod;
	int CSeq;

	bool gotSessionId;
	std::string sessionId;

	std::string setupPathString;

	MoSyncMutex methodMutex;
	MoSyncSemaphore userRequest;
};

int rtspCreateConnection(const char* url, RtspConnection*& conn);

#endif /* _TCP_CONNECTION_H_ */
