/*
 * FacebookManager.cpp
 *
 *  Created on: Jul 11, 2011
 *      Author: gabi
 */

#include "GraphAPI/GetConnections/ConnectionsManagerListener.h"
#include "GraphAPI/GetConnections/ConnectionsManager.h"

#include "GraphAPI/GetFacebookObjects/FacebookObjectManager.h"

#include "GraphAPI/Publish/FacebookPublisher2.h"
#include "GraphAPI/Publish/PublishListener.h"
#include "GraphAPI/Publish/UnixTimeStamp.h"

#include "HTTP/Facebook.h"

#include "FacebookManager.h"

/*
 * constructor
 * @param appId - the application's id
 * @param appSecret - the application's secret.
 */
FacebookManager::FacebookManager(const MAUtil::String &appId)
{
	MAUtil::Set<MAUtil::String> permissions;

	permissions.insert("offline_access");
	permissions.insert("read_stream");
	permissions.insert("publish_stream");
	permissions.insert("user_photos");
	permissions.insert("user_checkins");
	permissions.insert("friends_checkins");
	permissions.insert("user_events");
	permissions.insert("user_groups");
	permissions.insert("friends_groups");
	permissions.insert("user_notes");
	permissions.insert("friends_notes");
	permissions.insert("user_photos");
	permissions.insert("friends_photos");
	permissions.insert("user_photo_video_tags");
	permissions.insert("friends_photo_video_tags");
	permissions.insert("user_videos");
	permissions.insert("friends_videos");
	permissions.insert("video_upload");
	permissions.insert("photo_upload");

	initialize(appId, permissions);
}

/*
 * constructor
 * @param appId - the application's id
 * @param permissions - the permissions that the application wants to request.
 * @param appSecret - the application's secret.
 */
FacebookManager::FacebookManager(const MAUtil::String &appId, const MAUtil::Set<MAUtil::String> &permissions)
{
	initialize(appId, permissions);
}

/*
 * initializes the FacebookManager: creates the Facebook object, and creates the ConnectionsManager, the FacebookObjectManager
 * and the FacebookPublisher2, and registers the FacebookManager to them.
 * @param id - the application id.
 * @param permissions - the permissions to request from Facebook.
 * @param appSecret - the appliction's secret.
 */
void FacebookManager::initialize(const MAUtil::String &appId, const MAUtil::Set<MAUtil::String> &permissions)
{
	mFacebook = new Facebook(appId, permissions);

	mObjectManager = new FacebookObjectManager(mFacebook);
	mConnectionsManager = new ConnectionsManager(mFacebook);
	mPublishingManager = new FacebookPublisher2(mFacebook);
}

/*
 * sets the access token retrieved from Facebook.
 */
void FacebookManager::setAccessToken(const MAUtil::String &token)
{
	mFacebook->setAccessToken(token);
}

/*
 * Returns the link for redirecting the user to the Facebook OAuth Dialog.
 */
const MAUtil::String &FacebookManager::getOAuthUrl() const
{
	return mFacebook->getOAuthUrl();
}

/*
 * Sets the ObjectRequestListener. The listener will be notified when a Facebook object
 * request is completed, or if the request has failed.
 */
void FacebookManager::setObjectRequestListener(ObjectRequestListener *listener)
{
	mObjectManager->setListener(listener);
}

///////////////////////////////////////////////////////////////////////////////////////
//							request connections
///////////////////////////////////////////////////////////////////////////////////////

/*
 * Requests a connection for an object.
 * @param connType - the requested connection
 * @param id - the id of the object for which the connection was requested.
 */
void FacebookManager::setConnectionRequestListener(ConnectionsManagerListener *listener)
{
	mConnectionsManager->setListener(listener);
}

/*
 * Requests a connection for an object.
 * @param connType - the requested connection
 * @param id - the id of the object for which the connection was requested.
 */
void FacebookManager::requestConnection(const MAUtil::String &connType, const MAUtil::String &id)
{
	mConnectionsManager->request(connType, id);
}

void FacebookManager::requestConnection(const MAUtil::String &connType, const MAUtil::Vector<MAUtil::String> &fields,
		const MAUtil::String &id )
{
	mConnectionsManager->request(connType, fields, id);
}

