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
#error Do not include this file directly.
#endif

//******************************************************************************
// THashFunction
//******************************************************************************

template<> hash_val_t MAUtil::THashFunction<MAUtil::String>(const MAUtil::String& s) {
	return hash_fun_default(s.c_str());
}

template<> hash_val_t MAUtil::THashFunction<int>(const int& data) {
	int key = data;
	key = key + ~(key << 15); // key = key - (key << 15) - 1;
	key = key ^ (key >> 10);
	key = key + (key << 3);
	key = key ^ (key >> 6);
	key = key * 16389; // key = (key + (key << 2)) + (key << 14);
	key = key ^ (key >> 16);
	return key;
}

//******************************************************************************
// HashMap
//******************************************************************************

template<class Key, class Value>
MAUtil::HashMap<Key, Value>::HashNode::HashNode() {
	memset((hnode_t*)this, 0, sizeof(hnode_t));
}

template<class Key, class Value>
MAUtil::HashMap<Key, Value>::HashMap(HashFunction hf, CompareFunction cf, int init_bits) {
	int init_size = 1 << init_bits;
	hnode_t** table = (hnode_t**)malloc(sizeof(hnode_t*) * init_size);
	hash_init(&mHash, HASHCOUNT_T_MAX, (hash_comp_t)cf, (hash_fun_t)hf, table, init_size);
	mHash.hash_dynamic = 1;
	mHash.hash_highmark = init_size * 2;
	mHash.hash_lowmark = init_size / 2;
	hash_set_allocator(&mHash, &alloc, &free, this);
}

template<class Key, class Value>
MAUtil::HashMap<Key, Value>::HashMap(const HashMap& o) {
	//setup for clear()
	mHash.hash_table = NULL;
	mHash.hash_nchains = 0;
	operator=(o);
}

template<class Key, class Value>
MAUtil::HashMap<Key, Value>& MAUtil::HashMap<Key, Value>::operator=(const HashMap& o) {
	clear();
	::free(mHash.hash_table);
	mHash = o.mHash;
	mHash.hash_table = (hnode_t**)calloc(mHash.hash_nchains, sizeof(hnode_t*));
	mHash.hash_nodecount = 0;
	for(hashcount_t i=0; i<mHash.hash_nchains; i++) {
		const HashNode* oldNode = (HashNode*)o.mHash.hash_table[i];
		if(oldNode == NULL) {
			mHash.hash_table[i] = NULL;
		} else {
			while(oldNode) {
				HashNode* newNode = new HashNode(*oldNode);
				newNode->hash_key = &newNode->data.first;
				hash_clone_insert(&mHash, newNode);
				oldNode = (HashNode*)oldNode->hash_next;
			}
		}
	}
	return *this;
}

template<class Key, class Value>
MAUtil::HashMap<Key, Value>::~HashMap() {
	clear();
	::free(mHash.hash_table);
}

template<class Key, class Value>
void MAUtil::HashMap<Key, Value>::clear() {
	hash_free_nodes(&mHash);
}

template<class Key, class Value>
MAUtil::Pair<typename MAUtil::HashMap<Key, Value>::Iterator, bool>
MAUtil::HashMap<Key, Value>::insert(const Key& key, const Value& value) {
	Pair<Iterator, bool> pair = { Iterator(), false };
	HashNode* newNode = new HashNode();
	newNode->data.first = key;
	HashNode* resNode = (HashNode*)hash_insert(&mHash, newNode, &newNode->data.first);
	if(resNode == NULL) {	//success
		pair.second = true;
		hash_scan_init(&pair.first.mScan, &mHash, newNode);
		newNode->data.second = value;
	} else {	//this key was already in the table
		pair.second = false;
		hash_scan_init(&pair.first.mScan, &mHash, resNode);
	}
	return pair;
}

template<class Key, class Value>
typename MAUtil::HashMap<Key, Value>::Iterator
MAUtil::HashMap<Key, Value>::find(const Key& key) {
	Iterator itr;
	hnode_t* node = hash_lookup(&mHash, &key);
	hash_scan_init(&itr.mScan, &mHash, node);
	return itr;
}

template<class Key, class Value>
typename MAUtil::HashMap<Key, Value>::ConstIterator
MAUtil::HashMap<Key, Value>::find(const Key& key) const {
	//call the non-const function. it's safe, promise :)
	return ((HashMap*)this)->find(key);
}

template<class Key, class Value>
bool MAUtil::HashMap<Key, Value>::erase(const Key& key) {
	hnode_t* node = hash_lookup(&mHash, &key);
	if(node == NULL)
		return false;
	hash_delete_free(&mHash, node);
	return true;
}

template<class Key, class Value>
void MAUtil::HashMap<Key, Value>::erase(Iterator itr) {
	hash_scan_delete(&mHash, itr.mScan.hash_next);
}

template<class Key, class Value>
size_t MAUtil::HashMap<Key, Value>::size() const {
	return hash_count(&mHash);
}

