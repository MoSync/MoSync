/*
 * HttpRequest.cpp
 *
 *  Created on: Jul 20, 2011
 *      Author: gabi
 */

#include "HttpRequest.h"

HttpRequest::HttpRequest(const String& url, int method): mUrl(url), mMethod(method)
{
	mPostdata = NULL;
	mDataSize = 0;
}

int HttpRequest::getMethod() const {
	return mMethod;
}

const String& HttpRequest::getUrl() const {
	return mUrl;
}

void HttpRequest::setMessageBodyParams(const Map<String, String>& params)
{
	for(MAUtil::Map<MAUtil::String, MAUtil::String>::ConstIterator it = params.begin(); it!=params.end(); ++it)
	{
		mMessageBodyParams.insert(it->first, it->second);
	}
}

void HttpRequest::setRequestHeaders(const Map<String, String>& params)
{
	for(MAUtil::Map<MAUtil::String, MAUtil::String>::ConstIterator it = params.begin(); it!=params.end(); ++it)
	{
		mRequestHeaders.insert(it->first, it->second);
	}
}

const Map<String, String>& HttpRequest::getMessageBodyParams() const
{
	return mMessageBodyParams;
}

const Map<String, String>& HttpRequest::getRequestHeaders() const
{
	return mRequestHeaders;
}

int HttpRequest::getDataSize() const {
	return mDataSize;
}

const byte* HttpRequest::getData() const {
	return mPostdata;
}

void HttpRequest::setPostdata(const MAUtil::String &postdata)
{
	if (postdata.size()>0)
	{
		if(mPostdata!=NULL)
		{
			delete []mPostdata;
		}

		mPostdata = new byte[postdata.length() + 1];
		strcpy((char*) mPostdata, postdata.c_str());
		mDataSize = postdata.length();
	}
}

HttpRequest::~HttpRequest()
{
	delete []mPostdata;
}
