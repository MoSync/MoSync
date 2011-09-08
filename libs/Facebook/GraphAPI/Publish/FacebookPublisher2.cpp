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
 * FacebookPublisher2.cpp
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

/*
 * constructor.
 * @param facebook - pointer to a Facebook object. This object will actually make the request,
 * and will inform the FacebookPublisher2 when it retrieves a response from the server.
 * @param postdataSeparator - the separator used for the post data parameters.
 * 							e.g: name=John&message="Hello"
 */
FacebookPublisher2::FacebookPublisher2(Facebook *facebook,  const MAUtil::String &postdataSeparator)
{
	mFacebook = facebook;
	mFacebook->setPublishingListener(this);
	mPostdataSeparator = postdataSeparator;
	mMultipartFormBoundary = MAUtil::String("---------------------------109074266748897678777839994");
}

/*
 * Sets the listener of FacebookPublisher2.
 * @param listener - the listener of the FacebookPublisher2. It is informed by the result of the request.
 * On success the id of the new object is send (or a boolean value, if the request was a delete, like or
 * unlike).
 * On failure the HTTP error code and the id of the object on which the request was made is send to the listener.
 */
void FacebookPublisher2::setListener(PublishingListener *listener)
{
	mListener = listener;
}

/*
 * Sends  a like request to Facebook.
 * @param id - the id of the object to be liked.
 */
void FacebookPublisher2::Like(const MAUtil::String &id)
{
	MAUtil::String postdata = "method=POST";
	mFacebook->requestGraph(PUBLISH, BOOLEAN, id + "/likes", HTTP_POST, postdata);

}

/*
 * Sends  a unlike request to Facebook.
 * @param id - the id of the object to be disliked.
 */
void FacebookPublisher2::Unlike(const MAUtil::String &id)
{
	MAUtil::Map<MAUtil::String, MAUtil::String> messageBodyParams;
	messageBodyParams["method"]="DELETE";
	mFacebook->requestGraph(PUBLISH, BOOLEAN, id + "/likes", HTTP_GET, messageBodyParams);
}

///////////////////////////////////////////////////////////////////////////////////////
//						add (create) objects
///////////////////////////////////////////////////////////////////////////////////////

/*
 * Adds a comment to an object. The object can be a Post, Photo, Album, another Comment, a Link ect.
 * @param OBJECT_ID - the id of the object on which the comment will be made.
 * @param message - the text of the comment
 */
void FacebookPublisher2::addComment(const MAUtil::String &OBJECT_ID, const MAUtil::String &message)
{
	MAUtil::String postdata = "message=" + message;
	mFacebook->requestGraph(PUBLISH, STRING, OBJECT_ID + "/comments", HTTP_POST, postdata);
}

/*
 * Adds a User to an object. The object can be a Group, FriendList ect.
 * @param OBJECT_ID - the id of the object (e.g. FriendList) to which the user will be added.
 * @param userID - the ID of the User that will be added.
 */
void FacebookPublisher2::addUser(const MAUtil::String &OBJECT_ID, const MAUtil::String &userID)
{
	MAUtil::String postdata = "method=POST";
	mFacebook->requestGraph(PUBLISH, STRING, OBJECT_ID + "/members/" + userID, HTTP_POST, postdata);
}

/*
 * Adds a FriendList.
 * @param OBJECT_ID - the id of the User for which the FriendList object is created.
 * @param name - the name of the new FriendList.
 */
void FacebookPublisher2::addFriendList(const MAUtil::String &PROFILE_ID, const MAUtil::String &name)
{
	MAUtil::Map<MAUtil::String, MAUtil::String> params;
	params["name"]=name;
	MAUtil::String postdata = "method=POST" + mPostdataSeparator;
	postdata += "name=";
	postdata += name;
	mFacebook->requestGraph(PUBLISH, STRING, PROFILE_ID + "/friendlists", HTTP_POST, params, postdata);
}

/*
 * Adds a EventResponse to an event.
 * @param EVENT_ID - the id of the Event to which a response is given.
 * @param response - the response to the event. Can be: attending, maybe, declined
 */
