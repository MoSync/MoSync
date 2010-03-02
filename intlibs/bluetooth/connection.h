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

#ifndef CONNECTION_H
#define CONNECTION_H

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
#include "helpers/CPP_IX_CONNSERVER.h"

class HttpConnection;

class Closable {
public:
	//Thread-safe. Will cause all active operations to return with CONNERR_CANCELED.
	//Wait for them to do so before deleting the Closable.
	virtual void close() = 0;

	//returns immediately.
	virtual int getAddr(MAConnAddr& addr) = 0;

	virtual ~Closable() {}
};

//Abstract base class. All is synchronous. No function should ever return zero.
class Connection : public Closable {
public:
	//The constructor sets up the variables to be used for the connection.
	//This function uses those variables and returns >0 or CONNERR code.
	virtual int connect() = 0;

	//Reads 1 to <max> bytes into <dst>.
	//Returns nBytes read or CONNERR code.
	virtual int read(void* dst, int max) = 0;

	//Writes <len> bytes from <src>.
	//Returns >0 or CONNERR code.
	virtual int write(const void* src, int len) = 0;

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

#ifdef BLUEZ_SUPPORTED
class BluezBtSppConnection : public BtSppConnection {
public:
	BluezBtSppConnection(const MABtAddr* address, uint port);
	~BluezBtSppConnection();

	int connect();
	int read(void* dst, int max);
	int write(const void* src, int len);
	void close();
private:
};
#endif

#ifdef WINSOCK_SUPPORTED
class WinsockBtSppConnection : public BtSppConnection {
public:
	WinsockBtSppConnection(const MABtAddr* address, uint port);
	~WinsockBtSppConnection();

	int connect();
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
	int read(void* dst, int max);
	int write(const void* src, int len);
	void close();
private:

};
#endif	//BLUESOLEIL_SUPPORTED

#endif	//CONNECTION_H
