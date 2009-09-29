/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

#ifndef HELPERS_H
#define HELPERS_H

#include <maassert.h>

#ifdef __cplusplus

#include <MAUtil/String.h>
#include <MAUtil/Vector.h>

using namespace MAUtil;

class DataHandler {
public:
	DataHandler(MAHandle data) : size(maGetDataSize(data)), mData(data), mPos(0) {}
	bool read(void* dst, int len) {
		if(mPos + len > size)
			return false;
		if(len == 0)
			return true;
		maReadData(mData, dst, mPos, len);
		mPos += len;
		return true;
	}
	void write(const void* src, int len) {
		if(len == 0)
			return;
		maWriteData(mData, src, mPos, len);
		mPos += len;
	}
	const int size;
private:
	const MAHandle mData;
	int mPos;
};

#endif	//__cplusplus

#define TEST(func) if(!(func)) return false;

#define WHITE 0xffffff
#define BLACK 0
#define SKY_BLUE 0x60C0FF

#endif	//HELPERS_H
