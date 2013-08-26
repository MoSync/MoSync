/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/**
 * @file ObjectRequestListener.h
 * @author Gabriela Rata
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

/**
 * \brief Listener for the retrieving Facebook objects. No access_token needed.
 * See:http://developers.facebook.com/docs/reference/api/
 */

struct ObjectRequestListener: public ErrorListener
{
	/**
	 * called when an Album object was received
	 */
	virtual void facebookObjectReceived(const Album &object){};

	/**
	 * called when an Checkin object was received
	 */
	virtual void facebookObjectReceived(const Checkin &object){};

	/**
	 * called when an Comment object was received
	 */
	virtual void facebookObjectReceived(const Comment &object){};

	/**
	 * called when an Event object was received
	 */
	virtual void facebookObjectReceived(const Event &object){};

	/**
	 * called when an FriendList object was received
	 */

	virtual void facebookObjectReceived(const FriendList &object){};

	/**
	 * called when an Group object was received
	 */
	virtual void facebookObjectReceived(const Group &object){};

	/**
	 * called when an Link object was received
	 */
	virtual void facebookObjectReceived(const Link &object){};

	/**
	 * called when an Note object was received
	 */
	virtual void facebookObjectReceived(const Note &object){};

	/**
	 * called when an Photo object was received
	 */
	virtual void facebookObjectReceived(const Photo &object){};

	/**
	 * called when an Page object was received
	 */
	virtual void facebookObjectReceived(const Page &object){};

	/**
	 * called when an Post object was received
	 */
	virtual void facebookObjectReceived(const Post &object){};

	/**
	 * called when an StausMessage object was received
	 */
	virtual void facebookObjectReceived(const StausMessage &object){};

	/**
	 * called when an User object was received
	 */

	virtual void facebookObjectReceived(const User &object){};

	/**
	 * called when an Video object was received
	 */
	virtual void facebookObjectReceived(const Video &object){};

	/**
	 * destructor
	 */
	virtual ~ObjectRequestListener() {}
};

#endif /* OBJECTREQUESTLISTENER_H_ */
