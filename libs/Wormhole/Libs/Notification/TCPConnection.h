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
