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
