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
 * @file Client
 * @author Mikael Kindborg
 *
 * Description:
 *
 *  This program connects to a Bluetooth service
 *  with a specific UUID on a hardcoded address.
 *  It will then send character and touch input to
 *  that service.
 *
 * How to use:
 *
 *  See header comment in file Common.h
 *
 *  Note that the file Common.h is located in project
 *  BluetoothServer, and included into this file.
 *  It is assumed that both project folders BluetoothClient
 *  and BluetoothServer are located in the same directory.
 *
 * Further information:
 *
 *  This example shows the easiest way to connect to a
 *  Bluetooth server, using a connect method that takes
 *  a single url as its parameter:
 *
 *   int BluetoothConnection::connect(const char* url);
 *
 *  In our example the url contains the server address
 *  and a service UUID, for example:
 *
 *   btspp://0023769FD7C2:102030405060708090A0B0C0D0E0F010
 *
 *  Another (more advanced) option when connecting using a
 *  service UUID, is to use the connect methods that takes
 *  a MABtAddr and a MAUUID structure as parameters:
 *
 *   int BluetoothConnection::connect(
 *       const MABtAddr& address,
 *       const MAUUID& uuid);
 *
 *  When using this method, you need to fill in the data
 *  structures, and then pass them to the connect method.
 *  For example:
 *
 *   MABtAddr serverAddress =
 *       {{ 0x00,0x23,0x79,0x9F,0xD7,0xC2 }};
 *   MAUUID serviceUUID =
 *       {{ 0x10203040,0x50607080,0x90A0B0C0,0xD0E0F010 }};
 *   int result = mConnection.connect(serverAddress, serviceUUID);
 */

#include <MAUtil/Moblet.h>
#include <MAUtil/BluetoothConnection.h>
#include <ma.h>
#include <maassert.h>
#include <conprint.h>
#include <mastring.h>

// Here we include the file Common.h in the BluetoothServer project.
#include "../BluetoothServer/Common.h"

using namespace MAUtil;

/**
 * This constant contains the server Bluetooth device address.
 * You must run the server application, note the address, and
 * enter it here, before the client will work.
 */
static const char* sServerAddress = "0022FC7739B4";

/**
 * Moblet for the Bluetooth client.
 */
class ClientMoblet :
	public Moblet,
	public ConnectionListener
{
public:
	/**
	 * Constructor. Here we connect to the server.
	 * If the SERVER_ADDRESS is an empty string, we issue
	 * a panic. This address must be filled in.
	 */
	ClientMoblet() :
		mConnection(this),
		mWriteBufferPosition(0),
		mIsWriting(false),
		mIsConnected(false)
	{
		// Print instructions.
		printf("BluetoothClient example\n");
		printf("Press BACK or 0 to exit.\n");
		printDivider();
		printf("Connecting to server...\n");

		// Check that the length of the address string is valid.
		if (strlen(sServerAddress) != 12)
		{
			maPanic(1, "Please fill in sServerAddress in Client.cpp.");
		}

		// Set initial write buffer.
		mCurrentBuffer = mWriteBuffer1;

		// Create the connect string.
		char connectString[64];
		sprintf(
			connectString,
			"btspp://%s:%s",
			sServerAddress,
			sServiceUUID);

		// Connect to the server.
		int result = mConnection.connect(connectString);

		if (result < 0)
		{
			printf("Connect failed. Error: %i\n", result);
		}
		else
		{
			printf("Connect initiated...\n");
		}
		printDivider();
	}

	/**
	 * Called when a key is pressed on the keypad.
	 */
	void keyPressEvent(int keyCode, int nativeCode)
	{
		// Close the app if zero if back is pressed.
		if (MAK_0 == keyCode || MAK_BACK == keyCode)
		{
			close();
		}
		// If it is a printable character, write it to the server.
		else if (keyCode >= MAK_SPACE && keyCode <= MAK_Z)
		{
			printf("Key pressed: %i\n", keyCode);

			// Write character and send to server.
			writeCharacter(keyCode);
			writeBuffer();
		}
	}

	/**
	 * Called on a touch down event.
	 */
	void pointerPressEvent(MAPoint2d point)
	{
		printDivider();
		printf("Touch event: %i,%i\n", point.x, point.y);

		// Generate a string with touch data.
		char buf[32];
		sprintf(buf, "Touch event: %i,%i", point.x, point.y);

		// Write the characters in the string.
		for (size_t i = 0; i < strlen(buf); ++i)
		{
			writeCharacter(buf[i]);
		}

		// Send data to server.
		writeBuffer();
	}

	/**
	 * Write a character to the output buffer and send
	 * data if we are not already writing.
	 * @param charCode The code of the character to write.
	 * Should be a printable character.
	 */
	void writeCharacter(int charCode)
	{
		// If we are connected we write the character to the output buffer.
		if (mIsConnected)
		{
			// If there is room, add the character to the
			// current write buffer.
			if (mWriteBufferPosition < sBufferSize)
			{
				mCurrentBuffer[mWriteBufferPosition++] = charCode;
				printf("Writing: %c\n", charCode);
			}
		}
	}

	/**
	 * Write output buffer to server.
	 */
	void writeBuffer()
	{
		// Only send data if we are connected and not already writing.
		if (mIsConnected && !mIsWriting)
		{
			// Send data to server.
			printf("Sending data...\n");
			mConnection.write(mCurrentBuffer, mWriteBufferPosition);

			// We are now writing.
			mIsWriting = true;

			// Reset buffer position, and swap buffers to avoid
			// overwriting data that is still being sent.
			mWriteBufferPosition = 0;
			mCurrentBuffer = (mCurrentBuffer == mWriteBuffer1)
				? mWriteBuffer2
				: mWriteBuffer1;
		}
	}

	/**
	 * Called when the connect operation is finished.
	 */
	void connectFinished(Connection* connection, int result)
	{
		// If there was an error (result <= 0) we consider
		// the connection to be closed.
		mIsConnected = result > 0;

		// Print instructions.
		if (result < 0)
		{
			printf("connectFinished error: %i\n", result);
			printf(
				"Could not connect to server:\n%s\n",
				sServerAddress);
		}
		else
		{
			printf("Connected to server.\n");
			printf("Press key or touch screen\n");
			printf("to send data to the server.\n");
		}
		printDivider();
	}

	/**
	 * Called when the write operation is finished.
	 */
	void connWriteFinished(Connection* connection, int result)
	{
		// If there was an error (result <= 0) we consider
		// the connection to be closed.
		mIsConnected = result > 0;

		// Writing is finished.
		mIsWriting = false;

		// Print instructions.
		if (result < 0)
		{
			printf("connWriteFinished error: %i\n", result);
		}
		else
		{
			printf("Data successfully written.\n");
		}
		printDivider();
	}

private:
	/** The connection to the server. */
	BluetoothConnection mConnection;

	/** First write buffer. */
	char mWriteBuffer1[sBufferSize];

	/** Second write buffer. */
	char mWriteBuffer2[sBufferSize];

	/** Pointer to currently used write buffer. */
	char* mCurrentBuffer;

	/** Current buffer position. */
	int mWriteBufferPosition;

	/** True if we are sending data to the server. */
	bool mIsWriting;

	/** True if we are connected to the server. */
	bool mIsConnected;
};

/**
 * Main function that starts the program.
 */
extern "C" int MAMain()
{
	Moblet::run(new ClientMoblet());
	return 0;
}
