/* Copyright (C) 2011 Mobile Sorcery AB

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
