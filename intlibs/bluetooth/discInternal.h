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

#ifndef DISCINTERNAL_H
#define DISCINTERNAL_H

#include "config_bluetooth.h"

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#endif	//WIN32

#include <string>
#include <vector>

#include <helpers/log.h>

#include "discovery.h"

//***************************************************************************
//Data types
//***************************************************************************

#ifdef _WIN32_WCE
typedef std::basic_string<wchar_t> tstring;
#else
typedef std::string tstring;
#endif

struct BtService {
	BtService() : port(-1) {}
	int port;
	tstring name;
	std::vector<MAUUID> uuids;
};

struct BtDevice {
#ifndef NO_DEVICE_NAME
	tstring name;
#endif
	MABtAddr address;
};

struct BtServiceSearch {
	std::vector<BtService> services;
	uint nextService;
#ifdef WIN32
	HANDLE thread;
#endif
	Bluetooth::MABtCallback cb;
	MAUUID uuid;
	MABtAddr address;
	~BtServiceSearch() {
		LOGBT("~BtServiceSearch\n");
	}
};

struct Bt {
	int discoveryState;
	std::vector<BtDevice> devices;
	uint nextDevice;
	Bluetooth::MABtCallback deviceCallback;
	BtServiceSearch serviceSearch;
	bool names;

#ifdef WIN32
	//protects devices, nextDevice, discoveryState, services and nextService.
	CRITICAL_SECTION critSec;
#endif
};

extern Bt gBt;

namespace Bluetooth {
	extern BluetoothStack gBluetoothStack;
}

//***************************************************************************
//Shared functions
//***************************************************************************
void setDiscoveryState(int state);

//***************************************************************************
//Platform-implemented functions
//***************************************************************************

#ifdef WIN32
//Returns 0 and calls SetLastError on error.
//int MASdpEnumAttrs(LPBYTE pSDPStream, ULONG streamSize, std::vector<MAUUID>* pUUIDs);

//returns >0 on success, CONNERR <0 on error.
int handleSdpResponse(std::vector<BtService>* services, LPWSAQUERYSET pQs);
#endif

#endif	//DISCINTERNAL_H