void FacebookManager::limitTheNumberOfObjectsToFetch(int maxNumberOfObjects)
{
	mConnectionsManager->limitTheNumberOfObjectsToFetch(maxNumberOfObjects);
}

void FacebookManager::fetchAllObjects()
{
	mConnectionsManager->fetchAllObjects();
}

///////////////////////////////////////////////////////////////////////////////////////
//							publish on Facebook
///////////////////////////////////////////////////////////////////////////////////////


/*
 * Sets the PublishingListener, that will be about the result of the publish request.
 */
void FacebookManager::setPublishingListener(PublishingListener *listener)
{
//	LOG("\t\t!!FacebookManager::setPublishingListener(%d)", listener);
	mPublishingManager->setListener(listener);
}

/*
 * Publishes a Like
 * @param id - the id of the object to be liked ( a post, message, comment ect).
 */
void FacebookManager::Like(const MAUtil::String &id)
{
	mPublishingManager->Like(id);
}

/*
 * Publishes a Unlike
 * @param id - the id of the object to be disliked ( a post, message, comment ect).
 */
void FacebookManager::Unlike(const MAUtil::String &id)
{
	mPublishingManager->Unlike(id);
}

/*
 * Publishes a comment
 * @param OBJECT_ID - the id of the object on which the comment is done.
 * @param message - the text of the comment.
 */
void FacebookManager::addComment(const MAUtil::String &OBJECT_ID, const MAUtil::String &message)
{
	mPublishingManager->addComment(OBJECT_ID, message);
}

/*
 * Adds a user to a group or a friend list.
 * @param OBJECT_ID - the id of the user.
 * @param userID - the id of the user to be added.
 */
void FacebookManager::addUser(const MAUtil::String &OBJECT_ID, const MAUtil::String &userID)
{
	mPublishingManager->addUser(OBJECT_ID, userID);
}

/*
 * Adds a FriendList.
 * @param OBJECT_ID - the id of the User for which the FriendList object is created.
 * @param name - the name of the new FriendList.
 */
void FacebookManager::addFriendList(const MAUtil::String &OBJECT_ID, const MAUtil::String &name)
{
	mPublishingManager->addFriendList(OBJECT_ID, name);
}

/*
 * Adds a EventResponse to an event.
 * @param EVENT_ID - the id of the Event to which a response is given.
 * @param response - the response to the event. Can be: attending, maybe, declined
 */
void FacebookManager::addEventResponse(const MAUtil::String &EVENT_ID, EVENT_RESPONSE_TYPE response)
{
	mPublishingManager->addEventResponse(EVENT_ID, response);
}

/*
 * Adds a Note.
 * @param OBJECT_ID - the id of the User for which the Note object is created.
 * @param subject - the subject of the Note.
 * @param message - the message of the Note.
 */
void FacebookManager::addNote(const MAUtil::String &OBJECT_ID, 	const MAUtil::String &subject,
			 const MAUtil::String &message)
{
	mPublishingManager->addNote(OBJECT_ID, subject, message);
}

/*
 * Adds a Album object.
 * @param OBJECT_ID - the id of the User for which the Album object is created.
 * @param name - the name of the Album.
 * @param message - the message of the Album.
 */
void FacebookManager::addAlbum(const MAUtil::String &PROFILE_ID, const MAUtil::String &name,
		const MAUtil::String &message)
{
	mPublishingManager->addAlbum(PROFILE_ID, name, message);
}

/*
 * Adds a Event object for a User.
 * @param PROFILE_ID - the id of the User to which the event is added.
 * @param eventName - the name of the event.
 * @param eventStartTime - the start time of the Event.
 * @param eventEndTime - the end time of the Event.
 * @param message - the message to be posted alog with the event
 * @param location - the name of the location
 */
void FacebookManager::addEvent(const MAUtil::String &PROFILE_ID,  const MAUtil::String &eventName,
			  const UnixTimeStamp &eventStart_time,  const UnixTimeStamp &eventEnd_time,
			  const MAUtil::String &message,         const MAUtil::String &location,
		      const MAUtil::String &privacyType)
{
	mPublishingManager->addEvent(PROFILE_ID, eventName, eventStart_time, eventEnd_time,
								 message, location, privacyType);
}

