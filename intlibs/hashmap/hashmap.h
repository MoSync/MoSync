/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef HASHMAP_H
#define HASHMAP_H

#include <helpers/types.h>

#define HASHMAP_BASE_SIZE 16

// map of int to void*. abstract base class.
class HashMapBase {
public:
	struct BasePair {
		int key;
		void* value;
	};

public:
	//Leaves if key already exists. Make sure the value is on the cleanup stack.
	void insert(int key, void* value, bool in_rehash = false);

	void* find(int key) const;	//Returns NULL if key does not exist.
	void erase(int key);	//Leaves if key does not exist.
	void close();	//Must be called before destruction, or BFE will occur.
	uint size() const;	//Returns the number of pairs in this map.

	//Invalidated by rehashes, which can be caused by InsertL.
	class TIteratorC {
	public:
		bool hasMore() const;
		const BasePair& next();
	protected:
		const BasePair* mPos;
		const BasePair* const mEnd;

		friend class HashMapBase;
		TIteratorC(const BasePair* pos, const BasePair* end);
		void proceed();
	};

	TIteratorC begin() const {
		return TIteratorC(m.base, m.base + m.baseLen);
	}

	HashMapBase();
	virtual ~HashMapBase();

private:
	struct ZEROABLE {
		BasePair* base;
		uint numElem;

		//chache
		uint baseLen;	//a function of base
		uint loadLimit;	//a function of baseLen

		//statistics
		mutable struct STATISTICS {
			uint rehashes;
			uint inserts;
			uint linearInsertSteps;
			uint insertHashDupes;	//key dupes doesn't count
			uint rehashInserts;
			uint finds;
			uint linearFindSteps;
			//erases = inserts - numElem;
			uint erases;	//for double-checking
			uint linearEraseSteps;
			uint getsArray;
		} stat;
	} m;

	void rehash();
	BasePair* findIndex(int key, uint& statLinearSteps) const;
	void updateBaseLen();	//updates baseLen and loadLimit

protected:
	virtual void dispose(BasePair& pair) = 0;
};

enum HashMapError {
	KHMEAlready = 42,
	KHMENoExist,
	KHMEUninitialized
};

// template map of int to T*.
// does not deleted objects.
template<class T> class HashMapNoDelete : protected HashMapBase {
public:
	struct Pair {
		uint key;
		T* value;
	};

	class TIteratorC : protected HashMapBase::TIteratorC {
	public:
		TIteratorC(const HashMapBase::TIteratorC& o) : HashMapBase::TIteratorC(o) {}
		bool hasMore() const { return HashMapBase::TIteratorC::hasMore(); }
		const Pair& next() { return (const Pair&)HashMapBase::TIteratorC::next(); }
	};

	TIteratorC begin() const { return HashMapBase::begin(); }

	void insert(int key, T* value) { HashMapBase::insert(key, value); }
	T* find(int key) const { return (T*)HashMapBase::find(key); }
	void erase(int key) { HashMapBase::erase(key); }
	void close() { HashMapBase::close(); }
	uint size() const { return HashMapBase::size(); }
protected:
	virtual void dispose(HashMapBase::BasePair&) {}
};

// deletes objects, but does not allocate them.
template<class T> class HashMap : public HashMapNoDelete<T> {
protected:
	virtual void dispose(HashMapBase::BasePair& pair) { delete (T*)pair.value; }
};

// map of int to const string.
// stores copies of the input strings. allocates and frees the storage needed.
class StringMap : protected HashMap<const char> {
public:
	//returns the value
	const char* insert(int key, const char* s1, uint len);
	const char* find(int key) const { return HashMap<const char>::find(key); }

	void erase(int key) { HashMap<const char>::erase(key); }
	void close() { HashMap<const char>::close(); }
	uint size() const { return HashMap<const char>::size(); }
protected:
	virtual void dispose(BasePair& pair);
};

#endif	//HASHMAP_H
