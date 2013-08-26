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
