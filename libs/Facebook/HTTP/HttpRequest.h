/*
 * HttpRequest.h
 *
 *  Created on: Jul 20, 2011
 *      Author: gabi
 */

#ifndef HTTPREQUEST_H_
#define HTTPREQUEST_H_

#include <MAUtil/String.h>
#include <MAUtil/Map.h>

using namespace MAUtil;

class HttpRequest {
public:
	HttpRequest(const String& url, int method);

	int getMethod() const;
	const String& getUrl() const;

	void setMessageBodyParams(const Map<String, String>& params);
	void setRequestHeaders(const Map<String, String>& params);

	const Map<String, String>& getMessageBodyParams() const;
	const Map<String, String>& getRequestHeaders() const;

	int getDataSize() const;
	const byte* getData() const;

	void setPostdata(const MAUtil::String &postdata);

	virtual ~HttpRequest();

protected:
	String 				mUrl;

	Map<String, String> mMessageBodyParams;
	Map<String, String> mRequestHeaders;

	int 				mMethod;
	const byte* 		mPostdata;
	int 				mDataSize;


};

#endif /* HTTPREQUEST_H_ */
