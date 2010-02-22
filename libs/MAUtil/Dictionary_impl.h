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

#ifndef _SE_MSAB_MAUTIL_DICTIONARY_H_
#error Do not include this file directly.
#endif

//******************************************************************************
// Dictionary
//******************************************************************************

template<class Key, class Storage>
MAUtil::Dictionary<Key, Storage>::DictNode::DictNode() {
	memset((dnode_t*)this, 0, sizeof(dnode_t));
}

template<class Key, class Storage>
void MAUtil::Dictionary<Key, Storage>::init(CompareFunction cf) {
	dict_init(&mDict, DICTCOUNT_T_MAX, (dict_comp_t)cf);
	dict_set_allocator(&mDict, &alloc, &free, this);
}

template<class Key, class Storage>
MAUtil::Dictionary<Key, Storage>::Dictionary(CompareFunction cf, int keyOffset) {
	mKeyOffset = keyOffset;
	init(cf);
}

template<class Key, class Storage>
MAUtil::Dictionary<Key, Storage>::Dictionary(const Dictionary& o) {
	mKeyOffset = o.mKeyOffset;
	init((CompareFunction)o.mDict.dict_compare);
	operator=(o);
}

template<class Key, class Storage>
MAUtil::Dictionary<Key, Storage>&
MAUtil::Dictionary<Key, Storage>::operator=(const Dictionary& o) {
	clear();
	dict_load_t load;
	dict_load_begin(&load, &mDict);
	ConstIterator itr = o.begin();
	while(itr != o.end()) {
		DictNode* newNode = new DictNode;
		newNode->data = *itr;
		char* ptr = (char*)&newNode->data;
		dict_load_next(&load, newNode, ptr + mKeyOffset);
		++itr;
	}
	dict_load_end(&load);
	return *this;
}

template<class Key, class Storage>
MAUtil::Dictionary<Key, Storage>::~Dictionary() {
	clear();
}

template<class Key, class Storage>
void MAUtil::Dictionary<Key, Storage>::clear() {
	dict_free_nodes(&mDict);
}

template<class Key, class Storage>
MAUtil::Pair<class MAUtil::Dictionary<Key, Storage>::Iterator, bool>
MAUtil::Dictionary<Key, Storage>::insert(const Storage& data) {
	Pair<Iterator, bool> pair = { Iterator(&mDict), false };
	DictNode* newNode = new DictNode;
	dnode_init(newNode, NULL);
	newNode->data = data;
	char* ptr = (char*)&newNode->data;
	Key* kp = (Key*)(ptr + mKeyOffset);
	DictNode* res = (DictNode*)dict_insert(&mDict, newNode, kp);
	if(res != NULL) {	//insert->dupe
		delete newNode;
		pair.first.mNode = res;
		pair.second = false;
	} else {	//insert successful
		pair.first.mNode = newNode;
		pair.second = true;
	}
	return pair;
}

template<class Key, class Storage>
typename MAUtil::Dictionary<Key, Storage>::Iterator
MAUtil::Dictionary<Key, Storage>::begin() {
	Iterator itr(&mDict);
	itr.mNode = (DictNode*)dict_first(&mDict);
	return itr;
}

template<class Key, class Storage>
typename MAUtil::Dictionary<Key, Storage>::ConstIterator
MAUtil::Dictionary<Key, Storage>::begin() const {
	ConstIterator itr(&mDict);
	itr.mNode = (DictNode*)dict_first((dict_t*)&mDict);
	return itr;
}

template<class Key, class Storage>
typename MAUtil::Dictionary<Key, Storage>::Iterator
MAUtil::Dictionary<Key, Storage>::end() {
	Iterator itr(&mDict);
	itr.mNode = NULL;
	return itr;
}

template<class Key, class Storage>
typename MAUtil::Dictionary<Key, Storage>::ConstIterator
MAUtil::Dictionary<Key, Storage>::end() const {
	ConstIterator itr(&mDict);
	itr.mNode = NULL;
	return itr;
}

template<class Key, class Storage>
size_t MAUtil::Dictionary<Key, Storage>::size() const {
	return dict_count((dict_t*)&mDict);
}

template<class Key, class Storage>
bool MAUtil::Dictionary<Key, Storage>::erase(const Key& key) {
	dnode_t* node = dict_lookup(&mDict, &key);
	if(node == NULL)
		return false;
	dict_delete_free(&mDict, node);
	return true;
}

template<class Key, class Storage>
typename MAUtil::Dictionary<Key, Storage>::Iterator
MAUtil::Dictionary<Key, Storage>::find(const Key& key) {
	Iterator itr(&mDict);
	itr.mNode = (DictNode*)dict_lookup(&mDict, &key);
	return itr;
}

template<class Key, class Storage>
typename MAUtil::Dictionary<Key, Storage>::ConstIterator
MAUtil::Dictionary<Key, Storage>::find(const Key& key) const {
	ConstIterator itr(&mDict);
	itr.mNode = (DictNode*)dict_lookup((dict_t*)&mDict, &key);
	return itr;
}

//******************************************************************************
// Iterator
//******************************************************************************

template<class Key, class Storage>
MAUtil::Dictionary<Key, Storage>::Iterator::Iterator(dict_t* dict) :
mNode(NULL), mDict(dict) {}

