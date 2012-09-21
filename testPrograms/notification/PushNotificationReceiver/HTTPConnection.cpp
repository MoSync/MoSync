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
 * @file HTTPConnection.cpp
 * @author Emma
 *
 * @brief Utility class for sending HTTP request to the web application.
 * Used on Android devices, to interact with the GCM web server provided
 * as a demo on Google pages: http://developer.android.com/guide/google/gcm/demo.html
 */

#define CONNECTION_ERROR_TITLE "Error"
#define CONNECTION_ERROR_MESSAGE "Could not connect to server!"
#define CANNOT_SEND_DATA_ERROR "Cannot send data to server!"
#define CONNECTION_RESPONSE_ERROR "No content-length response header"

// Default size for buffers.
#define BUF_SIZE 256

#define CONNECTION_BUFFER_SIZE 1024

#include "HTTPConnection.h"
#include "Listeners.h"

/**
 * Constructor.
 */
HTTPConnection::HTTPConnection(HTTPListener *listener):
	mHttp(this),
	mIsConnected(false),
	mListener(listener)
{
}

/**
 * Destructor.
 */
HTTPConnection::~HTTPConnection()
{
	if (mIsConnected){
		mHttp.close();
	}
	delete mListener;
}

int HTTPConnection::sendApiKey(const MAUtil::String address, const MAUtil::String& registration)
{
	MAUtil::String url =  "http://"+address + ":8080/gcm-demo/register?regId=" + registration;
	int resultCode = mHttp.create(url.c_str(), HTTP_POST);
	if(resultCode < 0) {
        maMessageBox(CONNECTION_ERROR_TITLE, "cannot connect");//CONNECTION_ERROR_MESSAGE);
	 }
	 else {
		mHttp.setRequestHeader("User-Agent","PushNotificationReceiver");
//		mHttp.setRequestHeader("Content-Type", content.c_str());
//		mHttp.setRequestHeader("Host","api.flickr.com");
		mHttp.setRequestHeader("Content_length:","200");
		mHttp.finish();
		mIsConnected = true;
	 }
	return resultCode;
}

/**
 * Inherited from HttpConnectionListener.
 * This callback function is called when a connection with the server is
 * established.
 * @param http The HttpConnection that ran the operation.
 * @param result The HTTP response code (eg 200 or 404) on success,
 * or a \link #CONNERR_GENERIC CONNERR \endlink code \< 0 on failure.
 */
void HTTPConnection::httpFinished(MAUtil::HttpConnection *conn, int result)
 {
	if ( result >= 0)
	{
		MAUtil::String contentLengthStr;
		  int responseBytes = mHttp.getResponseHeader("content-length",
				  &contentLengthStr);
		  int contentLength = 0;
		  if(responseBytes == CONNERR_NOHEADER)
		  {
			  maMessageBox(CONNECTION_ERROR_TITLE, CONNECTION_RESPONSE_ERROR);
		  }
		  else
		  {
			  contentLength = atoi(contentLengthStr.c_str());
		  }
		  if(contentLength >= CONNECTION_BUFFER_SIZE || contentLength == 0)
		  {
			  mHttp.recv(mBuffer, CONNECTION_BUFFER_SIZE);
		   }
		  else
		  {
		    mBuffer[contentLength] = 0;
		    mHttp.read(mBuffer, contentLength);
		  }
	}
	else
	{
        maMessageBox(CONNECTION_ERROR_TITLE, "httpFinished error");//CANNOT_SEND_DATA_ERROR);
		mHttp.close();
		mIsConnected = false;
	}
}

/**
 * It is called when the read is done.
 * @param conn The Connection that ran the operation.
 * @param result \> 0 on success,
 * or a \link #CONNERR_GENERIC CONNERR \endlink code \< 0 on failure.
 */
void HTTPConnection::connReadFinished(MAUtil::Connection* conn, int result)
{
	mListener->messageSent();
//	if(result >= 0)
//	{
//		mListener->messageSent();
//	}
//	else
//	{
//		maMessageBox(CONNECTION_ERROR_TITLE, CANNOT_SEND_DATA_ERROR);
//	}

	mHttp.close();
	mIsConnected = false;
}

/**
 * It is called each time a new chunk of data is received.
 * @param conn The Connection that ran the operation.
 * @param result The number of bytes read on success,
 * or a \link #CONNERR_GENERIC CONNERR \endlink code \< 0 on failure.
 */
void HTTPConnection::connRecvFinished(MAUtil::Connection * conn, int result)
{
    if(result >= 0)
    {
		// Read next chunk of data.
        mHttp.recv(mBuffer, CONNECTION_BUFFER_SIZE);
        return;
    }
    else if (result == CONNERR_CLOSED)
    {
		mListener->messageSent();
    }
    else
    {
		maMessageBox(CONNECTION_ERROR_TITLE,"Recv error");// CANNOT_SEND_DATA_ERROR);
    }

    mHttp.close();
    mIsConnected = false;
}
