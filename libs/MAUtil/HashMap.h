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

/** \file HashMap.h Copyright (c) Mobile Sorcery AB 2007 **/

#ifndef _SE_MSAB_MAUTIL_HASHMAP_H_
#define _SE_MSAB_MAUTIL_HASHMAP_H_

#include <malloc.h>
#include <new>
#include <ma.h>
#include "Vector.h"

namespace MAUtil {
	typedef int (*HashFunctionPointer)(const void* data, int size);

	//returns zero iff *a is equal to *b.
	typedef int (*CompareFunctionPointer)(const void* a, const void* b, int size);

	typedef void (*DisposeFunctionPointer)(byte* pair);
	typedef void (*CopyFunctionPointer)(byte* pair, const void* key, const void* value);

	class HashMapImpl {
	public:
	private:
		struct BaseNode {
			BaseNode* next;
			byte pair[1];	//key[mKeySize], value[mValueSize]
		};
	public:

		//Invalidated by rehashes, which can be caused by insert.
		class IteratorImpl {
		public:
			bool operator==(const IteratorImpl& o) const { return node == o.node; }
			bool operator!=(const IteratorImpl& o) const { return node != o.node; }
		protected:
			IteratorImpl() {}
			IteratorImpl(HashMapImpl* im, BaseNode** n, int ix)
				: impl(im), node(n), index(ix) {}
			HashMapImpl* impl;
			BaseNode** node;
			int index;

			IteratorImpl& set(IteratorImpl& o) {
				impl = o.impl;
				node = o.node;
				index = o.index;
				return *this;
			}
			void proceed();
		};

		class BaseConstIterator : public IteratorImpl {
		public:
			BaseConstIterator(BaseConstIterator& o) { set(o); }
			BaseConstIterator& operator=(BaseConstIterator& o) { return (BaseConstIterator&)set(o); }
			BaseConstIterator operator++(int) {
				BaseConstIterator old(*this);
				proceed();
				return old;
			}
		protected:
			friend class HashMapImpl;
			BaseConstIterator(const HashMapImpl* im, BaseNode* const * n, int ix)
				: IteratorImpl((HashMapImpl*)im, (BaseNode**)n, ix) {}
		};

		class BaseIterator : public BaseConstIterator {
		public:
			BaseIterator(BaseConstIterator& o) : BaseConstIterator(o) {}
			BaseIterator& operator=(BaseIterator& o) { return (BaseIterator&)set(o); }
			BaseIterator operator++(int) {
				BaseIterator old(*this);
				proceed();
				return old;
			}
		protected:
			friend class HashMapImpl;
			BaseIterator(HashMapImpl* im, BaseNode** n, int ix)
				: BaseConstIterator(im, n, ix) {}
		};

		//Returns false if key already exists.
		bool insert(void* key, void* value);

		//Invalidates all iterators that point to the erased element.
		//Make an incremented copy if you want to continue iteration.
		//For example, erase(itr++) would let itr remain valid.
		void erase(BaseIterator itr);

		BaseConstIterator find(const void* key) const;	//Returns end() if key does not exist.
		BaseIterator find(const void* key) {
			return *(BaseIterator*)&(((const HashMapImpl*)this)->find(key));
		}
		int count() const { return m.count; }	//Returns the number of pairs in this map.
		int capacity() const { return m.capacity; }
		double loadFactor() const { return mLoadFactor; }

		void reset();	//A convenience function that erases all elements.

		BaseIterator begin() {
			BaseIterator itr(this, &m.base[0], 0);
			if(*itr.node == NULL)
				itr.proceed();
			return itr;
		}
		BaseConstIterator end() const {
			return BaseConstIterator(this, NULL, 0);
		}

		HashMapImpl(int keySize, int valueSize, HashFunctionPointer hashFunc,
			CompareFunctionPointer compareFunc, DisposeFunctionPointer disposeFunc,
			CopyFunctionPointer copyFunc, int initialCapacity, double loadFactor);
		virtual ~HashMapImpl();

	private:
		//Vector<BaseNode*> mVector;
		const double mLoadFactor;
		const HashFunctionPointer mHashFunction;
		const CompareFunctionPointer mCompareFunction;
		const DisposeFunctionPointer mDisposeFunction;
		const CopyFunctionPointer mCopyFunction;
		const int mKeySize, mValueSize;
		const int mNodeSize;	//cache of (4 + mKeySize + mValueSize)
		struct ZEROABLE {
			BaseNode** base;
			int capacity;
			int count;

			//chache
			int loadLimit;	//a function of mLoadFactor & mVector.size()

#ifdef MAUTIL_HASHMAP_STATS
			//statistics
			mutable struct STATISTICS {
				uint rehashes;
				uint inserts;
				uint linearInsertSteps;
				uint insertHashDupes;	//key dupes don't count
				uint rehashInserts;
				uint finds;
				uint linearFindSteps;
				//erases = inserts - numElem;
				uint erases;	//for double-checking
				uint linearEraseSteps;
				uint getsArray;
			} stat;
#endif	//MAUTIL_HASHMAP_STATS
		} m;

		void rehash();
	};

	template<class KeyType, class ValueType>
	class HashMapBase : protected HashMapImpl {
	public:
		HashMapBase(int keySize, int valueSize, HashFunctionPointer hashFunc,
			CompareFunctionPointer compareFunc, int initialCapacity, double loadFactor) :
		HashMapImpl(keySize, valueSize, hashFunc, compareFunc, &HashMapBase::dispose,
			&HashMapBase::copy, initialCapacity, loadFactor) {}

