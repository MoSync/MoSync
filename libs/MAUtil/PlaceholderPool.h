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

/** \file PlaceholderPool.h
* \brief Dynamic pool of MoSync placeholder resource handles.
*/

#ifndef _SE_MSAB_MAUTIL_PLACEHOLDERPOOL_H_
#define _SE_MSAB_MAUTIL_PLACEHOLDERPOOL_H_

#include <ma.h>

namespace MAUtil {

/**
* \brief A dynamic pool of MoSync placeholder resource handles.
*
* Such handles are used to construct new MoSync resources,
* such as images, sound clips or data objects.
*
* \deprecated Use maCreatePlacholder() and maDestroyPlaceholder() instead.
*/
namespace PlaceholderPool {
	/**
	* Retrieves and removes a MAHandle from the pool, or,
	* if the pool is empty, allocates a new MAHandle.
	*/
	MAHandle alloc() GCCATTRIB(deprecated);

	/**
	* Adds a MAHandle to the pool. If the MAHandle refers to a valid MoSync Object,
	* that object is destroyed.
	*/
	void put(MAHandle h) GCCATTRIB(deprecated);
}

}

#endif	//_SE_MSAB_MAUTIL_PLACEHOLDERPOOL_H_
