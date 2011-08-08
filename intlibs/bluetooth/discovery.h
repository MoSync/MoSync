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

#ifndef DISCOVERY_H
#define DISCOVERY_H

#include "btinit.h"

namespace Bluetooth
{

class MABtServiceAuto : public MABtServiceNative {
public:
	int nUuids;

	MABtServiceAuto() { name = NULL; uuids = NULL; }
	~MABtServiceAuto() { delete name; delete uuids; }

	//calls maBtGetNextServiceSize, allocates memory and calls maBtGetNewService.
	//returns >0 on success, 0 if there is no new service.
	int fillWithNew();
private:
};

typedef void (*MABtCallback)();

// <0 CONNERR on failure. 0 while still working. (number of devices/services found) + 1 on success.
int maBtDiscoveryState();

//only one discovery operation may be active at a time
int maBtStartDeviceDiscovery(MABtCallback cb, bool names);
int maBtGetNewDevice(MABtDeviceNative* dst);

int maBtCancelDiscovery();

//Takes a device address and the UUID of the service class to search for.
//For example, pass the RFCOMM UUID and you'll get all connectable services.
//Pass the OBEX Object Push UUID and you'll get only that service.
int maBtStartServiceDiscovery(const MABtAddr* address, const MAUUID* uuid, MABtCallback cb);

//returns >0 on success.
int maBtGetNewService(MABtServiceNative* dst);

//does not remove a service from the queue. returns >0 on success.
int maBtGetNextServiceSize(MABtServiceSize* dst);

}	//namespace Bluetooth

#define BLUETOOTH(func) Bluetooth::func

#endif	//DISCOVERY_H
