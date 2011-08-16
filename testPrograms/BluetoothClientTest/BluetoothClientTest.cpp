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

/*
 Bluetooth test client.
 Author: Micke

 How to use:
 Start the corresponding BluetoothServerTest application
 on another device.
 Make server device discoverable.
 Start this program.
 Tap screen or press 1 to start test.
 Test has passed when display says "Test PASSED".
*/

#include <conprint.h>
#include <maassert.h>
#include <maprofile.h>
#include <MAUtil/mauuid.h>
#include <MAUtil/Vector.h>
#include <MAUtil/Moblet.h>
#include <MAUtil/Connection.h>
#include <MAUtil/BluetoothConnection.h>

using namespace MAUtil;

// Class forward declarations.
class MyMoblet;

/**
 * Class that holds device info.
 */
class Device
{
public:
	MABtDevice mDeviceInfo;
	char mNameBuf[256];

	Device()
	{
		mDeviceInfo.name = mNameBuf;
		mDeviceInfo.nameBufSize = sizeof(mNameBuf);
	}

	virtual ~Device()
	{
	}
};

// Function declarations.
static void startConnectionTest(void* moblet, Vector<Device>* deviceList);
static int btaddr2string(const MABtAddr& a, char* buf);

/**
 * This class collects devices in a list.
 */
class DeviceDiscoveryListener : public BluetoothListener
{
public:
	MyMoblet* mMoblet;

	DeviceDiscoveryListener(MyMoblet* moblet)
	{
		mMoblet = moblet;
	}

	virtual void bluetoothEvent(int state)
	{
		if (0 == state)
		{
			printf("Bluetooth device found (still working)\n");
		}
		else if (CONNERR_CANCELED == state)
		{
			printf("Bluetooth state: CONNERR_CANCELED\n");
		}
		else if (state > 0)
		{
			printf("Found %i devices:\n", state - 1);

			Vector<Device>* deviceList = new Vector<Device>();;

			Device device;
			int result;
			char addrBuf[128];

			while (result = maBtGetNewDevice(&device.mDeviceInfo))
			{
				// Add device to list.
				deviceList->add(device);

				// Print device info.
				btaddr2string(device.mDeviceInfo.address, addrBuf);
				printf("%s  %s\n", device.mNameBuf, addrBuf);
			}

			startConnectionTest(mMoblet, deviceList);
		}
		else
		{
			printf("Other Bluetooth state: %i (should not happen)\n", state);
		}
	}
};

/**
 * Connection that reads and writes multiple times
 * on the same connection.
 */
class BasicConnectionTestListener : public ConnectionListener
{
public:
	int mId;
	BluetoothConnection* mConnection;
	//Connection* mConnection;
	char mInBuffer[64];
	char mOutBuffer[64];
	int mDataLength;
	int mSendCounter;
	Vector<Device>* mDeviceList;
	int mDeviceCounter;

	// Number of times to send data to the server.
	int mNumberOfSends;

	BasicConnectionTestListener(int id, Vector<Device>* deviceList)
	{
		mId = id;
		mConnection = NULL;
		mDataLength = 12;
		mSendCounter = 0;
		mDeviceList = deviceList;
		mDeviceCounter = 0;
		mNumberOfSends = 5;

		// Create connection object.
		mConnection = new BluetoothConnection(this);
		//mConnection = new Connection(this);
	}

	virtual ~BasicConnectionTestListener()
	{
		if (mConnection)
		{
			delete mConnection;
			mConnection = NULL;
		}
	}

	void connectToNextDevice()
	{
		if (NULL == mDeviceList)
		{
			return;
		}

		if (mDeviceCounter >= mDeviceList->size())
		{
			printf("No device with a running server found.\n");
			return;
		}

		// Connection url.
		char url[256];
		char addrBuf[128];
		Device device = (*mDeviceList)[mDeviceCounter++];
		btaddr2string(device.mDeviceInfo.address, addrBuf);

		printf("Attempting to connect to %s %s\n", device.mNameBuf, addrBuf);

		sprintf(
			url,
			"btspp://%s:%s",
			addrBuf,
			"dcbdb690c17111df851a0800200c9a66");

		connectToDevice(url);
	}

	void connectToDevice(const char* url)
	{
		printf("Url: %s\n", url);

		// We will handle the result in connectionFinished.
		int result = mConnection->connect(url);

		printf("Connection result: %i\n", result);

		if (result < 0)
		{
			printf("Could not initiate connection.\n");
		}
	}

	void sendData(Connection* connection)
	{
		if (mSendCounter < mNumberOfSends)
		{
			strcpy(mOutBuffer, "ABCDEF123456");
			connection->write(mOutBuffer, mDataLength);

			++mSendCounter;
		}
		else
		{
			printf("Done sending data. Close connection.\n");
			strcpy(mOutBuffer, "QBCDEF123456");
			connection->write(mOutBuffer, mDataLength);
		}
	}

	void receiveData(Connection* connection)
	{
		// Start to receive data.
		connection->read(mInBuffer, mDataLength);
	}

