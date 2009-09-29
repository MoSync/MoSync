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
