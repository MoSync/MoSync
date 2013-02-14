/* Copyright (C) 2009 Mobile Sorcery AB

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

/** \file Connection.h
* \brief Environment-based classes handling connection functions.
*
* Much of the documentation for these classes is shared with the functions in maapi.h.
* Links to that documentation are available where appropriate.
*/

#ifndef _MAUTIL_CONNECTION_H_
#define _MAUTIL_CONNECTION_H_

#include <ma.h>
#include "Environment.h"
#include "String.h"

namespace MAUtil {

class Connection;

/**
* \brief A listener for events from the Connection class.
* All the default implementations call maPanic().
*/
class ConnectionListener {
public:
	/**
	* Called when a connect operation finishes.
	* \param conn The Connection that ran the operation.
	* \param result \> 0 on success,
	* or a \link #CONNERR_GENERIC CONNERR \endlink code \< 0 on failure.
	*/
	virtual void ATTRIBUTE(noreturn, connectFinished(Connection* conn, int result));

	/**
	* Called when a recv operation finishes.
	* \param conn The Connection that ran the operation.
	* \param result The number of bytes read on success,
	* or a \link #CONNERR_GENERIC CONNERR \endlink code \< 0 on failure.
	*/
	virtual void ATTRIBUTE(noreturn, connRecvFinished(Connection* conn, int result));

	/**
	* Called when a write operation finishes.
	* \param conn The Connection that ran the operation.
	* \param result \> 0 on success,
	* or a \link #CONNERR_GENERIC CONNERR \endlink code \< 0 on failure.
	*/
	virtual void ATTRIBUTE(noreturn, connWriteFinished(Connection* conn, int result));

	/**
	* Called when a read operation finishes.
	* \param conn The Connection that ran the operation.
	* \param result \> 0 on success,
	* or a \link #CONNERR_GENERIC CONNERR \endlink code \< 0 on failure.
	*/
	virtual void ATTRIBUTE(noreturn, connReadFinished(Connection* conn, int result));
};

/**
* \brief An Environment-based communications class.
* Encapsulates MoSync's basic connection system, which supports,
* for example, TCP, HTTP and Bluetooth.
*/
class Connection : protected ConnListener {
public:
	/**
	* \param listener Will recieve events from this Connection.
	* \param conn Optional. If you have a connection handle,
	* you can pass it here to wrap it in a Connection.
	*/
	Connection(ConnectionListener* listener, MAHandle conn=0);

	/**
	* Closes the connection, if open.
	* \see close()
	*/
	virtual ~Connection();

	/*
	* Asynchronously opens a connection using the specified URL.
	*
	* This function returns immediately. If the connect operation could be started,
	* the return value will be \> 0, and connectFinished() of the
	* ConnectionListener associated with this Connection will be called
	* when the operation is finished.
	*
	* If the connect operation could NOT be started, the return value will be a
	* \link #CONNERR_GENERIC CONNERR \endlink code \< 0, and connectFinished() will
	* not be called.
	*
	* \param url See maConnect().
	*/
	int connect(const char* url);

	/**
	* Closes the connection, if open. Frees all resources allocated by this object.
	* Allows this Connection object to be reused for further connections.
	* \see maConnClose()
	*/
	void close();

	/**
	* Causes ConnectionListener::connWriteFinished() to be called when the operation is complete.
	* \see maConnWrite()
	* \warning The source data must remain available until the operation is complete, so don't
	* use local variables as input.
	*/
	void write(const void* src, int len);

	/**
	* Causes ConnectionListener::connWriteFinished() to be called when the operation is complete.
	* \see maConnWriteFromData()
	*/
	void writeFromData(MAHandle data, int offset, int len);

	/**
	* Causes ConnectionListener::connWriteFinished() to be called when the operation is complete.
	* \see maConnWriteTo()
	* \a dst may be discarded once this function returns.
	*/
	void writeTo(const void* src, int len, const MAConnAddr& dst);

