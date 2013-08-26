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

#ifndef CONNECTION_H
#define CONNECTION_H

#include "helpers/attribute.h"

#ifdef WIN32

#if defined(_MSC_VER)
#elif defined(__GNUC__)
#include <basetyps.h>	//has DEFINE_GUID, needed by Ws2bth.h
#else
#error Unsupported platform!
#endif	//_MSC_VER

#include <Winsock2.h>
#include <ws2bth.h>
#define WINSOCK_SUPPORTED

#endif	//WIN32

#include "btinit.h"

class HttpConnection;

class Closable {
public:
	//Thread-safe. Will cause all active operations to return with CONNERR_CANCELED.
	//Wait for them to do so before deleting the Closable.
	virtual void close() = 0;

	//returns immediately.
	//returns >0 on success, a CONNERR code otherwise.
	virtual int getAddr(MAConnAddr& addr) = 0;

	virtual ~Closable() {}
};

//Abstract base class. All is synchronous. No function should ever return zero.
class Connection : public Closable {
public:
	//The constructor sets up the variables to be used for the connection.
	//This function uses those variables and returns >0 or CONNERR code.
	virtual int connect() = 0;

	//When this returns false, connect() may be called safely.
	virtual bool isConnected() = 0;

	//Reads 1 to <max> bytes into <dst>.
	//Returns nBytes read or CONNERR code.
	virtual int read(void* dst, int max) = 0;

	virtual int readFrom(void* dst, int max, MAConnAddr& src) GCCATTRIB(noreturn);

	//Writes <len> bytes from <src>.
	//Returns >0 or CONNERR code.
	virtual int write(const void* src, int len) = 0;

	virtual int writeTo(const void* src, int len, const MAConnAddr& dst) GCCATTRIB(noreturn);

	//Writes the remote connection's address to \a addr.
	//Will fail if connect() has not completed.
	virtual int getAddr(MAConnAddr& addr) = 0;

	virtual HttpConnection* http() { return NULL; }

	//Reads exactly <len> bytes into <dst>.
	//Returns >0 or CONNERR code.
	int readFully(void* dst, int len);

	virtual ~Connection() {}	//subclasses should exit on fatal errors and log lesser errors.
};

class BtSppConnection : public Connection {
public:
	friend BtSppConnection* createBtSppConnection(const MABtAddr* address, uint port);
};
BtSppConnection* createBtSppConnection(const MABtAddr* address, uint port);

#ifdef WINSOCK_SUPPORTED
class WinsockBtSppConnection : public BtSppConnection {
public:
	WinsockBtSppConnection(const MABtAddr* address, uint port);
	~WinsockBtSppConnection();

	int connect();
	bool isConnected();
	int read(void* dst, int max);
	int write(const void* src, int len);
	void close();
	int getAddr(MAConnAddr& addr);
private:
	SOCKET mSock;
	SOCKADDR_BTH mAddress;

	WinsockBtSppConnection(SOCKET sock, const SOCKADDR_BTH& addr);
	friend class /*Winsock*/BtSppServer;
};
void sockAddrBth2BtAddr(const SOCKADDR_BTH& sockAddr, MAConnAddrBt& btAddr);
#endif

#ifdef BLUESOLEIL_SUPPORTED
class BlueSoleilBtSppConnection : public BtSppConnection {
public:
	BlueSoleilBtSppConnection(const MABtAddr* address, uint port);
	~BlueSoleilBtSppConnection();

	int connect();
	bool isConnected();
	int read(void* dst, int max);
	int write(const void* src, int len);
	void close();
private:

};
#endif	//BLUESOLEIL_SUPPORTED

#endif	//CONNECTION_H
