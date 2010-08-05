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

#include <helpers/CriticalSection.h>

#include "connection.h"
#include "bt_errors.h"
#include "discInternal.h"
#ifdef BLUEZ_SUPPORTED
#include "linux/bluez/connectionbluez.hpp"
#endif
#ifdef COCOA_SUPPORTED
#include "darwin/ConnectionCocoa.h"
#endif
#ifdef BROADCOM_SUPPORTED
#include "win32/broadcom.h"
#endif


#undef BLUESOLEIL_SUPPORTED	//TEMP HACK
#ifdef BLUESOLEIL_SUPPORTED
#include <BlueSoleil/Btsdk_ui.h>
#endif

#ifdef _WIN32_WCE
#include <bt_sdp.h>	//must be below other includes to avoid compilation errors.
#endif

using namespace MoSyncError;

//Connection
//**********
int Connection::readFully(void* dst, int len) {
	int pos = 0;
	while(pos < len) {
		int res = read((char*)dst + pos, len - pos);
		if(res <= 0)
			return res;
		pos += res;
	}
	return 1;
}

BtSppConnection* createBtSppConnection(const MABtAddr* address, uint port) {
	switch(Bluetooth::gBluetoothStack) {
#ifdef WINSOCK_SUPPORTED
	case Bluetooth::BTSTACK_WINSOCK:
		return new WinsockBtSppConnection(address, port);
#endif
#ifdef BROADCOM_SUPPORTED
	case Bluetooth::BTSTACK_BROADCOM:
		return Broadcom::createBtSppConnection(address, port);
#endif
#ifdef TOSHIBA_SUPPORTED
	case Bluetooth::BTSTACK_TOSHIBA:
		return new ToshibaBtSppConnection(address, port);
#endif
#ifdef BLUESOLEIL_SUPPORTED
	case Bluetooth::BTSTACK_BLUESOLEIL:
		return new BlueSoleilBtSppConnection(address, port);
#endif
#ifdef BLUEZ_SUPPORTED
	case Bluetooth::BTSTACK_BLUEZ:
		return new Bluetooth::BlueZ::BtSppConnectionBluez( address, port );
#endif
#ifdef COCOA_SUPPORTED
		case Bluetooth::BTSTACK_COCOA:
			return new Bluetooth::Darwin::BtSppConnectionCocoa( address, port );
#endif
	default:
		return NULL;
	}
}


#ifdef BLUESOLEIL_SUPPORTED
//BlueSoleilBtSppConnection
//**********************
BlueSoleilBtSppConnection::BlueSoleilBtSppConnection(const MABtAddr* address, uint port)
{
}

BlueSoleilBtSppConnection::~BlueSoleilBtSppConnection() {
}

int BlueSoleilBtSppConnection::connect() {
	BTDEVHDL handle = Btsdk_AddRemoteDevice(mAddress);
	if(handle == BTSDK_INVALID_HANDLE) {
		return CONNERR_GENERIC;
	}
	//scan services of device, find one matching our port number.
	//only then can we connect.
}

int BlueSoleilBtSppConnection::read(void* dst, int max);
int BlueSoleilBtSppConnection::write(const void* src, int len);
void BlueSoleilBtSppConnection::close();
#endif

#ifdef WINSOCK_SUPPORTED
//WinsockBtSppConnection
//**********************
WinsockBtSppConnection::WinsockBtSppConnection(const MABtAddr* address, uint port)
: mSock(INVALID_SOCKET)
{
	mAddress.addressFamily = AF_BTH;
	mAddress.btAddr = 0;

	for(int i=0; i<BTADDR_LEN; i++) {
		mAddress.btAddr |= ULONGLONG(address->a[i]) << (8 * (5-i));
	}

	mAddress.serviceClassId = RFCOMM_PROTOCOL_UUID;
	mAddress.port = port;
}

WinsockBtSppConnection::~WinsockBtSppConnection() {
	close();
}

int WinsockBtSppConnection::connect() {
	int res;

	mSock = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
	if(mSock == INVALID_SOCKET) {
		LOG("socket failed, code %i\n", WSAGetLastError());
		return CONNERR_GENERIC;
	}
#if 0
	LINGER ling;
	ling.l_linger = 0;
	ling.l_onoff = 1;
	res = setsockopt(mSock, SOL_SOCKET, SO_LINGER, (const char*)&ling, sizeof(LINGER));
	if(res == SOCKET_ERROR) {
		LOGBT("setsockopt failed, code %i\n", WSAGetLastError());
		return CONNERR_GENERIC;
	}
#endif
	res = ::connect(mSock, (const sockaddr*)&mAddress, sizeof(mAddress));
	if(res == SOCKET_ERROR) {
		LOGBT("connect failed, code %i\n", WSAGetLastError());
		return CONNERR_GENERIC;
	}
	return 1;
}

void WinsockBtSppConnection::close() {
	if(mSock != INVALID_SOCKET) {
		int res;
		res = shutdown(mSock, SD_BOTH);
		if(res == SOCKET_ERROR) {
			LOGBT("shutdown failed, code %i\n", WSAGetLastError());
		}
		res = closesocket(mSock);
		if(res == SOCKET_ERROR) {
			LOG("closesocket failed, code %i\n", WSAGetLastError());
		}
	}
}

int WinsockBtSppConnection::read(void *dst, int max) {
	int res = ::recv(mSock, (char*)dst, max, 0);
	if(res == SOCKET_ERROR) {
		LOGBT("read failed, code %i\n", WSAGetLastError());
		return CONNERR_GENERIC;
	} else if(res == 0) {
		return CONNERR_CLOSED;
	} else {
		return res;
	}
}

int WinsockBtSppConnection::write(const void* src, int len) {
	int res = ::send(mSock, (const char*)src, len, 0);
	if(res == SOCKET_ERROR) {
		LOGBT("send failed, code %i\n", WSAGetLastError());
		return CONNERR_GENERIC;
	} else if(res == 0) {
		return CONNERR_CLOSED;
	} else {
		return res;
	}
}

int WinsockBtSppConnection::getAddr(MAConnAddr& addr) {
	if(mSock <= 0)
		return CONNERR_GENERIC;
	addr.family = CONN_FAMILY_BT;
	sockAddrBth2BtAddr(mAddress, addr.bt);
	return 1;
}

void sockAddrBth2BtAddr(const SOCKADDR_BTH& sockAddr, MAConnAddrBt& btAddr) {
	for(int i=0; i<BTADDR_LEN; i++) {
		btAddr.addr.a[i] = (byte)(sockAddr.btAddr >> (8 * (5-i)));
	}
	btAddr.port = sockAddr.port;
}
#endif
