/*
 * FacebookRequest.cpp
 *
 *  Created on: Jul 20, 2011
 *      Author: gabi
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
