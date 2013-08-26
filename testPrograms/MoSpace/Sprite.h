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

#ifndef _MOBRIS_SPRITE_H_
#define _MOBRIS_SPRITE_H_

#include "types.h"

typedef struct Sprite_t {
	Handle* images; /* pointer to the start of the image pointer array */
	Integer numImages; 
	Integer animSpeed;
} Sprite;


#define SPRITE_WIDTH(sPtr) EXTENT_X(maGetImageSize(sPtr->images[0]))
#define SPRITE_HEIGHT(sPtr) EXTENT_Y(maGetImageSize(sPtr->images[0]))





#endif
