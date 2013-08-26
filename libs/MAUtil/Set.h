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

/** \file Set.h
* \brief Thin template sorted Set.
*/

#ifndef _SE_MSAB_MAUTIL_SET_H_
#define _SE_MSAB_MAUTIL_SET_H_

#include "Dictionary.h"

namespace MAUtil {

/** \brief Thin template sorted Set.
* \see Dictionary
*/
template<class Key>
class Set : public Dictionary<const Key, const Key> {
public:
	typedef Dictionary<const Key, const Key> D;
	typedef Key MutableStorage;

	Set(int (*cf)(const Key&, const Key&) = &Compare<const Key>) : D::Dictionary(cf, 0) {
	}
	Pair<typename D::Iterator, bool> insert(const Key& key) { return D::insert(key); }
};

}	//MAUtil

#endif	//_SE_MSAB_MAUTIL_SET_H_
