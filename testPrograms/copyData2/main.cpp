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

#include <conprint.h>

/* Attempt to copy 0xff from data[1] to dest[0] */
void CopyWithOffset(){
	unsigned char data[2] = { 0x7f, 0xff };
	unsigned char dest[2] = { 0, 0 };

	MAHandle placeholder = maCreatePlaceholder();
	maCreateData(placeholder, 2);
	maWriteData(placeholder, data, 0, 2);

	MAHandle dstPlaceholder = maCreatePlaceholder();
	maCreateData(dstPlaceholder, 1);

	MACopyData params = { dstPlaceholder, 0, placeholder, 1, 1};
	maCopyData(&params);

	maReadData(dstPlaceholder, dest, 0, 1);

	maDestroyObject(placeholder);
	maDestroyObject(dstPlaceholder);

	printf("dest[0] should be 255, is %i.", dest[0]);
}

extern "C" int MAMain() {
	CopyWithOffset();
	return 0;
}
