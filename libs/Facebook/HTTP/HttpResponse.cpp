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
 * HttpResponse.cpp
 */

#include "HttpResponse.h"

HttpResponse::HttpResponse(int code) :
	mCode(code), mDataSize(0), mData(0)
{
}

HttpResponse::HttpResponse(int code, int dataSize, const byte* data) :
	mCode(code), mDataSize(dataSize), mData(data)
{
}

int HttpResponse::getCode() const {
	return mCode;
}

int HttpResponse::getDataSize() const {
	return mDataSize;
}

const byte* HttpResponse::getData() const {
	return mData;
}
