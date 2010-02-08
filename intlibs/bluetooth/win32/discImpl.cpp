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

#include "../config_bluetooth.h"

#include <Winsock2.h>
#include <Ws2bth.h>
#include <vector>

#ifdef _WIN32_WCE
#include <bthapi.h>
#include <bt_api.h>
#include <bt_sdp.h>
#include <ws2bth.h>
#else
#include <BluetoothAPIs.h>
#endif

#define WSASERVICE_NOT_FOUND             10108L

#include <helpers/helpers.h>
#include <helpers/array.h>
#include <helpers/CriticalSection.h>

#include "../discInternal.h"
using namespace Bluetooth;
#include <MAUtil/mauuid.h>

#include "../bt_errors.h"
using namespace MoSyncError;

extern MAUUID GUID2MAUUID(GUID guid);
extern GUID MAUUID2GUID(MAUUID uuid);

//***************************************************************************
//Function declarations
//***************************************************************************

static bool dumpSED(const SDP_ELEMENT_DATA& sed, std::vector<MAUUID>* uuids);
static bool dumpStream(LPBYTE pValueStream, ULONG cbStreamSize, std::vector<MAUUID>* uuids);

//***************************************************************************
//Helpers
//***************************************************************************

static void logLevel(int level) {
	for(int i=0; i<level; i++) {
		LOGBT("  ");
	}
}

#ifdef BLUETOOTH_DEBUGGING_MODE
static int getSEDInt(const SDP_ELEMENT_DATA& sed, int size) {
	switch(size) {
	case 0:
		return sed.data.int8;
	case 1:
		return sed.data.int16;
	case 2:
		return sed.data.int32;
	default:
		DEBIG_PHAT_ERROR;
		return 0;
	}
}
#endif

static void dumpSEDUUID(const SDP_ELEMENT_DATA& sed, int size, std::vector<MAUUID>* uuids) {
	LOGBT("UUID ");
	MAUUID uuid = Bluetooth_Base_MAUUID;
	switch(size) {
	case 1:
		LOGBT("%04X\n", sed.data.uuid16);
		uuid.i[0] = sed.data.uuid16;
		break;
	case 2:
		LOGBT("%08lX\n", sed.data.uuid32);
		uuid.i[0] = sed.data.uuid32;	//slightly unsure about this.
		break;
	case 4:
		uuid = GUID2MAUUID(sed.data.uuid128);
		LOGBT("%08X-%08X-%08X-%08X\n", uuid.i[0], uuid.i[1], uuid.i[2], uuid.i[3]);
		break;
	default:
		DEBIG_PHAT_ERROR;
	}
	//save!
	uuids->push_back(uuid);
}

static bool dumpNonSpecificSED(const SDP_ELEMENT_DATA& sed, std::vector<MAUUID>* uuids) {
	switch(sed.type) {
	case SDP_TYPE_NIL:
		LOGBT("NIL\n");
		break;
	case SDP_TYPE_STRING:
		/*{
			WCHAR uniString[512];
			ULONG uniLen = 512;
			char cString[1024];
			TEST_NZ(BluetoothSdpGetString(sed.data.string.value, sed.data.string.length, NULL,
				STRING_NAME_OFFSET, uniString, &uniLen));
			wcstombs(cString, uniString, uniLen);
			LOGBT("\"%s\"", cString);
		}*/
		LOGBT("String: \"");
		LOGBTBIN(sed.data.string.value, sed.data.string.length);
		LOGBT("\"\n");
		break;
	case SDP_TYPE_BOOLEAN:
		LOGBT("%s\n", sed.data.booleanVal ? "true" : "false");
		break;
	case SDP_TYPE_URL:
		LOGBT("URL: ");
		LOGBTBIN(sed.data.url.value, sed.data.url.length);
		LOGBT("\n");
		break;
	case SDP_TYPE_SEQUENCE:
	case SDP_TYPE_ALTERNATIVE:
		//sed.data is a union. sequence and alternative are identical,
		//so I can just pick any one of them here.
		LOGBT("%s, %lu bytes.\n", sed.type == SDP_TYPE_SEQUENCE ? "Sequence" : "Alternative",
			sed.data.sequence.length);
		TEST(dumpStream(sed.data.sequence.value, sed.data.sequence.length, uuids));	//next level
		break;
	case SDP_TYPE_CONTAINER:
		LOGBT("Container!\n");
		break;
	default:
		DEBIG_PHAT_ERROR;
	}
	return true;
}

