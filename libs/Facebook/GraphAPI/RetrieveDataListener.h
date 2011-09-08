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
 * RetrieveDataListener.h
 */

#ifndef RETRIEVEDATALISTENER_H_
#define RETRIEVEDATALISTENER_H_

#include <maapi.h>
#include <MAUtil/String.h>
#include "../JSON_lib/YAJLDom.h"
#include "ErrorListener.h"

class FacebookRequest;
class MAUtil::YAJLDom::Value;

/*
 * Listener for the retrieving data from the server. All responses are JSON objects
 */
class RetrieveDataListener: public ErrorListener
{
public:
	/*
	 * This function is called when the requested data was not a image or a video.
	 */
	virtual void jsonDataReceived(MAUtil::YAJLDom::Value* result, const MAUtil::String &connType, const MAUtil::String &objectId) {};

	/*
	 * This function is called when the requested data was a image. The data retrieved
	 * from the server is transformed into a image handle.
	 */
	virtual void imageReceived(MAHandle image, const MAUtil::String &connType, const MAUtil::String &objectId) {};

	/*
	 * This function is called when the requested data was a image. The data retrieved
	 * from the server is transformed into a video handle.
	 */
	virtual void videoReceived(MAHandle video, const MAUtil::String &connType, const MAUtil::String &objectId) {};

	virtual ~RetrieveDataListener(){}
};

#endif /* RETRIEVEDATALISTENER_H_ */
