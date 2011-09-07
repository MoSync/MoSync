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
