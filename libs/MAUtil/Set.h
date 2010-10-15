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

/** \file Set.h
* \brief Thin template sorted Set.
*/

#ifndef _SE_MSAB_MAUTIL_SET_H_
#define _SE_MSAB_MAUTIL_SET_H_

#include "Dictionary.h"

namespace MAUtil {

/// Thin template sorted Set.
template<class Key>
class Set : public Dictionary<Key, Key> {
public:
	typedef Dictionary<Key, Key> D;

	Set(typename D::CompareFunction cf = &Compare<Key>) : D::Dictionary(cf, 0) {
	}

	/**
	* Inserts a new element into the Set.
	*
	* Returns a Pair. The Pair's second element is true if the element was inserted,
	* or false if the element already existed in the set.
	* The Pair's first element is an Iterator that points to the element in the Set.
	*
	* An element which is equal to the new one may already be present in the Set;
	* in that case, this operation does nothing, and the Iterator returned will point to
	* the old element.
	*/
	Pair<typename D::Iterator, bool> insert(const Key& key) { return D::insert(key); }
};

}	//MAUtil

#endif	//_SE_MSAB_MAUTIL_SET_H_
