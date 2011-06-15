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

#ifndef SOAP_H
#define SOAP_H

#include <MAUtil/Connection.h>
#include <MTXml/MTXml.h>

using namespace MAUtil;

class SoapListener : public Mtx::XmlListener {
public:
	virtual void soapError(int code) = 0;	//If this is called, the operation stops.
	virtual void soapEnd() = 0;	//This is called when all of the SOAP body has been parsed.
};

class SoapRequest : protected HttpConnectionListener, Mtx::MtxListener, Mtx::XmlListener {
private:
	HttpConnection mHttp;

	virtual void connWriteFinished(Connection* conn, int result);
	virtual void connRecvFinished(Connection* conn, int result);
	virtual void httpFinished(HttpConnection* http, int result);

	virtual void mtxDataRemains(const char* data, int len);
	virtual void mtxEncoding(const char* name);
	virtual void mtxTagStart(const char* name, int len);
	virtual void mtxTagAttr(const char* attrName, const char* attrValue);
	virtual void mtxTagStartEnd();
	virtual void mtxTagData(const char* data, int len);
	virtual void mtxTagEnd(const char* name, int len);
	virtual void mtxParseError();
	virtual void mtxEmptyTagEnd();
	virtual unsigned char mtxUnicodeCharacter(int unicode);

	enum State {
		eIdle, eWritingHeader, eWritingData, eWritingFooter, eFinishing, eReading, eError
	} mState;
	enum { BUFLEN=1024 };

	SoapListener* mListener;
	const void* mData;
	int mDataLen;
	char mResponseBuffer[BUFLEN];
	char* mResponsePtr;
	Mtx::Context mContext;
	bool mInBody;

public:
	SoapRequest();
	virtual ~SoapRequest();

	/**
	* Starts a SOAP operation. The \a listener recieves callbacks when an XML element is parsed or an error occurs.
	* \a listener must remain valid until the operation has stopped.
	* \a data must remain valid until the first callback.
	* \param url The HTTP POST url to be used in the SOAP call.
	* \param data The SOAP request body. Its format depends on the SOAP function you're calling, but it's normally
	* some kind of XML.
	* \param dataLen The length of the buffer pointed to by \a data, in bytes.
	* \param listener A pointer to an object that implements the SoapListener interface.
	* The object will recieve callbacks during the operation.
	* \returns 0 on success, \< 0 on failure.
	*/
	int start(const char* url, const void* data, int dataLen, SoapListener* listener);

	/**
	* Will cause SoapError(CONNERR_CANCELED), unless you're too late and the parsing is already finished,
	* in which case this function does nothing.
	*/
	void cancel();
};

#endif	//SOAP_H
