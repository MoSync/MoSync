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

/** \brief Thin template HashMap.
*
* The HashMap is a data storage mechanism that stores unique keys,
* and a value associated with each key.
*
* It has very fast insert, erase and lookup operations,
* assuming a good hash function is being used. For a perfect hash function
* (which generally doesn't exist), lookups and erases take constant time,
* while inserts run in constant time amortized over O(n).
*
* It has an Iterator, which allows you to access all elements.
* The HashMap is not sorted. For each iteration, you will get an undefined,
* semi-random order of elements.
*
* This particular implementation is built on top of kazlib's hash table system,
* which makes it quite small, even when used with multiple data types.
*/
template<class Key, class Value>
class HashMap {
public:
	typedef Pair<Key, Value> PairKV;
protected:
	struct HashNode : hnode_t {
		HashNode();
		PairKV data;
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
	public:
		PairKV& operator*();
		PairKV* operator->();

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
		Iterator();
		friend class HashMap;
		friend class ConstIterator;
	};

	/**
	* A ConstIterator is just like an ordinary Iterator, except
	* all its methods and return values are const.
	*/
	class ConstIterator {
	public:
		const PairKV& operator*() const;
		const PairKV* operator->() const;

		ConstIterator& operator++();
		ConstIterator& operator--();

		bool operator==(const ConstIterator&) const;
		bool operator!=(const ConstIterator&) const;

		ConstIterator& operator=(const ConstIterator&);
		ConstIterator(const ConstIterator&);
		ConstIterator(const Iterator&);
	protected:
		hscan_t mScan;
		ConstIterator();
		friend class HashMap;
	};

	typedef hash_val_t (*HashFunction)(const Key&);
	typedef int (*CompareFunction)(const Key&, const Key&);

	/**
	* Constructs an empty HashMap.
	* \param hf The hash function.
	* \param cf The compare function.
	* \param init_bits The intial size of the hash table is 2 to the power of this number.
	* While the table grows and shrinks dynamically, it is possible to optimize
	* if you're doing a known number of insertions directly after constructing the
	* HashMap.
	*/
	HashMap(HashFunction hf = &THashFunction<Key>,
		CompareFunction cf = &Compare<Key>,
		int init_bits = 6);

	/// Constructs a copy of another HashMap. All elements are also copied.
	HashMap(const HashMap&);
	/// Clears this HashMap, then copies the other HashMap to this one.
	HashMap& operator=(const HashMap&);
	/// The destructor deletes all elements.
	~HashMap();

	/**
	* Inserts a new value into the HashMap.
	*
	* Returns a Pair. The Pair's second element is true if the value was indeed inserted.
	* The Pair's first element is an Iterator that points to the element in the HashMap.
	*
	* An element which compares equal to the new one may already be present in the HashMap;
	* in that case, this operation does nothing, and the Iterator returned will point to
	* the old element.
	*/
	Pair<Iterator, bool> insert(const Key&, const Value&);
	/**
	* Searches the HashMap for a specified Key. The returned Iterator points to
	* the element matching the Key if one was found, or to HashMap::end() if not.
	*/
	Iterator find(const Key&);
	ConstIterator find(const Key&) const;
	/**
	* Deletes an element, matching the specified Key, from the HashMap.
	* Returns true if an element was erased, or false if there was no element matching the Key.
	*/
	bool erase(const Key&);
	/**
	* Deletes an element, pointed to by the specified Iterator.
	* The Iterator is invalidated, so if you want to continue iterating through the HashMap,
	* you must use a different Iterator instance.
	* \warning If the Iterator is bound to a different HashMap, or if it
	* points to end(), the system will crash.
	*/
	void erase(Iterator);
	/**
	* Returns the number of elements in the HashMap.
	*/
	size_t size() const;
	/**
	* Deletes all elements.
	*/
	void clear();

	/**
	* Returns an Iterator pointing to the first element in the HashMap.
	*/
	Iterator begin();
	ConstIterator begin() const;

	/**
	* Returns an Iterator pointing to a place beyond the last element of the HashMap.
	* This Iterator is often used to determine when another Iterator has reached its end.
	*/
	Iterator end();
	ConstIterator end() const;

	/**
	* The square bracket operator returns a reference to the Value
	* corresponding to the specified Key.
	*
	* If the Key doesn't yet exist in the HashMap, it will be inserted
	* with a default Value.
	*
	* There is no const variant of this function, because each call might
	* potentially modify the HashMap by inserting a new key.
	* Use find() if you have a const HashMap.
	*/
	Value& operator[](const Key&);

protected:
	hash_t mHash;
	HashFunction mHashFunction;

	static hnode_t* alloc(void*) { return new HashNode; }
	static void free(hnode_t* node, void*) { delete (HashNode*)node; }

	void init();
};

}	//MAUtil

#include "HashMap_impl.h"

#endif	//_SE_MSAB_MAUTIL_HASHMAP_H_
