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

//******************************************************************************
// MoToothMoblet
//******************************************************************************

#include <maassert.h>
#include <conprint.h>
#define printfln printf

#include <MAUtil/Moblet.h>
#include <MAUtil/BluetoothDiscovery.h>
#include <MAUtil/Connection.h>
#include <MAUtil/mauuid.h>
#include <MAUtil/util.h>

#include "MinUI.h"
#include "MoTooth.h"

using namespace MinUI;

//******************************************************************************
// forward declarations
//******************************************************************************

class MenuView;
class ListboxView;
class ConnectionView;

//******************************************************************************
// ConnectionView
//******************************************************************************

class ConnectionView : public View, private ConnectionListener {
public:
	ConnectionView() : mConn(this) {}

	void setMenuView(View* menu) {
		mMenuView = menu;
	}

	void activate(int deviceIndex, int serviceIndex) {
		this->show();
		const DEVICE& dev(gDatabase.devices[deviceIndex]);
		const BtService& serv(dev.services[serviceIndex]);
		char buf[32];
		char* ptr = buf;
		ptr += sprintf(ptr, "btspp://");
		ptr += btaddr2string(ptr, dev.address);
		ptr += sprintf(ptr, ":%i", serv.port);
		printfln("%s", buf);
		int res = mConn.connect(buf);
		if(res < 0) {
			printf("Error: %i\n", res);
		}
		mOnline = false;
		mWriting = false;
	}

	//View
	void keyPressEvent(int keyCode) {
		if(keyCode == MAK_0)
			mMenuView->show();

		// If it's a printable key and we're connected...
		if(keyCode >= MAK_SPACE && keyCode <= MAK_Z && mOnline) {
			// Store the digit.
			mCurrentWriteBuf[mWritePos++] = keyCode;
			// If we're not still writing...
			if(!mWriting) {
				// Send data to server.
				mConn.write(mCurrentWriteBuf, mWritePos);
				mWriting = true;
				mWritePos = 0;
				// Swap buffers, to avoid overwriting data that's still being sent.
				mCurrentWriteBuf = (mCurrentWriteBuf == mWriteBuf1) ? mWriteBuf2 : mWriteBuf1;
			}
		}
	}
	void keyReleaseEvent(int keyCode) {
	}
	void doHide() {
		mConn.close();
	}

	//ConnectionListener
	void connectFinished(Connection* conn, int result) {
		printf("connect %i\n", result);
		if(result < 0)
			return;
		mConn.recv(mRecvBuf, sizeof(mRecvBuf) - 1);
		mOnline = true;
	}
	void connRecvFinished(Connection* conn, int result) {
		mOnline = result > 0;
		if(result < 0) {
			printf("recv %i\n", result);
			return;
		}
		mRecvBuf[result] = 0;
		printfln("%s", mRecvBuf);
		mConn.recv(mRecvBuf, sizeof(mRecvBuf) - 1);
	}
	void connWriteFinished(Connection* conn, int result) {
		printf("write %i\n", result);
		mOnline = result > 0;
		mWriting = false;
	}
private:
	View* mMenuView;
	Connection mConn;
	char mRecvBuf[128];

	char mWriteBuf1[512], mWriteBuf2[512];
	char* mCurrentWriteBuf;
	int mWritePos;
	bool mWriting, mOnline;
};

//******************************************************************************
// BluetoothLabel
//******************************************************************************

class BluetoothLabel : public StringLabel {
public:
	BluetoothLabel(const String& str, int dev, int serv)
		: StringLabel(str, WHITE), deviceIndex(dev), serviceIndex(serv) {}
	const int deviceIndex, serviceIndex;
};

//******************************************************************************
// ListboxView
//******************************************************************************

class ListboxView : public View {
public:
	ListboxView() : mLayout(1) {
		View::mRootWidget = &mLayout;

		mLayout.setBackground(true, BLACK);

		MAExtent s = maGetScrSize();
		MAPoint2d scrSize = { EXTENT_X(s), EXTENT_Y(s) };
		mLayout.setSize(scrSize);
	}
	void setViews(MenuView* menu, ConnectionView* conn) {
		mMenuView = menu;
		mConnectionView = conn;
	}
	virtual ~ListboxView() {}

