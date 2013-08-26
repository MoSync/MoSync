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

#include "base/Stream.h"
#include "Image.h"

// On Android images are stored on the Java side.
// We store a dummy int array on the C-side.
// See code in file runtimes\cpp\base\Syscall.cpp,
// in function Syscall::loadResources, switch case
// RT_IMAGE.

#define TYPES(m)\
	m(RT_BINARY, Base::Stream, delete)\
	m(RT_PLACEHOLDER, void, NUL)\
	m(RT_LABEL, Label, delete) \
	m(RT_IMAGE, int, delete)\
	m(RT_FLUX, void, NUL)\

#endif // _RESOURCE_DEFS_H_
