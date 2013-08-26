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

/** \file Map.h
* \brief Thin template sorted Map.
*/

#ifndef _SE_MSAB_MAUTIL_MAP_H_
#define _SE_MSAB_MAUTIL_MAP_H_

#include "Dictionary.h"

namespace MAUtil {

/** \brief Thin template sorted Map.
* \see Dictionary
*/
template<class Key, class Value>
class Map : public Dictionary<const Key, Pair<const Key, Value> > {
public:
	typedef Pair<const Key, Value> PairKV;
	typedef Pair<Key, Value> MutableStorage;
protected:
	typedef Dictionary<const Key, PairKV> D;
	typedef typename D::DictNode DN;
public:

	Map(int (*cf)(const Key&, const Key&) = &Compare<const Key>)
		: D::Dictionary(cf, OFFSETOF(PairKV, first)) {}
	Pair<typename D::Iterator, bool> insert(const Key& key, const Value& value) {
		PairKV pkv(key, value);
		return D::insert(pkv);
	}
	Pair<typename D::Iterator, bool> insert(const MutableStorage& pkv) {
		return D::insert(pkv);
	}
	Value& operator[](const Key& key) {
		DN* node = (DN*)dict_lookup(&this->mDict, &key);
		if(node == NULL) {
			PairKV p(key, Value());
			node = new DN(p);
			dict_insert(&this->mDict, node, &(node->data.first));
		}
		return node->data.second;
	}
};

}

#endif
