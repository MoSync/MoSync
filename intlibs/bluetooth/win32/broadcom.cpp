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
#include "../connection.h"
#include "../bt_errors.h"

using namespace MoSyncError;


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
public:
   CBTWidcommStack();
   virtual ~CBTWidcommStack();

   virtual void OnDeviceResponded(BD_ADDR bda, DEV_CLASS devClass,
	   BD_NAME bdName, BOOL bConnected) {
   }
   virtual void OnDiscoveryComplete() {
   }

   virtual void OnInquiryComplete(BOOL bSuccess, short nResponses) {
   }
};

BtSppConnection* Broadcom::createBtSppConnection(const MABtAddr* address, uint port) {
	return new BroadcomBtSppConnection(address, port);
}


/*
//BlueSoleilTest
#define BST(func) { int res = (func); if(res != BTSDK_OK) {\
	LOG("%s:%i %i\n", __FILE__, __LINE__, res);\
	setDiscoveryState(CONNERR_GENERIC); gBt.deviceCallback(); return; } }
*/


/*
static void InquiryResult(BTDEVHDL dev_hdl);
static void InquiryComplete();
static void ConnEvent(BTCONNHDL conn_hdl, BTUINT16 event, BTUINT8 *arg);
static const char* btaddr2string(const MABtAddr& a);
*/

void Broadcom::setup() {
/*
	BtSdkCallbackStru cb;

	cb.type = BTSDK_INQUIRY_RESULT_IND;
	cb.func = (void*)InquiryResult;
	Btsdk_RegisterCallback4ThirdParty(&cb);

	cb.type = BTSDK_INQUIRY_COMPLETE_IND;
	cb.func = (void*)InquiryComplete;
	Btsdk_RegisterCallback4ThirdParty(&cb);

	cb.type = BTSDK_CONNECTION_EVENT_IND;
	cb.func = (void*)ConnEvent;
	Btsdk_RegisterCallback4ThirdParty(&cb);
	*/
}

void Broadcom::startDeviceDiscovery() {
	/*
	BST(Btsdk_StartDeviceDiscovery(0, 0, 0));
	*/
}

/*
static void InquiryResult(BTDEVHDL dev_hdl) {
	BtDevice dev;

	BTUINT32 dev_class;
	BST(Btsdk_GetRemoteDeviceClass(dev_hdl, &dev_class));

	BtSdkRemoteDevicePropertyStru props;
	BST(Btsdk_GetRemoteDeviceProperty(dev_hdl, &props));

	byte nameBuf[256];
	WORD len = sizeof(nameBuf);
	BST(Btsdk_GetRemoteDeviceName(dev_hdl, nameBuf, &len));
	dev.name = (char*)nameBuf;

	BST(Btsdk_GetRemoteDeviceAddress(dev_hdl, dev.address.a));

	{
		CriticalSectionHandler csh(&gBt.critSec);
		LOGBT("d%i: \"%s\" (%s)\n", gBt.devices.size(),
			dev.name.c_str(), btaddr2string(dev.addr));
		gBt.devices.push_back(dev);
	}
	gBt.deviceCallback();
}
*/

/*
static void InquiryComplete() {
	{
		CriticalSectionHandler csh(&gBt.critSec);
		LOGBT("Discovery done\n");
		setDiscoveryState(gBt.devices.size() + 1);
	}
	gBt.deviceCallback();
}

static const char* btaddr2string(const MABtAddr& a) {
	static char buffer[16];
	sprintf(buffer, "%02x%02x%02x%02x%02x%02x", a.a[0], a.a[1], a.a[2], a.a[3], a.a[4], a.a[5]);
	return buffer;
}

static void ConnEvent(BTCONNHDL conn_hdl, BTUINT16 event, BTUINT8 *arg) {
	LOG("ConnEvent\n");
}
*/


//BroadcomBtSppConnection
//***********************
BroadcomBtSppConnection::BroadcomBtSppConnection(const MABtAddr* address, uint port) {
	memcpy(mRemoteBdAddr, address->a, BD_ADDR_LEN);
	/*for(int i=0; i<BD_ADDR_LEN; i++) {
		mRemoteBdAddr[i] = address->a[(BD_ADDR_LEN - 1) - i];
	}*/
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
	//TODO: implement
	return CONNERR_GENERIC;
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
	//start write
	//wait until finished
	//restart write if not everything was written
	return -1;
}

#endif	//BROADCOM_SUPPORTED
