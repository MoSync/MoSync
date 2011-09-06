/*
 * HttpResponse.h
 *
 *  Created on: Jul 20, 2011
 *      Author: gabi
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
