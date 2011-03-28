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

#include <vector>

#include <helpers/helpers.h>
#include <helpers/array.h>
#include <helpers/CriticalSection.h>

#include "discovery.h"
using namespace Bluetooth;
#include <MAUtil/mauuid.h>
#include "discInternal.h"

#include "bt_errors.h"
using namespace MoSyncError;

#ifdef WIN32
#include <Winsock2.h>
#include <Ws2bth.h>

#ifdef _WIN32_WCE
#include <bthapi.h>
#include <bt_api.h>
#include <bt_sdp.h>
#include <ws2bth.h>
#include <bthutil.h>
typedef ULONGLONG BTH_ADDR;
#else
#include <BluetoothAPIs.h>
#endif

#ifdef BROADCOM_SUPPORTED
#include "win32/broadcom.h"
#endif

#define WSASERVICE_NOT_FOUND             10108L
#include "win32/blueSoleil.h"

//***************************************************************************
//Defines
//***************************************************************************

#ifdef BLUETOOTH_DEBUGGING_MODE
#define WSAFAIL { LOG("Failure %i ", WSAGetLastError()); IN_FILE_ON_LINE; }
#else
#define WSAFAIL
#endif	//BLUETOOTH_DEBUGGING_MODE

#define WSAT_BASE(a) if((a) == SOCKET_ERROR) { WSAFAIL;\
	setDiscoveryState(CONNERR_GENERIC); callback(); return; }
#define WSATD WSAT_BASE
#define WSATS WSAT_BASE

//#define NO_DEVICE_NAME

#if 0//def _WIN32_WCE
#define CHECK_CANCELED(action)
#else
#define CHECK_CANCELED(action) if(sCanceled) { setDiscoveryState(CONNERR_CANCELED); action; }
#endif

//***************************************************************************
//Variables
//***************************************************************************

Bt gBt;
static HANDLE sLookup;
static bool sCanceled;

//***************************************************************************
//Helpers
//***************************************************************************

#ifdef UNICODE
void TCHARtoCHAR(char *dst, const TCHAR *src, int num) {
	while(num && *src) {
		*dst = (char)*src;
		src++;
		dst++;
		num--;
	}
	*dst = '\0';
}
#else
#define TCHARtoCHAR(x, y, z) strncpy(x, y, z)
#endif

#if 0//def BLUETOOTH_DEBUGGING_MODE
static void BTHAtoBytes(byte* bytes, BTH_ADDR btha) {
	for(int i=0; i<BTADDR_LEN; i++) {
		bytes[i] = (BYTE)(btha >> (40 - i*8));
	}
}
#endif
static BTH_ADDR bytesToBTHA(const byte* bytes) {
	BTH_ADDR btha = 0;
	for(int i=0; i<BTADDR_LEN; i++) {
		btha |= ((ULONGLONG)bytes[i]) << (40 - i*8);
	}
	return btha;
}

GUID MAUUID2GUID(MAUUID uuid) {
	GUID guid;
	guid.Data1 = uuid.i[0];
	guid.Data2 = u16(uuid.i[1] >> 16);
	guid.Data3 = u16(uuid.i[1]);
	for(int i=0; i<2; i++) {
		for(int j=0; j<4; j++) {
			guid.Data4[i*4+j] = u8(uuid.i[2+i] >> (8*(3-j)));
		}
	}
	return guid;
}
MAUUID GUID2MAUUID(GUID guid) {	//untested
	MAUUID mauuid;
	mauuid.i[0] = guid.Data1;
	mauuid.i[1] = (u32(guid.Data2) << 16) | guid.Data3;
	for(int i=0; i<2; i++) {
		mauuid.i[2+i] = 0;
		for(int j=0; j<4; j++) {
			mauuid.i[2+i] |= u32(guid.Data4[i*4+j]) << (8*(3-j));
		}
	}
	return mauuid;
}

void setDiscoveryState(int state) {
	CriticalSectionHandler csh(&gBt.critSec);
	gBt.discoveryState = state;
}

