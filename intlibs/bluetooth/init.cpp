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

#include "btinit.h"

#if defined (WIN32) || defined (_WIN32_WCE)
#include <Winsock2.h>
#include <Ws2bth.h>
#if !defined(_WIN32_WCE)
#include <BluetoothAPIs.h>
#endif

#ifdef BLUESOLEIL_SUPPORTED
#include <BlueSoleil/Btsdk_ui.h>
static bool sBlueSoleilInited = false;
#include "win32/blueSoleil.h"
#endif
#include "discInternal.h"

namespace Bluetooth {
	BluetoothStack gBluetoothStack = 
#ifdef BLUESOLEIL_SUPPORTED
		BTSTACK_BLUESOLEIL;	//HACK
	//TODO: check if Winsock has any Bluetooth radios available, and, if so,
	//use it in preference to BlueSoleil,
	//but only for connections. For discovery, BlueSoleil may be better.
#else
		BTSTACK_WINSOCK;
#endif
}

void Bluetooth::MABtInit() {
	gBt.discoveryState = CONNERR_UNINITIALIZED;
	gBt.nextDevice = 0;
	gBt.deviceCallback = NULL;

	InitializeCriticalSection(&gBt.critSec);

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

#ifdef BLUESOLEIL_SUPPORTED
	if(!Btsdk_IsSDKInitialized()) {
		int res = Btsdk_Init();
		if(res == BTSDK_OK) {
			sBlueSoleilInited = true;
		} else {
			LOG("Btsdk_Init() failed: %i\n", res);
		}
	}
	if(Btsdk_IsSDKInitialized()) {
		Soleil::setup();
	}
#endif
}

void Bluetooth::MABtClose() {
#ifdef BLUESOLEIL_SUPPORTED
	if(sBlueSoleilInited)
		Btsdk_Done();
#endif
	WSACleanup();
	DeleteCriticalSection(&gBt.critSec);
}

#if !defined(_WIN32_WCE)
int Bluetooth::getLocalAddress(MABtAddr& addr) {
	HANDLE h;
	BLUETOOTH_RADIO_INFO bri;
	BLUETOOTH_FIND_RADIO_PARAMS bfrp;
	bfrp.dwSize = sizeof(bfrp);
	if(BluetoothFindFirstRadio(&bfrp, &h) == NULL) {
		LOG("GLE %i, ", GetLastError());
		return CONNERR_GENERIC;
	}

	bri.dwSize = sizeof(bri);
	if(BluetoothGetRadioInfo(h, &bri) != ERROR_SUCCESS) {
		LOG("GLE %i, ", GetLastError());
		return CONNERR_GENERIC;
	}
	for(int i=0; i<BTADDR_LEN; i++) {
		addr.a[i] = bri.address.rgBytes[(BTADDR_LEN-1)-i];
	}
	return 1;
}
#else
int Bluetooth::getLocalAddress(MABtAddr& addr) {
	/*
	BT_ADDR addr;
	if(BthReadLocalAddr(&addr) == ERROR_SUCCESS) {
		for(int i=0; i<6; i++) {
			addr.a[i] = (addr>>(i<<3))&0xff;
		}
		return 1;
	} else {
		return CONNERR_GENERIC;
	}
	*/
	SOCKET s = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
	if( s == INVALID_SOCKET ) {
		return CONNERR_GENERIC;
	}
	SOCKADDR_BTH sab;
	memset (&sab, 0, sizeof(sab));
	sab.addressFamily = AF_BTH;
	sab.port = 0; // auto-allocate server channel
	if(bind(s, (sockaddr*)&sab, sizeof(sab)) != 0) {
		closesocket(s);
		return CONNERR_GENERIC;
	}

	int returnCode;
	int len = sizeof(sab);
	if( getsockname(s, (sockaddr*)&sab, &len) == 0 ) {
		// convert btaddr
		for(int i=0; i<6; i++) {
			addr.a[i] = (byte)((sab.btAddr>>((5-i)<<3))&0xff);
		}
		returnCode = 1;
	} else {
		returnCode = CONNERR_GENERIC;
	}
	closesocket(s);
	return returnCode;
}
#endif	//_WIN32_WCE 	//WIN32

#elif defined( BLUEZ_SUPPORTED )

    // Code is in linux/bluez/interface.cpp

#else

namespace Bluetooth {

void MABtInit() {
}

void MABtClose() {
}

int getLocalAddress(MABtAddr& addr) {
	return -1;
}

}

#endif	//WIN32
