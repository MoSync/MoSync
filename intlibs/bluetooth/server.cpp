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
