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
 * FacebookPublisher2.cpp
 *
 *  Created on: Jul 8, 2011
 *      Author: gabi
 */

#include <MAUtil/util.h>

#include "../GetFacebookObjects/FacebookObjects/Post.h"
#include "../GetFacebookObjects/FacebookObjects/StatusMessage.h"
#include "../GetFacebookObjects/FacebookObjects/Link.h"

#include "../../HTTP/Facebook.h"

#include "../../HTTP/FacebookRequest.h"

#include "UnixTimeStamp.h"
#include "FacebookPublisher2.h"
#include "../../HTTP/HttpQueryDownloader.h"
#include <mastdlib.h>

using namespace MAUtil;

FacebookPublisher2::FacebookPublisher2(Facebook *facebook, const String &postdataSeparator)
{
	mFacebook = facebook;
	mFacebook->setPublishingListener(this);
	mPostdataSeparator = postdataSeparator;
	mMultipartFormBoundary = String("---------------------------109074266748897678777839994");
}

void FacebookPublisher2::setListener(PublishingListener *listener)
{
	mListener = listener;
}

void FacebookPublisher2::Like(const String &id)
{
	String postdata = "method=POST";
	mFacebook->requestGraph(PUBLISH, BOOLEAN, id + "/likes", HTTP_POST, postdata);

}

void FacebookPublisher2::Unlike(const String &id)
{
	Map<String, String> messageBodyParams;
	messageBodyParams["method"]="DELETE";
	mFacebook->requestGraph(PUBLISH, BOOLEAN, id + "/likes", HTTP_GET, messageBodyParams);
}

///////////////////////////////////////////////////////////////////////////////////////
// add (create) objects
///////////////////////////////////////////////////////////////////////////////////////

void FacebookPublisher2::addComment(const String &OBJECT_ID, const String &message)
{
	String postdata = "message=" + message;
	mFacebook->requestGraph(PUBLISH, STRING, OBJECT_ID + "/comments", HTTP_POST, postdata);
}

void FacebookPublisher2::addUser(const String &OBJECT_ID, const String &userID)
{
	String postdata = "method=POST";
	mFacebook->requestGraph(PUBLISH, STRING, OBJECT_ID + "/members/" + userID, HTTP_POST, postdata);
}

void FacebookPublisher2::addFriendList(const String &PROFILE_ID, const String &name)
{
	Map<String, String> params;
	params["name"]=name;
	String postdata = "method=POST" + mPostdataSeparator;
	postdata += "name=";
	postdata += name;
	mFacebook->requestGraph(PUBLISH, STRING, PROFILE_ID + "/friendlists", HTTP_POST, params, postdata);
}

void FacebookPublisher2::addEventResponse(const String &EVENT_ID, EVENT_RESPONSE_TYPE response)
{
	String strResponse;

	switch(response)
	{
	case attending:
		strResponse = "attending";
	break;
	case maybe:
		strResponse = "maybe";
	break;
	case declined:
		strResponse = "declined";
	break;
	}

	String postdata = "method=POST";
	mFacebook->requestGraph(PUBLISH, BOOLEAN, EVENT_ID + "/" + strResponse, HTTP_POST, postdata);
}

void FacebookPublisher2::addNote(const String &PROFILE_ID, const String &subject, const String &message)
{
	String postdata = "method=POST" + mPostdataSeparator;
	postdata += "subject=";
	postdata += subject;
	postdata += mPostdataSeparator;
	postdata += "message=" + message;
	mFacebook->requestGraph(PUBLISH, STRING, PROFILE_ID + "/notes", HTTP_POST, postdata);
}

void FacebookPublisher2::addAlbum(const String &PROFILE_ID, const String &name, const String &message)
{
	String postdata = "name=" + name;
	if(message.size()>0)
	{
		postdata += mPostdataSeparator + "message=" + message;
	}
	mFacebook->requestGraph(PUBLISH, STRING, PROFILE_ID + "/albums", HTTP_POST, postdata);
}

void FacebookPublisher2::addPhoto(const String &OBJECT_ID, const int *pixels, int pixelsArraySize,
	const String &message)
{
	String path = OBJECT_ID + "/photos";
	mFacebook->postMultipartFormData(
		"me",
		"photos",
		pixels,
		pixelsArraySize,
		"IMAGE/jpeg",
		"jpeg",
		"Tami.jpg",
		"testing uploading a file");
}

void FacebookPublisher2::addPhoto(const String &OBJECT_ID, const String &picture, const String &message)
{
	String path = OBJECT_ID + "/photos";

	mFacebook->postMultipartFormData(
		"me",
		"photos",
		picture,
		"IMAGE/jpeg",
		"jpeg",
		"Tami.jpg",
		"testing uploading a file");
}

