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

/** \file HashMap.h
* \brief Thin template HashMap.
*/

#ifndef _SE_MSAB_MAUTIL_HASHMAP_H_
#define _SE_MSAB_MAUTIL_HASHMAP_H_

#include "HashDict.h"

namespace MAUtil {

/** \brief Thin template HashMap.
* \see HashDict
*/
template<class Key, class Value>
class HashMap : public HashDict<const Key, Pair<const Key, Value> > {
public:
	typedef Pair<const Key, Value> PairKV;
	typedef Pair<Key, Value> MutableStorage;
protected:
	typedef HashDict<const Key, PairKV> D;
public:
	HashMap(typename D::HashFunction hf = &THashFunction<Key>,
		typename D::CompareFunction cf = &Compare<Key>,
		int init_bits = 6)
		: D(OFFSETOF(PairKV, first), hf, cf, init_bits) {}

	Pair<typename D::Iterator, bool> insert(const Key& k, const Value& v) {
		PairKV p(k, v);
		return D::insert(p);
	}

	Pair<typename D::Iterator, bool> insert(const PairKV& pkv) {
		return D::insert(pkv);
	}

	/**
	* The square bracket operator returns a reference to the Value
	* corresponding to the specified Key.
	*
	* If the Key doesn't yet exist in the HashMap, it will be inserted
	* with a default Value.
	*
	* There is no const variant of this function,
	* because in order to return a valid reference,
	* the HashMap may have to be modified by inserting a new key.
	* Use find() if you have a const HashMap.
	*/
	Value& operator[](const Key&);
};

}	//MAUtil

#include "HashMap_impl.h"

#endif	//_SE_MSAB_MAUTIL_HASHMAP_H_
