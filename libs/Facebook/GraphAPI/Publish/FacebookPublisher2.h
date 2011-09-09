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
 * FacebookPublisher2.h
 */

#ifndef FACEBOOKPUBLISHER2_H_
#define FACEBOOKPUBLISHER2_H_

#include <MAUtil/Vector.h>
#include <MAUtil/String.h>

#include "UnixTimeStamp.h"
#include "PublishListener.h"

class Facebook;

/*
 * class that manages publishing to Facebook.
 * If a "PublishingListener" object is provided, it will be informed by the result of request.
 * If the publish request is successful the id of the new object is send. If the request was
 * a like, unlike or delete then a boolean value will be send to the listener.
 * If the publish failed then the HTTP error code and the id of the object on which the request
 * was made is send to the listener.
 */
class FacebookPublisher2 : public PublishingListener
{
public:
	/*
	 * constructor.
	 * @param facebook - pointer to a Facebook object. This object will actually make the request,
	 * and will inform the FacebookPublisher2 when it retrieves a response from the server.
	 * @param postdataSeparator - the separator used for the post data parameters.
	 * 							e.g: name=John&message="Hello"
	 */
	FacebookPublisher2(Facebook *facebook, const MAUtil::String &postdataSeparator = "&");

	/*
	 * Sets the listener of FacebookPublisher2.
	 * @param listener - the listener of the FacebookPublisher2. It is informed by the result of the request.
	 * On success the id of the new object is send (or a boolean value, if the request was a delete, like or
	 * unlike).
	 * On failure the HTTP error code and the id of the object on which the request was made is send to the listener.
	 */
	void setListener(PublishingListener *listener);

	/*
	 * Sends  a like request to Facebook.
	 * @param id - the id of the object to be liked.
	 */
	void Like(const MAUtil::String &id);

	/*
	 * Sends  a unlike request to Facebook.
	 * @param id - the id of the object to be disliked.
	 */
	void Unlike(const MAUtil::String &id);

	///////////////////////////////////////////////////////////////////////////////////////
	//						add (create) objects
	///////////////////////////////////////////////////////////////////////////////////////

	/*
	 * Adds a comment to an object. The object can be a Post, Photo, Album, another Comment, a Link ect.
	 * @param OBJECT_ID - the id of the object on which the comment will be made.
	 * @param message - the text of the comment
	 */
	void addComment(const MAUtil::String &OBJECT_ID, const MAUtil::String &message);

	/*
	 * Adds a User to an object. The object can be a Group, FriendList ect.
	 * @param OBJECT_ID - the id of the object (e.g. FriendList) to which the user will be added.
	 * @param userID - the ID of the User that will be added.
	 */
	void addUser(const MAUtil::String &OBJECT_ID, const MAUtil::String &userID);

	/*
	 * Adds a FriendList.
	 * @param OBJECT_ID - the id of the User for which the FriendList object is created.
	 * @param name - the name of the new FriendList.
	 */
	void addFriendList(const MAUtil::String &OBJECT_ID, const MAUtil::String &name);

	/*
	 * Adds a EventResponse to an event.
	 * @param EVENT_ID - the id of the Event to which a response is given.
	 * @param response - the response to the event. Can be: attending, maybe, declined
	 */
	void addEventResponse(const MAUtil::String &EVENT_ID, EVENT_RESPONSE_TYPE response);

	/*
	 * Adds a Note.
	 * @param OBJECT_ID - the id of the User for which the Note object is created.
	 * @param subject - the subject of the Note.
	 * @param message - the message of the Note.
	 */
	void addNote(const MAUtil::String &OBJECT_ID, const MAUtil::String &subject, const MAUtil::String &message);

	/*
	 * Adds a Album object.
	 * @param OBJECT_ID - the id of the User for which the Album object is created.
	 * @param name - the name of the Album.
	 * @param message - the message of the Album.
	 */
	void addAlbum(const MAUtil::String &OBJECT_ID,const MAUtil::String &name, const MAUtil::String &message="");

	/*
	 * Adds a Photo object to a User's wall or to an Album.
	 * @param OBJECT_ID - the id of a User or an Album to which the Photo is added. If the the photo si added to a User, the picture
	 * will appear to the User's wall.
	 * @param pixels - the raw data of the image.
	 * @param message - the message to be posted along with the Photo.
	 */
	void addPhoto(const MAUtil::String &OBJECT_ID, const byte *pixels, int pixelsArraySize,
			const MAUtil::String &message="");

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
	void addEvent(const MAUtil::String &PROFILE_ID, const MAUtil::String &eventName,  const UnixTimeStamp &eventStartTime,
					const UnixTimeStamp &eventEnd_time = UnixTimeStamp(), const MAUtil::String &message="",
					const MAUtil::String &location = "" ,const MAUtil::String &privacyType = "OPEN");

	/*
	 * Adds a Video object to a User's wall..
	 * @param PROFILE_ID - the id of the User for which the Video is posted.
	 * @param videoSource - the raw data of the video.
	 * @param videoDescpription - the video description, that will be posted along with the video.
	 */
	void addVideo(const MAUtil::String &PROFILE_ID, int *videoSource, const MAUtil::String &videoTitle = "",
			 const MAUtil::String &videoDescription = "");

