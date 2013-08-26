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

//system headers
#include <MAUtil/util.h>
#include <conprint.h>
#include <maassert.h>

//local headers
#include "soap.h"

//static const data
static const char sSoapHeader[] =
"<?xml version=\"1.0\" encoding=\"utf-8\"?>"
"<soap12:Envelope xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
"xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" "
"xmlns:soap12=\"http://www.w3.org/2003/05/soap-envelope\">"
"<soap12:Body>\n";
static const int sSoapHeaderLength = sizeof(sSoapHeader) - 1;
static const char sSoapFooter[] =
"</soap12:Body>"
"</soap12:Envelope>\n";
static const int sSoapFooterLength = sizeof(sSoapFooter) - 1;


SoapRequest::SoapRequest() : mHttp(this), mState(eIdle) {
}
SoapRequest::~SoapRequest() {
	cancel();
}

int SoapRequest::start(const char* url, const void* data, int dataLen, SoapListener* listener) {
	MAASSERT(mState == eIdle);
	int res = mHttp.create(url, HTTP_POST);
	if(res < 0) {
		return res;
	}

	char buf[32];
	sprintf(buf, "%i", sSoapHeaderLength + dataLen + sSoapFooterLength);
	mHttp.setRequestHeader("Content-Length", buf);

	mHttp.setRequestHeader("Content-Type", "application/soap+xml; charset=utf-8");

	mHttp.write(sSoapHeader, sSoapHeaderLength);
	mState = eWritingHeader;
	mListener = listener;
	mData = data;
	mDataLen = dataLen;
	return 0;
}

void SoapRequest::cancel() {
	if(mState != eIdle)
		mHttp.close();
	mState = eIdle;
}

void SoapRequest::connWriteFinished(Connection* conn, int result) {
	MAASSERT(conn == &mHttp);
	if(result < 0) {
		mListener->soapError(result);
		return;
	}
	switch(mState) {
	case eWritingHeader:
		mHttp.write(mData, mDataLen);
		mState = eWritingData;
		break;
	case eWritingData:
		mHttp.write(sSoapFooter, sSoapFooterLength);
		mState = eWritingFooter;
		break;
	case eWritingFooter:
		mHttp.finish();
		mState = eFinishing;
		break;
	default:
		BIG_PHAT_ERROR;
	}
}

void SoapRequest::httpFinished(HttpConnection* http, int result) {
	MAASSERT(http == &mHttp);
	if(result < 200 || result >= 300) {
		mListener->soapError(result);
		return;
	}
	MAASSERT(mState == eFinishing);

	printf("recv\n");
	mResponsePtr = mResponseBuffer;
	mHttp.recv(mResponseBuffer, BUFLEN - 1);	// -1 for null terminator
	mState = eReading;

	mContext.init(this);
	mInBody = false;
}

void SoapRequest::connRecvFinished(Connection* conn, int result) {
	printf("connRecvFinished %i\n", result);
	MAASSERT(conn == &mHttp);
	if(result < 0) {
		mListener->soapError(result);
		return;
	}

	mResponsePtr[result] = 0;
	mResponsePtr = mResponseBuffer;
	mContext.feed(mResponseBuffer);
	if(mState == eReading)
		mHttp.recv(mResponsePtr, BUFLEN - 1 - (mResponsePtr - mResponseBuffer));
}

//******************************************************************************
// MtxListener
//******************************************************************************

//#define DEBUG
#ifdef DEBUG
#define LOG printf
#else
#define LOG(...)
#endif

void SoapRequest::mtxEncoding(const char* name) {
	LOG("Encoding %i\n", name);
	if(mInBody)
		mListener->mtxEncoding(name);
}

void SoapRequest::mtxDataRemains(const char* data, int len) {
	if(mResponseBuffer != data) {	//valid value: don't memcpy.
		memcpy(mResponseBuffer, data, len);
	}
	mResponsePtr = mResponseBuffer + len;
}
void SoapRequest::mtxParseError() {
	LOG("parseError\n");
	if(mInBody)
		mListener->mtxParseError();
	mState = eError;
}
void SoapRequest::mtxTagStart(const char* name, int len) {
	LOG("TagStart %i %s\n", len, name);
	if(mInBody)
		mListener->mtxTagStart(name, len);
	if(strcmp(name, "soap:Body") == 0)
		mInBody = true;
}
void SoapRequest::mtxTagAttr(const char* attrName, const char* attrValue) {
	LOG("TagAttrib %s %s\n", attrName, attrValue);
	if(mInBody)
		mListener->mtxTagAttr(attrName, attrValue);
}
void SoapRequest::mtxTagData(const char* data, int len) {
	LOG("TagData %i \"%s\"\n", len, data);
	if(mInBody)
		mListener->mtxTagData(data, len);
}
void SoapRequest::mtxTagEnd(const char* name, int len) {
	LOG("TagEnd %i \"%s\"\n", len, name);
	if(strcmp(name, "soap:Body") == 0) {
		mInBody = false;
		mListener->soapEnd();
	}
	if(mInBody)
		mListener->mtxTagEnd(name, len);
}
void SoapRequest::mtxEmptyTagEnd() {
	LOG("EmptyTagEnd\n");
	if(mInBody)
		mListener->mtxEmptyTagEnd();
}