	void update() {
		mLayout.clear();

		//add devices and services to list
		for(int i=0; i<gDatabase.devices.size(); i++) {
			const DEVICE& dev(gDatabase.devices[i]);
			String str = btaddr2string(dev.address);
			if(dev.name.size() > 0) {
				str = dev.name + " (" + str + ")";
			}
			mLayout.add(new BluetoothLabel(str, i, -1), 0);

			for(int j=0; j<dev.services.size(); j++) {
				const BtService& serv(dev.services[j]);
				String servString = " " + integerToString(serv.port) + ": " + serv.name;
				mLayout.add(new BluetoothLabel(servString, i, j), 0);
			}
		}

		if(gDatabase.devices.size() > 0) {
			mLayout.scrollTo(0,0);
			mIndex = 0;
			((Label*)mLayout.getWidget(0, mIndex))->setBackground(true, SKY_BLUE);
		}
	}

	//View
	void keyPressEvent(int keyCode) {
		if(keyCode == MAK_0 || keyCode == MAK_BACK)
			maExit(0);
		if(keyCode == MAK_FIRE || keyCode == MAK_SOFTLEFT) {
			//connect to that service.
			connect();
			return;
		}
		int newIndex = mIndex;
		if(keyCode == MAK_DOWN && mIndex < (mLayout.getNRows() - 1)) {
			newIndex = mIndex + 1;
		}
		if(keyCode == MAK_UP && mIndex > 0) {
			newIndex = mIndex - 1;
		}

		if(newIndex != mIndex) {
			//move pointer
			mLayout.scrollTo(0, newIndex);
			((Label*)mLayout.getWidget(0, newIndex))->setBackground(true, SKY_BLUE);
			((Label*)mLayout.getWidget(0, mIndex))->setBackground(false);
			mIndex = newIndex;

			//TODO: start timer for repeat action
		}
	}
	void keyReleaseEvent(int keyCode) {
	}
private:
	GridLayout mLayout;
	MenuView* mMenuView;
	ConnectionView* mConnectionView;
	int mIndex;

	void connect() {
		BluetoothLabel* w = (BluetoothLabel*)mLayout.getWidget(0, mIndex);
		if(w == NULL)
			return;
		if(w->serviceIndex < 0)
			return;
		mConnectionView->activate(w->deviceIndex, w->serviceIndex);
	}
};

//******************************************************************************
// MenuView
//******************************************************************************

