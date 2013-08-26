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

#ifndef SERVER_H
#define SERVER_H

#include "connection.h"

#define ANY_PORT (-1)

class BtSppServer : public Closable {
public:
	//BtSppServer();

	//Returns the opened port number (1-30) or a CONNERR code.
	//The UUID is registered with the SDP system.
	int open(const MAUUID& uuid, const char* name, int port = ANY_PORT);

	//Stores the local Bluetooth address of an open server socket.
	int getAddr(MAConnAddr& addr);

	//Stores the address of a new, pre-connected BtSppConnection in connP.
	//This connection should be deleted when no longer needed.
	//Returns >0 or CONNERR code.
	int accept(BtSppConnection*& connP);

	void close();

	virtual ~BtSppServer() { close(); }

private:
#if defined(WIN32) && !defined(_WIN32_WCE)
	SOCKET mSock;
	SOCKADDR_BTH mAddr;
#endif
};

#endif	//SERVER_H