template<class Key, class Value>
typename MAUtil::HashMap<Key, Value>::Iterator MAUtil::HashMap<Key, Value>::begin() {
	Iterator itr;
	hash_scan_begin(&itr.mScan, &mHash);
	return itr;
}
template<class Key, class Value>
typename MAUtil::HashMap<Key, Value>::ConstIterator MAUtil::HashMap<Key, Value>::begin() const {
	ConstIterator itr;
	hash_scan_begin(&itr.mScan, &mHash);
	return itr;
}

template<class Key, class Value>
typename MAUtil::HashMap<Key, Value>::Iterator MAUtil::HashMap<Key, Value>::end() {
	Iterator itr;
	itr.mScan.hash_table = &mHash;
	itr.mScan.hash_next = NULL;
	return itr;
}
template<class Key, class Value>
typename MAUtil::HashMap<Key, Value>::ConstIterator MAUtil::HashMap<Key, Value>::end() const {
	ConstIterator itr;
	itr.mScan.hash_table = &mHash;
	itr.mScan.hash_next = NULL;
	return itr;
}

template<class Key, class Value>
Value& MAUtil::HashMap<Key, Value>::operator[](const Key& key) {
	HashNode* node = (HashNode*)hash_lookup(&mHash, &key);
	if(node == NULL) {
		node = new HashNode();
		node->data.first = key;
		hash_insert(&mHash, node, &node->data.first);
	}
	return node->data.second;
}

//******************************************************************************
// Iterator
//******************************************************************************

template<class Key, class Value>
MAUtil::HashMap<Key, Value>::Iterator::Iterator() {
}

template<class Key, class Value>
MAUtil::HashMap<Key, Value>::Iterator::Iterator(const Iterator& o) : mScan(o.mScan) {
}

template<class Key, class Value>
typename MAUtil::HashMap<Key, Value>::Iterator&
MAUtil::HashMap<Key, Value>::Iterator::operator=(const Iterator& o) {
	mScan = o.mScan;
	return *this;
}

template<class Key, class Value>
typename MAUtil::HashMap<Key, Value>::PairKV&
MAUtil::HashMap<Key, Value>::Iterator::operator*() {
	MAASSERT(mScan.hash_next != NULL);
	return ((HashNode*)mScan.hash_next)->data;
}

template<class Key, class Value>
typename MAUtil::HashMap<Key, Value>::PairKV*
MAUtil::HashMap<Key, Value>::Iterator::operator->() {
	MAASSERT(mScan.hash_next != NULL);
	return &((HashNode*)mScan.hash_next)->data;
}

template<class Key, class Value>
typename MAUtil::HashMap<Key, Value>::Iterator&
MAUtil::HashMap<Key, Value>::Iterator::operator++() {
	hash_scan_next(&mScan);
	return *this;
}

template<class Key, class Value>
bool MAUtil::HashMap<Key, Value>::Iterator::operator==(const Iterator& o) const {
	return mScan.hash_next == o.mScan.hash_next;
}

template<class Key, class Value>
bool MAUtil::HashMap<Key, Value>::Iterator::operator!=(const Iterator& o) const {
	return mScan.hash_next != o.mScan.hash_next;
}

//******************************************************************************
// ConstIterator
//******************************************************************************

template<class Key, class Value>
MAUtil::HashMap<Key, Value>::ConstIterator::ConstIterator() {
}

template<class Key, class Value>
MAUtil::HashMap<Key, Value>::ConstIterator::ConstIterator(const ConstIterator& o) : mScan(o.mScan) {
}

template<class Key, class Value>
MAUtil::HashMap<Key, Value>::ConstIterator::ConstIterator(const Iterator& o) : mScan(o.mScan) {
}

template<class Key, class Value>
typename MAUtil::HashMap<Key, Value>::ConstIterator&
MAUtil::HashMap<Key, Value>::ConstIterator::operator=(const ConstIterator& o) {
	mScan = o.mScan;
	return *this;
}

template<class Key, class Value>
const typename MAUtil::HashMap<Key, Value>::PairKV&
MAUtil::HashMap<Key, Value>::ConstIterator::operator*() const {
	MAASSERT(mScan.hash_next != NULL);
	return ((HashNode*)mScan.hash_next)->data;
}

template<class Key, class Value>
const typename MAUtil::HashMap<Key, Value>::PairKV*
MAUtil::HashMap<Key, Value>::ConstIterator::operator->() const {
	MAASSERT(mScan.hash_next != NULL);
	return &((HashNode*)mScan.hash_next)->data;
}

template<class Key, class Value>
typename MAUtil::HashMap<Key, Value>::ConstIterator&
MAUtil::HashMap<Key, Value>::ConstIterator::operator++() {
	hash_scan_next(&mScan);
	return *this;
}

template<class Key, class Value>
bool MAUtil::HashMap<Key, Value>::ConstIterator::operator==(const ConstIterator& o) const {
	return mScan.hash_next == o.mScan.hash_next;
}

template<class Key, class Value>
bool MAUtil::HashMap<Key, Value>::ConstIterator::operator!=(const ConstIterator& o) const {
	return mScan.hash_next != o.mScan.hash_next;
}