/*
 * Adds a Photo object to a User's wall or to an Album.
 * @param OBJECT_ID - the id of a User or an Album to which the Photo is added. If the the photo si added to a User, the picture
 * will appear to the User's wall.
 * @param pixels - the raw data of the image.
 * @param message - the message to be posted along with the Photo.
 */
void FacebookManager::addPhoto(const MAUtil::String &OBJECT_ID, const byte *pixels,
		int pixelsArraySize,
		const MAUtil::String &message)
{
	mPublishingManager->addPhoto(OBJECT_ID, pixels, pixelsArraySize, message);
}

/*
 * Adds a Video object to a User's wall..
 * @param PROFILE_ID - the id of the User for which the Video is posted.
 * @param videoSource - the raw data of the video.
 * @param videoDescpription - the video description, that will be posted along with the video.
 */
void FacebookManager::addVideo(const MAUtil::String &PROFILE_ID, int *videoSource, const MAUtil::String &videoTitle,
		 const MAUtil::String &videoDescription)
{
	mPublishingManager->addVideo(PROFILE_ID, videoSource, videoTitle, videoDescription);
}

/*
 * Adds a Checkin object for a User
 * @param PROFILE_ID - the id of the User for which the Checkin object is created.
 * @param placeId - the id of the place to which the checkin is made.
 * @param coord - the coordinates of the checkin place.
 * @param tags - list of tagged friends. String containing comma separated list of user id's.
 * @param message - the message that will be posted along with the Checkin
 */
void FacebookManager::addCheckin(const MAUtil::String &PROFILE_ID, const MAUtil::String &placeId, const Coordinate &coord,
				const MAUtil::String &tags,  const MAUtil::String &message)
{
	mPublishingManager->addCheckin(PROFILE_ID, placeId, coord, tags, message);
}

/*
 * Adds a Post object to a to a User wall.
 * @param message - the message to be posted
 * @param link - the link to be included in post.
 * @param pictureUrl - a picture url to be included in post.
 * @param actions - a Vector of Action objects (objects containing name and link) to be included in the post.
 * @param id - the id of the user on which wall the post will be displayed.
 */
void FacebookManager::addPostOnWall(const MAUtil::String &ID, const MAUtil::String &message, const MAUtil::String &link,
		const MAUtil::String &name, const MAUtil::String &caption, const MAUtil::String &description)
{
	mPublishingManager->addPostOnWall(ID, message, link, name, caption, description);
}

/*
 * Adds a Link object to a to a User wall.
 * @param link - the link to be posted.
 * @param message - the message to be posted with the link
 * @param id - the id of the user on which wall the link will be displayed.
 */
void FacebookManager::addLinkOnWall(const MAUtil::String &ID, const MAUtil::String &link, const MAUtil::String &message)
{
	mPublishingManager->addLinkOnWall(ID, link, message);
}

/*
 * Adds a StatusMessage object to a to a User wall.
 * @param message - the message to be posted
 * @param id - the id of the user on which wall the status message will be displayed.
 */
void FacebookManager::addStatusMessageOnWall(const MAUtil::String &ID, const MAUtil::String &message)
{
	mPublishingManager->addStatusMessageOnWall(ID, message);
}

///////////////////////////////////////////////////////////////////////////////////////
//							delete from Facebook
///////////////////////////////////////////////////////////////////////////////////////

/*
 * Removes a user from a friend list.
 * @param ID - the id of the group or friend list.
 * @param userId - the id of the user to be removed.
 */
void FacebookManager::removeUserFrom(const MAUtil::String &LIST_ID, const MAUtil::String &userId)
{
	mPublishingManager->removeUser(LIST_ID, userId);
}

/*
 * Removes a Comment
 * @param id - the id of the Comment
 */
void FacebookManager::removeComment(const MAUtil::String &id)
{
	mPublishingManager->removeObject(id);
}

/*
 * Removes a FriendList
 * @param id - the id of the FriendList
 */
void FacebookManager::removeFriendList(const MAUtil::String &id)
{
	mPublishingManager->removeObject(id);
}

/*
 * Removes a Note
 * @param id - the id of the Note
 */
void FacebookManager::removeNote(const MAUtil::String &id)
{
	mPublishingManager->removeObject(id);
}

/*
 * destructor
 */
FacebookManager::~FacebookManager()
{
	delete mObjectManager;
	delete mConnectionsManager;
	delete mPublishingManager;
	delete mFacebook;
}
