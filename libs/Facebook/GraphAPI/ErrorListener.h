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
 * ErrorListener.h
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