static bool dumpSED(const SDP_ELEMENT_DATA& sed, std::vector<MAUUID>* uuids) {
	static int level = -1;
	MYASSERT(level < 16, BTERR_MAX_SDP_LEVEL);
	level++;
	//logLevel(level);
	//LOGBT("Type: 0x%02X, 0x%04X\n", sed.type, sed.specificType);
	logLevel(level);
	//LOGBT("Data: ");
	if(sed.specificType == SDP_ST_NONE) {
		TEST(dumpNonSpecificSED(sed, uuids));
	} else {
		int mainType = (sed.specificType & 0xF0) >> 4;
		int size = (sed.specificType & 0xF00) >> 8;
		switch(mainType) {
		case 1:	//UINT
		case 2:	//INT
			LOGBT("%sint%i: ", (mainType == 1 ? "u" : ""), 8 << size);
			if(size <= 2) {	// <= 32-bit
#ifdef BLUETOOTH_DEBUGGING_MODE
				int data = getSEDInt(sed, size);
#endif
				char format[32];
				sprintf(format, "%%%c (0x%%0*X)\n", mainType == 1 ? 'u' : 'i');
				LOGBT(format, data, 2 * (1 << size), data);
			} else if(size == 3) {	//64-bit
				char format[32];
				sprintf(format, "%%" INT64PREFIX "%c (0x%%" INT64PREFIX "016X)\n", mainType == 1 ? 'u' : 'i');
				LOGBT(format, sed.data.int64, sed.data.int64);
			} else if(size == 4) {	//128-bit
				LOGBT("%016" INT64PREFIX "X%016" INT64PREFIX "X\n", sed.data.int128.HighPart,
					sed.data.int128.LowPart);
			} else {
				DEBIG_PHAT_ERROR;
			}
			break;
		case 3:	//UUID
			dumpSEDUUID(sed, size, uuids);
			break;
		default:
			DEBIG_PHAT_ERROR;
		}
	}
	level--;
	return true;
}

static BOOL CALLBACK BtEnumAttributesCallback(ULONG uAttribId, LPBYTE pValueStream,
																			 ULONG cbStreamSize, LPVOID pvParam)
{
	CriticalSectionHandler csh(&gBt.critSec);
	LOGBT("Attribute ID %lu, streamSize: %lu bytes\n", uAttribId, cbStreamSize);
	SDP_ELEMENT_DATA sed;
	TEST_NZ(BluetoothSdpGetElementData(pValueStream, cbStreamSize, &sed));
	TEST(dumpSED(sed, (std::vector<MAUUID>*)pvParam));
	return true;
}

static bool dumpStream(LPBYTE pValueStream, ULONG cbStreamSize, std::vector<MAUUID>* uuids) {
	HBLUETOOTH_CONTAINER_ELEMENT handle = NULL;
	for(;;) {
		SDP_ELEMENT_DATA sed;
		int res = BluetoothSdpGetContainerElementData(pValueStream, cbStreamSize, &handle, &sed);
		if(res == ERROR_NO_MORE_ITEMS) {
			break;
		} else {
			TEST_NZ(res);
			TEST(dumpSED(sed, uuids));
		}
	}
	return true;
}

int MASdpEnumAttrs(LPBYTE pSDPStream, ULONG streamSize, std::vector<MAUUID>* pUUIDs) {
	//should fill serv.uuids with what we're looking for, if everything went well.
	GLE(BluetoothSdpEnumAttributes(pSDPStream, streamSize,
		BtEnumAttributesCallback, pUUIDs));
	return 1;
}

int handleSdpResponse(std::vector<BtService>* services, LPWSAQUERYSET pQs) {
	int port = 0;
	if(pQs->lpcsaBuffer) {
		port = ((SOCKADDR_BTH *)pQs->lpcsaBuffer->RemoteAddr.lpSockaddr)->port;
	}
	BtService serv;
	serv.port = port;
	if(pQs->lpszServiceInstanceName)
		serv.name = pQs->lpszServiceInstanceName;
	LOGBT("dSS 6\n");

	//should fill serv.uuids with what we're looking for, if everything went well.
	GLE(MASdpEnumAttrs(pQs->lpBlob->pBlobData, pQs->lpBlob->cbSize, &serv.uuids));
	LOGBT("dSS 7\n");
	if(serv.uuids.size() == 0) {	//this seems to happen when the server says "no services".
		return 0;
	}
	services->push_back(serv);
	return 1;
}
