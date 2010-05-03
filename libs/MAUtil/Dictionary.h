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

/** \file Dictionary.h
* \brief Thin template sorted dictionary.
*/

#ifndef _SE_MSAB_MAUTIL_DICTIONARY_H_
#define _SE_MSAB_MAUTIL_DICTIONARY_H_

#include <ma.h>
#include <maassert.h>
#include <kazlib/dict.h>
#include "collection_common.h"

namespace MAUtil {

//******************************************************************************
// Dictionary
//******************************************************************************

/** \brief Thin template sorted Dictionary.
*
* The Dictionary is a data storage mechanism that stores unique values.
* The Dictionary is sorted, by a method selectable by the user.
* It has fairly fast insert, erase and lookup operations. (O(log n), specifically.)
* It has an Iterator, which allows you to access all elements in order.
*
* This particular implementation is built on top of kazlib's dictionary system,
* which makes it quite small, even when used with multiple data types.
*
* This class is not meant to be instantiated directly.
* It is the base class for Set and Map.
*
* \warning It is techically possible to use an Iterator to change the Key of an element.
* DON'T DO THAT!. It would break the sorting order of the Dictionary.
* The consequenses are undefined, and will likely cause your program to crash.
*
* If you must change a Key, erase the original element from the dictionary,
* make your change and then re-insert it.
*/
template<class Key, class Storage>
class Dictionary {
protected:
	/** Internal. */
	struct DictNode : dnode_t {
		DictNode();
		Storage data;
	};
public:
	class ConstIterator;

	/**
	* An Iterator is bound to a specific Dictionary object.
	* The Iterator can point to a specific element in that Dictionary, or at Dictionary::end(),
	* which is "beyond" the last element of the Dictionary.
	* If the iterator points at Dictionary::end(), attempting to access the element
	* will cause a crash.
	* The Iterator is bidirectional; it can move backwards or forwards through the Dictionary.
	*/
	class Iterator {
	public:
		Storage& operator*();
		Storage* operator->();
	
		/**
		* Causes the Iterator to point to the next element in the Dictionary to which it is bound.
		* If the Iterator points to Dictionary::end(), this operation will cause a crash.
		*/
		Iterator& operator++();
		Iterator operator++(int);

		/**
		* Causes the Iterator to point to the previous element in the Dictionary to which it is bound.
		* \note If the iterator points to the first node,
		* this operation will cause it to point to Dictionary::end().
		*/
		Iterator& operator--();
		Iterator operator--(int);

		bool operator==(const Iterator&) const;
		bool operator!=(const Iterator&) const;

		Iterator& operator=(const Iterator&);
		Iterator(const Iterator&);
	protected:
		DictNode* mNode;
		dict_t* mDict;
		Iterator(dict_t*);
		friend class Dictionary;
		friend class ConstIterator;
	};

	/**
	* A ConstIterator is just like an ordinary Iterator, except
	* all its methods and return values are const.
	*/
	class ConstIterator {
	public:
		const Storage& operator*() const;
		const Storage* operator->() const;
	
		ConstIterator& operator++();
		ConstIterator operator++(int);
		ConstIterator& operator--();
		ConstIterator operator--(int);

		bool operator==(const ConstIterator&) const;
		bool operator!=(const ConstIterator&) const;

		ConstIterator& operator=(const ConstIterator&);
		ConstIterator(const ConstIterator&);
		ConstIterator(const Iterator&);
	protected:
		const DictNode* mNode;
		const dict_t* mDict;
		ConstIterator(const dict_t*);
		friend class Dictionary;
	};

	typedef int (*CompareFunction)(const Key&, const Key&);

	//constructors
	/// Constructs a copy of another Dictionary. All elements are also copied.
	Dictionary(const Dictionary&);
	/// Clears this Dictionary, then copies the other Dictionary to this one.
	Dictionary& operator=(const Dictionary&);
	/// The destructor deletes all elements.
	~Dictionary();

	//methods
	/**
	* Searches the Dictionary for a specified Key. The returned Iterator points to
	* the element matching the Key if one was found, or to Dictionary::end() if not.
	*/
	Iterator find(const Key&);
	ConstIterator find(const Key&) const;
	/**
	* Deletes an element, matching the specified Key, from the Dictionary.
	* Returns true if an element was erased, or false if there was no element matching the Key.
	*/
	bool erase(const Key&);
	/**
	* Deletes an element, pointed to by the specified Iterator.
	* The Iterator is invalidated, so if you want to continue iterating through the Dictionary,
	* you must use a different Iterator instance.
	* \warning If the Iterator is bound to a different Dictionary, or if it
	* points to end(), the system will crash.
	*/
	void erase(Iterator);
	/**
	* Returns an Iterator pointing to the first element in the Dictionary.
	*/
	Iterator begin();
	ConstIterator begin() const;
	/**
	* Returns an Iterator pointing to a place beyond the last element of the Dictionary.
	* This Iterator is often used to determine when another Iterator has reached its end.
	*/
	Iterator end();
	ConstIterator end() const;
	/**
	* Returns the number of elements in the Dictionary.
	*/
	size_t size() const;
	/**
	* Deletes all elements.
	*/
	void clear();

protected:
	dict_t mDict;
	int mKeyOffset;

	//******************************************************************************
	// DictAllocator
	//******************************************************************************

#ifdef KAZLIB_OPAQUE_DEBUG
#error Need full definition of dnode_t
#endif

	static dnode_t* alloc(void*) { return new DictNode; }
	static void free(dnode_t* node, void*) { delete (DictNode*)node; }

	void init(CompareFunction);

	/// Constructs an empty Dictionary.
	Dictionary(CompareFunction cf, int keyOffset);

	/**
	* Inserts a new value into the Dictionary.
	*
	* Returns a Pair. The Pair's second element is true if the value was indeed inserted.
	* The Pair's first element is an Iterator that points to the element in the Dictionary.
	*
	* An element which compares equal to the new one may already be present in the Dictionary;
	* in that case, this operation does nothing, and the Iterator returned will point to
	* the old element.
	*/
	Pair<Iterator, bool> insert(const Storage&);
};

}	//MAUtil

#include "Dictionary_impl.h"

#endif	//_SE_MSAB_MAUTIL_DICTIONARY_H_
