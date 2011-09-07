/*
 * ErrorListener.h
 *
 *  Created on: Jul 20, 2011
 *      Author: gabi
 */

#ifndef ERROR_LISTENER_H_
#define ERROR_LISTENER_H_

#include <MAUtil/String.h>
class FacebookRequest;

class ErrorListener
{
public:
	/*
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
	/*
	 * destructor
	 */
	virtual ~ErrorListener(){}
};





#endif /* ERROR_LISTENER_H_ */
