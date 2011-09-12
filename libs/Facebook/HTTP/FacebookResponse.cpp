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
 * FacebookResponse.cpp
 */

#include <MAUtil/PlaceholderPool.h>
#ifdef NEWLIB
#include <locale.h>
#endif
#include "FacebookResponse.h"

FacebookResponse::FacebookResponse(int code, int dataSize, const byte* data) : HttpResponse(code, dataSize, data) {}

YAJLDom::Value* FacebookResponse::getJsonData() const {
#ifdef NEWLIB
	// this must be set to make sure multi-byte conversions are correct.
	setlocale(LC_CTYPE, "en_US.UTF-8");
#endif
	YAJLDom::Value* value = NULL;

	if(getDataSize()>0)
	{
		value = YAJLDom::parse(getData(), getDataSize());
	}
//	if(!(value = YAJLDom::parse(getData(), getDataSize()))) {
//		maPanic(1, "Parsing failed!");
//	}

	return value;
}

MAHandle FacebookResponse::getImageData() const {
	MAHandle data = MAUtil::PlaceholderPool::alloc();
	maCreateData(data, getDataSize());
	maWriteData(data, getData(), 0, getDataSize());
	MAHandle image = MAUtil::PlaceholderPool::alloc();
	maCreateImageFromData(image, data, 0, getDataSize());
	MAUtil::PlaceholderPool::put(data);
	return image;
}
