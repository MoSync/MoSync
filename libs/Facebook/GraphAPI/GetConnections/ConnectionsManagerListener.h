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
 * FacebookConnectionListener.h
 */

#ifndef FACEBOOKCONNECTIONLISTENER_H_
#define FACEBOOKCONNECTIONLISTENER_H_

class Photo;
class User;
class Video;
class Link;
class Group;
class Post;
class Album;
class Note;
class Event;
class Comment;
class Link;
class Checkin;
class StatusMessage;
#include "../GetFacebookObjects/FacebookObjects/Photo.h"
#include "../GetFacebookObjects/FacebookObjects/User.h"
#include "../GetFacebookObjects/FacebookObjects/Video.h"
#include "../GetFacebookObjects/FacebookObjects/Link.h"
#include "../GetFacebookObjects/FacebookObjects/Group.h"
#include "../GetFacebookObjects/FacebookObjects/Post.h"
#include "../GetFacebookObjects/FacebookObjects/Album.h"
#include "../GetFacebookObjects/FacebookObjects/Note.h"
#include "../GetFacebookObjects/FacebookObjects/Event.h"
#include "../GetFacebookObjects/FacebookObjects/Comment.h"
#include "../GetFacebookObjects/FacebookObjects/Link.h"
#include "../GetFacebookObjects/FacebookObjects/Checkin.h"
#include "../GetFacebookObjects/FacebookObjects/StatusMessage.h"

#include "../GetFacebookObjects/FacebookObjects/Utilities.h"
#include "../../HTTP/Facebook.h"

/*
 * Listener for the ConnectionsManager class
 * If you register a ConnectionsManagerListener to a ConnectionsManager object
 * by calling ConnectionsManager::setListener, the listener will be notified when the response
 * of a connection request is retrieved. If the request was successful a vector with the
 * requested objects will be send to the listener. For example if the "feed" connection was
 * requested, a vector of Post objects will be send to the listener.
 */
class ConnectionsManagerListener
{
public:
	/*
	 *  This function is called when a "photos" connection was requested.
	 *  @param photos - the Photos objects retrieved from the server
	 */
	virtual void received(const MAUtil::Vector<Photo> &photos, const MAUtil::String &connType, const MAUtil::String &objectId) {}

	/*
	 *  This function is called when a "albums" connection was requested.
	 */
	virtual void received(const MAUtil::Vector<Album> &albums, const MAUtil::String &connType, const MAUtil::String &objectId) {}

	/*
	 *  This function is called when the "friends", "members" or "tags"  connection was requested.
	 *  @param connType - the connectionType. It can have the following values: "friends", "members" or "tags"
	 */
	virtual void received(const MAUtil::Vector<IdNamePair> &friends, const MAUtil::String &connType, const MAUtil::String &objectId) {}

	/*
	 *  This function is called when the "noreply", "maybe", "attending" or "declined"  connection was requested.
	 */
	virtual void received(const MAUtil::Vector<EventResponse> &responses, const MAUtil::String &connType, const MAUtil::String &objectId) {}

	/*
	 *  This function is called when the "groups"  connection was requested.
	 */
	virtual void received(const MAUtil::Vector<Group> &groups, const MAUtil::String &connType, const MAUtil::String &objectId) {}

	/*
	 *  This function is called when the "feed", "home" or "posts" connection was requested.
	 *  @param connType - the connectionType. It can have the following values: "feed", "home" or "posts".
	 */
	virtual void received(const MAUtil::Vector<Post> &posts, const MAUtil::String &connType, const MAUtil::String &objectId) {}

	/*
	 *  This function is called when the "video" or "videos/uploaded" connection was requested.
	 *  @param connType - the connectionType. It can have the following values: "video" or "videos/uploaded".
	 */
	virtual void received(const MAUtil::Vector<Video> &video, const MAUtil::String &connType, const MAUtil::String &objectId) {}

	/*
	 *  This function is called when the "likes", "movies", "music", "books", "accounts", "activity"
	 *  "friendlists", "interests" and "television" connection was requested.
	 *  @param connType - the connectionType. It can have the following values: "likes", "movies", "music",
	 *  "books", "accounts", "activity", "friendlists", "interests" and "television"
	 */
	virtual void received(const MAUtil::Vector<CategoryData> &likes, const MAUtil::String &connType, const MAUtil::String &objectId) {}

	/*
	 *  This function is called when the "notes" connection was requested.
	 */
	virtual void received(const MAUtil::Vector<Note> &notes, const MAUtil::String &connType, const MAUtil::String &objectId) {}

	/*
	 *  This function is called when the "events" connection was requested.
	 */
	virtual void received(const MAUtil::Vector<Event> &events, const MAUtil::String &connType, const MAUtil::String &objectId) {}

	/*
	 *  This function is called when the "comments" connection was requested.
	 */
	virtual void received(const MAUtil::Vector<Comment> &comments, const MAUtil::String &connType, const MAUtil::String &objectId) {}

	/*
	 *  This function is called when the "checkins" connection was requested.
	 */
	virtual void received(const MAUtil::Vector<Checkin> &comments, const MAUtil::String &connType, const MAUtil::String &objectId) {}

	/*
	 *  This function is called when the "links" connection was requested.
	 */
	virtual void received(const MAUtil::Vector<Link> &links, const MAUtil::String &connType, const MAUtil::String &objectId) {}

	/*
	 *  This function is called when the "statuses" connection was requested.
	 */
	virtual void received(const MAUtil::Vector<StatusMessage> &messages, const MAUtil::String &connType, const MAUtil::String &objectId) {}

	/*
	 *  This function is called when the "picture" connection was requested.
	 */
	virtual void received(const MAHandle pictureHandle, const MAUtil::String &connType, const MAUtil::String &objectId) {}

	/*
	 *  This function is called when the the request failed.
	 *  @param code - the HTTP error code received from the server.
	 *  @param connType - the requested connection
	 *  @param id - the id of the object for which the connection was requested.
	 */
	virtual void errorReceivingConnection(int code, const MAUtil::String &connType, const MAUtil::String &id) = 0;
};


#endif /* FACEBOOKCONNECTIONLISTENER_H_ */