static void doSearch2() {
	LOGBT("doServiceSearch\n");
#define SEARCH gBt.serviceSearch	//Use ONLY when protected by critical section

	WSAQUERYSET qs;
	SOCKADDR_BTH sockAddr;
	MABtCallback callback;

	{
		CriticalSectionHandler csh(&gBt.critSec);
		sockAddr.addressFamily = AF_BTH;
		sockAddr.port = 0;
		sockAddr.serviceClassId = MAUUID2GUID(SEARCH.uuid);
		sockAddr.btAddr = bytesToBTHA(SEARCH.address.a);

#if 0
		//test
		const MAUUID& u(SEARCH.uuid);
		const GUID& g(sockAddr.serviceClassId);
		LOG("%08X-%08X-%08X-%08X\n", u.i[0], u.i[1], u.i[2], u.i[3]);
		const byte* b(g.Data4);
		LOG("%08X-%04X-%04X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X\n", g.Data1, g.Data2, g.Data3,
			b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7]);
#endif

		callback = SEARCH.cb;
	}
	LOGBT("dSS 1\n");	//TODO: cleanup

	ZERO_OBJECT(qs);
	qs.dwSize = sizeof(WSAQUERYSET);
	qs.dwNameSpace = NS_BTH;

#ifdef _WIN32_WCE
	//set address
	CSADDR_INFO csai;
	ZERO_OBJECT(csai);
	csai.RemoteAddr.lpSockaddr = (LPSOCKADDR)&sockAddr;
	csai.RemoteAddr.iSockaddrLength = sizeof(sockAddr);
	qs.lpcsaBuffer = &csai;
	qs.dwNumberOfCsAddrs = 1;

	//set further restrictions
	BTHNS_RESTRICTIONBLOB rb;
	ZERO_OBJECT(rb);
	rb.type = SDP_SERVICE_SEARCH_ATTRIBUTE_REQUEST;
	rb.uuids[0].u.uuid128 = sockAddr.serviceClassId;
	rb.uuids[0].uuidType = SDP_ST_UUID128;

	rb.pRange[0].minAttribute = SDP_ATTRIB_PROTOCOL_DESCRIPTOR_LIST;
	rb.pRange[0].maxAttribute = SDP_ATTRIB_PROTOCOL_DESCRIPTOR_LIST;
	/*rb.pRange[1].minAttribute = SDP_ATTRIB_SERVICE_ID;
	rb.pRange[1].maxAttribute = SDP_ATTRIB_SERVICE_ID;
	rb.pRange[2].minAttribute = SDP_ATTRIB_CLASS_ID_LIST;
	rb.pRange[2].maxAttribute = SDP_ATTRIB_CLASS_ID_LIST;
	rb.pRange[3].minAttribute = SDP_ATTRIB_PROFILE_DESCRIPTOR_LIST;
	rb.pRange[3].maxAttribute = SDP_ATTRIB_PROFILE_DESCRIPTOR_LIST;
	rb.pRange[4].minAttribute = 0x100;	//service name, in the default language
	rb.pRange[4].maxAttribute = 0x100;*/
	rb.numRange = 1;

	BLOB blob;
	blob.cbSize = sizeof(rb);
	blob.pBlobData = (BYTE *)&rb;
	qs.lpBlob = &blob;

	sockAddr.serviceClassId = GUID_NULL;	//test
#else
	qs.lpServiceClassId = &sockAddr.serviceClassId;

	TCHAR addrString[128];
	DWORD addrStrLen = sizeof(addrString);
	WSATS(WSAAddressToString((LPSOCKADDR)&sockAddr, sizeof(sockAddr), NULL, addrString, &addrStrLen));
	qs.lpszContext = addrString;
#endif
	LOGBT("dSS 2\n");

	CHECK_CANCELED(return);

	INT res = WSALookupServiceBegin(&qs,
#ifndef _WIN32_WCE 
		LUP_FLUSHCACHE
#else
		0
#endif
		,&sLookup);
	WSATS(res);
	LOGBT("dSS 3\n");

