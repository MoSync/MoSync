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

#include <ma.h>
#include <maheap.h>
#include <maassert.h>
#include "HashMap.h"

using namespace MAUtil;

int MAUtil::integralHash(const void* data, int size) {
	int key = *(int*)data;
	key = key + ~(key << 15); // key = key - (key << 15) - 1;
	key = key ^ (key >> 10);
	key = key + (key << 3);
	key = key ^ (key >> 6);
	key = key * 16389; // key = (key + (key << 2)) + (key << 14);
	key = key ^ (key >> 16);
	return key;
}

static int hashBytes(int hash, const char* data, int size) {
	while(size > 0)
		hash = (hash<<4)^(hash>>28)^data[size--];
	return hash;
}

int MAUtil::memHash(const void* data, int size) {
	int hash = 0x9e3779b9;
	{
		int byteAddr = (int)data & 3;
		if(byteAddr != 0) {
			int nBytes = 4 - byteAddr;
			hash = hashBytes(hash, (const char*)data, nBytes);
			(*(int*)&(data)) += nBytes;
			size -= nBytes;
		}
	}
	const int* d4 = (const int*)data;
	const int* d4end = d4 + (size & ~3);
	while(d4 < d4end) {
		hash = (hash<<4)^(hash>>28)^(*(d4++));
	}
	size &= 3;
	return hashBytes(hash, (const char*)d4end, size);
}

int MAUtil::integralCompare(const void* a, const void* b, int /*size*/) {
	return *(int*)a != *(int*)b;
}

//Invalidated by rehashes, which can be caused by insert.
/*class ConstIterator {
	bool hasMore() const;
	const BasePair& next();
	ConstIterator(const BasePair* pos, const BasePair* end);
	void proceed();
};*/

#if 0
template<class T> T& ref(int index) {
	return (*(type*)((byte*)m.base + mNodeSize * (index)));
}
#else
#define BREF(type, index) (*(type*)((byte*)m.base + mNodeSize * (index)))
#endif

int findNearestBase2(int val) {
	int i = 1;
	while(i<val) {
		i<<=1;
	}
	return i;
}

HashMapImpl::HashMapImpl(int keySize, int valueSize, HashFunctionPointer hashFunc,
												 CompareFunctionPointer compareFunc,
												 DisposeFunctionPointer disposeFunc,
												 CopyFunctionPointer copyFunc,
												 int initialCapacity, double loadFactor) :
mLoadFactor(loadFactor), mHashFunction(hashFunc),
mCompareFunction(compareFunc), mDisposeFunction(disposeFunc), mCopyFunction(copyFunc),
mKeySize(keySize), mValueSize(valueSize)
#if 1
, mNodeSize(sizeof(void*) + keySize + valueSize)
#endif
{
	MAASSERT(keySize > 0);
	MAASSERT(valueSize >= 0);
	MAASSERT(initialCapacity > 0);
	MAASSERT(loadFactor > 0);

	memset(&m, 0, sizeof(m));

#if 1
	m.capacity = findNearestBase2(initialCapacity);

	m.base = (BaseNode**)calloc(m.capacity, mNodeSize);
	MAASSERT(m.base);

	m.loadLimit = int(m.capacity * mLoadFactor);
#else
	mVector.resize(initialCapacity);
	memset(mVector.pointer(), 0, initialCapacity * sizeof(BaseNode));
	m.loadLimit = int(mVector.size() * mLoadFactor);
#endif
}

//Returns false if key already exists.
bool HashMapImpl::insert(void* key, void* value) {
	MAASSERT(value != NULL);
	if(m.count >= m.loadLimit) {
		rehash();
	}
	unsigned int hash = mHashFunction(key, mKeySize);
	int index = hash % (unsigned int) m.capacity;
	BaseNode*& node = m.base[index];
	while(node) {
		if(mCompareFunction(node->pair, key, mKeySize) == 0)
			return false;
		node = node->next;
	};
	node = (BaseNode*)malloc(mNodeSize);
	mCopyFunction(node->pair, key, value);
	node->next = NULL;
	m.count++;
	return true;
}

//Returns NULL if key does not exist.
HashMapImpl::BaseConstIterator HashMapImpl::find(const void* key) const {
	unsigned int hash = mHashFunction(key, mKeySize);
	int index = hash % (unsigned int) m.capacity;
	BaseNode* const * node = &m.base[index];
	while(*node) {
		if(mCompareFunction((*node)->pair, key, mKeySize) == 0)
			return BaseConstIterator(this, node, index);
		node = &((*node)->next);
	};
	return end();
}

//Invalidates all iterators that point to the erased element.
//Make an incremented copy if you want to continue iteration.
//erase(itr++) would work.
void HashMapImpl::erase(BaseIterator itr) {
	mDisposeFunction((*itr.node)->pair);
#if 0
	if(itr.prev) {	//the iterator points to an allocated node
		//delete this node
		itr.prev->next = itr.cur->next;
		delete itr.cur;
	} else if(itr.cur->next) {	//there is an allocated node attached to the erased one
		//copy that node to this, then delete it
		*itr.cur = *itr.cur->next;
		delete itr.cur->next;
	} else {
		//mark this node unused
		itr.cur->pair.value = NULL;
	}
#endif
	BaseNode* next = (*itr.node)->next;
	free(*itr.node);
	*itr.node = next;

	m.count--;
}

void HashMapImpl::rehash() {
	BIG_PHAT_ERROR;
}

void HashMapImpl::reset() {
	BaseIterator itr(begin());
	while(itr != end()) {
		erase(itr++);
	}
#if 0
	for(int i=0; i<mVector.size(); i++) {
		BaseNode* node = mVector[i];
		while(node) {
			mDisposeFunction(pair);
			node = node->next;
		}
	}
#endif
}

void HashMapImpl::IteratorImpl::proceed() {
	if(*node == NULL) while(*node == NULL) {
		index++;
		node = &impl->m.base[index];
	} else {
		node = &(*node)->next;
	}
}

HashMapImpl::~HashMapImpl() {
	reset();

#ifdef MAUTIL_HASHMAP_STATS
	LOG("HashMap statistics:\n");
#define DUMPSTAT(i) LOG("%s: %i\n", #i, m.stat.i)

	LOG("Elements left: %i\n", m.numElem);
	DUMPSTAT(rehashes);
	DUMPSTAT(inserts);
	DUMPSTAT(rehashInserts);
	DUMPSTAT(linearInsertSteps);
	DUMPSTAT(insertHashDupes);
	DUMPSTAT(finds);
	DUMPSTAT(linearFindSteps);
	LOG("projected erases: %i\n", m.stat.inserts - m.numElem);
	DUMPSTAT(erases);	//should match the projected
	DUMPSTAT(linearEraseSteps);
	DUMPSTAT(getsArray);
	LOG("\n");
#endif	//MAUTIL_HASHMAP_STATS
}
