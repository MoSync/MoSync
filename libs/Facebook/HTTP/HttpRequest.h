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

/*
 * HttpRequest.h
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
	String				mUrl;

	Map<String, String> mMessageBodyParams;
	Map<String, String> mRequestHeaders;

	int					mMethod;
	const byte* 		mPostdata;
	int					mDataSize;


};

#endif /* HTTPREQUEST_H_ */
