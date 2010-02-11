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

#include "MoTooth.h"

//This is the format of the Store
/*struct STORE {
	int nDevices;
	STORE_DEVICE devices[nDevices];
};
struct STORE_DEVICE {
	byte address[BTADDR_LEN];
	int nameLen;	//includes terminating zero
	char name[nameLen];
	int nServices;
	int services[nServices];
};*/

void writeDatabase() {
	maDestroyObject(RES_STORE);
	maCreateData(RES_STORE, calculateDatabaseSize());
	DataHandler data(RES_STORE);

	int nDevices = gDevices.size();
	data.write(&nDevices, sizeof(int));
	for(int i=0; i<nDevices; i++) {
		writeDevice(data, gDevices[i]);
	}
}

void writeDevice(DataHandler& data, DEVICE& dev) {
	//address
	data.write(&dev.address, BTADDR_LEN);

	//name
	int strLen = dev.name.length();
	data.write(&strLen, sizeof(int));
	data.write(dev.name.c_str(), strLen);

	//services
	int nServices = dev.services.size();
	data.write(&nServices, sizeof(int));
	for(int i=0; i<dev.services.size(); i++) {
		const SERVICE& serv(dev.services[i]);
		data.write(&serv.port, sizeof(int));
		strLen = serv.name.length();
		data.write(&strLen, sizeof(int));
		data.write(serv.name.c_str(), strLen);
	}
}

int calculateDatabaseSize() {
	int size = sizeof(int);
	for(int i=0; i<gDevices.size(); i++) {
		const DEVICE& dev(gDevices[i]);
		size += BTADDR_LEN;
		size += sizeof(int) + dev.name.length();

		size += sizeof(int);
		for(int j=0; j<dev.services.size(); j++) {
			const SERVICE& serv(dev.services[j]);
			size += sizeof(int) + sizeof(int) + serv.name.length();
		}
	}
	return size;
}

//returns false on failure
bool readDatabase() {
	DataHandler data(RES_STORE);
	int nDevices;
	TEST(data.read(&nDevices, sizeof(int)));
	gDevices.resize(nDevices);
	for(int i=0; i<nDevices; i++) {
		TEST(readDevice(data, gDevices[i]));
	}
	return true;
}

//returns false on failure
bool readDevice(DataHandler& data, DEVICE& dev) {
	//address
	TEST(data.read(&dev.address, BTADDR_LEN));

	//name
	int strLen;
	TEST(data.read(&strLen, sizeof(int)));
	StringData* sd = new StringData(strLen);
	dev.name.setData(sd);
	MAASSERT(sd->getRefCount() == 1);
	TEST(data.read(sd->pointer(), strLen));
	dev.name[strLen] = 0;

	//services
	int nServices;
	TEST(data.read(&nServices, sizeof(int)));
	dev.services.resize(nServices);
	for(int i=0; i<nServices; i++) {
		SERVICE& serv(dev.services[i]);
		TEST(data.read(&serv.port, sizeof(int)));
		TEST(data.read(&strLen, sizeof(int)));
		sd = new StringData(strLen);
		serv.name.setData(sd);
		MAASSERT(sd->getRefCount() == 1);
		TEST(data.read(sd->pointer(), strLen));
		serv.name[strLen] = 0;
	}
	gnServices += nServices;
	return true;
}

void addDevice(const MABtDevice& d) {
	DEVICE dev;
	dev.address = d.address;
	dev.name = d.name;

	//if a device with this address already exists, overwrite it.
	for(int i=0; i<gDevices.size(); i++) {
		const DEVICE& gd(gDevices[i]);
		if(memeq(gd.address.a, d.address.a, BTADDR_LEN)) {
			gnServices -= gd.services.size();
			gDevices[i] = dev;
			return;
		}
	}
	gDevices.add(dev);
}

void addService(int deviceIndex, const MABtService& s) {
	DEVICE& d(gDevices[deviceIndex]);
	SERVICE serv;
	serv.port = s.port;
	serv.name = s.name;
	//if a service with this port already exists, overwrite it.
	for(int j=0; j<d.services.size(); j++) {
		if(d.services[j].port == serv.port) {
			d.services[j] = serv;
			return;
		}
	}
	d.services.add(serv);
	gnServices++;
	return;
}
