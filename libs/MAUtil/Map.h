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
class Map : public Dictionary<const Key, Pair<const Key, Value> > {
public:
	typedef Pair<const Key, Value> PairKV;
protected:
	typedef Dictionary<const Key, PairKV> D;
	typedef typename D::DictNode DN;
public:

	Map(int (*cf)(const Key&, const Key&) = &Compare<const Key>)
		: D::Dictionary(cf, OFFSETOF(PairKV, first)) {}
	Pair<typename D::Iterator, bool> insert(const Key& key, const Value& value) {
		PairKV pkv = { key, value };
		return D::insert(pkv);
	}
	Value& operator[](const Key& key) {
		DN* node = (DN*)dict_lookup(&this->mDict, &key);
		if(node == NULL) {
			PairKV p = { key, Value() };
			node = new DN(p);
			dict_insert(&this->mDict, node, &(node->data.first));
		}
		return node->data.second;
	}
};

}

#endif
