/* Copyright (C) 2010 MoSync AB

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

/** \file Server.h
* \brief Encapsulates MoSync's server connections
* \author Fredrik Eldh
*/

#ifndef _MAUTIL_SERVER_H_
#define _MAUTIL_SERVER_H_

#include <MAUtil/Connection.h>

namespace MAUtil {

class Server;

/// Receives events from MAUtil::Server.
class ServerListener {
public:
	/**
	* Called when a Server::accept() operation fails.
	* The default implementation does nothing.
	* \param server The Server.
	* \param result One of the \link #CONNERR_GENERIC CONNERR \endlink error codes.
	*/
	virtual void serverAcceptFailed(Server* server, int result);
	/**
	* Called when a Server::accept() operation succeeds.
	* \param server The Server.
	* \param newConnection The incoming Connection. When you're done with it,
	* be sure to delete it, to avoid memory leaks.
	*/
	virtual void serverAccepted(Server* server, Connection* newConnection) = 0;
};

/**
* \brief Encapsulates MoSync's server connections.
* Use this class to listen for incoming connections.
*/
class Server : protected ConnListener {
public:
	Server(ServerListener* listener) : mListener(listener), mServ(0) {}

	/**
	* Closes the server.
	*/
	virtual ~Server();

	/**
	* Starts the server. This involves opening a socket, binding it to a port,
	* and, for Bluetooth, adding an entry to the service advertisement list.
	* Once this function returns, you should call accept() to begin receiving connections.
	* \param url A server URL. See maConnect() for details.
	* \returns \> 0 on success, \< 0 on error.
	*/
	int start(const char* url);

	/**
	* Retrieves the local address of this server.
	* \param dst A pointer to the struct that will receive the address.
	* \returns \> 0 on success, \< 0 on error.
	*/
	int getAddr(MAConnAddr* dst);

	/**
	* Asynchronously accepts one incoming connection.
	* Will cause either ServerListener::serverAccepted() or ServerListener::serverAcceptFailed()
	* to be called when the operation is complete.
	* \param cl When a connection is accepted, this will be its listener.
	* \returns \> 0 on success, \< 0 on error.
	*/
	int accept(ConnectionListener* cl);

	/**
	* Closes the server. Removes its service advertisement, if any.
	*/
	void close();

	/**
	* Returns true if the server is open, false otherwise.
	* The server is considered open once start() succeeds.
	*/
	bool isOpen() const;

protected:
	// ConnListener
	virtual void connEvent(const MAConnEventData& data);

	ServerListener* mListener;
	MAHandle mServ;
	ConnectionListener* mCl;
};

}

#endif	//_MAUTIL_SERVER_H_
