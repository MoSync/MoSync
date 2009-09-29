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

#ifndef HASHMAP_H
#define HASHMAP_H

#include <e32base.h>
#include <stringpool.h>

#include <helpers/types.h>

#define HASHMAP_BASE_SIZE 16

class HashMapBase {
public:
	struct BasePair {
		int key;
		TAny* value;
	};

public:
	//Leaves if key already exists. Make sure the value is on the cleanup stack.
	void InsertL(int key, TAny* value, bool rehash = false);

	TAny* FindL(int key) const;	//Returns NULL if key does not exist.
	void EraseL(int key);	//Leaves if key does not exist.
	void Close();	//Must be called before destruction, or BFE will occur.
	uint Size() const;	//Returns the number of pairs in this map.
	
	//Invalidated by rehashes, which can be caused by InsertL.
	class TIteratorC {
	public:
		bool HasMore() const;
		const BasePair& Next();
	protected:
		const BasePair* mPos;
		const BasePair* const mEnd;
		
		friend class HashMapBase;
		TIteratorC(const BasePair* pos, const BasePair* end);
		void Proceed();
	};
	
	TIteratorC Begin() const {
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

	void RehashL();
	BasePair* FindIndexL(int key, uint& statLinearSteps) const;
	void UpdateBaseLen();	//updates baseLen and loadLimit

protected:
	virtual void Dispose(BasePair& pair) = 0;
};

enum HashMapError {
	KHMEAlready = 42,
	KHMENoExist,
	KHMEUninitialized
};

template<class T> class HashMap : protected HashMapBase {
public:
	struct Pair {
		uint key;
		T* value;
	};

	void InsertL(int key, T* value) { HashMapBase::InsertL(key, value); }
	T* FindL(int key) const { return (T*)HashMapBase::FindL(key); }
	void EraseL(int key) { HashMapBase::EraseL(key); }
	void Close() { HashMapBase::Close(); }
	uint Size() const { return HashMapBase::Size(); }
protected:
	virtual void Dispose(BasePair& pair) { delete (T*)pair.value; }
};

class StringMap : protected HashMap<const char> {
public:
	//returns the value
	const char* InsertL(int key, const TDesC8& s1);
	const char* FindL(int key) const { return HashMap<const char>::FindL(key); }

	void EraseL(int key) { HashMap<const char>::EraseL(key); }
	void Close() { HashMap<const char>::Close(); }
	uint Size() const { return HashMap<const char>::Size(); }
protected:
	virtual void Dispose(BasePair& pair);
};

class String2Map : protected HashMap<HBufC8> {
public:
	struct Pair {
		const TDesC8& key;
		const TDesC8& value;
	};
	
	class TIteratorC : protected HashMapBase::TIteratorC {
	public:
		bool HasMore() const { return HashMapBase::TIteratorC::HasMore(); }
		Pair Next();
	protected:
		friend class String2Map;
		TIteratorC(const RStringPool& pool, HashMapBase::TIteratorC baseItr)
		: HashMapBase::TIteratorC(baseItr), mPool(pool) {}
		
		const RStringPool& mPool;
	};
	
	TIteratorC Begin() { return TIteratorC(mPool, HashMapBase::Begin()); }

	void OpenL();	//call before doing anything else

	//returns the value
	const TDesC8& InsertL(const TDesC8& key, const TDesC8& value);
	const TDesC8* FindL(const TDesC8& key) const;

	void EraseL(const TDesC8& key);
	void Close();
	uint Size() const { return HashMap<HBufC8>::Size(); }
protected:
	virtual void Dispose(BasePair& pair);
private:
	RStringPool mPool;
};

#endif	//HASHMAP_H
