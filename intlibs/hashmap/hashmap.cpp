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

#include <limits.h>
#include <string.h>
#include "hashmap.h"

#define CONFIG_H	//hack
//#define LOGGING_ENABLED
#include <helpers/helpers.h>

#ifdef SYMBIAN
#include <e32base.h>
#define LOG_HASH(a...) LOGD(a)
#define malloc User::AllocL
#define free User::Free
#define PUSH CleanupStack::PushL
#define POPD CleanupStack::PopAndDestroy
#define POP CleanupStack::Pop
#else
#define LOG_HASH LOGD
#define PUSH(a)
#define POPD(a)
#define POP(a)
#endif

using namespace MoSyncError;

//******************************************************************************
//The hash function
//******************************************************************************

static uint hash32shift(uint key) {
	key = key + ~(key << 15); // key = key - (key << 15) - 1;
	key = key ^ (key >> 10);
	key = key + (key << 3);
	key = key ^ (key >> 6);
	key = key * 16389; // key = (key + (key << 2)) + (key << 14);
	key = key ^ (key >> 16);
	return key;
}

//******************************************************************************
//HashMapBase
//******************************************************************************

HashMapBase::HashMapBase() {
	memset(&m, 0, sizeof(m));
}

HashMapBase::~HashMapBase() {
	DEBUG_ASSERT(m.base == NULL);
}

uint HashMapBase::size() const {
	return m.numElem;
}

void HashMapBase::close() {
	if(m.base == NULL)
		return;

	LOG_HASH("HashMap statistics:\n");
#define DUMPSTAT(i) LOG_HASH("%s: %i\n", #i, m.stat.i)

	LOG_HASH("Elements left: %i\n", m.numElem);
	DUMPSTAT(rehashes);
	DUMPSTAT(inserts);
	DUMPSTAT(rehashInserts);
	DUMPSTAT(linearInsertSteps);
	DUMPSTAT(insertHashDupes);
	DUMPSTAT(finds);
	DUMPSTAT(linearFindSteps);
	LOG_HASH("projected erases: %i\n", m.stat.inserts - m.numElem);
	DUMPSTAT(erases);	//should match the projected
	DUMPSTAT(linearEraseSteps);
	DUMPSTAT(getsArray);
	LOG_HASH("\n");

	for(uint i=0; i<m.baseLen; i++) {
		BasePair& pair = m.base[i];
		if(pair.value) {
			LOG_HASH("Disposing of key %i\n", pair.key);
			this->dispose(pair);
		}
	}
	free(m.base);
	m.base = NULL;
	//DebugMarkEnd();
}

void HashMapBase::insert(int key, void* value, bool in_rehash) {
	LOG_HASH("Insert %i\n", key);
	DEBUG_ASSERT(value != NULL);
	if(m.base == NULL) {
		DEBUG_ASSERT(sizeof(BasePair) == 8);

		//requested Size = (Length * sizeof)
		m.base = (BasePair*)malloc(HASHMAP_BASE_SIZE * sizeof(BasePair));

		updateBaseLen();
	}
	if(m.numElem >= m.loadLimit) {
		rehash();
	}

	(in_rehash ? m.stat.rehashInserts : m.stat.inserts) ++;

	uint baseMask = m.baseLen - 1;	//broken if !isPowerOf2(m.baseLen)
	uint hash = hash32shift(key);
	uint index = hash & baseMask;

	if(m.base[index].value != NULL) {	//another element has this hash value
		uint newIndex = index;
		do {	//linear search
			m.stat.linearInsertSteps++;
			BasePair& pair(m.base[newIndex]);
			if(pair.value == NULL)
				break;
			if(pair.key == key) {
				//User::Leave(KHMEAlready);
				DEBIG_PHAT_ERROR;
			}
			newIndex++;
			newIndex &= baseMask;
		} while(newIndex != index);
		DEBUG_ASSERT(newIndex != index);
		index = newIndex;
		m.stat.insertHashDupes++;
	}

	LOG_HASH("Insert %i @ %i\n", key, index);
	m.base[index].key = key;
	m.base[index].value = value;
	m.numElem++;
}

