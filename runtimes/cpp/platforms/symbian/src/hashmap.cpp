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

#include <LIMITS.H>
#include "config_platform.h"
#include "hashmap.h"
#include "symbian_helpers.h"

#define LOG_HASH(a...) LOGD(a)

//******************************************************************************
//The hash function
//******************************************************************************

uint hash32shift(unsigned int key) {
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
	ZERO_OBJECT(m);
	//DebugMarkStart();
}

HashMapBase::~HashMapBase() {
	DEBUG_ASSERT(m.base == NULL)
}

uint HashMapBase::Size() const {
	return m.numElem;
}

void HashMapBase::Close() {
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
			this->Dispose(pair);
		}
	}
	User::Free(m.base);
	m.base = NULL;
	//DebugMarkEnd();
}

void HashMapBase::InsertL(int key, TAny* value, bool rehash) {
	LOG_HASH("Insert %i\n", key);
	DEBUG_ASSERT(value != NULL);
	if(m.base == NULL) {
		DEBUG_ASSERT(sizeof(BasePair) == 8);

		//requested Size = (Length * sizeof)
		m.base = (BasePair*)User::AllocL(HASHMAP_BASE_SIZE * sizeof(BasePair));

		UpdateBaseLen();
	}
	if(m.numElem >= m.loadLimit) {
		RehashL();
	}

	(rehash ? m.stat.rehashInserts : m.stat.inserts) ++;

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

void HashMapBase::RehashL() {
	m.stat.rehashes++;
	LOG_HASH("Rehash\n");

	DEBUG_ASSERT(sizeof(BasePair) == 8);
	BasePair* oldBase = m.base;
	CleanupStack::PushL(oldBase);
	uint oldLen = m.baseLen;
	uint oldNumElem = m.numElem;
	m.base = (BasePair*)User::AllocL(m.baseLen * 2 * sizeof(BasePair));
	UpdateBaseLen();
	m.numElem = 0;
	
	for(uint i=0; i<oldLen; i++) {
		BasePair& pair(oldBase[i]);
		if(pair.value != NULL) {
			InsertL(pair.key, pair.value, true);
		}			
	}
	CleanupStack::PopAndDestroy(oldBase);
	DEBUG_ASSERT(m.numElem == oldNumElem);
}

void HashMapBase::UpdateBaseLen() {
	uint nBytes = User::AllocLen(m.base);	//allocated Size (bytes)
	LOG_HASH("%i bytes\n", nBytes);
	Mem::FillZ(m.base, nBytes);
	m.baseLen = nBytes >> 3;	//allocated Length (number of Pairs)
	LOG_HASH("%i Pairs\n", m.baseLen);
	m.baseLen = biggestPowerOf2(4, m.baseLen);
	MYASSERT_EXTRA(
		/*isPowerOf2(m.baseLen) &&*/	//not always true
		m.baseLen > 4 && m.baseLen < (INT_MAX / 3),	//sanity check
		ERR_INTERNAL,
		DUMPINT(m.baseLen));
	m.loadLimit = (m.baseLen * 3) >> 2;	//75%
}

TAny* HashMapBase::FindL(int key) const {
	m.stat.finds++;
	const BasePair* pair(FindIndexL(key, m.stat.linearFindSteps));
	if(pair)
		return pair->value;
	else
		return NULL;
}

void HashMapBase::EraseL(int key) {
	m.stat.erases++;
	LOG_HASH("Erase %i\n", key);
	BasePair* pair(FindIndexL(key, m.stat.linearEraseSteps));
	DEBUG_ASSERT(pair);
	Dispose(*pair);
	pair->value = NULL;
	m.numElem--;
}

HashMapBase::BasePair* HashMapBase::FindIndexL(int key, uint& statLinearSteps) const {
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

void HashMapBase::Dispose(BasePair&) {
	DEBIG_PHAT_ERROR;
}

//******************************************************************************
//HashMapBase::TIteratorC
//******************************************************************************

HashMapBase::TIteratorC::TIteratorC(const BasePair* pos, const BasePair* end)
	: mPos(pos), mEnd(end)
{
	if(mPos->value == NULL)
		Proceed();
}

void HashMapBase::TIteratorC::Proceed() {
	while(mPos != mEnd) {
		mPos++;
		if(mPos->value != NULL)
			break;
	}
}

bool HashMapBase::TIteratorC::HasMore() const {
	if(mPos == mEnd)
		return false;
	return mPos->value != NULL;
}

const HashMapBase::BasePair& HashMapBase::TIteratorC::Next() {
	const BasePair& result = *mPos;
	Proceed();
	return result;
}

//******************************************************************************
//StringMap
//******************************************************************************

void StringMap::Dispose(BasePair& pair) {
	User::Free(pair.value);
}

const char* StringMap::InsertL(int key, const TDesC8& s1) {
	char* value = (char*)User::AllocLC(s1.Size() + 1);
	Mem::Copy(value, s1.Ptr(), s1.Size());
	value[s1.Size()] = 0;
	HashMapBase::InsertL(key, value);
	CleanupStack::Pop(value);
	return value;
}

//******************************************************************************
//String2Map
//******************************************************************************

void String2Map::OpenL() {
	mPool.OpenL();
}

void String2Map::Close() {
	HashMap<HBufC8>::Close();
	mPool.Close();
}

void String2Map::Dispose(BasePair& pair) {
	RMyString string(mPool, pair.key);
	string.Close();
	delete (HBufC8*)pair.value;
}

const TDesC8& String2Map::InsertL(const TDesC8& key, const TDesC8& value) {
	HBufC8* buf = HBufC8::NewLC(value.Length());
	*buf = value;
	RMyString string(mPool.OpenStringL(key));
	CleanupClosePushL(string);
	HashMap<HBufC8>::InsertL(string.Val(), buf);
	CleanupStack::Pop();	//string
	CleanupStack::Pop(buf);
	return *buf;
}

const TDesC8* String2Map::FindL(const TDesC8& key) const {
	RMyString string(mPool.OpenStringL(key));
	int val = string.Val();
	string.Close();
	return HashMap<HBufC8>::FindL(val);
}

void String2Map::EraseL(const TDesC8& key) {
	RMyString string(mPool.OpenStringL(key));
	int val = string.Val();
	string.Close();
	HashMap<HBufC8>::EraseL(val);
}

//******************************************************************************
//String2Map::TIteratorC
//******************************************************************************

String2Map::Pair String2Map::TIteratorC::Next() {
	Pair result = { RMyString(mPool, mPos->key).DesC(),
		*(HBufC8*)mPos->value };
	Proceed();
	return result;
}