	/**
	* Reads between 1 and \a maxlen bytes to \a dst.
	* Causes ConnectionListener::connRecvFinished() to be called when the operation is complete.
	* Use this function when you don't care how much data you get, as long as you get something.
	*/
	void recv(void* dst, int maxlen);

	/**
	* Reads between 1 and \a maxlen bytes to \a data, starting at \a offset.
	* Causes ConnectionListener::connRecvFinished() to be called when the operation is complete.
	*/
	void recvToData(MAHandle data, int offset, int maxlen);

	/**
	* Reads between 1 and \a maxlen bytes to \a dst. Stores the sender address in \a src.
	* Causes ConnectionListener::connRecvFinished() to be called when the operation is complete.
	* \warning \a src must remain valid until the operation is complete.
	* \see maConnReadFrom()
	*/
	void recvFrom(void* dst, int maxlen, MAConnAddr* src);

	/**
	* Reads exactly \a len bytes to \a dst.
	* Causes ConnectionListener::connReadFinished() to be called when the operation is complete.
	* Use this function when you know exactly how much data you want, and are not interested in
	* anything less.
	*/
	void read(void* dst, int len);

	/**
	* Reads exactly \a len bytes to \a data, starting at \a offset.
	* Causes ConnectionListener::connReadFinished() to be called when the operation is complete.
	*/
	void readToData(MAHandle data, int offset, int len);

	/**
	* Replaces the listener for this object.
	* \warning If you do this while an operation is active,
	* the new listener will recieve the callback, not the old one.
	* This may break your program if it is not aware.
	*/
	void setListener(ConnectionListener* listener);

	/**
	* Returns true if the connection is open, false otherwise.
	* The connection is considered open even during the connect() operation.
	*/
	bool isOpen() const;

	/**
	* Retrieves the remote address of this connection.
	* \returns \> 0 on success, \< 0 on error.
	*/
	int getAddr(MAConnAddr* dst);

protected:
	MAHandle mConn;
	ConnectionListener* mListener;

	//for read*()
	int mRemain;	//controller. recv if == 0.
	byte* mDst;
	MAHandle mData;
	int mOffset;

	void (Connection::*mRecv)(int prevRes);

	void recvMem(int prevRes);
	void recvData(int prevRes);

	//ConnListener
	virtual void connEvent(const MAConnEventData& data);
};


class HttpConnection;

/**
* \brief A listener for events from the HttpConnection class.
*/
class HttpConnectionListener : public ConnectionListener {
public:
	/**
	* Called when a finish operation is done.
	* \param http The HttpConnection that ran the operation.
	* \param result The HTTP response code (eg 200 or 404) on success,
	* or a \link #CONNERR_GENERIC CONNERR \endlink code \< 0 on failure.
	*/
	virtual void httpFinished(HttpConnection* http, int result) = 0;
};

/**
* \brief This class describes a single HTTP 1.0 operation.
*/
class HttpConnection : public Connection {
public:
	/**
	* Initializes the new HttpConnection with the specified HttpConnectionListener.
	*/
	HttpConnection(HttpConnectionListener* listener);

	/**
	* \returns \>0 on success, or a \link #CONNERR_GENERIC CONNERR \endlink code \< 0 on failure.
	* \see maHttpCreate
	*/
	int create(const char* url, int method);

	/**
	* \see maHttpSetRequestHeader
	*/
	void setRequestHeader(const char* key, const char* value);

	/**
	* Stores an HTTP response header value in a String, if the header exists.
	* Resizes the String appropriately.
	* Does not modify the String if the header does not exist.
	* \returns The length of the header value, or
	* #CONNERR_NOHEADER if the header doesn't exist.
	* \see maHttpGetResponseHeader
	*/
	int getResponseHeader(const char* key, String* str);

	/**
	* This function returns immediately. When the operation is complete,
	* httpFinished() of the HttpConnectionListener associated with this
	* HttpConnection will be called.
	*
	* \see maHttpFinish
	*/
	void finish();

protected:
	//ConnectionListener
	virtual void connEvent(const MAConnEventData& data);
};

}	//namespace MAUtil

#endif	//_MAUTIL_CONNECTION_H_
