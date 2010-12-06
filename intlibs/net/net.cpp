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

#define CONFIG_H	//hack
#define LOGGING_ENABLED

#include "net.h"
#include "helpers/log.h"
#include "helpers/cpp_defs.h"
#include "helpers/helpers.h"
#include "net_errors.h"

using namespace MoSyncError;

#ifdef WIN32
typedef int socklen_t;
#endif

#ifdef _WIN32_WCE
#if _WIN32_WCE >= 0x500
#include <initguid.h>
#include <connmgr.h>  
#endif
#endif

//******************************************************************************
// for symbian
//******************************************************************************

int atoiLen(const char* str, int len);

//note: line is not zero-terminated!
int readProtocolResponseCode(const char* protocolSlash, const char* line, int len) {
	//check protocol
	int responseCode = CONNERR_PROTOCOL;
	if(len >= (int)sizeof("HTTP/x.x xxx") - 1) if(sstrcmp(line, protocolSlash) == 0) {
		//const char* line = baseLine + sizeof("HTTP/") - 1;
		int pos = sizeof("HTTP/") - 1;
		if(isdigit(line[pos++])) if(line[pos++] == '.') if(isdigit(line[pos++]))	if(line[pos++] == ' ')
		{
			//check status-code
			const char* code = line + pos;
			int i = 0;
			for(; i<3; i++) {
				if(!isdigit(code[i]))
					break;
			}
			pos += 3;
			if(i == 3) {
				//check end of status-code
				if(line[pos] == ' ' || pos >= len) {
					responseCode = atoiLen(code, 3);
					if(line[pos] == ' ') {
						//read reason-phrase
						pos++;
#ifdef SOCKET_DEBUGGING_MODE
						LOG("%s %i ", protocolSlash, responseCode);
						LOGBIN(line + pos, len - pos);
						LOG("\n");
#endif
					}
				}
			}
		}
	}
	if(responseCode < 100) {
#ifdef SOCKET_DEBUGGING_MODE
		LOG("bad first line: \"%s\"\n", line);
#endif
		return CONNERR_PROTOCOL;
	}
	return responseCode;
}

#ifndef SYMBIAN

//******************************************************************************
// TcpConnection helpers
//******************************************************************************
#ifdef _WIN32_WCE
#if _WIN32_WCE >= 0x500
int ensureconnected()
{
	HRESULT _res;

	static HANDLE _conn = 0;
	DWORD _status = 0;
	DWORD _timeout = 20000;
	CONNMGR_CONNECTIONINFO _info = {0};

	ConnMgrConnectionStatus(&_conn, &_status);
    if (_status==CONNMGR_STATUS_CONNECTED)
		return 0;

	memset(&_info, 0, sizeof(CONNMGR_CONNECTIONINFO));
	_info.cbSize = sizeof(CONNMGR_CONNECTIONINFO);
	_info.dwParams = CONNMGR_PARAM_GUIDDESTNET;
	_info.dwFlags = CONNMGR_FLAG_PROXY_HTTP;
	_info.dwPriority = CONNMGR_PRIORITY_USERINTERACTIVE;
	_info.guidDestNet = IID_DestNetInternet;

	_res = ConnMgrEstablishConnectionSync(&_info, &_conn,_timeout,&_status);
	if (_res == E_FAIL)
	{
			return -1;
	}

	return 0;
}
#endif
#endif