#define DUMPPTR(name) LOGBT("%s: 0x%p\n", #name, name)
	for(;;) {
		LOGBT("dSS 4\n");
		CHECK_CANCELED(break);
		char buffer[4096];
		DWORD qsSize = sizeof(buffer);
		LPWSAQUERYSET pQs = (LPWSAQUERYSET) buffer;
		ZeroMemory(pQs, sizeof(WSAQUERYSET));
		pQs->dwSize = sizeof(WSAQUERYSET);
		res = WSALookupServiceNext(sLookup, LUP_RETURN_COMMENT | LUP_RETURN_NAME |
			LUP_RETURN_ADDR | LUP_RETURN_BLOB, &qsSize, pQs);
		DUMPPTR((void*)res);
		if(res == SOCKET_ERROR && (WSAGetLastError() == WSA_E_NO_MORE || WSAGetLastError() == WSAENOMORE))
			break;
		WSATS(res);
		LOGBT("dSS 5\n");

		DUMPPTR(pQs->lpcsaBuffer);
		DUMPPTR(pQs->lpszServiceInstanceName);
		DUMPPTR(pQs->lpBlob);
		if(pQs->lpBlob) {
			DUMPPTR(pQs->lpBlob->pBlobData);
			DUMPPTR((void*)pQs->lpBlob->cbSize);
			for(uint i=0; i<pQs->lpBlob->cbSize; i++) {
				if((i & 3) == 0 && i >= 4) {
					LOG(" ");
				}
				LOG("%02X", pQs->lpBlob->pBlobData[i]);
			}
			LOG("\n");
		}

		std::vector<BtService> services;
		res = handleSdpResponse(&services, pQs);
		if(res < 0) {
			{
				CriticalSectionHandler csh(&gBt.critSec);
				setDiscoveryState(res);
			}
			callback();
			return;
		}

		{
			CriticalSectionHandler csh(&gBt.critSec);
			for(uint i=0; i<services.size(); i++) {
				const BtService& serv(services[i]);
				LOGBT("sh s%i: port %i. %i UUIDs. Name:\"%s\" Comment:\"%s\"\n", SEARCH.services.size(),
					serv.port, serv.uuids.size(), pQs->lpszServiceInstanceName, pQs->lpszComment);
				for(size_t j=0; j<serv.uuids.size(); j++) {
					LOGBT("UUID: %08X-%08X-%08X-%08X\n",
						serv.uuids[j].i[0], serv.uuids[j].i[1], serv.uuids[j].i[2], serv.uuids[j].i[3]);
				}
				SEARCH.services.push_back(serv);
			}
		}
		LOGBT("dSS 8\n");
		callback();
		LOGBT("dSS 9\n");
	}
	LOGBT("dSS a\n");
	WSATD(WSALookupServiceEnd(sLookup));
	{
		CriticalSectionHandler csh(&gBt.critSec);
		if(!sCanceled) {
			LOGBT("Search done\n");
			setDiscoveryState(SEARCH.services.size() + 1);
		}
	}
	LOGBT("dSS b\n");
	callback();
	LOGBT("dSS c\n");
}

static DWORD WINAPI doSearch(void*) {
	doSearch2();
	return 0;
}

static void doDiscovery2() {
	LOGBT("doDeviceDiscovery\n");
	WSAQUERYSET qs;
	MABtCallback callback = gBt.deviceCallback;

	ZERO_OBJECT(qs);
	qs.dwSize = sizeof(WSAQUERYSET);
	qs.dwNameSpace = NS_BTH;

	CHECK_CANCELED(return);
	
	INT res = WSALookupServiceBegin(&qs, LUP_CONTAINERS
#ifndef _WIN32_WCE 
		| LUP_FLUSHCACHE
#endif
		,&sLookup);
	WSATD(res);

	for(;;) {
		char buffer[4096];
		DWORD qsSize = sizeof(buffer);
		LPWSAQUERYSET pQs = (LPWSAQUERYSET) buffer;
		ZeroMemory(pQs, sizeof(WSAQUERYSET));
		pQs->dwSize = sizeof(WSAQUERYSET);
		pQs->dwNameSpace = NS_BTH;
		pQs->lpBlob = NULL;

		CHECK_CANCELED(break);

		res = WSALookupServiceNext(sLookup,
			(gBt.names ? LUP_RETURN_NAME : 0) | LUP_RETURN_ADDR, &qsSize, pQs);
		if(res == SOCKET_ERROR && (WSAGetLastError() == WSA_E_NO_MORE || WSAGetLastError() == WSAENOMORE))
			break;
		WSATD(res);
		DEBUG_ASSERT(pQs->dwNumberOfCsAddrs == 1);
		//some results have empty names
		//bHaveName = pwsaResults->lpszServiceInstanceName && *(pwsaResults->lpszServiceInstanceName);
		if(pQs->lpszServiceInstanceName == NULL) {
#ifdef _WIN32_WCE 
			pQs->lpszServiceInstanceName = (LPWSTR)TEXT("");
#else
			pQs->lpszServiceInstanceName = (LPSTR)TEXT("");				
#endif
		}
		if(*(pQs->lpszServiceInstanceName) == 0) {
			//scan again, until you get a name
		}

		BTH_ADDR b = ((SOCKADDR_BTH *)pQs->lpcsaBuffer->RemoteAddr.lpSockaddr)->btAddr;
		BtDevice dev;
#ifndef NO_DEVICE_NAME
		dev.name = pQs->lpszServiceInstanceName;
#endif
		for(int i=0; i<BTADDR_LEN; i++) {
			dev.address.a[i] = BYTE(b >> (40 - i*8));
		}

		{
			CriticalSectionHandler csh(&gBt.critSec);
			LOGBT("d%i: \"%s\" (%04x%08lx)\n", gBt.devices.size(),
				pQs->lpszServiceInstanceName, GET_NAP(b), GET_SAP(b));
			gBt.devices.push_back(dev);
			//LOGBT("pushed\n");
		}
		callback();
	}
	WSATD(WSALookupServiceEnd(sLookup));
	{
		CriticalSectionHandler csh(&gBt.critSec);
		if(!sCanceled) {
			LOGBT("Discovery done\n");
			setDiscoveryState(gBt.devices.size() + 1);
		}
	}
	callback();
}
static DWORD WINAPI doDiscovery(void*) {
	doDiscovery2();
	return 0;
}

