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

#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <helpers/types.h>
#include <helpers/cpp_defs.h>

namespace Bluetooth
{
	void MABtInit();
	void MABtClose();

	//returns <0 on failure.
	int getLocalAddress(MABtAddr&);

	enum BluetoothStack {
		BTSTACK_UNKNOWN,
		BTSTACK_WINSOCK,
		BTSTACK_BROADCOM,
		BTSTACK_TOSHIBA,
		BTSTACK_BLUESOLEIL,
		BTSTACK_BLUEZ,
		BTSTACK_COCOA
	};
}

#ifdef WIN32
#ifdef _WIN32_WCE
#include <objbase.h>
#elif defined(_MSC_VER)
#include <guiddef.h>
#else
#include <basetyps.h>
#endif	//_WIN32_WCE

GUID MAUUID2GUID(MAUUID uuid);
MAUUID GUID2MAUUID(GUID guid);
#endif	//WIN32

#endif	//BLUETOOTH_H