void FacebookPublisher2::addEventResponse(const MAUtil::String &EVENT_ID, EVENT_RESPONSE_TYPE response)
{
	MAUtil::String strResponse;

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

	MAUtil::String postdata = "method=POST";
	mFacebook->requestGraph(PUBLISH, BOOLEAN, EVENT_ID + "/" + strResponse, HTTP_POST, postdata);
}

/*
 * Adds a Note.
 * @param OBJECT_ID - the id of the User for which the Note object is created.
 * @param subject - the subject of the Note.
 * @param message - the message of the Note.
 */
void FacebookPublisher2::addNote(const MAUtil::String &PROFILE_ID, const MAUtil::String &subject, const MAUtil::String &message)
{
	MAUtil::String postdata = "method=POST" + mPostdataSeparator;
	postdata += "subject=";
	postdata += subject;
	postdata += mPostdataSeparator;
	postdata += "message=" + message;
	mFacebook->requestGraph(PUBLISH, STRING, PROFILE_ID + "/notes", HTTP_POST, postdata);
}

/*
 * Adds a Album object.
 * @param OBJECT_ID - the id of the User for which the Album object is created.
 * @param name - the name of the Album.
 * @param message - the message of the Album.
 */
void FacebookPublisher2::addAlbum(const MAUtil::String &PROFILE_ID, const MAUtil::String &name, const MAUtil::String &message)
{
	MAUtil::String postdata = "name=" + name;
	if(message.size()>0)
	{
		postdata += mPostdataSeparator + "message=" + message;
	}
	mFacebook->requestGraph(PUBLISH, STRING, PROFILE_ID + "/albums", HTTP_POST, postdata);
}

/*
 * Adds a Photo object to a User's wall or to an Album.
 * @param OBJECT_ID - the id of a User or an Album to which the Photo is added. If the the photo si added to a User, the picture
 * will appear to the User's wall.
 * @param pixels - the raw data of the image.
 * @param message - the message to be posted along with the Photo.
 */
void FacebookPublisher2::addPhoto(const MAUtil::String &OBJECT_ID, const byte *pixels,
		int pixelsArraySize,
		const MAUtil::String &message)
{

	mFacebook->postMultipartFormData(
			OBJECT_ID,
			"photos",
			pixels,
			pixelsArraySize,
			"IMAGE/jpeg",
			"jpeg",
			"Tami.jpg",
			"testing uploading a file");

}

/*
 * Adds a Event object for a User.
 * @param PROFILE_ID - the id of the User to which the event is added.
 * @param eventName - the name of the event.
 * @param eventStartTime - the start time of the Event.
 * @param eventEndTime - the end time of the Event.
 * @param message - the message to be posted alog with the event
 * @param location - the name of the location
 * @param privacyType - a string that can have the following values: "PUBLIC",
 */
