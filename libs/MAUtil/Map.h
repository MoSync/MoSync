/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

/** \file Map.h
* \brief Thin template sorted Map.
*/

#ifndef _SE_MSAB_MAUTIL_MAP_H_
#define _SE_MSAB_MAUTIL_MAP_H_

#include "Dictionary.h"

#ifndef OFFSETOF
#define OFFSETOF(struct, member) ((int)(((char*)&(((struct*)1)->member)) - 1))
#endif

namespace MAUtil {

template<class Key, class Value>
class Map : public Dictionary<Key, Pair<Key, Value> > {
public:
	typedef Pair<Key, Value> PairKV;
protected:
	typedef Dictionary<Key, PairKV> D;
	typedef typename D::DictNode DN;
public:

	Map(typename D::CompareFunction cf = &Compare<Key>)
		: D::Dictionary(cf, OFFSETOF(PairKV, first)) {}

	/**
	* Inserts a new element into the Map.
	*
	* Returns a Pair. The Pair's second element is true if the element was inserted,
	* or false if the element already existed in the map.
	* The Pair's first element is an Iterator that points to the element in the Map.
	*
	* An element which has the same key as the new one may already be present in the Map;
	* in that case, this operation does nothing, and the Iterator returned will point to
	* the old element.
	*/
	Pair<typename D::Iterator, bool> insert(const Key& key, const Value& value) {
		PairKV pkv = { key, value };
		return D::insert(pkv);
	}

	/**
	* The square bracket operator returns a reference to the Value
	* corresponding to the specified Key.
	*
	* If the Key doesn't yet exist in the Map, it will be inserted
	* with a default Value.
	*
	* There is no const variant of this function, because each call might
	* potentially modify the Map by inserting a new key.
	* Use find() if you have a const Map.
	*/
	Value& operator[](const Key& key) {
		DN* node = (DN*)dict_lookup(&this->mDict, &key);
		if(node == NULL) {
			node = new DN();
			node->data.first = key;
			dict_insert(&this->mDict, node, &(node->data.first));
		}
		return node->data.second;
	}
};

}

#endif
