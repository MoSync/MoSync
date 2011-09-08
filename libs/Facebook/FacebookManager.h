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
 * FacebookManager.h
 */

#ifndef FACEBOOKMANAGER_H_
#define FACEBOOKMANAGER_H_

#include <MAUtil/String.h>
#include <MAUtil/Set.h>

#include "GraphAPI/GetFacebookObjects/FacebookObjectManager.h"

#include "GraphAPI/Publish/UnixTimeStamp.h"

class	ObjectRequestListener;
class	ConnectionsManager;
class	ConnectionsManagerListener;
class	PublishingListener;
class	FacebookPublisher2;
class	Facebook;


/*
 * Class that manages the connections requests, Facebook objects and publishing.
 * It can handle the following connections requests:
 *		"friends", "home", "feed", "likes", "movies", "music", "books", "notes", "permissions"
 *		"photos", "picture", "albums", "videos", "videos/uploaded", "events", "groups", "checkins",
 *		"comments", "noreply", "maybe", "attending", "declined", "members", "accounts", "activity",
 *		"friendlists", "interests", "links", "posts", "statuses", "television"
 *
 */
class FacebookManager
{
public:
	/*
	 * constructor
	 * @param appId - the application's id
	 * @param appSecret - the application's secret.
	 */
	FacebookManager(const MAUtil::String &appId);

	/*
	 * constructor
	 * @param appId - the application's id
	 * @param permissions - the permissions that the application wants to request.
	 * @param appSecret - the application's secret.
	 */
	FacebookManager(const MAUtil::String &appId, const MAUtil::Set<MAUtil::String> &permissions);

	/*
	 * sets the access token retrieved from Facebook.
	 */
	void setAccessToken(const MAUtil::String &token);

	/*
	 * Returns the link for redirecting the user to the Facebook OAuth Dialog.
	 */
	const MAUtil::String &getOAuthUrl() const;

	///////////////////////////////////////////////////////////////////////////////////////
	//							request objects
	///////////////////////////////////////////////////////////////////////////////////////
	/*
	 * Sets the ObjectRequestListener. The listener will be notified when a Facebook object
	 * request is completed, or if the request has failed.
	 */
	void setObjectRequestListener(ObjectRequestListener *listener);

	/*
	 * Requests a Facebook object.
	 * The "Type" template parameter is the type of the object that is requested.
	 * @param id - the id of the requested object.
	 */
	template<class Type>
	void requestObject(const MAUtil::String &id);

	template<class Type>
	void requestObject(const MAUtil::String &id, const MAUtil::Vector<MAUtil::String> &fields);

	///////////////////////////////////////////////////////////////////////////////////////
	//							request connections
	///////////////////////////////////////////////////////////////////////////////////////
	/*
	 * Sets a ConnectionsManagerListener, that will be informed when a connection request
	 * is completed, or it had failed.
	 */
	void setConnectionRequestListener(ConnectionsManagerListener *listener);

	/*
	 * Requests a connection for an object.
	 * @param connType - the requested connection
	 * @param id - the id of the object for which the connection was requested.
	 */
	void requestConnection(const MAUtil::String &connType, const MAUtil::String &id = "me");
	void requestConnection(const MAUtil::String &connType, const MAUtil::Vector<MAUtil::String> &fields,
				const MAUtil::String &id = "me");

	void limitTheNumberOfObjectsToFetch(int maxNumberOfObjects);

	void fetchAllObjects();

	///////////////////////////////////////////////////////////////////////////////////////
	//							publish on Facebook
	///////////////////////////////////////////////////////////////////////////////////////

	/*
	 * Sets the PublishingListener, that will be about the result of the publish request.
	 */
	void setPublishingListener(PublishingListener *listener);

	/*
	 * Publishes a Like
	 * @param id - the id of the object to be liked ( a post, message, comment ect).
	 */
	void Like(const MAUtil::String &id);

	/*
	 * Publishes a Unlike
	 * @param id - the id of the object to be disliked ( a post, message, comment ect).
	 */
	void Unlike(const MAUtil::String &id);

	/*
	 * Publishes a comment
	 * @param OBJECT_ID - the id of the object on which the comment is done.
	 * @param message - the text of the comment.
	 */
	void addComment(const MAUtil::String &OBJECT_ID, const MAUtil::String &message);

	/*
	 * Adds a user to a group or a friend list.
	 * @param OBJECT_ID - the id of the user.
	 * @param userID - the id of the user to be added.
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
	void addNote(const MAUtil::String &OBJECT_ID, 	const MAUtil::String &subject,
				 const MAUtil::String &message);

	/*
	 * Adds a Album object.
	 * @param OBJECT_ID - the id of the User for which the Album object is created.
	 * @param name - the name of the Album.
	 * @param message - the message of the Album.
	 */
	void addAlbum(const MAUtil::String &PROFILE_ID, const MAUtil::String &name,
			      const MAUtil::String &message="");

