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

#ifndef NET_H
#define NET_H

int readProtocolResponseCode(const char* protocolSlash, const char* line, int len);

#ifndef __SYMBIAN32__

#include <string>

#include "helpers/types.h"
#include "bluetooth/connection.h"
#include "helpers/hash_map.h"

#if defined(WIN32) || defined(_WIN32_WCE)
//#include <windows.h>
#include <winsock2.h>
typedef SOCKET MoSyncSocket;
#define SOCKET_ERRNO WSAGetLastError()

#else	//GNU libc

#include <sys/socket.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/tcp.h>
typedef int MoSyncSocket;
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)
#define SOCKET_ERRNO errno
#define closesocket ::close
#endif


MoSyncSocket MASocketOpen(const char* address, u16 port, int& result, uint& inetAddr);
MoSyncSocket MASocketCreate(const char* address, int& result, uint& inetAddr);
int MASocketConnect(MoSyncSocket sock, uint inetAddr, u16 port);

static const char http_string[] = "http://";
static const char https_string[] = "https://";
static const char socket_string[] = "socket://";
static const char ssl_string[] = "ssl://";
static const char btspp_string[] = "btspp://";

//***************************************************************************
//Connection classes
//***************************************************************************

class TcpConnection : public Connection {
public:
	TcpConnection(const std::string& hostname, u16 port, MoSyncSocket sock=INVALID_SOCKET)
		: mSock(sock), mHostname(hostname), mPort(port) {}
	virtual ~TcpConnection();

	virtual int connect();
	bool isConnected();
	virtual int read(void* dst, int max);
	virtual int write(const void* src, int len);
	virtual void close();
	int getAddr(MAConnAddr& addr);
protected:
	MoSyncSocket mSock;
	const std::string mHostname;
	const u16 mPort;
	uint mInetAddr;
};

class ProtocolConnection : public Connection {
public:
	// takes ownership of transport.
	ProtocolConnection(Connection* transport, const std::string& path);
	virtual ~ProtocolConnection();

	virtual int connect();	//calls sendHeaders and readHeaders
	bool isConnected();
	virtual int read(void* dst, int max);
	virtual int write(const void* src, int len); //calls sendHeaders if necessary.
	virtual void close();
	int getAddr(MAConnAddr& addr);

	void SetRequestHeader(std::string key, const std::string& value);

	//returns NULL if value doesn't exist. The returned pointer should be discarded ASAP.
	const std::string* GetResponseHeader(std::string key) const;

	int finish();	//calls sendHeaders if necessary. always calls readHeaders.

	enum State {
		SETUP=1, WRITING, FINISHING, FINISHED
	} mState;

protected:
	virtual std::string methodString() = 0;
	virtual std::string protocolVersion() = 0;
	virtual std::string pathString();
	virtual int readResponseCode(const char* line, int len) = 0;

private:
	typedef std::pair<std::string, std::string> HeaderPair;
	typedef hash_map<std::string, std::string> HeaderMap;
	typedef HeaderMap::iterator HeaderItr;
	typedef HeaderMap::const_iterator HeaderItrC;

	Connection* mTransport;
	const std::string mPath;
	char mBuffer[1024];
	int mPos, mSize;
	HeaderMap mRequestHeaders, mResponseHeaders;
	bool mHeadersSent;

	int readLine(const char*& lineP);
	int sendHeaders();
	int readHeaders();
};

enum ProtocolUrlParseResult {
	URL_MISSING_SLASH=-2,
	URL_INVALID_PORT=-1,
	SUCCESS=1
};
ProtocolUrlParseResult parseProtocolURL(const char *parturl, u16 *port,
	u16 defaultPort, const char **path, std::string &address);

class HttpConnection : public ProtocolConnection {
public:
	HttpConnection(Connection* transport, const std::string& hostname,
		const std::string& path, int method);
protected:
	//ProtocolConnection
	std::string methodString();
	std::string protocolVersion();

	HttpConnection* http();

	int readResponseCode(const char* line, int len);

	virtual int write(const void* src, int len);

	const int mMethod;
};

#define ANY_PORT (-1)

class TcpServer : public Closable {
public:
	//Returns the opened port number or a CONNERR code.
	int open(int port = ANY_PORT);

	//Stores the address of a new, pre-connected BtSppConnection in connP.
	//This connection should be deleted when no longer needed.
	//Returns >0 or CONNERR code.
	int accept(TcpConnection*& connP);

	void close();

	int getAddr(MAConnAddr& addr);

	TcpServer();
	virtual ~TcpServer() { close(); }

private:
	MoSyncSocket mSock;
};

#endif	//SYMBIAN

#endif	//NET_H
