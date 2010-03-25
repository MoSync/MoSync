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

#include "BluetoothDiscovery.h"
#include "Environment.h"

using namespace MAUtil;

BluetoothDiscoverer::BluetoothDiscoverer() : mDD(NULL), mSD(NULL) {}

int BluetoothDiscoverer::startDeviceDiscovery(BluetoothDeviceDiscoveryListener* listener,
	bool names)
{
	ASSERT_MSG(listener != NULL, "NULL listener");
	ASSERT_MSG(mDD == NULL && mSD == NULL, "another operation in progress");

	mDD = listener;
	Environment::getEnvironment().setBluetoothListener(this);
	int result = maBtStartDeviceDiscovery(names);
	if(result < 0)
		mDD = NULL;
	return result;
}

int BluetoothDiscoverer::startServiceDiscovery(const MABtAddr& address, const MAUUID& uuid,
	BluetoothServiceDiscoveryListener* listener)
{
	ASSERT_MSG(listener != NULL, "NULL listener");
	ASSERT_MSG(mDD == NULL && mSD == NULL, "another operation in progress");

	mSD = listener;
	Environment::getEnvironment().setBluetoothListener(this);
	int result = maBtStartServiceDiscovery(&address, &uuid);
	if(result < 0)
		mSD = NULL;
	return result;
}

void BluetoothDiscoverer::bluetoothEvent(int state) {
	if(mDD) {
		handleDD(state);
	} else if(mSD) {
		handleSD(state);
	}
}

#define NAME_BUF_SIZE 256

void BluetoothDiscoverer::handleSD(int state) {
	//process new services
	MABtService s;
	char sNameBuf[NAME_BUF_SIZE];
	int res;
	BtService serv;
	Vector<MAUUID>& uuidBuf(serv.uuids);

	s.name = sNameBuf;
	s.nameBufSize = NAME_BUF_SIZE;
	s.uuids = uuidBuf.pointer();

	do {
		MABtServiceSize ss;
		res = maBtGetNextServiceSize(&ss);
		if(res) {
			if(res < 0) {
				//if one fail, all fails
				if(state >= 0)
					state = res;
				break;
			}
			while(ss.nUuids > uuidBuf.capacity()) {
				uuidBuf.resize(0);
				uuidBuf.reserve(uuidBuf.capacity() * 2);
				s.uuids = uuidBuf.pointer();
			}
			uuidBuf.resize(ss.nUuids);
			res = maBtGetNewService(&s);
			if(res) {
				if(res < 0) {
					//if one fail, all fails
					if(state >= 0)
						state = res;
					break;
				}
				serv.port = s.port;
				if(ss.nameBufSize <= 0) {
					serv.name = String();
				} else {
					serv.name = s.name;
				}
				//serv.uuids is already taken care of.
				mSD->btNewService(serv);
			}
		}
	} while(res > 0);

	if(state != 0) {
		Environment::getEnvironment().removeBluetoothListener();
		BluetoothServiceDiscoveryListener* temp = mSD;
		mSD = NULL;	//ready for more
		temp->btServiceDiscoveryFinished(state);
	}
}

void BluetoothDiscoverer::handleDD(int state) {
	//process new devices
	int res;
	MABtDevice d;
	char namebuf[NAME_BUF_SIZE];
	d.name = namebuf;
	d.nameBufSize = NAME_BUF_SIZE;

	do {
		res = maBtGetNewDevice(&d);
		if(res) {
			if(res < 0) {
				//if one fail, all fails
				if(state >= 0)
					state = res;
				break;
			}
			//printf("d%i: %i %s\n", ndev++, res, d.name);
			//printf("%s\n", btaddr2string(d.address));

			BtDevice dev;
			dev.address = d.address;
			dev.name = d.name;
			mDD->btNewDevice(dev);
		}
	} while(res > 0);

	if(state != 0) {
		Environment::getEnvironment().removeBluetoothListener();
		BluetoothDeviceDiscoveryListener* temp = mDD;
		mDD = NULL;	//ready for more
		temp->btDeviceDiscoveryFinished(state);
	}
}