void HashMapBase::rehash() {
	m.stat.rehashes++;
	LOG_HASH("Rehash\n");

	DEBUG_ASSERT(sizeof(BasePair) == 8);
	BasePair* oldBase = m.base;
	PUSH(oldBase);
	uint oldLen = m.baseLen;
	uint oldNumElem = m.numElem;
	m.baseLen <<= 1;
	m.base = (BasePair*)malloc(m.baseLen * sizeof(BasePair));
	updateBaseLen();
	m.numElem = 0;

	for(uint i=0; i<oldLen; i++) {
		BasePair& pair(oldBase[i]);
		if(pair.value != NULL) {
			insert(pair.key, pair.value, true);
		}			
	}
	POPD(oldBase);
	DEBUG_ASSERT(m.numElem == oldNumElem);
}

void HashMapBase::updateBaseLen() {
	memset(m.base, 0, m.baseLen * sizeof(BasePair));
	LOG_HASH("%i Pairs\n", m.baseLen);
	MYASSERT_EXTRA(
		/*isPowerOf2(m.baseLen) &&*/	//not always true
		m.baseLen > 4 && m.baseLen < (INT_MAX / 3),	//sanity check
		ERR_INTERNAL,
		DUMPINT(m.baseLen));
	m.loadLimit = (m.baseLen * 3) >> 2;	//75%
}

void* HashMapBase::find(int key) const {
	m.stat.finds++;
	const BasePair* pair(findIndex(key, m.stat.linearFindSteps));
	if(pair)
		return pair->value;
	else
		return NULL;
}

void HashMapBase::erase(int key) {
	m.stat.erases++;
	LOG_HASH("Erase %i\n", key);
	BasePair* pair(findIndex(key, m.stat.linearEraseSteps));
	DEBUG_ASSERT(pair);
	dispose(*pair);
	pair->value = NULL;
	m.numElem--;
}

HashMapBase::BasePair* HashMapBase::findIndex(int key, uint& statLinearSteps) const {
	if(m.base == NULL)
		return NULL;
	int baseMask = m.baseLen - 1;
	int baseIndex = hash32shift(key) & baseMask;
	int index = baseIndex;
	//assumes that the map isn't smack full,
	//that is, at least one NULL will "break our fall".
	while(true) {
		BasePair* pair = m.base + index;
		if(pair->key == key) {
			return pair;
		}
		index++;
		index &= baseMask;
		statLinearSteps++;
		//search around the entire map
		if(index == baseIndex) {
			return NULL;
		}
	}
}

#if 0
void HashMapBase::dispose(BasePair&) {
	DEBIG_PHAT_ERROR;
}
#endif

//******************************************************************************
//HashMapBase::TIteratorC
//******************************************************************************

HashMapBase::TIteratorC::TIteratorC(const BasePair* pos, const BasePair* end)
: mPos(pos), mEnd(end)
{
	if(mPos->value == NULL)
		proceed();
}

void HashMapBase::TIteratorC::proceed() {
	while(mPos != mEnd) {
		mPos++;
		if(mPos->value != NULL)
			break;
	}
}

bool HashMapBase::TIteratorC::hasMore() const {
	if(mPos == mEnd)
		return false;
	return mPos->value != NULL;
}

const HashMapBase::BasePair& HashMapBase::TIteratorC::next() {
	const BasePair& result = *mPos;
	proceed();
	return result;
}

//******************************************************************************
//StringMap
//******************************************************************************

void StringMap::dispose(BasePair& pair) {
	free(pair.value);
}

const char* StringMap::insert(int key, const char* s1, uint len) {
	char* value = (char*)malloc(len + 1);
	PUSH(value);
	memcpy(value, s1, len);
	value[len] = 0;
	HashMapBase::insert(key, value);
	POP(value);
	return value;
}
