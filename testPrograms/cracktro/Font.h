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

#ifndef _SE_MSAB_MOSYNC_FONT_H_
#define _SE_MSAB_MOSYNC_FONT_H_

#include <maapi.h>

typedef struct MAFont_t {   //  Size   | Acc. size
	int charWidth;			//     4   |     4
	int charHeight;			//     4   |     8
	int charsetWidth;		//     4   |    12
	int charsetHeight;		//     4   |    16
	int asciiStart;			//     4   |    20
	int charsPerLine;		//	   4   |    24
	MAHandle handle;			//     4   |    28
} MAFont;

MAHandle maInitFont(MAHandle resource, MAExtent charSize, MAExtent charsetSize, int asciiStart);
void maDrawChar(MAHandle font, char c, int x, int y);
void maDrawString(MAHandle font, const char* str, int x, int y);
MAExtent maGetStringExtents(MAHandle font, const char *str);


#endif
