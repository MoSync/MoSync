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

#include "config_bluetooth.h"

#include <helpers/helpers.h>
#include <helpers/log.h>

#include "server.h"
#include "bt_errors.h"

using namespace MoSyncError;

#ifdef WIN32
WinsockBtSppConnection::WinsockBtSppConnection(SOCKET sock, const SOCKADDR_BTH& addr)
: mSock(sock), mAddress(addr)
{
}

int BtSppServer::open(const MAUUID& uuid, const char* name, int port) {
	MYASSERT(name != NULL, BTERR_NONAME);
	//open the port
	mAddr.addressFamily = AF_BTH;
	mAddr.btAddr = 0;
	//addr.serviceClassId = NULL_GUID;
	mAddr.port = port;
	mSock = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
	if(mSock == INVALID_SOCKET) {
		LOG("socket failed, code %i\n", WSAGetLastError());
		return CONNERR_GENERIC;
	}
	if(bind(mSock, (const sockaddr*)&mAddr, sizeof(mAddr)) == SOCKET_ERROR) {
		LOG("bind failed, code %i\n", WSAGetLastError());
		return CONNERR_GENERIC;
	}
	if(listen(mSock, SOMAXCONN) == SOCKET_ERROR) {
		LOG("listen failed, code %i\n", WSAGetLastError());
		return CONNERR_GENERIC;
	}
	int addrLen = sizeof(mAddr);
	if(getsockname(mSock, (sockaddr*)&mAddr, &addrLen) == SOCKET_ERROR) {
		LOG("getsockname failed, code %i\n", WSAGetLastError());
		return CONNERR_GENERIC;
	}

	//register the UUID
	WSAQUERYSET qs;
	ZeroMemory(&qs, sizeof(qs));
	qs.dwSize = sizeof(qs);
	qs.lpszServiceInstanceName = (char*)name;	//should be safe
	GUID guid = MAUUID2GUID(uuid);
	qs.lpServiceClassId = &guid;
	qs.dwNameSpace = NS_BTH;
	qs.dwNumberOfCsAddrs = 1;
	CSADDR_INFO csai;
	csai.iProtocol = BTHPROTO_RFCOMM;
	csai.iSocketType = SOCK_STREAM;
	csai.LocalAddr.lpSockaddr = (LPSOCKADDR)&mAddr;
	csai.LocalAddr.iSockaddrLength = sizeof(mAddr);
	csai.RemoteAddr = csai.LocalAddr;
	qs.lpcsaBuffer = &csai;

	if(WSASetService(&qs, RNRSERVICE_REGISTER, 0) == SOCKET_ERROR) {
		LOG("WSASetService failed, code %i\n", WSAGetLastError());
		return CONNERR_GENERIC;
	}

	return mAddr.port;
}

int BtSppServer::getAddr(MAConnAddr& addr) {
	if(mSock <= 0)
		return CONNERR_GENERIC;
	addr.family = CONN_FAMILY_BT;
	sockAddrBth2BtAddr(mAddr, addr.bt);
	return 1;
}

int BtSppServer::accept(BtSppConnection*& connP) {
	SOCKADDR_BTH addr;
	int addrlen = sizeof(addr);
	SOCKET sock = ::accept(mSock, (sockaddr*)&addr, &addrlen);
	if(sock == INVALID_SOCKET) {
		LOG("accept failed, code %i\n", WSAGetLastError());
		return CONNERR_GENERIC;
	}
	connP = new WinsockBtSppConnection(sock, addr);
	return 1;
}

void BtSppServer::close() {
	if(closesocket(mSock) == SOCKET_ERROR) {
		LOG("closesocket failed, code %i\n", WSAGetLastError());
	}
}

#else	//WIN32

int BtSppServer::open(const MAUUID& uuid, const char* name, int port) {
	return -1;
}

int BtSppServer::getAddr(MAConnAddr& addr) {
	return -1;
}

int BtSppServer::accept(BtSppConnection*& connP) {
	return -1;
}

void BtSppServer::close() {
}

#endif	//WIN32
