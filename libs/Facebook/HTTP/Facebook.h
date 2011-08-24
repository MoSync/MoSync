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
 * Facebook.h
 *
 * A facebook implementation for MoSync
 *
 *  Created on: Feb 22, 2011
 *      Author: Magnus Hult <magnus@magnushult.se>
 */

#ifndef FACEBOOK_H_
#define FACEBOOK_H_

#include <MAUtil/String.h>
#include <MAUtil/Map.h>
#include <MAUtil/Set.h>

#include "HttpProtocol.h"
#include "FacebookRequest.h"
#include "FacebookResponse.h"

using namespace MAUtil;

class Facebook;
class HttpRequest;
class HttpResponse;
class PublishingListener;
class RetrieveDataListener;

class Facebook : public HttpProtocol, HttpProtocolListener {
public:
	Facebook(const String& appId, const MAUtil::Set<MAUtil::String>& permissions);

	void setAppSecret(const MAUtil::String &appSecret);

	const String& getOAuthUrl() const;
	void setPermissions(const MAUtil::Set<MAUtil::String>& permissions);

	void setAccessToken(const String& accessToken);
	const MAUtil::String &getAccessToken() const;

	void setRetrieveDataListener(RetrieveDataListener* listener);
	void setPublishingListener(PublishingListener* listener);

	void requestGraph(FACEBOOK_REQUEST_TYPE type,
		FACEBOOK_RESPONSE_TYPE responseType,
		const String& path,
		int method,
		const String postdata="");

	void requestGraph(FACEBOOK_REQUEST_TYPE type,
		FACEBOOK_RESPONSE_TYPE responseType,
		const String& path,
		int method,
		const Map<String, String>& messageBodyParams,
		const String postdata="");

	void requestGraph(FACEBOOK_REQUEST_TYPE type,
		FACEBOOK_RESPONSE_TYPE responseType,
		const String& path,
		int method,
		const Map<String, String>& messageBodyParams,
		const Map<String, String>& requestHeaders,
		const String postdata="");

	void postMultipartFormData(const MAUtil::String &ID, const MAUtil::String &reqType,
		const int *pixels, int pictureArraySize,
		const MAUtil::String &contentType,
		const MAUtil::String &fileType, const MAUtil::String &fileName, const MAUtil::String &message ="" );

	void postMultipartFormData(const MAUtil::String &ID, const MAUtil::String &reqType,
		const MAUtil::String &picture,
		const MAUtil::String &contentType,
		const MAUtil::String &fileType, const MAUtil::String &fileName, const MAUtil::String &message ="" );

	virtual void response(HttpRequest* request, HttpResponse* response);

	inline virtual ~Facebook() {}

private:
	bool extractConnectionTypeAndId(FacebookRequest *request, MAUtil::String &connectionType, MAUtil::String &id) const;

private:
	String mAppId;
	String mAppSecret;

	String mAccessToken;
	String mOAuthUrl;

	RetrieveDataListener *mConnectionListener;
	PublishingListener *mPublishingListener;
};

#endif /* FACEBOOK_H_ */
