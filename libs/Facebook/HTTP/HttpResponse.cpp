/*
 * HttpResponse.cpp
 *
 *  Created on: Jul 20, 2011
 *      Author: gabi
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
