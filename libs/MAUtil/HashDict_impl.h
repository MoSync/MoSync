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

/** \file HashDict_impl.h
* \brief HashDict implementation
*/

#ifndef _SE_MSAB_MAUTIL_HASHDICT_H_
#error Do not include this file directly.
#endif

//******************************************************************************
// HashDict
//******************************************************************************

template<class Key, class Storage>
MAUtil::HashDict<Key, Storage>::HashNode::HashNode(Storage p)
: data(p)
{
	memset((hnode_t*)this, 0, sizeof(hnode_t));
}

template<class Key, class Storage>
MAUtil::HashDict<Key, Storage>::HashDict(int keyOffset, HashFunction hf,
	CompareFunction cf, int init_bits)
{
	mKeyOffset = keyOffset;

	int init_size = 1 << init_bits;
	hnode_t** table = (hnode_t**)malloc(sizeof(hnode_t*) * init_size);
	hash_init(&mHash, HASHCOUNT_T_MAX, (hash_comp_t)cf, (hash_fun_t)hf, table, init_size);
	mHash.hash_dynamic = 1;
	mHash.hash_highmark = init_size * 2;
	mHash.hash_lowmark = init_size / 2;
	hash_set_allocator(&mHash, &alloc, &free, this);
}

template<class Key, class Storage>
MAUtil::HashDict<Key, Storage>::HashDict(const HashDict& o) {
	//setup for clear()
	mHash.hash_table = NULL;
	mHash.hash_nchains = 0;
	operator=(o);
}

template<class Key, class Storage>
MAUtil::HashDict<Key, Storage>& MAUtil::HashDict<Key, Storage>::operator=(const HashDict& o) {
	clear();
	::free(mHash.hash_table);
	mHashFunction = o.mHashFunction;
	mKeyOffset = o.mKeyOffset;
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
				char* ptr = (char*)&newNode->data;
				newNode->hash_key = ptr + mKeyOffset;
				hash_clone_insert(&mHash, newNode);
				oldNode = (HashNode*)oldNode->hash_next;
			}
		}
	}
	return *this;
}

template<class Key, class Storage>
MAUtil::HashDict<Key, Storage>::~HashDict() {
	clear();
	::free(mHash.hash_table);
}

template<class Key, class Storage>
void MAUtil::HashDict<Key, Storage>::clear() {
	hash_free_nodes(&mHash);
}

template<class Key, class Storage>
MAUtil::Pair<typename MAUtil::HashDict<Key, Storage>::Iterator, bool>
MAUtil::HashDict<Key, Storage>::insert(const Storage& p) {
	Pair<Iterator, bool> pair(Iterator(), false);
	HashNode* newNode = new HashNode(p);
	char* ptr = (char*)&newNode->data;
	HashNode* resNode = (HashNode*)hash_insert(&mHash, newNode, ptr + mKeyOffset);
	if(resNode == NULL) {	//success
		pair.second = true;
		hash_scan_init(&pair.first.mScan, &mHash, newNode);
	} else {	//this key was already in the table
		pair.second = false;
		hash_scan_init(&pair.first.mScan, &mHash, resNode);
	}
	return pair;
}

template<class Key, class Storage>
typename MAUtil::HashDict<Key, Storage>::Iterator
MAUtil::HashDict<Key, Storage>::find(const Key& key) {
	Iterator itr;
	hnode_t* node = hash_lookup(&mHash, &key);
	hash_scan_init(&itr.mScan, &mHash, node);
	return itr;
}

template<class Key, class Storage>
typename MAUtil::HashDict<Key, Storage>::ConstIterator
MAUtil::HashDict<Key, Storage>::find(const Key& key) const {
	//call the non-const function. it's safe, promise :)
	return ((HashDict*)this)->find(key);
}

template<class Key, class Storage>
bool MAUtil::HashDict<Key, Storage>::erase(const Key& key) {
	hnode_t* node = hash_lookup(&mHash, &key);
	if(node == NULL)
		return false;
	hash_delete_free(&mHash, node);
	return true;
}

template<class Key, class Storage>
void MAUtil::HashDict<Key, Storage>::erase(Iterator itr) {
	hash_scan_delete(&mHash, itr.mScan.hash_next);
}

template<class Key, class Storage>
size_t MAUtil::HashDict<Key, Storage>::size() const {
	return hash_count((hash_t*)&mHash);
}

template<class Key, class Storage>
typename MAUtil::HashDict<Key, Storage>::Iterator MAUtil::HashDict<Key, Storage>::begin() {
	Iterator itr;
	hash_scan_begin(&itr.mScan, &mHash);
	return itr;
}
template<class Key, class Storage>
typename MAUtil::HashDict<Key, Storage>::ConstIterator MAUtil::HashDict<Key, Storage>::begin() const {
	ConstIterator itr;
	hash_scan_begin(&itr.mScan, (hash_t*)&mHash);
	return itr;
}

