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
 * HttpResponse.h
 */

#ifndef HTTPRESPONSE_H_
#define HTTPRESPONSE_H_

#include <maapi.h>

class HttpResponse {
public:
	HttpResponse(int code);
	HttpResponse(int code, int dataSize, const byte* data);

	virtual int getCode() const;
	virtual int getDataSize() const;
	virtual const byte* getData() const;

protected:
	int mCode;
	int mDataSize;
	const byte* mData;
};

#endif /* HTTPRESPONSE_H_ */