// Creates and prepares a socket, but does not connect it.
MoSyncSocket MASocketCreate(const char* address, int& result, uint& inetAddr) {
	int iRet;

#ifdef _WIN32_WCE
#if _WIN32_WCE >= 0x500
	if(ensureconnected()!=0) {
		result = CONNERR_GENERIC;
		return INVALID_SOCKET;
	}
#endif
#endif

	MoSyncSocket mySocket;
	// Create socket
	mySocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(mySocket == INVALID_SOCKET)
	{
		LOG("MASocketOpen: socket returned error code %d\n", SOCKET_ERRNO);
		result = CONNERR_GENERIC;
		return INVALID_SOCKET;
	}

	inetAddr = inet_addr(address);
	if(inetAddr == INADDR_NONE) {
		hostent *hostEnt;
		if((hostEnt = gethostbyname(address)) == NULL) {
			LOG("MASocketOpen: DNS resolve failed. %d\n", SOCKET_ERRNO);
			result = CONNERR_DNS;
			return INVALID_SOCKET;
		}
		inetAddr = (uint)*((uint*)hostEnt->h_addr_list[0]);
		if(inetAddr == INADDR_NONE) {
			LOG("MASocketOpen: Could not parse the resolved ip address. %d\n", SOCKET_ERRNO);
			result = CONNERR_URL;
			return INVALID_SOCKET;
		}
	}

	// Make sure Nagle's algorithm is disabled
	int v;
	socklen_t len = sizeof(v);
	iRet = getsockopt(mySocket, IPPROTO_TCP, TCP_NODELAY, (char*)&v, &len);
	if(iRet == SOCKET_ERROR) {
		result = CONNERR_INTERNAL;
		return INVALID_SOCKET;
	}
	//LOG("MASocketOpen: TCP_NODELAY default value: %i\n", v);
	v = 1;
	iRet = setsockopt(mySocket, IPPROTO_TCP, TCP_NODELAY, (char*)&v, len);
	if(iRet == SOCKET_ERROR) {
		result = CONNERR_INTERNAL;
		return INVALID_SOCKET;
	}
	iRet = getsockopt(mySocket, IPPROTO_TCP, TCP_NODELAY, (char*)&v, &len);
	if(iRet == SOCKET_ERROR) {
		result = CONNERR_INTERNAL;
		return INVALID_SOCKET;
	}
	//LOG("MASocketOpen: TCP_NODELAY set value: %i\n", v);

	result = 1;
	return mySocket;
}

int MASocketConnect(MoSyncSocket sock, uint inetAddr, u16 port) {
	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inetAddr;
	clientService.sin_port = htons( port );

	// Connect to the Server
	int iRet = connect(sock, (sockaddr*) &clientService, sizeof(clientService));
	if(SOCKET_ERROR == iRet)
	{
		LOG("MASocketConnect: connect returned error code %d\n", SOCKET_ERRNO);
		return CONNERR_GENERIC;
	}
	return 1;
}

//returns INVALID_SOCKET on failure. puts CONNERR-compliant code in result.
MoSyncSocket MASocketOpen(const char* address, u16 port, int& result, uint& inetAddr) {
	MoSyncSocket mySocket = MASocketCreate(address, result, inetAddr);
	if(mySocket == INVALID_SOCKET)
		return INVALID_SOCKET;

	result = MASocketConnect(mySocket, inetAddr, port);
	if(result <= 0)
		return INVALID_SOCKET;
	return mySocket;
}

//******************************************************************************
// TcpConnection proper
//******************************************************************************

int TcpConnection::connect() {
	int result;
	mSock = MASocketCreate(mHostname.c_str(), result, mInetAddr);
	if(mSock == INVALID_SOCKET)
		return result;

	return MASocketConnect(mSock, mInetAddr, mPort);
}

bool TcpConnection::isConnected() {
	return mSock != INVALID_SOCKET;
}

int TcpConnection::getAddr(MAConnAddr& addr) {
	if(mSock == INVALID_SOCKET)
		return CONNERR_GENERIC;
	addr.family = CONN_FAMILY_INET4;
	addr.inet4.addr = mInetAddr;
	addr.inet4.port = mPort;
	return 1;
}

TcpConnection::~TcpConnection() {
	close();
}

void TcpConnection::close() {
	if(mSock != INVALID_SOCKET) {
		int res;
		res = shutdown(mSock, 2);	//stop both reading and writing
		if(SOCKET_ERROR == res) {
			LOG("TcpConnection::shutdown failed: %i\n", SOCKET_ERRNO);
		}
		res = closesocket(mSock);
		if(SOCKET_ERROR == res) {
			LOG("TcpConnection::closesocket failed: %i\n", SOCKET_ERRNO);
		}
		mSock = INVALID_SOCKET;
	}
}