template<class Key, class Storage>
typename MAUtil::HashDict<Key, Storage>::Iterator MAUtil::HashDict<Key, Storage>::end() {
	Iterator itr;
	itr.mScan.hash_table = &mHash;
	itr.mScan.hash_next = NULL;
	return itr;
}
template<class Key, class Storage>
typename MAUtil::HashDict<Key, Storage>::ConstIterator MAUtil::HashDict<Key, Storage>::end() const {
	ConstIterator itr;
	itr.mScan.hash_table = (hash_t*)&mHash;
	itr.mScan.hash_next = NULL;
	return itr;
}

//******************************************************************************
// Iterator
//******************************************************************************

template<class Key, class Storage>
MAUtil::HashDict<Key, Storage>::Iterator::Iterator() {
}

template<class Key, class Storage>
MAUtil::HashDict<Key, Storage>::Iterator::Iterator(const Iterator& o) : mScan(o.mScan) {
}

template<class Key, class Storage>
typename MAUtil::HashDict<Key, Storage>::Iterator&
MAUtil::HashDict<Key, Storage>::Iterator::operator=(const Iterator& o) {
	mScan = o.mScan;
	return *this;
}

template<class Key, class Storage>
Storage&
MAUtil::HashDict<Key, Storage>::Iterator::operator*() {
	MAASSERT(mScan.hash_next != NULL);
	return ((HashNode*)mScan.hash_next)->data;
}

template<class Key, class Storage>
Storage*
MAUtil::HashDict<Key, Storage>::Iterator::operator->() {
	MAASSERT(mScan.hash_next != NULL);
	return &((HashNode*)mScan.hash_next)->data;
}

template<class Key, class Storage>
typename MAUtil::HashDict<Key, Storage>::Iterator&
MAUtil::HashDict<Key, Storage>::Iterator::operator++() {
	hash_scan_next(&mScan);
	return *this;
}

template<class Key, class Storage>
typename MAUtil::HashDict<Key, Storage>::Iterator
MAUtil::HashDict<Key, Storage>::Iterator::operator++(int) {
	Iterator old = *this;
	++*this;
	return old;
}

template<class Key, class Storage>
bool MAUtil::HashDict<Key, Storage>::Iterator::operator==(const Iterator& o) const {
	return mScan.hash_next == o.mScan.hash_next;
}

template<class Key, class Storage>
bool MAUtil::HashDict<Key, Storage>::Iterator::operator!=(const Iterator& o) const {
	return mScan.hash_next != o.mScan.hash_next;
}

//******************************************************************************
// ConstIterator
//******************************************************************************

template<class Key, class Storage>
MAUtil::HashDict<Key, Storage>::ConstIterator::ConstIterator() {
}

template<class Key, class Storage>
MAUtil::HashDict<Key, Storage>::ConstIterator::ConstIterator(const ConstIterator& o) : mScan(o.mScan) {
}

template<class Key, class Storage>
MAUtil::HashDict<Key, Storage>::ConstIterator::ConstIterator(const Iterator& o) : mScan(o.mScan) {
}

template<class Key, class Storage>
typename MAUtil::HashDict<Key, Storage>::ConstIterator&
MAUtil::HashDict<Key, Storage>::ConstIterator::operator=(const ConstIterator& o) {
	mScan = o.mScan;
	return *this;
}

template<class Key, class Storage>
const Storage&
MAUtil::HashDict<Key, Storage>::ConstIterator::operator*() const {
	MAASSERT(mScan.hash_next != NULL);
	return ((HashNode*)mScan.hash_next)->data;
}

template<class Key, class Storage>
const Storage*
MAUtil::HashDict<Key, Storage>::ConstIterator::operator->() const {
	MAASSERT(mScan.hash_next != NULL);
	return &((HashNode*)mScan.hash_next)->data;
}

template<class Key, class Storage>
typename MAUtil::HashDict<Key, Storage>::ConstIterator&
MAUtil::HashDict<Key, Storage>::ConstIterator::operator++() {
	hash_scan_next(&mScan);
	return *this;
}

template<class Key, class Storage>
typename MAUtil::HashDict<Key, Storage>::ConstIterator
MAUtil::HashDict<Key, Storage>::ConstIterator::operator++(int) {
	ConstIterator old = *this;
	++*this;
	return old;
}

template<class Key, class Storage>
bool MAUtil::HashDict<Key, Storage>::ConstIterator::operator==(const ConstIterator& o) const {
	return mScan.hash_next == o.mScan.hash_next;
}

template<class Key, class Storage>
bool MAUtil::HashDict<Key, Storage>::ConstIterator::operator!=(const ConstIterator& o) const {
	return mScan.hash_next != o.mScan.hash_next;
}
