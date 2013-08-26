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
 * FacebookResponse.cpp
 */

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
	MAHandle data = maCreatePlaceholder();
	maCreateData(data, getDataSize());
	maWriteData(data, getData(), 0, getDataSize());
	MAHandle image = maCreatePlaceholder();
	maCreateImageFromData(image, data, 0, getDataSize());
	maDestroyPlaceholder(data);
	return image;
}
