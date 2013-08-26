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
 * @file RetrieveDataListener.h
 * @author Gabriela Rata
 */

#ifndef RETRIEVEDATALISTENER_H_
#define RETRIEVEDATALISTENER_H_

#include <maapi.h>
#include <MAUtil/String.h>
#include <yajl/YAJLDom.h>
#include "ErrorListener.h"

class FacebookRequest;
class MAUtil::YAJLDom::Value;

/**
 * \brief Listener for the retrieving data from the server.
 * All responses are JSON objects.
 */
class RetrieveDataListener: public ErrorListener
{
public:
	/**
	 * This function is called when the requested data was not a image or
	 * a video.
	 */
	virtual void jsonDataReceived(MAUtil::YAJLDom::Value* result,
			const MAUtil::String &connType, const MAUtil::String &objectId) {};

	/**
	 * This function is called when the requested data was a image. The data
	 * retrieved from the server is transformed into a image handle.
	 */
	virtual void imageReceived(MAHandle image, const MAUtil::String &connType,
			const MAUtil::String &objectId) {};

	/**
	 * This function is called when the requested data was a image. The data
	 * retrieved from the server is transformed into a video handle.
	 */
	virtual void videoReceived(MAHandle video, const MAUtil::String &connType,
			const MAUtil::String &objectId) {};

	/**
	 * destructor
	 */
	virtual ~RetrieveDataListener(){}
};

#endif /* RETRIEVEDATALISTENER_H_ */
