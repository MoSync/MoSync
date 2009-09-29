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
	~BtService() {
		LOGBT("~BtService\n");
	}
};

struct BtDevice {
#ifndef NO_DEVICE_NAME
	tstring name;
#endif
	MABtAddr address;
	~BtDevice() {
		LOGBT("~BtDevice\n");
	}
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
