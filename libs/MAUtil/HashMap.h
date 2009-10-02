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

#ifndef _SE_MSAB_MAUTIL_HASHMAP_H_
#define _SE_MSAB_MAUTIL_HASHMAP_H_

#include <maassert.h>
#include <kazlib/hash.h>
#include "collection_common.h"

//namespace MAUtil {

/** \brief Thin template HashMap.
*
* The HashMap is a data storage mechanism that stores unique keys,
* and a value associated with each key.
* It has very fast insert, erase and lookup operations. (O(???), specifically.) // probably near constant time
* It has an Iterator, which allows you to access all elements.
* The HashMap is not sorted. For each iteration, you will get an undefined,
* semi-random order of elements.
*
* This particular implementation is built on top of kazlib's hash table system,
* which makes it quite small, even when used with multiple data types.
*/
template<class Key, class Value, class Comp=Comparator<Key> >
class HashMap {
protected:
	class HashNode : hnode_t {
	public:
		HashNode();
		Key k;
		Value v;
	};
public:
	class ConstIterator;

	/**
	* An Iterator is bound to a specific HashMap object.
	* The Iterator can point to a specific element in that HashMap, or at HashMap::end(),
	* which is "beyond" the last element of the HashMap.
	* If the iterator points at HashMap::end(), attempting to access the element
	* will cause a crash.
	* This Iterator is unidirectional; it can only move forwards through the HashMap.
	*/
	//TODO: postfix operator
	class Iterator {
		Key& operator*();
		Key* operator->();
	
		/**
		* Causes the Iterator to point to the next element in the HashMap to which it is bound.
		* If the Iterator points to HashMap::end(), this operation will cause a crash.
		*/
		Iterator& operator++();

		bool operator==(const Iterator&) const;
		bool operator!=(const Iterator&) const;

		Iterator& operator=(const Iterator&);
		Iterator(const Iterator&);
	protected:
		hscan_t mScan;
		Iterator(hscan_t*);
		friend class HashMap;
		friend class ConstIterator;
	};
	
	typedef hash_val_t (*HashFunction)(const Key&);
	
	HashMap(HashFunction hf = NULL);
	HashMap(const HashMap&);
	HashMap& operator=(const HashMap);
	~HashMap();
	
	Pair<Iterator, bool> insert(Key, Value);
	Iterator find(Key);
	bool erase(Key);
	void erase(Iterator);
	size_t size() const;
	void clear();

	Iterator begin();
	ConstIterator begin() const;

	Iterator end();
	ConstIterator end() const;
	
protected:
	hast_t mHash;

	static hnode_t* alloc(void*) { return new HashNode; }
	static void free(hnode_t* node, void*) { delete (HashNode*)node; }
	static int compare(const void*, const void*);

	void init();
};

#include "HashMap_impl.h"

//}	//MAUtil

#endif	//_SE_MSAB_MAUTIL_HASHMAP_H_
