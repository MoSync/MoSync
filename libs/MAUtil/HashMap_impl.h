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

/** \file HashMap_impl.h
* \brief HashMap implementation
*/

#ifdef MOSYNC_NATIVE
#include <string.h>
#endif

#ifndef _SE_MSAB_MAUTIL_HASHMAP_H_
#error Do not include this file directly.
#endif

//******************************************************************************
// HashMap
//******************************************************************************

template<class Key, class Value>
Value& MAUtil::HashMap<Key, Value>::operator[](const Key& key) {
	typename D::HashNode* node = (typename D::HashNode*)hash_lookup(&this->mHash, &key);
	if(node == NULL) {
		PairKV p(key, Value());
		node = new typename D::HashNode(p);
		hash_insert(&this->mHash, node, &node->data.first);
	}
	return node->data.second;
}
