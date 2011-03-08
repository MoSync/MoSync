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
