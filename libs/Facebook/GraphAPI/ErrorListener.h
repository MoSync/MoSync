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
 * @file ErrorListener.h
 * @author Gabriela Rata
 */

#ifndef ERROR_LISTENER_H_
#define ERROR_LISTENER_H_

#include <MAUtil/String.h>
class FacebookRequest;

/**
 * \brief Listener class for failed requests.
 */
class ErrorListener
{
public:
	/**
	 * Function called when a request fails.
	 * @param code - HTTP error code received from the server.
	 * @param path -
	 * 			1. In case of a publish request: contains the id of the object on which the publish request was
	 * 				made and the request name.  e.g: id/likes
	 * 			2. In case of a connection request: contains the id of the object for which the connection was
	 * 				requested and the connection name. e.g: id/friends
	 * 			3. In case of a Facebook object request: contains the id of the object the was requested from Facebook.
	 */

	virtual void queryError(int code, const MAUtil::String &path) {}
	/**
	 * destructor
	 */
	virtual ~ErrorListener(){}
};

#endif /* ERROR_LISTENER_H_ */
