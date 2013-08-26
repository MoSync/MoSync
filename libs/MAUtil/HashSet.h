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

/** \file HashSet.h
* \brief Thin template HashSet.
*/

#ifndef _SE_MSAB_MAUTIL_HASHSET_H_
#define _SE_MSAB_MAUTIL_HASHSET_H_

#include "HashDict.h"

namespace MAUtil {

/** \brief Thin template HashSet.
* \see HashDict
*/
template<class Key>
class HashSet : public HashDict<const Key, const Key> {
public:
	typedef Key MutableStorage;
protected:
	typedef HashDict<const Key, const Key> D;
public:
	HashSet(typename D::HashFunction hf = &THashFunction<Key>,
		typename D::CompareFunction cf = &Compare<Key>,
		int init_bits = 6)
		: D(0, hf, cf, init_bits) {}

	Pair<typename D::Iterator, bool> insert(const Key& k) {
		return D::insert(k);
	}
};

}	//MAUtil

#endif	//_SE_MSAB_MAUTIL_HASHSET_H_