void FacebookPublisher2::addEvent(const MAUtil::String &PROFILE_ID,		const MAUtil::String &name,
									const UnixTimeStamp &start_time,	const UnixTimeStamp &end_time,
									const MAUtil::String &message,		const MAUtil::String &location,
									const MAUtil::String &privacyType)
{
	MAUtil::String postdata = "name=" + name;

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

/*
 * Adds a Video object to a User's wall..
 * @param PROFILE_ID - the id of the User for which the Video is posted.
 * @param videoSource - the raw data of the video.
 * @param videoDescpription - the video description, that will be posted along with the video.
 */
void FacebookPublisher2::addVideo( const MAUtil::String &PROFILE_ID, int *videoSource,
		const MAUtil::String &videoTitle , const MAUtil::String &videoDescription )
{
//	MAUtil::String postdata = "source=" + videoSource;
//	if(videoTitle.size()>0)
//	{
//		postdata += mPostdataSeparator + "title=" + videoTitle;
//	}
//	if(videoDescription.size()>0)
//	{
//		postdata += mPostdataSeparator + "description=" + videoDescription;
//	}

//	mFacebook->requestGraph(PUBLISH, STRING, PROFILE_ID + "/videos", HTTP_POST, postdata);
}

/*
 * Adds a Checkin object for a User
 * @param PROFILE_ID - the id of the User for which the Checkin object is created.
 * @param placeId - the id of the place to which the checkin is made.
 * @param coord - the coordinates of the checkin place.
 * @param tags - list of tagged friends. String containing comma separated list of user id's.
 * @param message - the message that will be posted along with the Checkin
 */
void FacebookPublisher2::addCheckin(const MAUtil::String &PROFILE_ID,	const MAUtil::String &placeId,
									const Coordinate &coord,			const MAUtil::String &tags,
									const MAUtil::String &message)
{

	MAUtil::Map<MAUtil::String, MAUtil::String> params;

	params["place"] = placeId;

	MAUtil::String coordStr = "{\"latitude\":\"" + coord.mLatitude +"\",";
	coordStr += "\"longitude\":\"" + coord.mLongitude + "\"}";

	params["coordinates"] = coordStr;

	MAUtil::String postdata = "method=POST" + mPostdataSeparator;
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
//									PostOnWall
///////////////////////////////////////////////////////////////////////////////////////
/*
 * Adds a Post object to a to a User wall.
 * @param message - the message to be posted
 * @param link - the link to be included in post.
 * @param pictureUrl - a picture url to be included in post.
 * @param actions - a Vector of Action objects (objects containing name and link) to be included in the post.
 * @param id - the id of the user on which wall the post will be displayed.
 */
void FacebookPublisher2::addPostOnWall(const MAUtil::String &ID, const MAUtil::String &message, const MAUtil::String &link,
			const MAUtil::String &name, const MAUtil::String &caption, const MAUtil::String &description )
{
	MAUtil::Map<MAUtil::String, MAUtil::String> params;

	MAUtil::String postdata = "message=" + message;

	if(link.size()>0)
	{
		postdata += mPostdataSeparator + "link=" + link;
	}

//	if(pictureUrl.size()>0)
//	{
//		postdata += mPostdataSeparator + "picture=" + pictureUrl;
//	}
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

	mFacebook->requestGraph(PUBLISH, STRING, ID + "/feed", HTTP_POST, postdata);
}

/*
 * Adds a Link object to a to a User wall.
 * @param link - the link to be posted.
 * @param message - the message to be posted with the link
 * @param id - the id of the user on which wall the link will be displayed.
 */
void FacebookPublisher2::addLinkOnWall(const MAUtil::String &ID, const MAUtil::String &link, const MAUtil::String &message)
{
	MAUtil::String postdata = "link=" + link;
	if(message.size()>0)
	{
		postdata += mPostdataSeparator + "message=" + message;
	}
	mFacebook->requestGraph(PUBLISH, STRING, ID + "/feed", HTTP_POST, postdata);
}

/*
 * Adds a StatusMessage object to a to a User wall.
 * @param message - the message to be posted
 * @param id - the id of the user on which wall the status message will be displayed.
 */
void FacebookPublisher2::addStatusMessageOnWall(const MAUtil::String &ID, const MAUtil::String &message)
{
	MAUtil::String postdata = "message=" + message;
	mFacebook->requestGraph(PUBLISH, STRING, ID + "/feed", HTTP_POST, postdata);
}


///////////////////////////////////////////////////////////////////////////////////////
//									delete/remove) objects
///////////////////////////////////////////////////////////////////////////////////////

/*
 * Removes a user from a group of a friend list.
 * @param ID - the id of the group or friend list.
 * @param userId - the id of the user to be removed.
 */
void FacebookPublisher2::removeUser(const MAUtil::String &LIST_ID, const MAUtil::String &userID)
{
	MAUtil::Map<MAUtil::String, MAUtil::String> params;
	params["method"]="DELETE";
	mFacebook->requestGraph(PUBLISH, BOOLEAN, LIST_ID + "/members/" + userID, HTTP_GET, params);
}

/*
 * Removes an object from an user's account. Can be a comment, a like. It can
 * be also a Post object, but only if the application created the Post object.
 * @param id - the id of the object to be deleted.
 */
void FacebookPublisher2::removeObject(const MAUtil::String &ID)
{
	MAUtil::Map<MAUtil::String, MAUtil::String> params;
	params["method"]="DELETE";
	mFacebook->requestGraph(PUBLISH, BOOLEAN, ID, HTTP_GET, params);
}

/*
 * destructor
 */
FacebookPublisher2::~FacebookPublisher2()
{
}

/*
 * PublishingListener overwrite
 * FacebookPublisher2 registers itself as a PublishingListener of the mFacebook object. This will call the
 * FacebookPublisher2::publishingResponseReceived, sending the response from server in the "success" param.
 * @param success - the response from server, telling if the request was done.
 * This function is called when a remove or an unlike request was made.
 */
void FacebookPublisher2::publishingResponseReceived(bool success, const MAUtil::String &path)
{
	mListener->publishingResponseReceived(success, path);
}

/*
 * PublishingListener overwrite
 * FacebookPublisher2 registers itself as a PublishingListener of the mFacebook object. This will call the
 * FacebookPublisher2::publishingResponseReceived, sending the response from server in the "newId" param. The newId is
 * the id of new created object.
 * @param newId - the id of the new object.
 * @param path - contains the id of the object on which the publish request was made and the request name.
 * 				  e.g: id/feed, id/likes
 * This function is called when a new object is created (Album, Like, Comment, StatusMessage ect).
 */
void FacebookPublisher2::publishingResponseReceived(const MAUtil::String  &data, const MAUtil::String &path)
{
	MAUtil::String id;
	int found = data.find(":",0);
	if(found != MAUtil::String::npos)
	{
		found = data.find("\"", found);
		int last = data.findLastOf('"');
		if( (found != MAUtil::String::npos) && (last != MAUtil::String::npos))
		{
			id = data.substr(found+1, last-found);
		}

	}
	mListener->publishingResponseReceived(id, path);
}

/*
 * PublishingListener overwrite
 * FacebookPublisher2 registers itself as a PublishingListener of the mFacebook object. This will call the
 * FacebookPublisher2::queryError, in case the request failed, sending the HTTP error code and the request that was made.
 * @param code - HTTP error code received from the server.
 * @param path - contains the id of the object on which the publish request was made and the request name.
 * 				  e.g: id/feed, id/likes
 */
void FacebookPublisher2::FacebookPublisher2::queryError(int code, const MAUtil::String &path)
{
	mListener->queryError(code, path);
}

/*
 * Creates a string that can be send as a multipart/form-data. It is used to upload a Photo or a Video object.
 * @param pixels - the raw data of the image
 * @param fileName - the fileName to be uploaded
 * @param imageOrVideoType - the type of the image/video file to be uploaded.
 * 							e.g: .gif, jpg, divx, mp4 ect. See: http://developers.facebook.com/docs/reference/api/video/
 * 							and http://developers.facebook.com/docs/reference/api/user/
 */
//void FacebookPublisher2::getMultipartFormData(int *pixels, MAUtil::String &multipartFormData,
//		const MAUtil::String &filename,  const MAUtil::String &imageType)
//{
//	using MAUtil::String;
//	String crlf("\r\n");
////	String boundaryStr("---------------------------109074266748897678777839994");
//	String boundary = "--" + mMultipartFormBoundary + crlf;
//
//	multipartFormData += boundary;
//	multipartFormData+= String("Content-Disposition: form-data; name=\"file\"; filename=\"" +
//								filename + "\"" + crlf);
//	multipartFormData +=String("Content-Type: image/" + imageType + crlf + crlf);
//	MAUtil::String imageData;
//	convertIntArrayToString(pixels, imageData);
//	multipartFormData += imageData;
//	multipartFormData += String(crlf + "--" + mMultipartFormBoundary + "--" + crlf);
//}

void getMultipartFormData2(int *pixels, MAUtil::String &multipartFormData,
		const MAUtil::String &filename, const MAUtil::String &imageOrVideoType)
{

}

/*
 * Converts an array of ints into a string object.
 */
void FacebookPublisher2::convertIntArrayToString(int *pixels, MAUtil::String &str)
{
	char buff [sizeof(int)+1];

	int SIZE = sizeof(pixels)/sizeof(int);
	char result[SIZE*sizeof(int)];

	for(int i=0; i<SIZE; i++)
	{
		memcpy(buff, &pixels[i], sizeof(int));
		buff[sizeof(int)]='\0';
		memcpy(result + i*sizeof(int), buff, sizeof(int));

	}
	str += result;
}
