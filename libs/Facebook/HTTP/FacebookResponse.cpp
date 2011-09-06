/*
 * FacebookResponse.cpp
 *
 *  Created on: Jul 20, 2011
 *      Author: gabi
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
