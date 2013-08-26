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