int TcpConnection::read(void* dst, int max) {
	int bytesRecv = recv(mSock, (char*)dst, max, 0);
	if(SOCKET_ERROR == bytesRecv) {
		LOG("TcpConnection::read: recv failed. error code: %i\n", SOCKET_ERRNO);
		return CONNERR_GENERIC;
	} else if (bytesRecv == 0) {
		return CONNERR_CLOSED;
	} else {
		return bytesRecv;
	}
}

int TcpConnection::write(const void* src, int len) {
	int bytesSent = send(mSock, (const char*) src, len, 0);
	if(bytesSent != len || SOCKET_ERROR == bytesSent) {
		LOG("TcpConnection::write: send failed. error code: %i\n", SOCKET_ERRNO);
		return CONNERR_GENERIC;
	} else {
		return 1;
	}
}


//******************************************************************************
// ProtocolConnection helpers
//******************************************************************************

int atoiLen(const char* str, int len) {
	return atoi(std::string(str, len).c_str());
}


ProtocolUrlParseResult parseProtocolURL(const char *parturl, u16 *port,
										u16 defaultPort, const char **path, std::string &address)
{
	const char* port_m1 = strchr(parturl, ':');
	*path = strchr(parturl, '/');
	if(*path == NULL) {
		return URL_MISSING_SLASH;
	}
	if(port_m1 > *path) {
		port_m1 = NULL;	//ignore instead of throwing error
	}
	const char* hostname_end;
	//u16 port;
	if(port_m1 == NULL) {
		*port = defaultPort;
		hostname_end = *path;
	} else {
		hostname_end = port_m1;
		int iPort = atoi(port_m1 + 1);
		if(iPort <= 0 || iPort >= 1 << 16) {
			return URL_INVALID_PORT;
		}
		*port = (u16)iPort;
	}

	address = std::string(parturl, (size_t)(hostname_end - parturl));

	return SUCCESS;
}

//******************************************************************************
// ProtocolConnection proper
//******************************************************************************

ProtocolConnection::ProtocolConnection(Connection* transport, const std::string& path) :
mState(SETUP), mTransport(transport), mPath(path), mPos(0), mSize(0),
mHeadersSent(false)
{
	//spaces are not allowed in URLs.
	MYASSERT(mPath.find(' ') == mPath.npos, ERR_URL_SPACE);
}

ProtocolConnection::~ProtocolConnection() {
	close();
}

void ProtocolConnection::close() {
	if(mTransport)
		delete mTransport;
}

int ProtocolConnection::getAddr(MAConnAddr& addr) {
	return mTransport->getAddr(addr);
}

int ProtocolConnection::connect() {
	TLTZ_PASS(mTransport->connect());
	TLTZ_PASS(sendHeaders());
	return readHeaders();
}

bool ProtocolConnection::isConnected() {
	return mTransport->isConnected();
}

int ProtocolConnection::finish() {
	if(!mHeadersSent) {
		TLTZ_PASS(sendHeaders());
	}
	mState = FINISHING;
	return readHeaders();
}

std::string ProtocolConnection::pathString() {
	return mPath;
}

int ProtocolConnection::sendHeaders() {
	if(!mTransport->isConnected()) {
		TLTZ_PASS(mTransport->connect());
	}

	//start jabbering
	std::string outdata;
	outdata += methodString()+" "+pathString()+" " + protocolVersion() /*"HTTP/1.0"*/ + "\r\n";

	//add other headers
	for(HeaderItr itr = mRequestHeaders.begin(); itr != mRequestHeaders.end(); itr++) {
		outdata += itr->first+": "+itr->second+"\r\n";
	}

	outdata += "\r\n";
	TLTZ_PASS(mTransport->write(outdata.c_str(), outdata.length()));
	mHeadersSent = true;
	return 1;
}

