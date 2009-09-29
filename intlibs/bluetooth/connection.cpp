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
	switch(Bluetooth::gBluetoothStack)
        {
#ifdef WINSOCK_SUPPORTED
	case Bluetooth::BTSTACK_WINSOCK:
		return new WinsockBtSppConnection(address, port);
#endif
#ifdef BROADCOM_SUPPORTED
	case Bluetooth::BTSTACK_BROADCOM:
		return new BroadcomBtSppConnection(address, port);
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

#ifdef BROADCOM_SUPPORTED
//BroadcomBtSppConnection
//***********************
BroadcomBtSppConnection::BroadcomBtSppConnection(const MABtAddr* address, uint port) {
	memcpy(mRemoteBdAddr, address->a, BD_ADDR_LEN);
	mScn = (UINT8)port;
}

BroadcomBtSppConnection::~BroadcomBtSppConnection() {
	close();
}

int BroadcomBtSppConnection::connect() {
	int result = mPort.OpenClient(mScn, mRemoteBdAddr);
	LOGBT("OpenClient %i\n", result);
	if(result != CRfCommPort::SUCCESS)
		return CONNERR_GENERIC;
#if 0
	//wait for it...
	//WaitForSingleObject(mEvent, INFINITE);
	result = mPort.IsConnected(&mRemoteBdAddr);
	LOGBT("IsConnected: %i\n", result);
#endif
	while((result = mPort.IsConnected(&mRemoteBdAddr)) == 0) {
		LOGBT("IsConnected: %i\n", result);
		Sleep(1000);
	}
	return 1;//res;
}

void BroadcomBtSppConnection::close() {
	LOGBT("Close\n");
	mPort.Close();
	LOGBT("Closed\n");
}

int BroadcomBtSppConnection::read(void* dst, int max) {
	return mPort.read(dst, max);
}

BroadcomBtSppConnection::MyRfCommPort::MyRfCommPort() : mReadDst(NULL), mReadBufPos(0) {
	InitializeCriticalSection(&mCritSec);
	mEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	DEBUG_ASSERT(mEvent != NULL);
}
BroadcomBtSppConnection::MyRfCommPort::~MyRfCommPort() {
	DeleteCriticalSection(&mCritSec);
	BOOL res = CloseHandle(mEvent);
	DEBUG_ASSERT(res);
}

int BroadcomBtSppConnection::MyRfCommPort::read(void* dst, int max) {
	LOGBT("read %i\n", max);
	CriticalSectionHandler csh(&mCritSec);
	DEBUG_ASSERT(mReadDst == NULL);
	DEBUG_ASSERT(dst != NULL);
	if(mReadBufPos > 0) {
		//we've already got some data, read becomes instant
		int len = MIN(max, mReadBufPos);
		memcpy(dst, mReadBuffer, len);
		if(mReadBufPos > len) {	//if we have leftovers, move them down
			memcpy(mReadBuffer, mReadBuffer + len, mReadBufPos - len);
		}
		mReadBufPos -= len;
		LOGBT("took %i from buffer, left %i\n", len, mReadBufPos);
		return len;
	} else {
		//no data yet, we wait.
		mReadDst = dst;
		mReadLen = max;
		LOGBT("waiting for read...\n");
		LeaveCriticalSection(&mCritSec);
		DWORD result = WaitForSingleObject(mEvent, INFINITE);
		EnterCriticalSection(&mCritSec);
		if(result != WAIT_OBJECT_0) {
			LOGBT("WaitForSingleObject failed: %i\n", GetLastError());
			return CONNERR_GENERIC;
		}
		LOGBT("got %i\n", mReadLen);
		//mReadLen has been set to the number of bytes actually read by OnDataReceived
		mReadDst = NULL;
		return mReadLen;
	}
}

void BroadcomBtSppConnection::MyRfCommPort::OnDataReceived(void* p_data, UINT16 len) {
	LOGBT("OnDataReceived %i\n", len);
	CriticalSectionHandler csh(&mCritSec);
	if(mReadDst == NULL) {
		//if we're not reading, store data in our buffer.
		if(mReadBufPos + len > READBUFSIZE) {
			BIG_PHAT_ERROR(BTERR_READ_BUFFER_OVERFLOW);
		}
		memcpy(mReadBuffer + mReadBufPos, p_data, len);
		mReadBufPos += len;
	} else {
		//else, if we ARE reading, store as much data in the read dst as we can.
		mReadLen = MIN(mReadLen, len);
		memcpy(mReadDst, p_data, mReadLen);
		if(len > mReadLen) {
			//leftovers go in the buffer.
			DEBUG_ASSERT(mReadBufPos == 0);
			mReadBufPos = len - mReadLen;
			memcpy(mReadBuffer, (byte*)p_data + mReadLen, mReadBufPos);
		}
		//let read() know about it
		BOOL res = SetEvent(mEvent);
		DEBUG_ASSERT(res);
	}
}

void BroadcomBtSppConnection::MyRfCommPort::OnEventReceived(UINT32 event_code) {
	LOGBT("OnEventReceived %i\n", event_code);
	//BOOL res = SetEvent(mEvent);
	//DEBUG_ASSERT(res);
}

int BroadcomBtSppConnection::write(const void* src, int len) {
	//start write
	//wait until finished
	//restart write if not everything was written
	return -1;
}
#endif
