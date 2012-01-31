/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
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