int ProtocolConnection::readHeaders() {
	DEBUG_ASSERT(mState == FINISHING);
	//read status line
	int responseCode, lineLen;
	const char* baseLine;
	TLTZ_PASS(lineLen = readLine(baseLine));

	TLTZ_PASS(responseCode = readResponseCode(baseLine, lineLen));

	//read headers
	while(true) {
		//read a line
		TLTZ_PASS(readLine(baseLine));

		//an empty line signifies the end of headers.
		if(baseLine[0] == 0)
			break;

		//format: key ':' (' ')* value
		const char* colon = strchr(baseLine, ':');
		const char* valPtr = NULL;
		if(colon != NULL) {
			valPtr = colon + 1;
			while(*valPtr == ' ')
				valPtr++;
		}
		if(colon == NULL || valPtr == NULL) {
			LOG("bad header line: \"%s\"\n", baseLine);
			return CONNERR_PROTOCOL;
		}
		std::string key(baseLine, (colon - baseLine));
		std::string value(valPtr);
		LOGS("header %s: %s\n", key.c_str(), value.c_str());

		lower(key);	//headers are case-insensitive.

		//if the key is already present, comma-combine the values.
		HeaderItr itr = mResponseHeaders.find(key);
		if(itr != mResponseHeaders.end()) {
			LOGS("Combined!\n");
			itr->second += ", " + value;
		} else {
			mResponseHeaders.insert(HeaderPair(key, value));
		}
	}
	mState = FINISHED;
	return responseCode;
}

//puts a pointer to a line in lineP.
//a line is a zero-terminated string with no CR('\0xA', '\r') or LF('\0xD', '\n') bytes.
//returns strlen or CONNERR.
int ProtocolConnection::readLine(const char*& lineP) {
	int startPos = mPos;
	while(true) {
		//either a CR, an LF, or a CRLF pair will terminate a line.
		while(mPos < mSize) {
			int oldPos = mPos;
			switch(mBuffer[mPos]) {
			case '\r':
				if(mBuffer[mPos+1] == '\n') {
					//we got ourselves a good line
					mPos++;
				}
			case '\n':	//fallthrough is intentional
				mPos++;
				mBuffer[oldPos] = 0;
				lineP = mBuffer + startPos;
				return oldPos - startPos;	//strlen
			default:
				mPos++;
			}
		}

		//something clever could be done here when we want to support arbitrarily large headers.
		if(mPos == sizeof(mBuffer)) {
			if(startPos == 0) {
				LOG("header buffer full!\n");
				return CONNERR_INTERNAL;
			}
			int size = mPos - startPos;
			memcpy(mBuffer, mBuffer + startPos, size);
			mPos = size;
			startPos = 0;
		}

		int res;
		TLTZ_PASS(res = mTransport->read(mBuffer + mPos, sizeof(mBuffer) - mPos));
		mSize += res;
		mBuffer[mSize] = 0;	//for string functions
	}
}

int ProtocolConnection::read(void* dst, int max) {
	if(mPos < mSize) {	//there's still some data left in the buffer
		int len = MIN(mSize - mPos, max);
		memcpy(dst, mBuffer + mPos, len);
		mPos += len;
		return len;
	} else {	//we gotta read from outside
		return mTransport->read(dst, max);
	}
}

int ProtocolConnection::write(const void* src, int len) {
	if(!mHeadersSent) {
		TLTZ_PASS(sendHeaders());
	}
	return mTransport->write(src, len);
}

void ProtocolConnection::SetRequestHeader(std::string key, const std::string& value) {
	lower(key);
	HeaderItr itr = mRequestHeaders.find(key);
	if(itr != mRequestHeaders.end())	//key already exists
		itr->second = value;	//replace value
	else
		mRequestHeaders.insert(HeaderPair(key, value));
}