void FacebookPublisher2::addEvent(const String &PROFILE_ID, const String &name,
	const UnixTimeStamp &start_time, const UnixTimeStamp &end_time,
	const String &message, const String &location,
	const String &privacyType)
{
	String postdata = "name=" + name;

	postdata += mPostdataSeparator;
	postdata += "start_time=";
	postdata += start_time.getTimeStamp();


	if(!end_time.isEmpty())
	{
		postdata += mPostdataSeparator;
		postdata += "end_time=";
		postdata += end_time.getTimeStamp();
	}

	if(message.size()>0)
	{
		postdata += mPostdataSeparator + "message=" + message;
	}
	if(location.size()>0)
	{
		postdata += mPostdataSeparator + "location=" + location;
	}
	if(privacyType.size()>0)
	{
		postdata += mPostdataSeparator + "privacy_type =" + privacyType;
	}

	mFacebook->requestGraph(PUBLISH, STRING, PROFILE_ID + "/events", HTTP_POST, postdata);
}

void FacebookPublisher2::addCheckin(const String &PROFILE_ID, const String &placeId,
	const Coordinate &coord, const String &tags,
	const String &message)
{
	Map<String, String> params;

	params["place"] = placeId;

	String coordStr = "{\"latitude\":\"" + coord.mLatitude +"\",";
	coordStr += "\"longitude\":\"" + coord.mLongitude + "\"}";

	params["coordinates"] = coordStr;

	String postdata = "method=POST" + mPostdataSeparator;
	postdata += "place=";
	postdata += placeId + mPostdataSeparator;
	postdata += coordStr;
	if(tags.size()>0)
	{
		postdata = mPostdataSeparator + "tags=" + tags;
	}
	if(message.size()>0)
	{
		postdata = mPostdataSeparator + "message=" + message;
	}

	mFacebook->requestGraph(PUBLISH, STRING, PROFILE_ID + "/checkins", HTTP_POST, params, postdata);
}

///////////////////////////////////////////////////////////////////////////////////////
// PostOnWall
///////////////////////////////////////////////////////////////////////////////////////

void FacebookPublisher2::addPostOnWall(const String &ID, const String &message,
	const String &link, const String &pictureUrl,
	const String &name, const String &caption, const String &description,
	Vector<Action> &actions )
{
	Map<String, String> params;

	String postdata = "message=" + message;

	postdata += mPostdataSeparator + "link=" + link;

	if(pictureUrl.size()>0)
	{
		postdata += mPostdataSeparator + "picture=" + pictureUrl;
	}
	if(name.size()>0)
	{
		postdata += mPostdataSeparator + "name=" + name;
	}
	if(caption.size()>0)
	{
		postdata += mPostdataSeparator + "caption=" + caption;
	}
	if(description.size()>0)
	{
		postdata += mPostdataSeparator + "description=" + description;
	}
//	if(actions.size()>0)
//	{
//		postdata +=",actions={";
//	}

	mFacebook->requestGraph(PUBLISH, STRING, ID + "/feed", HTTP_POST, postdata);
}

void FacebookPublisher2::addLinkOnWall(const String &ID, const String &link, const String &message)
{
	String postdata = "link=" + link;
	if(message.size()>0)
	{
		postdata += mPostdataSeparator + "message=" + message;
	}
	mFacebook->requestGraph(PUBLISH, STRING, ID + "/feed", HTTP_POST, postdata);
}

void FacebookPublisher2::addStatusMessageOnWall(const String &ID, const String &message)
{
	String postdata = "message=" + message;
	mFacebook->requestGraph(PUBLISH, STRING, ID + "/feed", HTTP_POST, postdata);
}


///////////////////////////////////////////////////////////////////////////////////////
// delete/remove) objects
///////////////////////////////////////////////////////////////////////////////////////

void FacebookPublisher2::removeUser(const String &LIST_ID, const String &userID)
{
	Map<String, String> params;
	params["method"]="DELETE";
	mFacebook->requestGraph(PUBLISH, BOOLEAN, LIST_ID + "/members/" + userID, HTTP_GET, params);
}

void FacebookPublisher2::removeObject(const String &ID)
{
	Map<String, String> params;
	params["method"]="DELETE";
	mFacebook->requestGraph(PUBLISH, BOOLEAN, ID, HTTP_GET, params);
}

FacebookPublisher2::~FacebookPublisher2()
{
}

void FacebookPublisher2::publishingResponseReceived(bool success, const String &path)
{
	mListener->publishingResponseReceived(success, path);
}

void FacebookPublisher2::publishingResponseReceived(const String  &data, const String &path)
{
	String id;
	int found = data.find(":",0);
	if(found != String::npos)
	{
		found = data.find("\"", found);
		int last = data.findLastOf('"');
		if( (found != String::npos) && (last != String::npos))
		{
			id = data.substr(found+1, last-found);
		}

	}
	mListener->publishingResponseReceived(id, path);
}

void FacebookPublisher2::FacebookPublisher2::queryError(int code, const String &path)
{
	mListener->queryError(code, path);
}
