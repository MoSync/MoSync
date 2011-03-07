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

/** \file HashMap_impl.h
* \brief HashMap implementation
*/

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
