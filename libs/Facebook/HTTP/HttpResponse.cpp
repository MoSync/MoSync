/* Copyright (C) 2011 MoSync AB

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
