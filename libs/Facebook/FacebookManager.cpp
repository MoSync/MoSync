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
#include "UnixTimeStamp.h"

#include "HTTP/Facebook.h"

#include "FacebookManager.h"

FacebookManager::FacebookManager(const MAUtil::String &appId, const MAUtil::String &appSecret)
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

	initialize(appId, permissions, appSecret);
}

FacebookManager::FacebookManager(const MAUtil::String &appId, const MAUtil::Set<MAUtil::String> &permissions, const MAUtil::String &appSecret)
{
	initialize(appId, permissions, appSecret);
}

void FacebookManager::initialize(const MAUtil::String &appId, const MAUtil::Set<MAUtil::String> &permissions, const MAUtil::String &appSecret)
{
	mFacebook = new Facebook(appId, permissions);
	if(appSecret.size()>0)
	{
		mFacebook->setAppSecret(appId);
	}
	mObjectManager = new FacebookObjectManager(mFacebook);
	mConnectionsManager = new ConnectionsManager(mFacebook);
	mPublishingManager = new FacebookPublisher2(mFacebook);
}

void FacebookManager::setAccessToken(const MAUtil::String &token)
{
	mFacebook->setAccessToken(token);
}

const MAUtil::String &FacebookManager::getOAuthUrl() const
{
	return mFacebook->getOAuthUrl();
}

void FacebookManager::setObjectRequestListener(ObjectRequestListener *listener)
{
	mObjectManager->setListener(listener);
}

///////////////////////////////////////////////////////////////////////////////////////
// request connections
///////////////////////////////////////////////////////////////////////////////////////

void FacebookManager::setConnectionRequestListener(ConnectionsManagerListener *listener)
{
	mConnectionsManager->setListener(listener);
}

void FacebookManager::requestConnection(const MAUtil::String &connType, const MAUtil::String &id)
{
	mConnectionsManager->request(connType, id);
}

void FacebookManager::requestConnection(const MAUtil::String &connType, const MAUtil::Vector<MAUtil::String> &fields,
	const MAUtil::String &id )
{
	mConnectionsManager->request(connType, fields, id);
}

///////////////////////////////////////////////////////////////////////////////////////
// publish on Facebook
///////////////////////////////////////////////////////////////////////////////////////

void FacebookManager::setPublishingListener(PublishingListener *listener)
{
//	LOG("\t\t!!FacebookManager::setPublishingListener(%d)", listener);
	mPublishingManager->setListener(listener);
}

void FacebookManager::Like(const MAUtil::String &id)
{
	mPublishingManager->Like(id);
}

void FacebookManager::Unlike(const MAUtil::String &id)
{
	mPublishingManager->Unlike(id);
}

void FacebookManager::addComment(const MAUtil::String &OBJECT_ID, const MAUtil::String &message)
{
	mPublishingManager->addComment(OBJECT_ID, message);
}

void FacebookManager::addUser(const MAUtil::String &OBJECT_ID, const MAUtil::String &userID)
{
	mPublishingManager->addUser(OBJECT_ID, userID);
}

void FacebookManager::addFriendList(const MAUtil::String &OBJECT_ID, const MAUtil::String &name)
{
	mPublishingManager->addFriendList(OBJECT_ID, name);
}

void FacebookManager::addEventResponse(const MAUtil::String &EVENT_ID, EVENT_RESPONSE_TYPE response)
{
	mPublishingManager->addEventResponse(EVENT_ID, response);
}

void FacebookManager::addNote(const MAUtil::String &OBJECT_ID, 	const MAUtil::String &subject,
	const MAUtil::String &message)
{
	mPublishingManager->addNote(OBJECT_ID, subject, message);
}

void FacebookManager::addAlbum(const MAUtil::String &PROFILE_ID, const MAUtil::String &name,
	const MAUtil::String &message)
{
	mPublishingManager->addAlbum(PROFILE_ID, name, message);
}

void FacebookManager::addEvent(const MAUtil::String &PROFILE_ID, const MAUtil::String &eventName,
	const UnixTimeStamp &eventStart_time, const UnixTimeStamp &eventEnd_time,
	const MAUtil::String &message, const MAUtil::String &location,
	const MAUtil::String &privacyType)
{
	mPublishingManager->addEvent(PROFILE_ID, eventName, eventStart_time, eventEnd_time,
		message, location, privacyType);
}

void FacebookManager::addPhoto(const MAUtil::String &OBJECT_ID, const int *pixels, int pixelsArraySize,
	const MAUtil::String &message)
{
	mPublishingManager->addPhoto(OBJECT_ID, pixels, pixelsArraySize, message);
}

void FacebookManager::addPhoto(const MAUtil::String &OBJECT_ID, const MAUtil::String &picture, const MAUtil::String &message)
{
	mPublishingManager->addPhoto(OBJECT_ID, picture, message);
}

#if 0	// not implemented
void FacebookManager::addVideo(const MAUtil::String &PROFILE_ID, int *videoSource, const MAUtil::String &videoTitle,
	const MAUtil::String &videoDescription)
{
	mPublishingManager->addVideo(PROFILE_ID, videoSource, videoTitle, videoDescription);
}
#endif

void FacebookManager::addCheckin(const MAUtil::String &PROFILE_ID, const MAUtil::String &placeId, const Coordinate &coord,
	const MAUtil::String &tags,  const MAUtil::String &message)
{
	mPublishingManager->addCheckin(PROFILE_ID, placeId, coord, tags, message);
}

void FacebookManager::addPostOnWall(const MAUtil::String &ID, const MAUtil::String &message,
	const MAUtil::String &link, const MAUtil::String &pictureUrl,
	const MAUtil::String &name, const MAUtil::String &caption, const MAUtil::String &description,
	MAUtil::Vector<Action> &actions /*const Privacy &privacy,*/)
{
	mPublishingManager->addPostOnWall(ID, message, link, pictureUrl, name, caption, description, actions);
}

void FacebookManager::addLinkOnWall(const MAUtil::String &ID, const MAUtil::String &link, const MAUtil::String &message)
{
	mPublishingManager->addLinkOnWall(ID, link, message);
}

void FacebookManager::addStatusMessageOnWall(const MAUtil::String &ID, const MAUtil::String &message)
{
	mPublishingManager->addStatusMessageOnWall(ID, message);
}

///////////////////////////////////////////////////////////////////////////////////////
// delete from Facebook
///////////////////////////////////////////////////////////////////////////////////////

void FacebookManager::removeUserFrom(const MAUtil::String &LIST_ID, const MAUtil::String &userId)
{
	mPublishingManager->removeUser(LIST_ID, userId);
}

void FacebookManager::removeComment(const MAUtil::String &id)
{
	mPublishingManager->removeObject(id);
}

void FacebookManager::removeFriendList(const MAUtil::String &id)
{
	mPublishingManager->removeObject(id);
}

void FacebookManager::removeNote(const MAUtil::String &id)
{
	mPublishingManager->removeObject(id);
}

FacebookManager::~FacebookManager()
{
	delete mObjectManager;
	delete mConnectionsManager;
	delete mPublishingManager;
	delete mFacebook;
}
