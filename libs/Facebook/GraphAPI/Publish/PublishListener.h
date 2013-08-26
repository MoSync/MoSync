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