		struct Pair {
			Pair(const KeyType& k, const ValueType& v) : key(k), value(v) {}
			KeyType key;
			ValueType value;
		};

		class ConstIterator : public BaseConstIterator {
		public:
			const Pair& operator*() const { return (const Pair&)(*node)->pair; }
			const Pair* operator->() const { return (const Pair*)&(*node)->pair; }
		protected:
			ConstIterator(BaseConstIterator& o) : BaseConstIterator(o) {}
			friend class HashMapBase;
		};

		class Iterator : public BaseIterator {
		public:
			Pair& operator*() { return (Pair&)(*node)->pair; }
			Pair* operator->() { return (Pair*)&(*node)->pair; }
		protected:
			Iterator(BaseIterator& o) : BaseIterator(o) {}
			friend class HashMapBase;
		};

		bool insert(const KeyType& key, const ValueType& value) {
			return HashMapImpl::insert(new KeyType(key), new ValueType(value));
		}
		Iterator find(const KeyType& key) {
			return HashMapImpl::find(&key);
		}
		void erase(Iterator& itr) {
			return HashMapImpl::erase(itr);
		}
		int count() const {
			return HashMapImpl::count();
		}
		int capacity() const {
			return HashMapImpl::capacity();
		}
		double loadFactor() const {
			return HashMapImpl::loadFactor();
		}
		ConstIterator end() const {
			return HashMapImpl::end();
		}
	protected:
		static void dispose(byte* pair) {
			//delete ((KeyType*)(pair));
			//delete ((ValueType*)(pair + sizeof(KeyType)));
			reinterpret_cast<Pair*>(pair)->~Pair();
		}
		static void copy(byte* pair, const void* key, const void* value) {
			//new ((int*)pair) KeyType(*(KeyType*)key);
			//new (pair + sizeof(KeyType)) ValueType(*(ValueType*)value);
			//Pair* p = (Pair*)pair;
			//p->Pair(*(KeyType*)key, *(ValueType*)value);
			new (pair) Pair(*(KeyType*)key, *(ValueType*)value);
		}
	};

	//returns a hash of <size> bytes, starting at <data>.
	//is fast even on Java.
	int memHash(const void* data, int size);

	//hashes a 4-byte word, starting at <data>. ignores <size>.
	int integralHash(const void* data, int size);

	//compares two 4-byte words. ignores size.
	int integralCompare(const void* a, const void* b, int size);

	template<class KeyType> int selfCompare(const void* a, const void* b, int /*size*/) {
		return (const KeyType*)a == (const KeyType*)b;
	}

	template<class KeyType, class ValueType>
	class HashMap : public HashMapBase<KeyType, ValueType> {
	public:
		HashMap(int initialCapacity = 8, double loadFactor = 0.75,
			HashFunctionPointer hashFunction = (sizeof(KeyType) <= 4 ? integralHash : memHash),
			CompareFunctionPointer compareFunc = (sizeof(KeyType) <= 4 ? integralCompare :
			(CompareFunctionPointer)memcmp)) :
		HashMapBase<KeyType, ValueType>(sizeof(KeyType), sizeof(ValueType),
			hashFunction, compareFunc, initialCapacity, loadFactor)
		{}
	};

	//Use where KeyType can hash itself.
	template<class KeyType, class ValueType>
	class SelfHashMap : public HashMapBase<KeyType, ValueType> {
	private:
		typedef int (*SelfHashFunctionPointer)(const KeyType* data, int size);	//ignore size.
		HashFunctionPointer convertSelfHFP(SelfHashFunctionPointer shfp) {
			return (HashFunctionPointer)shfp;
		}
	public:
		SelfHashMap(int initialCapacity = 8, double loadFactor = 0.75)
			: HashMapBase<KeyType, ValueType>(sizeof(KeyType), sizeof(ValueType),
			convertSelfHFP(&KeyType::hash), &selfCompare<KeyType>, initialCapacity, loadFactor)
		{
			MAASSERT(sizeof(KeyType) <= sizeof(void*));
		}
	};

#if 0
	//Use where the key is small and integral (for example, an int or a pointer),
	//for better performance.
	template<class KeyType, class ValueType>
	class IntegralHashMap : public HashMapBase<KeyType, ValueType> {
	public:
		IntegralHashMap(int initialCapacity = 8, double loadFactor = 0.75,
			HashFunctionPointer hashFunction = integralHash,
			CompareFunctionPointer compareFunc = integralCompare)
			: HashMapBase<KeyType, ValueType>(sizeof(KeyType), sizeof(ValueType),
			hashFunction, compareFunc, initialCapacity, loadFactor)
		{
			MAASSERT(sizeof(KeyType) <= sizeof(void*));
		}

		virtual bool insert(const KeyType& key, const ValueType& value) {
			return HashMapImpl::insert((void*)key, new ValueType(value));
		}
		virtual ValueType* find(const KeyType& key) {
			return HashMapImpl::find((void*)key);
		}
		virtual bool erase(const KeyType& key) {
			return HashMapImpl::erase((void*)key);
		}
	protected:
		virtual void Dispose(BasePair& pair) {
			delete (ValueType*)pair.value;
		}
	};
#endif
}

#endif	//_SE_MSAB_MAUTIL_HASHMAP_H_
