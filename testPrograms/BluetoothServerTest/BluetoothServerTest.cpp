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
 Test for Bluetooth server.
 Author: Micke

 How to use:
 Use with the corresponding BluetoothClientTest application.
 Server starts automatically when the application is launched.
*/

#include <ma.h>
#include <conprint.h>
#include <maassert.h>
#include <maprofile.h>
#include <MAUtil/mauuid.h>
#include <MAUtil/Moblet.h>
#include <MAUtil/Connection.h>
#include <MAUtil/Server.h>
#include <MAUtil/BluetoothConnection.h>

using namespace MAUtil;

// Class forward declarations.
class MyMoblet;

// Function declarations.
static void serverAcceptedNewConnection(void* moblet, void* newConnection);

/**
 * Listen to server accept events.
 */
class MyServerListener : public ServerListener
{
public:
	MyMoblet* mMoblet;

	MyServerListener(MyMoblet* moblet)
	{
		mMoblet = moblet;
	}

	void serverAcceptFailed(Server* server, int result)
	{
		printf("MyServerListener::serverAcceptFailed: %i\n", result);
	}

	void serverAccepted(Server* server, Connection* newConnection)
	{
		serverAcceptedNewConnection(mMoblet, newConnection);
	}
};

/**
 * Handle a client connection.
 * Read and write until connection is closed.
 */
class ClientConnectionListener : public ConnectionListener
{
public:
	int mId;
	Connection* mConnection;
	char mInBuffer[64];
	char mOutBuffer[64];
	int mDataLength;

	ClientConnectionListener(int id)
	{
		mId = id;
		mDataLength = 12;
	}

	virtual ~ClientConnectionListener() {
		delete mConnection;
	}

	void startReceivingDataOnConnection(Connection* newConnection)
	{
		mConnection = newConnection;

		// Start to receive data.
		mConnection->read(mInBuffer, mDataLength);
	}

	void connWriteFinished(Connection* connnection, int result)
	{
		printf("connWriteFinished connectionId: %i result: %i\n", mId, result);
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
			mConnection->close(); // Do we need to do this?
			delete this; // should be safe ???
			return;
		}

		// We received data. Print it.
		//mInBuffer[result] = 0; // Null terminate.
		mInBuffer[mDataLength] = 0; // Null terminate.
		printf("connRecvFinished %i: %s\n", mId, mInBuffer);

		// Echo data back to client, modify first character.
		strcpy(mOutBuffer, mInBuffer);
		if ('Q' != mInBuffer[0])
		{
			// If this is not a request to quit/close the connection.
			// Put a 'Z' as first character. This is used by the client
			// When verifying data sent back.
			mOutBuffer[0] = 'Z';
		}
		mConnection->write(mOutBuffer, mDataLength);

		// Continue to receive data, unless first character is 'Q'.
		if ('Q' == mInBuffer[0])
		{
			mConnection->close();
			delete this; // For the courageous ones...
		}
		else
		{
			// Read more data.
			mConnection->read(mInBuffer, mDataLength);
		}
	}
};

class MyMoblet : public Moblet
{
public:
	Server* mServer;
	MyServerListener* mServerListener;
	int mNextId;
	ClientConnectionListener* mCurrentConnectionListener;

	MyMoblet()
	{
		mServer = NULL;
		mServerListener = NULL;
		mNextId = 1;
		mCurrentConnectionListener = NULL;

		printf("Bluetooth SERVER test program.\n");
		printf("Touch screen or press 1 to start server.\n");
		printf("Press Back or 0 to exit program.\n");

		// Uncomment to start server automatically when program is launched.
		//startServer();
	}

	virtual ~MyMoblet()
	{
		if (mServer)
		{
			printf("Destructing ~MyMoblet.\n");
			//mServer->close();
			delete mServer;
			mServer = NULL;
		}

		if (mServerListener)
		{
			delete mServerListener;
			mServerListener = NULL;
		}
	}

	void accept()
	{
		printf("WAITING for connection...");
		mCurrentConnectionListener = new ClientConnectionListener(mNextId++);
		mServer->accept(mCurrentConnectionListener); // TODO: try with NULL or use dummy listener.
	}

	/**
	 * The server has accepted a client connection.
	 * Start receiving data.
	 */
	void serverAcceptedNewConnection(Connection* newConnection)
	{
		// Print the remote address.
		MAConnAddr addr;
		int res = newConnection->getAddr(&addr);
		MAASSERT(res > 0);
		byte* a = addr.bt.addr.a;
		printf("ACCEPTED new connection: %02X%02X%02X%02X%02X%02X\n",
			a[0], a[1], a[2], a[3], a[4], a[5]);

		// Start communicating using the new connection.
		mCurrentConnectionListener->startReceivingDataOnConnection(newConnection);

		// Accept next connection.
		accept();
	}

	void startServer()
	{
		char* url = "btspp://localhost:dcbdb690c17111df851a0800200c9a66;"
					"name=Bluetooth test server";

		MyServerListener* serverListener = new MyServerListener(this);
		mServer = new Server(serverListener);

		// Start the server.
		printf("STARTING Bluetooth server.\n");
		int result = mServer->start(url);
		if (result < 0)
		{
			printf("Could not start server. Result: %i\n", result);

			return;
		}

		printf("Server started. Result: %i\n", result);

		// Print the server's address.
		MAConnAddr addr;
		result = mServer->getAddr(&addr);
		MAASSERT(result > 0);
		MAASSERT(addr.family == CONN_FAMILY_BT);
		byte* a = addr.bt.addr.a;
		printf("Server address: %02X%02X%02X%02X%02X%02X\n",
			a[0], a[1], a[2], a[3], a[4], a[5]);

		// Accept new connection.
		accept();
	}

	void keyPressEvent(int keyCode, int nativeCode)
	{
		if (keyCode == MAK_0 || keyCode == MAK_BACK)
		{
			printf("Exiting program.\n");
			mServer->close();
			close();
		}

		if (keyCode == MAK_1)
		{
			startServer();
		}
	}

	void pointerPressEvent(MAPoint2d point)
	{
		printf("Touchdown event.\n");

		// Server is started on program startup.
		startServer();
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
static void serverAcceptedNewConnection(void* moblet, void* newConnection)
{
	((MyMoblet*)moblet)->serverAcceptedNewConnection((Connection*)newConnection);
}

