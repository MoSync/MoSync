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
 * FacebookRequest.cpp
 */

#include "FacebookRequest.h"
#include "../LOG.h"

FacebookRequest::FacebookRequest(FACEBOOK_REQUEST_TYPE reqType,
								 FACEBOOK_RESPONSE_TYPE responseType,
								 const String& path,
								 String& accessToken,
								 int method,
								 const String postdata) : HttpRequest(FACEBOOK_OPEN_GRAPH_URLBASE + path, method),
	mPath(path.c_str()),
	mType(reqType),
	mResponseType(responseType)
{
	LOG("\n\n============================================================================================");
	LOG("\t\t\t\tmaking request:\n\t\t\t\t%s", MAUtil::String(FACEBOOK_OPEN_GRAPH_URLBASE + path).c_str());
	LOG("============================================================================================\n\n");

	if (accessToken.size() > 0) {
		mMessageBodyParams["access_token"] = accessToken.c_str();
	}

	// If postdata was supplied, copy it
	if (postdata.size()>0)
	{
		this->setPostdata(postdata);
	}
}

const String& FacebookRequest::getPath() const {
	return mPath;
}

FACEBOOK_RESPONSE_TYPE FacebookRequest::getResponseType() const {
	return mResponseType;
}

FACEBOOK_REQUEST_TYPE FacebookRequest::getType() const
{
	return mType;
}