	void connectFinished(Connection* connnection, int result)
	{
		printf("connectFinished connectionId: %i result: %i\n", mId, result);

		if (result > 0)
		{
			sendData(connnection);
		}

		if (result < 0)
		{
			// Connect failed, close the connection.
			mConnection->close();

			printf("Connect failed, closing connectionId: %i\n", mId);

			// If we have a device list, try to connect to the next
			// device in the list.
			if (mDeviceList)
			{
				connectToNextDevice();
			}
		}
	}

	void connWriteFinished(Connection* connection, int result)
	{
		printf("connWriteFinished connectionId: %i result: %i\n", mId, result);

		if (result > 0)
		{
			receiveData(connection);
		}
	}

	void connRecvFinished(Connection* connection, int result)
	{
		printf("connRecvFinished connectionId: %i result: %i\n", mId, result);
	}

	void connReadFinished(Connection* connection, int result)
	{
		if (result < 0) // error
		{
			printf("ERROR connReadFinished connectionId: %i result: %i\n", mId, result);
			mConnection->close();
			//delete this; // ???
			return;
		}

		// We received data. Print it.
		//mInBuffer[result] = 0; // Null terminate.
		mInBuffer[mDataLength] = 0; // Null terminate.
		printf("connReadFinished %i: %s\n", mId, mInBuffer);

		if ('Q' == mInBuffer[0])
		{
			// Close this connection.
			printf("Client done sending and receiving data.\n");
			printf("Test PASSED.\n");
			mConnection->close();
			//delete this; // ???
			return;
		}

		if ('Z' != mInBuffer[0])
		{
			printf("Test FAILED. First character != 'Z' Was: %i\n", (int)mInBuffer[0]);
			mConnection->close();
			//delete this; // ???
			return;
		}

		// Send more data.
		sendData(mConnection);
	}
};

class MyMoblet : public Moblet
{
public:
	int mNextId;

	MyMoblet()
	{
		mNextId = 1;

		printf("Bluetooth CLIENT test program.\n");
		printf("Start test: Press 1 or touch screen.\n");
		printf("Exit program: Press 0 or Back\n");
	}

	void startDeviceDiscovery()
	{
		printf("Starting device discovery\n");
		setBluetoothListener(new DeviceDiscoveryListener(this));
		int result = maBtStartDeviceDiscovery(1);
		printf("maBtStartDeviceDiscovery result: %d\n", result);
	}

	void startBasicConnectionTestUsingDeviceList(Vector<Device>* deviceList)
	{
		printf("Starting BASIC connection test\n");

		BasicConnectionTestListener* connectionListener =
			new BasicConnectionTestListener(mNextId++, deviceList);
		connectionListener->connectToNextDevice();
	}

	void startBasicConnectionTestUsingHardcodedDeviceAddress()
	{
		printf("Starting BASIC connection test\n");

		BasicConnectionTestListener* connectionListener =
			new BasicConnectionTestListener(mNextId++, NULL);

		// Connect to Micke's Nexus One
		connectionListener->connectToDevice("btspp://0023769FD7C2:dcbdb690c17111df851a0800200c9a66");

		// Connect to HTC Desire
		//connectionListener->connectToDevice("btspp://38E7D8C7605E:dcbdb690c17111df851a0800200c9a66");

		// Connect to HTC Wildfire
		//connectionListener->connectToDevice("btspp://38E7D898EBFA:dcbdb690c17111df851a0800200c9a66");

		// Connect to Samsung Galaxy
		//??? connectionListener->connectToDevice("btspp://432534BAA8EB:dcbdb690c17111df851a0800200c9a66");
		//connectionListener->connectToDevice("btspp://FCA13E5FB290:dcbdb690c17111df851a0800200c9a66");
	}

	void keyPressEvent(int keyCode, int nativeCode)
	{
		if (keyCode == MAK_0 || keyCode == MAK_BACK)
		{
			close();
		}

		if (keyCode == MAK_1)
		{
			startDeviceDiscovery();
			//startBasicConnectionTestUsingHardcodedDeviceAddress();
		}
	}

	void pointerPressEvent(MAPoint2d point)
	{
		printf("Touch event\n");

		MAExtent screenExtent = maGetScrSize();
		int itemHeight = EXTENT_Y(screenExtent) / 3;

		if (point.y < itemHeight * 3)
		{
			startDeviceDiscovery();
			//startBasicConnectionTestUsingHardcodedDeviceAddress();
		}
	}
};

extern "C" int MAMain()
{
	Moblet::run(new MyMoblet());
	return 0;
}

/**
 * Function that takes away the need to declare types in advance.
 */
static void startConnectionTest(void* moblet, Vector<Device>* deviceList)
{
	((MyMoblet*)moblet)->startBasicConnectionTestUsingDeviceList(deviceList);
}

/**
 * Convert Bluetooth address to string.
 */
static int btaddr2string(const MABtAddr& a, char* buf)
{
	return sprintf(buf, "%02X%02X%02X%02X%02X%02X",
		a.a[0], a.a[1], a.a[2], a.a[3], a.a[4], a.a[5]);
}
