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

/**
 * @file Server.cpp
 * @author Mikael Kindborg
 *
 * Description:
 *
 *  This program acts as a Bluetooth server.
 *  It sets up an RFCOMM service with a particular UUID
 *  and listens for connections. For each connection
 *  accepted, it prints any data received.
 *
 * How to use:
 *
 *  See header comment in file Common.h
 */

#include <MAUtil/Moblet.h>
#include <MAUtil/Connection.h>
#include <MAUtil/Server.h>
#include <ma.h>
#include <mastring.h>
#include <maassert.h>
#include <conprint.h>

#include "common.h"

using namespace MAUtil;

/**
 * Class that processes Bluetooth client connections.
 * Here we print data received from clients.
 */
class ClientConnectionHandler : public ConnectionListener
{
public:
	/**
	 * Constructor.
	 * @param id The id of the connection.
	 */
	ClientConnectionHandler(int id) :
		mConnection(NULL),
		mId(id)
	{
	}

	/**
	 * Destructor that deletes the connection.
	 */
	virtual ~ClientConnectionHandler()
	{
		delete mConnection;
	}

	/**
	 * Start the server.
	 * @param connection The connection user by the server.
	 */
	void start(Connection* connection)
	{
		// Save pointer to connection in an instance variable.
		mConnection = connection;

		// Print the remote address.
		MAConnAddr address;
		int result = mConnection->getAddr(&address);
		MAASSERT(result > 0);
		// The address is in the byte array named "a".
		byte* a = address.bt.addr.a;
		printf(
			"Connection id: %i\n"
			"Connection address:\n"
			"%02X%02X%02X%02X%02X%02X\n",
			mId,
			a[0], a[1], a[2], a[3], a[4], a[5]);

		// Start receiving data.
		mConnection->setListener(this);
		mConnection->recv(mBuffer, 4);
	}

	/**
	 * Overridden method from ConnectionListener.
	 * Called when a recv operation finishes.
	 * @param connection The Connection that ran the operation.
	 * @param result The number of bytes read on success,
	 * or < 0 on failure.
	 */
	void connRecvFinished(Connection* connection, int result)
	{
		// Was there an error?
		if (result < 0)
		{
			// Print the result code.
			printf(
				"Connection id: %i\n"
				"Error code: %i\n",
				mId, result);
			printf("Closing connection: %i\n", mId);
			printDivider();

			// Delete this connection, also closes the connection.
			delete this;
		}
		else
		{
			// We received data.
			// Avoid a buffer overrun if result is big.
			if (result < sBufferSize)
			{
				// Terminate the data string.
				mBuffer[result] = 0;

				// Print the data.
				printf("Connection id: %i\n", mId);
				printf("Data: %s\n", mBuffer);
				printDivider();
			}

			// Continue receiving data (make room for termination character).
			connection->recv(mBuffer, sBufferSize - 1);
		}
	}

private:
	/** The connection object */
	Connection* mConnection;

	/** Buffer that will contain connection data. */
	char mBuffer[sBufferSize];

	/** The id of this connection. */
	const int mId;
};

/**
 * Moblet for the Bluetooth server.
 */
class ServerMoblet :
	public Moblet,
	public ServerListener
{
public:
	ServerMoblet() :
		mServer(this),
		mNextId(1)
	{
		// Print instructions.
		printf("BluetoothServer example\n");
		printf("Press BACK or 0 to exit.\n");
		printDivider();
		printf("Starting server...\n");

		// Build the connect string.
		char connectString[256];
		sprintf(
			connectString,
			"btspp://localhost:%s;name=BluetoothServer",
			sServiceUUID);

		// Start the server.
		int result = mServer.start(connectString);
		printf("Start result: %i\n", result);

		if (result < 0)
		{
			printf("Error starting the server.\n");
		}
		else
		{
			printf("Server started successfully.\n");
			printDivider();

			// Print the server's Bluetooth address.
			MAConnAddr address;
			result = mServer.getAddr(&address);
			MAASSERT(result > 0);
			MAASSERT(address.family == CONN_FAMILY_BT);
			// The address is in the byte array named "a".
			byte* a = address.bt.addr.a;
			printf(
				"Bluetooth device address:\n"
				"(use this in Client.cpp)\n"
				"%02X%02X%02X%02X%02X%02X\n",
				a[0], a[1], a[2], a[3], a[4], a[5]);
			printf("Bluetooth port: %i\n", address.bt.port);
			printDivider();

			// Start accepting client connections.
			acceptConnection();
		}
	}

	/**
	 * Accept a new client connection.
	 */
	void acceptConnection()
	{
		// Create a handler for the new connection.
		// Give it an id, so we can tell different connections apart.
		// No need to save the pointer; the handler will delete itself
		// when the connection closes.
		mConnectionHandler = new ClientConnectionHandler(mNextId++);
		mServer.accept(mConnectionHandler);
	}

	/**
	 * Called when a key is pressed on the keypad.
	 */
	void keyPressEvent(int keyCode, int nativeCode)
	{
		if (MAK_BACK == keyCode || MAK_0 == keyCode)
		{
			close();
		}
	}

	/**
	 * This is a callback method in class ServerListener.
	 * It is called when a Server::accept() operation fails.
	 * @param server The Server.
	 * @param result An error code.
	 */
	void serverAcceptFailed(Server* server, int result)
	{
		printf("accept failed: %i\n", result);
	}

	/**
	 * This is a callback method in class ServerListener.
     * Called when a Server::accept() operation succeeds.
	 * @param server The Server.
	 * @param newConnection The incoming Connection.
	 * This is an instance of ClientConnectionHandler, which
	 * will delete itself when the connection closes.
	 */
	void serverAccepted(Server*, Connection* newConnection)
	{
		mConnectionHandler->start(newConnection);
		acceptConnection();
	}

private:
	/** The server object. */
	Server mServer;

	/** The next connection id. Used as an identifier in the output. */
	int mNextId;

	/** Pointer to the most recent connection handler. */
	ClientConnectionHandler* mConnectionHandler;
};

/**
 * Main function that starts the program.
 */
extern "C" int MAMain()
{
	Moblet::run(new ServerMoblet());
	return 0;
}