const std::string* ProtocolConnection::GetResponseHeader(std::string key) const {
	lower(key);
	HeaderItrC itr = mResponseHeaders.find(key);
	if(itr == mResponseHeaders.end())
		return NULL;
	else
		return &itr->second;
}

//******************************************************************************
// HttpConnection
//******************************************************************************

HttpConnection::HttpConnection(Connection* transport, const std::string& hostname,
	const std::string& path, int method) :
ProtocolConnection(transport, path), mMethod(method)
{
	SetRequestHeader("Host", hostname);
	SetRequestHeader("Connection", "close");
}

std::string HttpConnection::methodString() {
	switch(mMethod) {
	case HTTP_GET:
		return "GET";
	case HTTP_POST:
		return "POST";
	case HTTP_HEAD:
		return "HEAD";
	default:
		BIG_PHAT_ERROR(ERR_HTTP_METHOD_INVALID);
	}
}

std::string HttpConnection::protocolVersion() {
	return "HTTP/1.0";
}

int HttpConnection::readResponseCode(const char* line, int len) {
	int responseCode;
	TLTZ_PASS(responseCode = readProtocolResponseCode("HTTP/", line, len));
	return responseCode;
}

int HttpConnection::write(const void* src, int len) {
	MYASSERT(mMethod == HTTP_POST, ERR_HTTP_NONPOST_WRITE);
	return ProtocolConnection::write(src, len);
}

HttpConnection* HttpConnection::http() {
	return this;
}

//******************************************************************************
// TcpServer
//******************************************************************************

TcpServer::TcpServer() : mSock(INVALID_SOCKET) {
}

//TODO: fix all return 0;

int TcpServer::open(int port) {
	if(mSock != INVALID_SOCKET) {
		close();
	}

	mSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(mSock == INVALID_SOCKET)
	{
		LOG("MASocketOpen: socket returned error code %d\n", SOCKET_ERRNO);
		return 0;
	}

	sockaddr_in sa;
	char myname[/*MAXHOSTNAME*/256+1]; 
	struct hostent *hp; 

	memset(&sa, 0, sizeof(struct sockaddr_in));
	gethostname(myname, 256); /* who are we? */ 
	hp = gethostbyname(myname); /* get our address info */ 
	if (hp == NULL) /* we don't exist !? */ return 0; 
	sa.sin_family = hp->h_addrtype; /* this is our host address */ 
	sa.sin_port = htons( port );

	if(bind(mSock, (struct sockaddr*)&sa, sizeof(struct sockaddr_in))<0) {
		close();
		return 0;
	}

	if(listen(mSock, 5)<0) {
		close();
		return 0;
	}

	return 1;
}

int TcpServer::accept(TcpConnection*& connP) {
	if(mSock == INVALID_SOCKET) return 0;
	sockaddr_in sa;
	socklen_t size = sizeof(sa);
	MoSyncSocket sock = ::accept(mSock, (sockaddr*)&sa, &size);
	if(sock == INVALID_SOCKET) {
		return 0;
	} else {
		connP = new TcpConnection(inet_ntoa(sa.sin_addr), sa.sin_port, sock);
		return 1;
	}
}

void TcpServer::close() {
	if(mSock != INVALID_SOCKET) {
		closesocket(mSock);
		mSock = INVALID_SOCKET;
	}
}

int TcpServer::getAddr(MAConnAddr& addr) {
	if(mSock == INVALID_SOCKET)
		return CONNERR_GENERIC;
	addr.family = CONN_FAMILY_INET4;
	sockaddr_in sa;
	socklen_t saSize = sizeof(sa);
	int result = getsockname(mSock, (sockaddr*)&sa, &saSize);
	if(result == (int)INVALID_SOCKET) {
		LOG("getsockname error %d\n", SOCKET_ERRNO);
		return CONNERR_GENERIC;
	}
	addr.inet4.addr = sa.sin_addr.s_addr;
	addr.inet4.port = sa.sin_port;
	return 1;
}

#endif	//SYMBIAN
