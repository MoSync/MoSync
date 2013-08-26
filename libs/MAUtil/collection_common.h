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

/** \file collection_common.h
 * \brief Compare() and Pair for MAUtil.
 */

#ifndef _SE_MSAB_MAUTIL_COLLECTION_COMMON_H_
#define _SE_MSAB_MAUTIL_COLLECTION_COMMON_H_

#ifndef NULL
#define NULL 0
#endif

#ifndef OFFSETOF
#define OFFSETOF(struct, member) ((int)(((char*)&(((struct*)1)->member)) - 1))
#endif

/**
* \brief MoSync utility libraries
*/
namespace MAUtil {

//******************************************************************************
// Comparator
//******************************************************************************

/** \brief Template comparator function.
* Returns -1 if \a a \< \a b,
* 0 if \a a == \a b
* and 1 if \a a \> \a b.
*/
template<class T> int Compare(const T& a, const T& b) {
	if(a < b)
		return -1;
	else if(a == b)
		return 0;
	else
		return 1;
}

//******************************************************************************
// Pair
//******************************************************************************

/// Template storage for a pair of objects.
template<class F, class S> struct Pair {
	F first;
	S second;

	Pair() {}
	Pair(const F& f, const S& s) : first(f), second(s) {}

	template<class OF, class OS>
	Pair(const Pair<OF, OS>& o) : first(o.first), second(o.second) {}
};

}	//namespace MAUtil

#endif	//_SE_MSAB_MAUTIL_COLLECTION_COMMON_H_
