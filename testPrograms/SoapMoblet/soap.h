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

class SoapRequest : protected HttpConnectionListener, protected Mtx::MtxListener {
private:
	HttpConnection mHttp;

	virtual void connWriteFinished(Connection* conn, int result);
	virtual void connRecvFinished(Connection* conn, int result);
	virtual void httpFinished(HttpConnection* http, int result);

	virtual void mtxDataRemains(const char* data, int len);
	virtual void mtxEncoding(const char* name);
	virtual void mtxTagStart(const char* name, int len);
	virtual void mtxTagAttr(const char* attrName, const char* attrValue);
	virtual void mtxTagData(const char* data, int len);
	virtual void mtxTagEnd(const char* name, int len);
	virtual void mtxParseError();
	virtual void mtxEmptyTagEnd();

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
	*/
	int start(const char* url, const void* data, int dataLen, SoapListener* listener);

	/**
	* Will cause SoapError(CONNERR_CANCELED), unless you're too late and the parsing is already finished,
	* in which case this function does nothing.
	*/
	void cancel();
};

#endif	//SOAP_H
