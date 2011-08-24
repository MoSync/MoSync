/* Copyright (C) 2011 MoSync AB

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
