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
		BTSTACK_BLUEZ
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
