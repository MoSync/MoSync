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
 * Facebook.h
 *
 * A facebook implementation for MoSync
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
	Facebook(const String& appId, const MAUtil::Set<MAUtil::String> &permissions,
			const MAUtil::String &oAuthUrl);

	const String& getOAuthUrl() const;
	void setPermissions(const MAUtil::Set<MAUtil::String>& permissions);

	void setAccessToken(const String& accessToken);
	const MAUtil::String &getAccessToken() const;

	void setRetrieveDataListener(RetrieveDataListener* listener);
	void setPublishingListener(PublishingListener* listener);

	void requestGraph(	FACEBOOK_REQUEST_TYPE type,
						FACEBOOK_RESPONSE_TYPE responseType,
						const String& path,
						int method,
						const String postdata="");

	void requestGraph(	FACEBOOK_REQUEST_TYPE type,
						FACEBOOK_RESPONSE_TYPE responseType,
						const String& path,
						int method,
						const Map<String, String>& messageBodyParams,
						const String postdata="");

	void requestGraph(	FACEBOOK_REQUEST_TYPE type,
						FACEBOOK_RESPONSE_TYPE responseType,
						const String& path,
						int method,
						const Map<String, String>& messageBodyParams,
						const Map<String, String>& requestHeaders,
						const String postdata="");

	void postMultipartFormData(
			const MAUtil::String &ID,
			const MAUtil::String &reqType,
			const byte *pixels,
			int pictureArraySize,
			const MAUtil::String &contentType,
			const MAUtil::String &fileType,
			const MAUtil::String &fileName,
			const MAUtil::String &message ="" );

	virtual void response(HttpRequest* request, HttpResponse* res);

	inline virtual ~Facebook() {}

private:
	bool extractConnectionTypeAndId(FacebookRequest *request, MAUtil::String &connectionType, MAUtil::String &id) const;
	void initialize(const MAUtil::Set<MAUtil::String>& permissions);


private:
	String mAppId;

	String mAccessToken;
	String mOAuthUrl;

	RetrieveDataListener 	*mConnectionListener;
	PublishingListener 		*mPublishingListener;
};

#endif /* FACEBOOK_H_ */