// A view based on MoSync's text console.
class MenuView : public View, private BluetoothDeviceDiscoveryListener,
	private BluetoothServiceDiscoveryListener
{
private:
	MAHandle RES_STORE;
	ListboxView* mListboxView;
public:
	MenuView() : RES_STORE(maCreatePlaceholder()), mState(EBootup) {}
	void setListboxView(ListboxView* view) {
		mListboxView = view;
	}
	virtual ~MenuView() {}

	//View
	void doShow() {
		if(mState == EMainMenu) {
			startMainMenu();
			return;
		}
		printfln("reading store...");
		readStore();
	}

	void keyPressEvent(int keyCode) {
		if(keyCode == MAK_0 || keyCode == MAK_BACK)
			maExit(0);
		if(mState == EStoreCorrupted) {
			if(keyCode == MAK_5) {
				finishReadStore();
			} else if(keyCode == MAK_9) {
				deleteStore();
				finishReadStore();
			}
		} else if(mState == EMainMenu) {
			if(keyCode == MAK_5) {
				startScan();
			}
			if(keyCode == MAK_FIRE && gDatabase.nServices() != 0) {
				startSelect();
			}
		}
	}
	void keyReleaseEvent(int keyCode) {
	}

private:
	enum State { EBootup, EMainMenu, EStoreCorrupted, EScanningDevices, EScanningServices } mState;

	void startSelect() {
		mListboxView->show();
	}

	//****************************************************************************
	// Device Discovery
	//****************************************************************************
	BluetoothDiscoverer mDiscoverer;
	int mDiscoveryCount;
	int mStartTime;

	void startScan() {
		mStartTime = maGetMilliSecondCount();
		printfln("DevDisc...");
		int res = mDiscoverer.startDeviceDiscovery(this, true);
		if(res < 0) {
			printfln("failed: %i", res);
			return;
		}
		mState = EScanningDevices;
		mDiscoveryCount = 0;
	}

	void btNewDevice(const BtDevice& dev) {
		MAASSERT(mState == EScanningDevices);

		printfln("d%i: %s", ++mDiscoveryCount, dev.name.c_str());
		printfln("%s", btaddr2string(dev.address));
		gDatabase.addDevice(dev);
	}
	void btDeviceDiscoveryFinished(int state) {
		MAASSERT(mState == EScanningDevices);
		printfln("done(%i) in %i ms", state, maGetMilliSecondCount() - mStartTime);
		startServiceScan();
	}

	//****************************************************************************
	// Service Discovery
	//****************************************************************************
	int mDeviceIndex;
	int mServStartTime;

	void startServiceScan() {
		if(gDatabase.devices.size() > 0) {
			printfln("Scanning %i devices...\n", gDatabase.devices.size());
			mDeviceIndex = 0;
			mState = EScanningServices;
			startNextDevice();
		} else {
			startMainMenu();
		}
	}

	void startNextDevice() {
		MAASSERT(mState == EScanningServices);
		if(mDeviceIndex >= gDatabase.devices.size()) {	//we've reached the end
			printfln("Done, total %i ms", maGetMilliSecondCount() - mStartTime);
			writeStore();
			mListboxView->update();
			startMainMenu();
			return;
		}
		DEVICE& d(gDatabase.devices[mDeviceIndex]);
		printfln("ServDisc %s...", btaddr2string(d.address));
		int res = mDiscoverer.startServiceDiscovery(d.address, RFCOMM_PROTOCOL_MAUUID, this);
		if(res < 0) {
			printfln("failed: %i", res);
			return;
		}
		mDiscoveryCount = 0;
		mServStartTime = maGetMilliSecondCount();
	}

	void btNewService(const BtService& serv) {
		MAASSERT(mState == EScanningServices);
		printfln("s%i: %i %s", ++mDiscoveryCount, serv.port, serv.name.c_str());
		gDatabase.addService(mDeviceIndex, serv);
	}
	void btServiceDiscoveryFinished(int state) {
		MAASSERT(mState == EScanningServices);
		printfln("done(%i) in %i ms", state, maGetMilliSecondCount() - mServStartTime);
		mDeviceIndex++;
		startNextDevice();
	}

	//****************************************************************************
	// Store management
	//****************************************************************************

	void readStore() {
		//open store
		MAHandle store = maOpenStore(STORENAME, 0);
		if(store <= 0) {
			printfln("No store.");
			startMainMenu();
			return;
		}
		maReadStore(store, RES_STORE);
		maCloseStore(store, false);

		//read into database
		if(gDatabase.read(RES_STORE)) {
			mListboxView->update();
			finishReadStore();
		} else {
			gDatabase.devices.clear();
			printfln("Store corrupted.\n(5)ignore (9)delete\n");
			mState = EStoreCorrupted;
		}
	}
	void finishReadStore() {
		printfln("done");
		startMainMenu();
	}

	void writeStore() {
		//open store
		MAHandle store = maOpenStore(STORENAME, MAS_CREATE_IF_NECESSARY);
		if(store <= 0) {
			printf("writeStore error %i\n", store);
			return;
		}
		gDatabase.write(RES_STORE);
		maWriteStore(store, RES_STORE);
		maCloseStore(store, false);
	}

	void deleteStore() {
		//open store
		MAHandle store = maOpenStore(STORENAME, 0);
		if(store <= 0)
			return;
		maCloseStore(store, true);
	}

	void startMainMenu() {
		mState = EMainMenu;
		printfln("%i known services\n", gDatabase.nServices());
		if(gDatabase.nServices() > 0) {
			printfln("(F)select (5)scan (0)exit\n");
		} else {
			printf("(5)scan (0)exit\n");
		}
	}
};

//******************************************************************************
// Main
//******************************************************************************

class MyMoblet : public Moblet {
public:
	MyMoblet() {
		mMenuView.setListboxView(&mListboxView);
		mListboxView.setViews(&mMenuView, &mConnectionView);
		mConnectionView.setMenuView(&mMenuView);
		mMenuView.show();
	}
	void closeEvent() {}
private:
	MenuView mMenuView;
	ListboxView mListboxView;
	ConnectionView mConnectionView;
};

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;

	Moblet::run(new MyMoblet());
	return 0;
};

//******************************************************************************
// useful functions
//******************************************************************************
