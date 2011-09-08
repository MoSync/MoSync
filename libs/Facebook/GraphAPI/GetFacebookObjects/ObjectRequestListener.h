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
 * ObjectRequestListener.h
 */

#ifndef OBJECTREQUESTLISTENER_H_
#define OBJECTREQUESTLISTENER_H_

#include "../ErrorListener.h"

class Album;
class Checkin;
class Comment;
class Event;
class FriendList;
class Group;
class Link;
class Note;
class Photo;
class Page;
class Post;
class StausMessage;
class User;
class Video;

/*
 * Listener for the retrieving Facebook objects. No access_token needed.
 * See:http://developers.facebook.com/docs/reference/api/
 */
struct ObjectRequestListener: public ErrorListener
{
	/*
	 * called when an Album object was received
	 */
	virtual void facebookObjectReceived(const Album &object){};
	/*
	 * called when an Checkin object was received
	 */
	virtual void facebookObjectReceived(const Checkin &object){};
	/*
	 * called when an Comment object was received
	 */
	virtual void facebookObjectReceived(const Comment &object){};
	/*
	 * called when an Event object was received
	 */
	virtual void facebookObjectReceived(const Event &object){};
	/*
	 * called when an FriendList object was received
	 */
	virtual void facebookObjectReceived(const FriendList &object){};
	/*
	 * called when an Group object was received
	 */
	virtual void facebookObjectReceived(const Group &object){};
	/*
	 * called when an Link object was received
	 */
	virtual void facebookObjectReceived(const Link &object){};
	/*
	 * called when an Note object was received
	 */
	virtual void facebookObjectReceived(const Note &object){};
	/*
	 * called when an Photo object was received
	 */
	virtual void facebookObjectReceived(const Photo &object){};
	/*
	 * called when an Page object was received
	 */
	virtual void facebookObjectReceived(const Page &object){};
	/*
	 * called when an Post object was received
	 */
	virtual void facebookObjectReceived(const Post &object){};
	/*
	 * called when an StausMessage object was received
	 */
	virtual void facebookObjectReceived(const StausMessage &object){};
	/*
	 * called when an User object was received
	 */
	virtual void facebookObjectReceived(const User &object){};
	/*
	 * called when an Video object was received
	 */
	virtual void facebookObjectReceived(const Video &object){};
	virtual ~ObjectRequestListener() {}
};

#endif /* OBJECTREQUESTLISTENER_H_ */
