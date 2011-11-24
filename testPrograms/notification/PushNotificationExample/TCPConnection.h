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
 * @brief Send package to a server application using tcp socket.
 */

#ifndef TCPCONNECTION_H_
#define TCPCONNECTION_H_

#include <MAUtil/Environment.h>
#include <MAUtil/Connection.h>
#include <maapi.h>

#include "TCPListener.h"

/**
 * @brief Send package to a server application using tcp socket.
 */
class TCPConnection: public MAUtil::ConnectionListener
{

public:
	/**
	 * Constructor.
	 */
	TCPConnection(TCPListener* listener);

	/**
	 * Destructor.
	 */
	~TCPConnection();

	/**
	 * Sends data to server application.
	 * @param data String that will be sent to server.
	 * In case of error an alert meesage will be displayed to the user.
	 */
	virtual void sendData(const MAUtil::String& data);

	bool mMessageSent;
private:
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

private:
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
	bool mConnected;
	TCPListener* mListener;
};

#endif /* TCPCONNECTION_H_ */
