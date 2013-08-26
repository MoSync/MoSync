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

/*
 * HttpRequest.cpp
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