	/*
	 * Adds a Event object for a User.
	 * @param PROFILE_ID - the id of the User to which the event is added.
	 * @param eventName - the name of the event.
	 * @param eventStartTime - the start time of the Event.
	 * @param eventEndTime - the end time of the Event.
	 * @param message - the message to be posted along with the event
	 * @param location - the name of the location
	 * @param privacyType - string containing 'OPEN' (default), 'CLOSED', or 'SECRET'
	 */
	void addEvent(const MAUtil::String &PROFILE_ID, const MAUtil::String &eventName,
				const UnixTimeStamp &eventStart_time, const UnixTimeStamp &eventEnd_time = UnixTimeStamp(),
				const MAUtil::String &message="", const MAUtil::String &location = "",
				const MAUtil::String &privacyType = "OPEN");

	/*
	 * Adds a Photo object to a User's wall or to an Album.
	 * @param OBJECT_ID - the id of a User or an Album to which the Photo is added. If the the photo si added to a User, the picture
	 * will appear to the User's wall.
	 * @param pixels - the raw data of the image.
	 * @param message - the message to be posted along with the Photo.
	 */
	void addPhoto(const MAUtil::String &OBJECT_ID, const byte *pixels, int pixelsArraySize, const MAUtil::String &message="");

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
	void addCheckin(const MAUtil::String &PROFILE_ID, const MAUtil::String &placeId, const Coordinate &coord,
					const MAUtil::String &tags = "", const MAUtil::String &message = "");

	/*
	 * Adds a Post object to a to a User wall.
	 * @param ID - the id of the user on which wall the post will be displayed.
	 * @param message - the message to be posted
	 * @param link - the link to be included in post.
	 * @param name - the name of the post
	 * @param caption - the caption of the post
	 * @param description - a description of what is posted
	 */
	void addPostOnWall(const MAUtil::String &ID, const MAUtil::String &message, const MAUtil::String &link,
					const MAUtil::String &name = "", const MAUtil::String &caption = "", const MAUtil::String &description = "");

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
	//							delete from Facebook
	///////////////////////////////////////////////////////////////////////////////////////

	/*
	 * Removes a user from a friend list.
	 * @param ID - the id of the group or friend list.
	 * @param userId - the id of the user to be removed.
	 */
	void removeUserFrom(const MAUtil::String &ID, const MAUtil::String &userId);

	/*
	 * Removes a Comment
	 * @param id - the id of the Comment
	 */
	void removeComment(const MAUtil::String &id);

	/*
	 * Removes a FriendList
	 * @param id - the id of the FriendList
	 */
	void removeFriendList(const MAUtil::String &id);

	/*
	 * Removes a Note
	 * @param id - the id of the Note
	 */
	void removeNote(const MAUtil::String &id);

	//todo: implement
//	void removePost(const MAUtil::String &id);

	/*
	 * destructor
	 */
	~FacebookManager();

private:
	/*
	 * initializes the FacebookManager: creates the Facebook object, and creates the ConnectionsManager, the FacebookObjectManager
	 * and the FacebookPublisher2, and registers the FacebookManager to them.
	 * @param id - the application id.
	 * @param permissions - the permissions to request from Facebook.
	 * @param appSecret - the appliction's secret.
	 */
	void initialize(const MAUtil::String &appId, const MAUtil::Set<MAUtil::String> &permissions);

private:
	/*
	 *  FacebookObjectManager - handles the request of Facebook objects and the retrieving of the server's response.
	 */
	FacebookObjectManager			*mObjectManager;

	/*
	 * ConnectionsManager - handles the request of connections and the retrieving of the server's response.
	 */
	ConnectionsManager				*mConnectionsManager;

	/*
	 * FacebookPublisher2 - handles the publish requests and the retrieving of the server's response.
	 */
	FacebookPublisher2				*mPublishingManager;

	Facebook						*mFacebook;
};


/*
 * Requests a Facebook object.
 * The "Type" template parameter is the type of the object that is requested.
 * @param id - the id of the requested object.
 */
template<class Type>
void FacebookManager::requestObject(const MAUtil::String &id)
{
	mObjectManager->requestFacebookObject<Type>(id);
}

template<class Type>
void FacebookManager::requestObject(const MAUtil::String &id, const MAUtil::Vector<MAUtil::String> &fields)
{
	mObjectManager->requestFacebookObject<Type>(id, fields);
}

//const MAUtil::Vector<MAUtil::String> &fields

#endif /* FACEBOOKMANAGER_H_ */
