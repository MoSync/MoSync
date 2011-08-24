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
 * FacebookRequest.h
 *
 *  Created on: Jul 20, 2011
 *      Author: gabi
 */

#ifndef FACEBOOKREQUEST_H_
#define FACEBOOKREQUEST_H_

#include <MAUtil/String.h>
#include <MAUtil/Map.h>

#include "HttpRequest.h"

using namespace MAUtil;

#define FACEBOOK_OPEN_GRAPH_URLBASE "https://graph.facebook.com/"

enum FACEBOOK_REQUEST_TYPE { RETRIEVE_DATA = 0, PUBLISH = 1 };
enum FACEBOOK_RESPONSE_TYPE { JSON = 2, IMAGE = 3, VIDEO = 4, STRING = 5, BOOLEAN = 6 };

class FacebookRequest : public HttpRequest {
public:
	FacebookRequest(FACEBOOK_REQUEST_TYPE reqType,
		FACEBOOK_RESPONSE_TYPE responseType,
		const String& path,
		String& accessToken,
		int method,
		const String postdata = "");

	virtual const String& getPath() const;

	virtual FACEBOOK_RESPONSE_TYPE getResponseType() const;
	virtual FACEBOOK_REQUEST_TYPE getType() const;

	inline virtual ~FacebookRequest() {}

protected:
	String mPath;
	FACEBOOK_REQUEST_TYPE mType;
	FACEBOOK_RESPONSE_TYPE mResponseType;
};

#endif /* FACEBOOKREQUEST_H_ */
