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
