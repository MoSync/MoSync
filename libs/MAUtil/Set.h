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

template<class Key>
class Set : public Dictionary<Key, Key> {
public:
	typedef Dictionary<Key, Key> D;

	Set(int (*cf)(const Key&, const Key&) = &Compare<Key>) : D::Dictionary(cf, NULL) {
		//ugly hack to get around pointer-to-member NULL being stored as (-1).
		memset(&this->mKeyPtr, 0, sizeof(this->mKeyPtr));
	}
	Pair<typename D::Iterator, bool> insert(const Key& key) { return D::insert(key); }
};

}	//MAUtil

#endif	//_SE_MSAB_MAUTIL_SET_H_