template<class Key, class Storage>
MAUtil::Dictionary<Key, Storage>::Iterator::Iterator(const Iterator& o) :
mNode(o.mNode), mDict(o.mDict) {}

template<class Key, class Storage>
typename MAUtil::Dictionary<Key, Storage>::Iterator&
MAUtil::Dictionary<Key, Storage>::Iterator::operator=(const Iterator& o) {
	mNode = o.mNode;
	mDict = o.mDict;
	return *this;
}

template<class Key, class Storage>
Storage& MAUtil::Dictionary<Key, Storage>::Iterator::operator*() {
	MAASSERT(mNode != NULL);
	return mNode->data;
}

template<class Key, class Storage>
Storage* MAUtil::Dictionary<Key, Storage>::Iterator::operator->() {
	MAASSERT(mNode != NULL);
	return &mNode->data;
}

template<class Key, class Storage>
typename MAUtil::Dictionary<Key, Storage>::Iterator&
MAUtil::Dictionary<Key, Storage>::Iterator::operator++() {
	MAASSERT(mNode != NULL);
	mNode = (DictNode*)dict_next(mDict, mNode);
	return *this;
}

template<class Key, class Storage>
typename MAUtil::Dictionary<Key, Storage>::Iterator
MAUtil::Dictionary<Key, Storage>::Iterator::operator++(int) {
	Iterator old = *this;
	++*this;
	return old;
}

template<class Key, class Storage>
typename MAUtil::Dictionary<Key, Storage>::Iterator&
MAUtil::Dictionary<Key, Storage>::Iterator::operator--() {
	if(mNode == NULL) {
		mNode = (DictNode*)dict_last(mDict);
		MAASSERT(mNode != NULL);
		return *this;
	}
	mNode = (DictNode*)dict_prev(mDict, mNode);
	return *this;
}

template<class Key, class Storage>
typename MAUtil::Dictionary<Key, Storage>::Iterator
MAUtil::Dictionary<Key, Storage>::Iterator::operator--(int) {
	Iterator old = *this;
	--*this;
	return old;
}

template<class Key, class Storage>
bool MAUtil::Dictionary<Key, Storage>::Iterator::operator==(const Iterator& o) const {
	return mNode == o.mNode;
}

template<class Key, class Storage>
bool MAUtil::Dictionary<Key, Storage>::Iterator::operator!=(const Iterator& o) const {
	return mNode != o.mNode;
}

//******************************************************************************
// ConstIterator
//******************************************************************************

template<class Key, class Storage>
MAUtil::Dictionary<Key, Storage>::ConstIterator::ConstIterator(const dict_t* dict) :
mNode(NULL), mDict(dict) {}

template<class Key, class Storage>
MAUtil::Dictionary<Key, Storage>::ConstIterator::ConstIterator(const ConstIterator& o) :
mNode(o.mNode), mDict(o.mDict) {}

template<class Key, class Storage>
MAUtil::Dictionary<Key, Storage>::ConstIterator::ConstIterator(const Iterator& o) :
mNode(o.mNode), mDict(o.mDict) {}

template<class Key, class Storage>
typename MAUtil::Dictionary<Key, Storage>::ConstIterator&
MAUtil::Dictionary<Key, Storage>::ConstIterator::operator=(const ConstIterator& o) {
	mNode = o.mNode;
	mDict = o.mDict;
	return *this;
}

template<class Key, class Storage>
const Storage& MAUtil::Dictionary<Key, Storage>::ConstIterator::operator*() const {
	MAASSERT(mNode != NULL);
	return mNode->data;
}

template<class Key, class Storage>
const Storage* MAUtil::Dictionary<Key, Storage>::ConstIterator::operator->() const {
	MAASSERT(mNode != NULL);
	return &mNode->data;
}

template<class Key, class Storage>
typename MAUtil::Dictionary<Key, Storage>::ConstIterator&
MAUtil::Dictionary<Key, Storage>::ConstIterator::operator++() {
	MAASSERT(mNode != NULL);
	mNode = (DictNode*)dict_next((dict_t*)mDict, (DictNode*)mNode);
	return *this;
}

template<class Key, class Storage>
typename MAUtil::Dictionary<Key, Storage>::ConstIterator
MAUtil::Dictionary<Key, Storage>::ConstIterator::operator++(int) {
	ConstIterator old = *this;
	++*this;
	return old;
}

template<class Key, class Storage>
typename MAUtil::Dictionary<Key, Storage>::ConstIterator&
MAUtil::Dictionary<Key, Storage>::ConstIterator::operator--() {
	if(mNode == NULL) {
		mNode = (DictNode*)dict_last((dict_t*)mDict);
		MAASSERT(mNode != NULL);
		return *this;
	}
	mNode = (DictNode*)dict_prev((dict_t*)mDict, (DictNode*)mNode);
	return *this;
}

template<class Key, class Storage>
typename MAUtil::Dictionary<Key, Storage>::ConstIterator
MAUtil::Dictionary<Key, Storage>::ConstIterator::operator--(int) {
	ConstIterator old = *this;
	--*this;
	return old;
}

template<class Key, class Storage>
bool MAUtil::Dictionary<Key, Storage>::ConstIterator::operator==(const ConstIterator& o) const {
	return mNode == o.mNode;
}

template<class Key, class Storage>
bool MAUtil::Dictionary<Key, Storage>::ConstIterator::operator!=(const ConstIterator& o) const {
	return mNode != o.mNode;
}
