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

#ifndef _RESOURCE_DEFS_H_
#define _RESOURCE_DEFS_H_

#include <SDL/SDL.h>
#include "Stream.h"

#define TYPES(m)\
	m(RT_BINARY, Base::Stream, delete)\
	m(RT_PLACEHOLDER, void, NULA)\
	m(RT_LABEL, Label, delete) \
	m(RT_IMAGE, SDL_Surface, SDL_FreeSurface)\
	m(RT_FLUX, void, NULA)\

#endif // _RESOURCE_DEFS_H_
