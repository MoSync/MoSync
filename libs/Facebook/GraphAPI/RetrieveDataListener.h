/*
 * RetrieveDataListener.h
 *
 *  Created on: Jul 22, 2011
 *      Author: gabi
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
