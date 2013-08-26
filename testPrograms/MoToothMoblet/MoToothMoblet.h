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

#include <MAUtil/BluetoothDiscovery.h>
#include <MAUtil/Vector.h>
#include <MAUtil/DataHandler.h>

using namespace MAUtil;

#define STORENAME "MoTooth.sav"
#define TEST(func) if(!(func)) return false;

#define SKY_BLUE 0x60C0FF


struct DEVICE : public BtDevice {
	Vector<BtService> services;
};

class Database {
private:
	int mNServices;

	bool readDevice(DataHandler& data, DEVICE& dev);
	int Database::calculateDatabaseSize() const;
	void writeDevice(DataHandler& data, const DEVICE& dev) const;
public:
	Database() : mNServices(0) {}

	int nServices() const { return mNServices; }

	void addDevice(const BtDevice& d);
	void addService(int deviceIndex, const BtService& s);
	bool read(MAHandle data);
	void write(MAHandle data) const;

	Vector<DEVICE> devices;
};

extern Database gDatabase;

void writeStore();
void deleteStore();
const char* btaddr2string(const MABtAddr& a);
int btaddr2string(char* buf, const MABtAddr& a);