static bool haveRadio() {
#ifdef _WIN32_WCE
	if(gBluetoothStack == BTSTACK_WINSOCK) {
		DWORD mode;
		int res = BthGetMode(&mode);
		if(res != ERROR_SUCCESS)
			return false;
		return (mode == BTH_CONNECTABLE || mode == BTH_DISCOVERABLE);
	}
#ifdef BROADCOM_SUPPORTED
	else if(gBluetoothStack == BTSTACK_BROADCOM) {
		return Broadcom::haveRadio();
	}
#endif
	else {
		return false;
	}
#else
	BLUETOOTH_FIND_RADIO_PARAMS btfrp;
	btfrp.dwSize = sizeof(btfrp);
	HANDLE hRadio;
	HBLUETOOTH_RADIO_FIND hbtrf = BluetoothFindFirstRadio(&btfrp, &hRadio);
	if(hbtrf == NULL) {
		LOG("BluetoothFindFirstRadio error %lu\n", GetLastError());
		return false;
	}
	GLE(BluetoothFindRadioClose(hbtrf));
	return true;
#endif
}

//***************************************************************************
//Syscalls
//***************************************************************************

int Bluetooth::maBtDiscoveryState() {
	return gBt.discoveryState;
}

int Bluetooth::maBtStartDeviceDiscovery(MABtCallback cb, bool names) {
	LOGBT("maBtStartDeviceDiscovery\n");
	DEBUG_ASSERT(cb != NULL);
	MYASSERT(gBt.discoveryState != 0, BTERR_DISCOVERY_IN_PROGRESS);

	//check for active bluetooth radios in the system.
	//if we don't have one, tell the user.
	if(!haveRadio()) {
		return -1;
	}
	sCanceled = false;

	CriticalSectionHandler csh(&gBt.critSec);
	setDiscoveryState(0);
	gBt.nextDevice = 0;
	gBt.devices.clear();
	gBt.deviceCallback = cb;
	gBt.names = names;
	LOGBT("Current stack: %i\n", gBluetoothStack);
	switch(gBluetoothStack) {
	case BTSTACK_WINSOCK:
		{
			HANDLE hThread = CreateThread(NULL, 0, doDiscovery, NULL, 0, NULL);
			GLE(hThread);
			GLE(CloseHandle(hThread));
		}
		break;
#ifdef BLUESOLEIL_SUPPORTED
	case BTSTACK_BLUESOLEIL:
		Soleil::startDeviceDiscovery();
		break;
#endif
#ifdef BROADCOM_SUPPORTED
	case BTSTACK_BROADCOM:
		return Broadcom::startDeviceDiscovery();
#endif
	default:
		DEBIG_PHAT_ERROR;
	}
	return 0;
}

int Bluetooth::maBtCancelDiscovery() {
	CriticalSectionHandler csh(&gBt.critSec);
	if(gBt.discoveryState != 0)
		return 0;
#if	0//def _WIN32_WCE
	int res = WSALookupServiceEnd(sLookup);
	if(res == SOCKET_ERROR) {
		WSAFAIL;
		DEBIG_PHAT_ERROR;
	}
#else
#ifdef BROADCOM_SUPPORTED
	if(gBluetoothStack == BTSTACK_BROADCOM) {
		Broadcom::cancelDiscovery();
	} else
#endif
	{
		sCanceled = true;
	}
#endif
	return 1;
}


int Bluetooth::maBtGetNewDevice(MABtDeviceNative* dst) {
	CriticalSectionHandler csh(&gBt.critSec);
	LOGBT("maBtGetNewDevice(). next %i, size %i\n", gBt.nextDevice, gBt.devices.size());
	if(gBt.nextDevice == gBt.devices.size()) {
		return 0;
	}
	const BtDevice& dev = gBt.devices[gBt.nextDevice++];
	memcpy(dst->address.a, dev.address.a, BTADDR_LEN);
#ifndef NO_DEVICE_NAME
	if(gBt.names) {
		uint strLen = MIN(dev.name.size(), uint(dst->nameBufSize - 1));
		TCHARtoCHAR(dst->name, dev.name.c_str(), strLen);
		dst->name[strLen] = 0;
	}
#endif
	return 1;
}

