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

/*
 * HttpRequest.h
 */

#ifndef HTTPREQUEST_H_
#define HTTPREQUEST_H_

#include <MAUtil/String.h>
#include <MAUtil/Map.h>

using namespace MAUtil;

class HttpRequest {
public:
	HttpRequest(const String& url, int method);

	int getMethod() const;
	const String& getUrl() const;

	void setMessageBodyParams(const Map<String, String>& params);
	void setRequestHeaders(const Map<String, String>& params);

	const Map<String, String>& getMessageBodyParams() const;
	const Map<String, String>& getRequestHeaders() const;

	int getDataSize() const;
	const byte* getData() const;

	void setPostdata(const MAUtil::String &postdata);

	virtual ~HttpRequest();

protected:
	String				mUrl;

	Map<String, String> mMessageBodyParams;
	Map<String, String> mRequestHeaders;

	int					mMethod;
	const byte* 		mPostdata;
	int					mDataSize;


};

#endif /* HTTPREQUEST_H_ */
