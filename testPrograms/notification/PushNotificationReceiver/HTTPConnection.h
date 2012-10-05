/*
Copyright (C) 2012 MoSync AB

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
 * @file HTTPConnection.h
 * @author emma
 *
 * @brief Utility class for sending HTTP request to the web application.
 * Used on Android devices, to interact with the GCM web server provided
 * as a demo on Google pages: http://developer.android.com/guide/google/gcm/demo.html
 */

#ifndef HTTPCONNECTION_H_
#define HTTPCONNECTION_H_


// The library for strings.
#include <MAUtil/String.h>

// Included to provide the basic connection functionality.
#include <MAUtil/Connection.h>

// Include the library that provides atoi function, which converts the contents of an array to an int.
#include <mastdlib.h>

// Include the class with utility functions.
#include "Util.h"

#include "Listeners.h"

/**
 * @brief Send HTTP requests to GCM web server.
 */
class HTTPConnection: private MAUtil::HttpConnectionListener
{

public:
	/**
	 * Constructor.
	 */
	HTTPConnection(HTTPListener* listener);

	/**
	 * Destructor.
	 */
	~HTTPConnection();

	/**
	 * Post the registration key to the server.
	 * @return
	 */
	int sendApiKey(const MAUtil::String address, const MAUtil::String& registration);
private:
	/**
	 * Inherited from HttpConnectionListener.
	 * This callback function is called when a connection with the server is
	 * established.
	 * @param http The HttpConnection that ran the operation.
	 * @param result The HTTP response code (eg 200 or 404) on success,
	 * or a \link #CONNERR_GENERIC CONNERR \endlink code \< 0 on failure.
	 */
	void httpFinished(MAUtil::HttpConnection *conn, int result);

	/**
	 * It is called each time a new chunk of data is received.
	 * @param conn The Connection that ran the operation.
	 * @param result The number of bytes read on success,
	 * or a \link #CONNERR_GENERIC CONNERR \endlink code \< 0 on failure.
	 */
	void connRecvFinished(MAUtil::Connection *conn, int result);

	/**
	 * It is called when the read is done.
	 * @param conn The Connection that ran the operation.
	 * @param result \> 0 on success,
	 * or a \link #CONNERR_GENERIC CONNERR \endlink code \< 0 on failure.
	 */
	void connReadFinished(MAUtil::Connection *conn, int result);

private:
	/** The connection. **/
	MAUtil::HttpConnection   mHttp;

	/** Connection state. **/
	bool 					 mIsConnected;

	/* The response buffer. **/
	char 					 mBuffer[256];
	HTTPListener* mListener;
};

#endif /* HTTPCONNECTION_H_ */
