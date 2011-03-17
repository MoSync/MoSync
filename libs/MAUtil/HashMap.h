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
