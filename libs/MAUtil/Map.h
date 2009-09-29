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

/** \file Map.h
* \brief A generic map/dictionary class.
*/

#ifndef _SE_MSAB_MAUTIL_MAP_H_
#define _SE_MSAB_MAUTIL_MAP_H_

#include "Vector.h"

#ifndef NULL
#define NULL 0
#endif

namespace MAUtil {
	/** \brief A generic map/dictionary class.
	*
	* The cost for looking up a value is O(n) comparisons, where
	* n is the number of entries in the map. As such, it is best suited for use with a small numbers of
	* keys/values. Larger dictionaries should be implemented using a proper hash-based implementation.
	*/
	template<typename KeyType, typename ValueType> class Map {
	public:
		/**
		* A structure to hold the entries of the Map class.
		*/
		struct MapNode {
			MapNode(const KeyType &key, const ValueType &value)
				: key(key), value(value), next(0), prev(0)
			{
			}

			KeyType key;
			ValueType value;
			MapNode *next;
			MapNode *prev;
		};

		/**
		* Creates an empty Map.
		*/
		Map() : head(0), tail(0) {
		}

		/**
		* Removes all entries from the Map.
		*/
		void clear() {
			keys.clear();
			if(head) {
				MapNode *it = head;
				MapNode *temp;
				do {
					temp = it->next;
					delete it;
					it = temp;
				} while(it);
			}
			head = 0;
		}

		virtual ~Map() {
			clear();
		}

		Map& operator =(const Map& m) {
			clear();
			for(int i = 0; i < m.keys.size(); i++) {
				const ValueType* v = m.get(m.keys[i]);
				(*this)[m.keys[i]] = *v;
			}
			return *this;
		}

		Map(const Map& m) : head(0), tail(0) {
			(*this)=m;
		}

		/** Returns the value of the entry with the given key. If the key doesn't exist
		an entry will be created with the given key. If the value type is a primitive
		type the value will be uninitialzed. If the value type is a class the default
		constructor will be used.
		**/
		ValueType& operator[](const KeyType& key) {
			{
/*
				MapNode* node = getNode(key);
				if(node)
					return node->value;
*/
				ValueType *value = get(key);
				if(value) return *value;
			}
			ValueType val;
			MapNode* oldTail = tail;
			tail = new MapNode(key, val);
			keys.add(key);
			if(!head)
			{
				head = tail;
			} else {
				oldTail->next = tail;
				tail->prev = oldTail;
			}

			return tail->value;
		}

		/**
		* Returns a pointer to the value associated with the specified \a key,
		* or NULL if the key is not in the Map.
		*/
		ValueType* get(const KeyType& key) {
			templateVector_each(KeyType, i, keys) {
				if(*i == key) {
					MapNode* node = getNode(key);
					if(node)
						return &node->value;
				}
			}
			return NULL;
		}

		/**
		* Returns a pointer to the value associated with the specified \a key,
		* or NULL if the key is not in the Map.
		*/
		const ValueType* get(const KeyType& key) const {
			templateVector_each_const(KeyType, i, keys) {
				if(*i == key) {
					const MapNode* node = getNode(key);
					if(node)
						return &node->value;
				}
			}
			return NULL;
		}


		/**
		* Returns a const reference to the Vector containing all the keys in the
		* Map.
		*/
		const Vector<KeyType>& getKeySet() const {
			return keys;
		}

		/**
		* Removes the entry specified by \a key, if it exists.
		*/
		void remove(const KeyType& key) {
			MapNode *it = head;

			templateVector_each(KeyType, i, keys) {
				if(*i == key) {
					keys.remove(i);

					//find node
					MapNode* node = it;//getNode(key);
					if(node) {
						//unlink
						//if node == head, head = next.
						if(node == head)
							head = node->next;
						//if node == tail, tail = prev.
						if(node == tail)
							tail = node->prev;
						//if next, next->prev = prev.
						if(node->next)
							node->next->prev = node->prev;
						//if prev, prev->next = next.
						if(node->prev)
							node->prev->next = node->next;

						//delete
						delete node;
					}

					return;
				}
				it = it->next;
			}
		}

	protected:
		MapNode *head;
		MapNode *tail;
		Vector<KeyType> keys;

		MapNode* getNode(const KeyType& key) {
			if(head) {
				MapNode *it = head;
				while(it) {
					if(it->key == key) {
						return it;
					}
					it = it->next;
				}
			}
			return NULL;
		}

		const MapNode* getNode(const KeyType& key) const {
			if(head) {
				const MapNode *it = head;
				while(it) {
					if(it->key == key) {
						return it;
					}
					it = it->next;
				}
			}
			return NULL;
		}
	};
}

#endif
