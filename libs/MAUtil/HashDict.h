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

/** \file HashDict.h
* \brief Thin template HashDict.
*/

#ifndef _SE_MSAB_MAUTIL_HASHDICT_H_
#define _SE_MSAB_MAUTIL_HASHDICT_H_

#include <maassert.h>
#include <maheap.h>
#include <kazlib/hash.h>
#include "collection_common.h"
#include "String.h"

namespace MAUtil {

//Template hash function declaration.
//Note that there is no default implementation.
template<class Key> hash_val_t THashFunction(const Key&);

//We can have specializations for all the common types.
//But we'll start with just these few.

template<> hash_val_t THashFunction(const String&);
template<> hash_val_t THashFunction(const int&);

/** \brief Thin template unsorted dictionary.
*
* The HashDict is a data storage mechanism that stores unique keys,
* and a value associated with each key.
*
* It has very fast insert, erase and lookup operations,
* assuming a good hash function is being used. For a perfect hash function
* (which generally doesn't exist), lookups and erases take constant time,
* while inserts run in constant time amortized over O(n).
*
* It has an Iterator, which allows you to access all elements.
* The HashDict is not sorted. For each iteration, you will get an undefined,
* semi-random order of elements.
*
* This particular implementation is built on top of kazlib's hash table system,
* which makes it quite small, even when used with multiple data types.
*/
template<class Key, class Storage>
class HashDict {
protected:
	/** \brief Internal storage. */
	struct HashNode : hnode_t {
		HashNode(Storage p);
		Storage data;
	};
public:
	class ConstIterator;

	/**
	* \brief Iterator for a HashDict.
	* 
	* An Iterator is bound to a specific HashDict object.
	* The Iterator can point to a specific element in that HashDict, or at HashDict::end(),
	* which is "beyond" the last element of the HashDict.
	* If the iterator points at HashDict::end(), attempting to access the element
	* will cause a crash.
	* This Iterator is unidirectional; it can only move forwards through the HashDict.
	*/
	class Iterator {
	public:
		Storage& operator*();
		Storage* operator->();

		/**
		* Causes the Iterator to point to the next element in the HashDict to which it is bound.
		* If the Iterator points to HashDict::end(), this operation will cause a crash.
		*/
		Iterator& operator++();
		Iterator operator++(int);

		bool operator==(const Iterator&) const;
		bool operator!=(const Iterator&) const;

		Iterator& operator=(const Iterator&);
		Iterator(const Iterator&);
	protected:
		hscan_t mScan;
		Iterator();
		friend class HashDict;
		friend class ConstIterator;
	};

	/**
	* \brief Const Iterator for a HashDict.
	* 
	* A ConstIterator is just like an ordinary Iterator, except
	* all its methods and return values are const.
	*/
	class ConstIterator {
	public:
		const Storage& operator*() const;
		const Storage* operator->() const;

		ConstIterator& operator++();
		ConstIterator operator++(int);

		bool operator==(const ConstIterator&) const;
		bool operator!=(const ConstIterator&) const;

		ConstIterator& operator=(const ConstIterator&);
		ConstIterator(const ConstIterator&);
		ConstIterator(const Iterator&);
	protected:
		hscan_t mScan;
		ConstIterator();
		friend class HashDict;
	};

	typedef hash_val_t (*HashFunction)(const Key&);
	typedef int (*CompareFunction)(const Key&, const Key&);

	/// Constructs a copy of another HashDict. All elements are also copied.
	HashDict(const HashDict&);
	/// Clears this HashDict, then copies the other HashDict to this one.
	HashDict& operator=(const HashDict&);
	/// The destructor deletes all elements.
	~HashDict();

	/**
	* Searches the HashDict for a specified Key. The returned Iterator points to
	* the element matching the Key if one was found, or to HashDict::end() if not.
	*/
	Iterator find(const Key&);
	ConstIterator find(const Key&) const;

	/**
	* Deletes an element, matching the specified Key, from the HashDict.
	* Returns true if an element was erased, or false if there was no element matching the Key.
	*/
	bool erase(const Key&);

	/**
	* Deletes an element, pointed to by the specified Iterator.
	* The Iterator is invalidated, so if you want to continue iterating through the HashDict,
	* you must use a different Iterator instance.
	* \warning If the Iterator is bound to a different HashDict, or if it
	* points to end(), the system will crash.
	*/
	void erase(Iterator);

	/**
	* Returns the number of elements in the HashDict.
	*/
	size_t size() const;

	/**
	* Deletes all elements.
	*/
	void clear();

	/**
	* Returns an Iterator pointing to the first element in the HashDict.
	*/
	Iterator begin();
	ConstIterator begin() const;

	/**
	* Returns an Iterator pointing to a place beyond the last element of the HashDict.
	* This Iterator is often used to determine when another Iterator has reached its end.
	*/
	Iterator end();
	ConstIterator end() const;

protected:
	hash_t mHash;
	HashFunction mHashFunction;
	int mKeyOffset;

	static hnode_t* alloc(void*) GCCATTRIB(noreturn) { BIG_PHAT_ERROR; }
	static void free(hnode_t* node, void*) { delete (HashNode*)node; }

	/**
	* Constructs an empty HashDict.
	* \param keyOffset The offset from the start of Storage to the Key, in bytes.
	* Calculated by the macro #OFFSETOF.
	* \param hf The hash function.
	* \param cf The compare function. See Compare.
	* \param init_bits The intial size of the hash table is 2 to the power of this number.
	* While the table grows and shrinks dynamically, it is possible to optimize
	* if you're doing a known number of insertions directly after constructing the
	* HashDict.
	*/
	HashDict(int keyOffset, HashFunction hf = &THashFunction<Key>,
		CompareFunction cf = &Compare<Key>,
		int init_bits = 6);

	/**
	* Inserts a new element into the HashMap.
	*
	* Returns a Pair. The Pair's second element is true if the element was inserted,
	* or false if the element already existed in the map.
	* The Pair's first element is an Iterator that points to the element in the HashMap.
	*
	* An element which has the same key as the new one may already be present in the HashMap;
	* in that case, this operation does nothing, and the Iterator returned will point to
	* the old element.
	*/
	Pair<Iterator, bool> insert(const Storage&);
};

}	//MAUtil

#include "HashDict_impl.h"

#endif	//_SE_MSAB_MAUTIL_HASHDICT_H_
