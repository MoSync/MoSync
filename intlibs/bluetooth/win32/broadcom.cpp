/* Copyright (C) 2010 Mobile Sorcery AB

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

#include "../config_bluetooth.h"
#ifdef BROADCOM_SUPPORTED

#include <helpers/helpers.h>
#include <helpers/log.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <set>

#pragma push_macro("FAIL")
#undef FAIL
#ifdef _WIN32_WCE
#include <BtSdkCE.h>
#else
#include <BtwLib.h>
#endif	//_WIN32_WCE
#pragma pop_macro("FAIL")

#include <helpers/CriticalSection.h>

#include "broadcom.h"
#include "../discInternal.h"
#include "../bt_errors.h"

using namespace MoSyncError;
using namespace std;


class BroadcomBtSppConnection : public BtSppConnection {
public:
	BroadcomBtSppConnection(const MABtAddr* address, uint port);
	~BroadcomBtSppConnection();

	int connect();
	int read(void* dst, int max);
	int write(const void* src, int len);
	void close();
	int getAddr(MAConnAddr& addr);
private:
	class MyRfCommPort : public CRfCommPort {
	private:
		//CRfCommPort
		//These functions will be called in an interrupt context.
		//Call no Broadcom or other IO functions from them.
		//Thread safety is a must.
		void OnDataReceived(void* p_data, UINT16 len);
		void OnEventReceived(UINT32 event_code);

		void* mReadDst;	//valid if not NULL
		int mReadLen;	//valid if mReadDst not NULL

#define READBUFSIZE (64*1024)
		char mReadBuffer[READBUFSIZE];
		int mReadBufPos;	//always valid

		CRITICAL_SECTION mCritSec;
		HANDLE mEvent;

	public:
		MyRfCommPort();
		~MyRfCommPort();

		int read(void* dst, int max);
		HANDLE event() { return mEvent; }
	} mPort;

	UINT8 mScn;
	BD_ADDR mRemoteBdAddr;

	BroadcomBtSppConnection(...);
	friend class BroadcomBtSppServer;
};

class CBTWidcommStack : public CBtIf
{
private:
	// Gotta keep a list of all devices discovered during the current operation,
	// to weed out duplicates.
	struct MABtAddr_less : public binary_function<MABtAddr, MABtAddr, bool> {
		bool operator()(const MABtAddr& _Left, const MABtAddr& _Right) const {
			return memcmp(_Left.a, _Right.a, BTADDR_LEN) < 0;
		}
	};
	typedef set<MABtAddr, MABtAddr_less> AddrSet;
	AddrSet mDevices;
public:
	//CBTWidcommStack() {}
	//virtual ~CBTWidcommStack() {}

	BOOL StartInquiry() {
		mDevices.clear();
		return CBtIf::StartInquiry();
	}

	virtual void OnDeviceResponded(BD_ADDR bda, DEV_CLASS devClass,
		BD_NAME bdName, BOOL bConnected)
	{
		LOGBT("OnDeviceResponded()\n");
		{
			CriticalSectionHandler csh(&gBt.critSec);
			BtDevice dev;
			for(int i=0; i<BTADDR_LEN; i++) {
				memcpy(dev.address.a, bda, BTADDR_LEN);
			}
			pair<AddrSet::iterator, bool> res = mDevices.insert(dev.address);
			if(!res.second) {
				LOGBT("Duplicate: %02x%02x%02x%02x%02x%02x\n",
					bda[0], bda[1], bda[2], bda[3], bda[4], bda[5]);
				return;
			}
#ifndef NO_DEVICE_NAME
			//dev.name = (char*)bdName;
#endif
			LOGBT("d%i: \"%s\" (%02x%02x%02x%02x%02x%02x)\n", gBt.devices.size(),
				bdName, bda[0], bda[1], bda[2], bda[3], bda[4], bda[5]);
			gBt.devices.push_back(dev);
		}
		gBt.deviceCallback();
	}

	virtual void OnInquiryComplete(BOOL success, short num_responses) {
		LOGBT("OnInquiryComplete()\n");
		{
			CriticalSectionHandler csh(&gBt.critSec);
			if(success) {
				if(mDevices.size() != num_responses) {
					LOGBT("num_responses: %i, mDevices.size() %i\n", num_responses, mDevices.size());
				}
				setDiscoveryState(mDevices.size() + 1);
			} else {
				setDiscoveryState(CONNERR_GENERIC);
			}
		}
		gBt.deviceCallback();
	}
};

static CBTWidcommStack* sStack = NULL;

BtSppConnection* Broadcom::createBtSppConnection(const MABtAddr* address, uint port) {
	return new BroadcomBtSppConnection(address, port);
}

void Broadcom::setup() {
	if(sStack == NULL) {
		sStack = new CBTWidcommStack();
	}
}

bool Broadcom::haveRadio() {
	LOGBT("Broadcom::haveRadio(): %i\n", sStack->IsDeviceReady());
	return sStack->IsDeviceReady() == TRUE;
}

int Broadcom::startDeviceDiscovery() {
	LOGBT("Broadcom::startDeviceDiscovery()\n");
	if(sStack->StartInquiry())
		return 0;
	else
		return CONNERR_GENERIC;
}

void Broadcom::cancelDiscovery() {
	sStack->StopInquiry();
}

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
#if 1
	//wait for it...
	WaitForSingleObject(mPort.event(), INFINITE);
	result = mPort.IsConnected(&mRemoteBdAddr);
	LOGBT("IsConnected: %i\n", result);
	if(!result) {
		return CONNERR_GENERIC;
	}
#else
	while((result = mPort.IsConnected(&mRemoteBdAddr)) == 0) {
		LOGBT("IsConnected: %i\n", result);
		Sleep(1000);
	}
#endif
	return 1;//res;
}

void BroadcomBtSppConnection::close() {
	LOGBT("Close\n");
	mPort.Close();
	LOGBT("Closed\n");
}

int BroadcomBtSppConnection::getAddr(MAConnAddr& addr) {
	BD_ADDR remote;
	BOOL is = mPort.IsConnected(&remote);
	if(!is)
		return CONNERR_GENERIC;
	addr.family = CONN_FAMILY_BT;
	memcpy(addr.bt.addr.a, remote, BTADDR_LEN);
	addr.bt.port = mScn;
	return 1;
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
	BOOL res = SetEvent(mEvent);
	DEBUG_ASSERT(res);
}

int BroadcomBtSppConnection::write(const void* src, int len) {
	DEBUG_ASSERT(len > 0);
	//start write
	//continue write if not everything was written
	int bytesLeft = len;
	while(bytesLeft > 0) {
		UINT16 written;
		UINT16 toWrite = MIN(len, 1 << 15);
		CRfCommPort::PORT_RETURN_CODE res = mPort.Write((void*)src, toWrite, &written);
		if(res != CRfCommPort::SUCCESS) {
			LOG("RfCommPort::Write() returned %i\n", res);
			return CONNERR_GENERIC;
		}
		DEBUG_ASSERT(written <= toWrite);
		bytesLeft -= written;
	}
	return 1;
}

#endif	//BROADCOM_SUPPORTED
