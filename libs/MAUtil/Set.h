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

#ifndef SET_H
#define SET_H

#include <maassert.h>
#include <kazlib/dict.h>

#ifndef NULL
#define NULL 0
#endif

namespace MAUtil {

//******************************************************************************
// Comparator
//******************************************************************************

template<class T> class Comparator {
public:
	static int Compare(const T& a, const T& b) {
		if(a < b)
			return -1;
		else if(a == b)
			return 0;
		else
			return 1;
	}
};

#if 0	//may be unneeded if Set itself is specialized
//generic specialization for pointers
template<class T> class Less<T*> {
	static bool operator()(const T* a, const T* b) {
		return *a < *b;
	}
};
#endif

//******************************************************************************
// Pair
//******************************************************************************

template<class F, class S> struct Pair {
	F first;
	S second;
};

//******************************************************************************
// Set
//******************************************************************************

template<class Key, class Comp=Comparator<Key> >
class Set {
public:
	//typedefs
	struct DictNode : dnode_t {
		Key key;
	};

	class Iterator {
	public:
		Key& operator*();
		Key* operator->();
		Iterator& operator++();

		/**
		* \note If the iterator points to the first node,
		* decrementing it will cause it to point to Set::end().
		*/
		Iterator& operator--();

		bool operator==(const Iterator&) const;
		bool operator!=(const Iterator&) const;

		Iterator& operator=(const Iterator&);
		Iterator(const Iterator&);
	protected:
		DictNode* mNode;
		dict_t* mDict;
		Iterator(dict_t*);
		friend class Set;
	};

	//constructors
	Set();
	Set(const Set&);
	~Set();

	//methods
	Pair<bool, Iterator> insert(const Key&);
	Iterator find(const Key&);
	bool erase(const Key&);
	Iterator begin();
	Iterator end();
	size_t size() const;
	void clear();

protected:
	dict_t mDict;

	//******************************************************************************
	// DictAllocator
	//******************************************************************************

#ifdef KAZLIB_OPAQUE_DEBUG
#error Need full definition of dnode_t
#endif

	static dnode_t* alloc(void*) { return new DictNode; }
	static void free(dnode_t* node, void*) { delete (DictNode*)node; }
	static int compare(const void*, const void*);
};

#include "Set_impl.h"

}	//MAUtil

#endif SET_H