	/*
	 * Adds a Checkin object for a User
	 * @param PROFILE_ID - the id of the User for which the Checkin object is created.
	 * @param placeId - the id of the place to which the checkin is made.
	 * @param coord - the coordinates of the checkin place.
	 * @param tags - list of tagged friends. String containing comma separated list of user id's.
	 * @param message - the message that will be posted along with the Checkin
	 */
	void addCheckin(const MAUtil::String &PROFILE_ID, const MAUtil::String &placeId, 	  const Coordinate &coord,
					const MAUtil::String &tags = "",  const MAUtil::String &message = "");

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
	void addPostOnWall(const MAUtil::String &ID, const MAUtil::String &message, const MAUtil::String &link,	const MAUtil::String &name = "",
			           const MAUtil::String &caption = "", const MAUtil::String &description = "" );

	/*
	 * Adds a Link object to a to a User wall.
	 * @param link - the link to be posted.
	 * @param message - the message to be posted with the link
	 * @param id - the id of the user on which wall the link will be displayed.
	 */
	void addLinkOnWall(const MAUtil::String &ID, const MAUtil::String &link, const MAUtil::String &message ="");

	/*
	 * Adds a StatusMessage object to a to a User wall.
	 * @param message - the message to be posted
	 * @param id - the id of the user on which wall the status message will be displayed.
	 */
	void addStatusMessageOnWall(const MAUtil::String &ID, const MAUtil::String &message);


	///////////////////////////////////////////////////////////////////////////////////////
	//									delete/remove objects
	///////////////////////////////////////////////////////////////////////////////////////

	/*
	 * Removes a user from a group of a friend list.
	 * @param ID - the id of the group or friend list.
	 * @param userId - the id of the user to be removed.
	 */
	void removeUser(const MAUtil::String &ID, const MAUtil::String &userId);

	/*
	 * Removes an object from an user's account. Can be a comment, a like. It can
	 * be also a Post object, but only if the application created the Post object.
	 * @param id - the id of the object to be deleted.
	 */
	void removeObject(const MAUtil::String &id);

	/*
	 * destructor
	 */
	virtual ~FacebookPublisher2();

protected:
	friend class Facebook;

	/*
	 * PublishingListener overwrite
	 * FacebookPublisher2 registers itself as a PublishingListener of the mFacebook object. This will call the
	 * FacebookPublisher2::publishingResponseReceived, sending the response from server in the "success" param.
	 * @param success - the response from server, telling if the request was done.
	 * This function is called when a remove or an unlike request was made.
	 */
	virtual void publishingResponseReceived(bool success, const MAUtil::String &path);

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
	virtual void publishingResponseReceived(const MAUtil::String  &newId, const MAUtil::String &path);

	/*
	 * PublishingListener overwrite
	 * FacebookPublisher2 registers itself as a PublishingListener of the mFacebook object. This will call the
	 * FacebookPublisher2::queryError, in case the request failed, sending the HTTP error code and the request that was made.
	 * @param code - HTTP error code received from the server.
	 * @param path - contains the id of the object on which the publish request was made and the request name.
	 * 				  e.g: id/feed, id/likes
	 */
	virtual void queryError(int code, const MAUtil::String &path);

protected:

	/*
	 * Creates a string that can be send as a multipart/form-data. It is used to upload a Photo or a Video object.
	 * @param pixels - the raw data of the image
	 * @param fileName - the fileName to be uploaded
	 * @param imageOrVideoType - the type of the image/video file to be uploaded.
	 * 							e.g: .gif, jpg, divx, mp4 ect. See: http://developers.facebook.com/docs/reference/api/video/
	 * 							and http://developers.facebook.com/docs/reference/api/user/
	 */
	void getMultipartFormData(int *pixels, MAUtil::String &multipartFormData,
			const MAUtil::String &filename, const MAUtil::String &imageOrVideoType);

	void getMultipartFormData2(int *pixels, MAUtil::String &multipartFormData,
			const MAUtil::String &filename, const MAUtil::String &imageOrVideoType);


	/*
	 * Converts an array of ints into a string object.
	 */
	void convertIntArrayToString(int *pixels, MAUtil::String &str);

protected:
	/*
	 * Facebook object - performs the actually connection request.
	 */
	Facebook					*mFacebook;

	/*
	 * PublishingListener object - will be notified about the success or failure of the publish request.
	 */
	PublishingListener			*mListener;

	/*
	 * mPostdataSeparator - separator between the parameters in a message body of a request.
	 * e.g: name=John+mPostdataSeparator+message=someMessage
	 */
	MAUtil::String				 mPostdataSeparator;

	/*
	 * mMultipartFormBoundary - a boundary string needed to form the multipart/form-data request.
	 */
	MAUtil::String				 mMultipartFormBoundary;
};





#endif /* FACEBOOKPUBLISHER2_H_ */
