/*
 * FacebookResponse.h
 *
 *  Created on: Jul 20, 2011
 *      Author: gabi
 */

#ifndef FACEBOOKRESPONSE_H_
#define FACEBOOKRESPONSE_H_

#include <maapi.h>
#include "../JSON_lib/YAJLDom.h"
#include "HttpResponse.h"

using namespace MAUtil;

class FacebookResponse : public HttpResponse {
public:
public:
	FacebookResponse(int code, int dataSize, const byte* data);
	virtual YAJLDom::Value* getJsonData() const;
	virtual MAHandle getImageData() const;
};

#endif /* FACEBOOKRESPONSE_H_ */