int Bluetooth::maBtStartServiceDiscovery(const MABtAddr* address, const MAUUID* uuid,
	MABtCallback cb)
{
	DEBUG_ASSERT(cb != NULL);
	MYASSERT(gBt.discoveryState != 0, BTERR_DISCOVERY_IN_PROGRESS);
	if(!haveRadio()) {
		return -1;
	}
	sCanceled = false;

	CriticalSectionHandler csh(&gBt.critSec);
	BtServiceSearch& s(gBt.serviceSearch);
	s.cb = cb;
	s.services.clear();
	s.nextService = 0;
	setDiscoveryState(0);
	s.uuid = *uuid;
	s.address = *address;
	s.thread = CreateThread(NULL, 0, doSearch, 0, 0, NULL);
	return 0;
}


int Bluetooth::maBtGetNewService(MABtServiceNative* dst) {
	CriticalSectionHandler csh(&gBt.critSec);
	BtServiceSearch& s(gBt.serviceSearch);
	if(s.nextService == s.services.size()) {
		return 0;
	}
	MYASSERT(dst->nameBufSize >= 0, BTERR_NEGATIVE_BUFFER_SIZE);
	const BtService& serv = s.services[s.nextService++];
	dst->port = serv.port;
	int servNameLen = MIN(dst->nameBufSize - 1, (int)serv.name.size());
	TCHARtoCHAR(dst->name, serv.name.c_str(), servNameLen);
	dst->name[servNameLen] = 0;
	for(size_t i=0; i<serv.uuids.size(); i++) {
		dst->uuids[i] = serv.uuids[i];
	}
	return 1;
}

int Bluetooth::maBtGetNextServiceSize(MABtServiceSize* dst) {
	CriticalSectionHandler csh(&gBt.critSec);
	BtServiceSearch& s(gBt.serviceSearch);
	if(s.nextService == s.services.size()) {
		return 0;
	}
	const BtService& serv = s.services[s.nextService];
	dst->nameBufSize = serv.name.size() + 1;
	dst->nUuids = serv.uuids.size();
	return 1;
}

#elif defined( BLUEZ_SUPPORTED )

    // Code is in linux/bluez/interface.cpp

#elif defined( COCOA_SUPPORTED )

// Code is in darwin/interface.mm

#else
int Bluetooth::maBtDiscoveryState() {
	BIG_PHAT_ERROR(ERR_FUNCTION_UNIMPLEMENTED);
}

int Bluetooth::maBtStartDeviceDiscovery(MABtCallback cb, bool names) {
	BIG_PHAT_ERROR(ERR_FUNCTION_UNIMPLEMENTED);
}

int Bluetooth::maBtGetNewDevice(MABtDeviceNative* dst) {
	BIG_PHAT_ERROR(ERR_FUNCTION_UNIMPLEMENTED);
}

int Bluetooth::maBtStartServiceDiscovery(const MABtAddr* address, const MAUUID* uuid,
	MABtCallback cb)
{
	BIG_PHAT_ERROR(ERR_FUNCTION_UNIMPLEMENTED);
}

int Bluetooth::maBtGetNewService(MABtServiceNative* dst) {
	BIG_PHAT_ERROR(ERR_FUNCTION_UNIMPLEMENTED);
}

int Bluetooth::maBtGetNextServiceSize(MABtServiceSize* dst) {
	BIG_PHAT_ERROR(ERR_FUNCTION_UNIMPLEMENTED);
}

int Bluetooth::maBtCancelDiscovery(void) {
	BIG_PHAT_ERROR(ERR_FUNCTION_UNIMPLEMENTED);
}


#endif	//WIN32

//***************************************************************************
//C++ wrapper
//***************************************************************************

int MABtServiceAuto::fillWithNew() {
	int res;
	MABtServiceSize ss;
	res = maBtGetNextServiceSize(&ss);
	if(res == 0)
		return 0;
	this->name = new char[ss.nameBufSize];
	this->nameBufSize = ss.nameBufSize;
	this->nUuids = ss.nUuids;
	this->uuids = new MAUUID[ss.nUuids];
	if(!name || !uuids) {
		DEBIG_PHAT_ERROR;
	}
	res = maBtGetNewService(this);
	DEBUG_ASSERT(res > 0);
	return res;
}
