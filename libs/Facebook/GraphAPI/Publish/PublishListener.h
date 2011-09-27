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

/**
 * @file PublishListener.h
 * @author Gabriela Rata
 */

#ifndef PUBLISHLISTENER_H_
#define PUBLISHLISTENER_H_

#include <maapi.h>
#include <MAUtil/String.h>
#include "../ErrorListener.h"

class MAUtil::YAJLDom::Value;


/**
 * \brief Listener for the FacebookPublisher2 class
 * If you register a PublishingListener to a FacebookPublisher2 object
 * by calling FacebookPublisher2::setListener, the listener will be notified
 * when the response
 * of a publish request is retrieved.
 * If the request was successful one of the two overloads of the
 * publishingResponseReceived is called.
 * If the request failed, then the ErrorListener::queryErrorn function is
 * called.
 */
class PublishingListener: public ErrorListener
{
public:
	/**
	 * Called when the request was successful.
	 * @param success - the response from server, telling if the request was
	 * done.
	 * This function is called when a remove or an unlike request was made.
	 */
	virtual void publishingResponseReceived(bool success,
			const MAUtil::String &path) {};

	/**
	 * Called when the request was successful.
	 * @param newId - the id of the new object.
	 * @param path - contains the id of the object on which the publish request
	 * was made and the request name.
	 * 				  e.g: id/feed, id/likes
	 * This function is called when a new object is created (Album, Like,
	 * Comment, StatusMessage ect).
	 */
	virtual void publishingResponseReceived(const MAUtil::String  &data,
			const MAUtil::String &path){}
};


#endif /* PUBLISHLISTENER_H_ */
