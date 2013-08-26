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

#include <mastring.h>
#include <mavsprintf.h>

#include "MoTooth.h"

Database gDatabase;

void Database::addDevice(const BtDevice& d) {
	DEVICE dev;
	dev.address = d.address;
	dev.name = d.name;

	//if a device with this address already exists, overwrite it.
	for(int i=0; i<devices.size(); i++) {
		const DEVICE& gd(devices[i]);
		if(memeq(gd.address.a, d.address.a, BTADDR_LEN)) {
			mNServices -= gd.services.size();
			devices[i] = dev;
			return;
		}
	}
	devices.add(dev);
}

void Database::addService(int deviceIndex, const BtService& s) {
	DEVICE& d(devices[deviceIndex]);
	//if a service with this port already exists, overwrite it.
	for(int j=0; j<d.services.size(); j++) {
		if(d.services[j].port == s.port) {
			d.services[j] = s;
			return;
		}
	}
	d.services.add(s);
	mNServices++;
	return;
}

//returns false on failure
bool Database::readDevice(DataHandler& data, DEVICE& dev) {
	//address
	TEST(data.read(&dev.address, BTADDR_LEN));

	//name
	int strLen;
	TEST(data.read(&strLen, sizeof(int)));
	StringData<char>* sd = new StringData<char>(strLen);
	dev.name.setData(sd);
	MAASSERT(sd->getRefCount() == 1);
	TEST(data.read(sd->pointer(), strLen));
	dev.name[strLen] = 0;

	//services
	int nServ;
	TEST(data.read(&nServ, sizeof(int)));
	dev.services.resize(nServ);
	for(int i=0; i<nServ; i++) {
		BtService& serv(dev.services[i]);
		TEST(data.read(&serv.port, sizeof(int)));
		TEST(data.read(&strLen, sizeof(int)));
		sd = new StringData<char>(strLen);
		serv.name.setData(sd);
		MAASSERT(sd->getRefCount() == 1);
		TEST(data.read(sd->pointer(), strLen));
		serv.name[strLen] = 0;
	}
	mNServices += nServ;
	return true;
}

bool Database::read(MAHandle h) {
	DataHandler data(h);
	int nDevices;
	TEST(data.read(&nDevices, sizeof(int)));
	devices.resize(nDevices);
	for(int i=0; i<nDevices; i++) {
		TEST(readDevice(data, devices[i]));
	}
	return true;
}

void Database::write(MAHandle h) const {
	maDestroyObject(h);
	maCreateData(h, calculateDatabaseSize());
	DataHandler data(h);

	int nDevices = devices.size();
	data.write(&nDevices, sizeof(int));
	for(int i=0; i<nDevices; i++) {
		writeDevice(data, devices[i]);
	}
}

void Database::writeDevice(DataHandler& data, const DEVICE& dev) const {
	//address
	data.write(&dev.address, BTADDR_LEN);

	//name
	int strLen = dev.name.size();
	data.write(&strLen, sizeof(int));
	data.write(dev.name.c_str(), strLen);

	//services
	int nServ = dev.services.size();
	data.write(&nServ, sizeof(int));
	for(int i=0; i<nServ; i++) {
		const BtService& serv(dev.services[i]);
		data.write(&serv.port, sizeof(int));
		strLen = serv.name.size();
		data.write(&strLen, sizeof(int));
		data.write(serv.name.c_str(), strLen);
	}
}

int Database::calculateDatabaseSize() const {
	int size = sizeof(int);
	for(int i=0; i<devices.size(); i++) {
		const DEVICE& dev(devices[i]);
		size += BTADDR_LEN;
		size += sizeof(int) + dev.name.size();

		size += sizeof(int);
		for(int j=0; j<dev.services.size(); j++) {
			const BtService& serv(dev.services[j]);
			size += sizeof(int) + sizeof(int) + serv.name.size();
		}
	}
	return size;
}

const char* btaddr2string(const MABtAddr& a) {
	static char buffer[16];
	sprintf(buffer, "%02x%02x%02x%02x%02x%02x", a.a[0], a.a[1], a.a[2], a.a[3], a.a[4], a.a[5]);
	return buffer;
}

int btaddr2string(char* buf, const MABtAddr& a) {
	return sprintf(buf, "%02x%02x%02x%02x%02x%02x", a.a[0], a.a[1], a.a[2], a.a[3], a.a[4], a.a[5]);
}
