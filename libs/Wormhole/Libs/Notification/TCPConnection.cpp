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

/**
 * @file TCPConnection.cpp
 * @author Bogdan Iusco.
 * @date 10 Nov 2011
 *
 * @brief Send package to a server application using tcp socket.
 */

#define CONNECTION_ERROR_TITLE "Error"
#define CONNECTION_ERROR_MESSAGE "Could not connect to server!"
#define CANNOT_SEND_DATA_ERROR "Cannot send data to server!"

// Default size for buffers.
#define BUF_SIZE 256

#include <conprint.h>
#include <mastdlib.h>
#include <MAUtil/util.h>
#include "TCPConnection.h"

namespace Wormhole
{
	/**
	 * Constructor.
	 */
	TCPConnection::TCPConnection():
		mConnection(this),
		mConnectionStatus(-1),
		mConnected(false),
		mMessageSent(false)
	{
	}

	/**
	 * Destructor.
	 */
	TCPConnection::~TCPConnection()
	{
		mConnection.close();
	}

	/**
	 * Sends data to server application.
	 * @param data String that will be sent to server.
	 * In case of error an alert meesage will be displayed to the user.
	 */
	void TCPConnection::sendData(const MAUtil::String& data)
	{
		if (mConnectionStatus >= 0 && mConnected)
		{
			char temp[BUF_SIZE];
			int len = sprintf(temp, "%s\015\012", data.c_str());
			mConnection.write(temp, len);
		}
		else
		{
			// TODO: Add error handling, printf should not be used.
			//printf("Message not sent to server");
		}
	}

	/**
	 * Opens a connection to a given address.
	 * @param ipAddress
	 * @param port
	 * @return One of the CONNERR_GENERIC constants.
	 */
	int TCPConnection::connect(const MAUtil::String& ipAddress,
		const int port)
	{
		MAUtil::String address = "socket://" +
			ipAddress +
			":" +
			MAUtil::integerToString(port).c_str();
		mConnectionStatus = mConnection.connect(address.c_str());
		return mConnectionStatus;
	}

	/**
	* Called when a connect operation finishes.
	* \param conn The Connection that ran the operation.
	* \param result \> 0 on success,
	* or a \link #CONNERR_GENERIC CONNERR \endlink code \< 0 on failure.
	*/
	void TCPConnection::connectFinished(MAUtil::Connection* conn, int result)
	{
		if (result < 0)
		{
			// TODO: Add error handling, printf should not be used.
			//printf("mConnection.connectFinished failed. Error code: %d", result);
			mConnected = false;
		}
		else
		{
			mConnected = true;
		}
	}

	/**
	* Called when a write operation finishes.
	* \param conn The Connection that ran the operation.
	* \param result \> 0 on success,
	* or a \link #CONNERR_GENERIC CONNERR \endlink code \< 0 on failure.
	*/
	void TCPConnection::connWriteFinished(MAUtil::Connection* conn, int result)
	{
		if(result < 0)
		{
			mMessageSent = false;
			// TODO: printf should not be used.
			//printf("mConnection.write failed. Error code: %d", result);
			maMessageBox(CONNECTION_ERROR_TITLE, CANNOT_SEND_DATA_ERROR);
		}
		else
		{
			mMessageSent = true;
		}
	}
} // namespace
