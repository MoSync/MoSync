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
 * @file TCPConnection.h
 * @author Bogdan Iusco.
 * @date 10 Nov 2011
 *
 * @brief Send package to a server application using TCP socket.
 */

#ifndef WORMHOLE_LIBS_NOTIFICATION_TCPCONNECTION_H_
#define WORMHOLE_LIBS_NOTIFICATION_TCPCONNECTION_H_

#include <MAUtil/Connection.h>

namespace Wormhole
{
	/**
	 * @brief Send package to a server application using tcp socket.
	 */
	class TCPConnection: public MAUtil::ConnectionListener
	{
	public:
		/**
		 * Constructor.
		 */
		TCPConnection();

		/**
		 * Destructor.
		 */
		virtual ~TCPConnection();

		/**
		 * Sends data to server application.
		 * @param data String that will be sent to server.
		 * In case of error an alert meesage will be displayed to the user.
		 */
		virtual void sendData(const MAUtil::String& data);

		/**
		 * Opens a connection to a given address.
		 * @param ipAddress
		 * @param port
		 * @return One of the CONNERR_GENERIC constants.
		 */
		virtual int connect(const MAUtil::String& ipAddress,
			const int port);

	protected:
		/**
		* Called when a write operation finishes.
		* \param conn The Connection that ran the operation.
		* \param result \> 0 on success,
		* or a \link #CONNERR_GENERIC CONNERR \endlink code \< 0 on failure.
		*/
		virtual void connWriteFinished(MAUtil::Connection* conn, int result);

		/**
		* Called when a connect operation finishes.
		* \param conn The Connection that ran the operation.
		* \param result \> 0 on success,
		* or a \link #CONNERR_GENERIC CONNERR \endlink code \< 0 on failure.
		*/
		virtual void connectFinished(MAUtil::Connection* conn, int result);

	protected:
		/**
		 * Used for sending packages over TCP to a server application.
		 */
		MAUtil::Connection mConnection;

		/**
		 * Connection's status.
		 * If it's smaller than zero the connection with the server has
		 * not been established.
		 */
		int mConnectionStatus;

		/**
		 * Set if the connection with the server has been established.
		 */
		bool mConnected;

		/**
		 * Set if the message has been sent.
		 */
		bool mMessageSent;
	};
} // namespace

#endif
