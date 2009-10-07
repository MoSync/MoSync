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

#ifndef _SE_MSAB_MAUTIL_SET_H_
#error Do not include this file directly.
#endif

//******************************************************************************
// Set
//******************************************************************************

template<class Key>
MAUtil::Set<Key>::DictNode::DictNode() {
	memset((dnode_t*)this, 0, sizeof(dnode_t));
}

template<class Key>
void MAUtil::Set<Key>::init(CompareFunction cf) {
	dict_init(&mDict, DICTCOUNT_T_MAX, (dict_comp_t)cf);
	dict_set_allocator(&mDict, &alloc, &free, this);
}

template<class Key>
MAUtil::Set<Key>::Set(CompareFunction cf) {
	init(cf);
}

template<class Key>
MAUtil::Set<Key>::Set(const Set& o) {
	init((CompareFunction)o.mDict.dict_compare);
	operator=(o);
}

template<class Key>
MAUtil::Set<Key>& MAUtil::Set<Key>::operator=(const Set& o) {
	clear();
	dict_load_t load;
	dict_load_begin(&load, &mDict);
	ConstIterator itr = o.begin();
	while(itr != o.end()) {
		DictNode* newNode = new DictNode;
		newNode->key = *itr;
		dict_load_next(&load, newNode, &newNode->key);
		++itr;
	}
	dict_load_end(&load);
	return *this;
}

template<class Key>
MAUtil::Set<Key>::~Set() {
	clear();
}

template<class Key>
void MAUtil::Set<Key>::clear() {
	dict_free_nodes(&mDict);
}

template<class Key>
MAUtil::Pair<class MAUtil::Set<Key>::Iterator, bool> MAUtil::Set<Key>::insert(const Key& key) {
	Pair<Iterator, bool> pair = { Iterator(&mDict), false };
	DictNode* newNode = new DictNode;
	dnode_init(newNode, NULL);
	newNode->key = key;
	DictNode* res = (DictNode*)dict_insert(&mDict, newNode, &newNode->key);
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

template<class Key>
typename MAUtil::Set<Key>::Iterator MAUtil::Set<Key>::begin() {
	Iterator itr(&mDict);
	itr.mNode = (DictNode*)dict_first(&mDict);
	return itr;
}

template<class Key>
typename MAUtil::Set<Key>::ConstIterator MAUtil::Set<Key>::begin() const {
	ConstIterator itr(&mDict);
	itr.mNode = (DictNode*)dict_first((dict_t*)&mDict);
	return itr;
}

template<class Key>
typename MAUtil::Set<Key>::Iterator MAUtil::Set<Key>::end() {
	Iterator itr(&mDict);
	itr.mNode = NULL;
	return itr;
}

template<class Key>
typename MAUtil::Set<Key>::ConstIterator MAUtil::Set<Key>::end() const {
	ConstIterator itr(&mDict);
	itr.mNode = NULL;
	return itr;
}

template<class Key>
size_t MAUtil::Set<Key>::size() const {
	return dict_count(&mDict);
}

template<class Key>
bool MAUtil::Set<Key>::erase(const Key& key) {
	dnode_t* node = dict_lookup(&mDict, &key);
	if(node == NULL)
		return false;
	dict_delete_free(&mDict, node);
	return true;
}

template<class Key>
typename MAUtil::Set<Key>::Iterator MAUtil::Set<Key>::find(const Key& key) {
	Iterator itr(&mDict);
	itr.mNode = (DictNode*)dict_lookup(&mDict, &key);
	return itr;
}

template<class Key>
typename MAUtil::Set<Key>::ConstIterator MAUtil::Set<Key>::find(const Key& key) const {
	ConstIterator itr(&mDict);
	itr.mNode = (DictNode*)dict_lookup(&mDict, &key);
	return itr;
}

//******************************************************************************
// Iterator
//******************************************************************************

template<class Key>
MAUtil::Set<Key>::Iterator::Iterator(dict_t* dict) : mNode(NULL), mDict(dict) {}

template<class Key>
MAUtil::Set<Key>::Iterator::Iterator(const Iterator& o) : mNode(o.mNode), mDict(o.mDict) {}

template<class Key>
typename MAUtil::Set<Key>::Iterator&
MAUtil::Set<Key>::Iterator::operator=(const Iterator& o) {
	mNode = o.mNode;
	mDict = o.mDict;
	return *this;
}

template<class Key>
Key& MAUtil::Set<Key>::Iterator::operator*() {
	MAASSERT(mNode != NULL);
	return mNode->key;
}

template<class Key>
Key* MAUtil::Set<Key>::Iterator::operator->() {
	MAASSERT(mNode != NULL);
	return &mNode->key;
}

template<class Key>
typename MAUtil::Set<Key>::Iterator& MAUtil::Set<Key>::Iterator::operator++() {
	MAASSERT(mNode != NULL);
	mNode = (DictNode*)dict_next(mDict, mNode);
	return *this;
}

template<class Key>
typename MAUtil::Set<Key>::Iterator& MAUtil::Set<Key>::Iterator::operator--() {
	if(mNode == NULL) {
		mNode = (DictNode*)dict_last(mDict);
		MAASSERT(mNode != NULL);
	}
	mNode = (DictNode*)dict_prev(mDict, mNode);
	return *this;
}

template<class Key>
bool MAUtil::Set<Key>::Iterator::operator==(const Iterator& o) const {
	return mNode == o.mNode;
}

template<class Key>
bool MAUtil::Set<Key>::Iterator::operator!=(const Iterator& o) const {
	return mNode != o.mNode;
}

//******************************************************************************
// ConstIterator
//******************************************************************************

template<class Key>
MAUtil::Set<Key>::ConstIterator::ConstIterator(const dict_t* dict) :
mNode(NULL), mDict(dict) {}

template<class Key>
MAUtil::Set<Key>::ConstIterator::ConstIterator(const ConstIterator& o) :
mNode(o.mNode), mDict(o.mDict) {}

template<class Key>
MAUtil::Set<Key>::ConstIterator::ConstIterator(const Iterator& o) :
mNode(o.mNode), mDict(o.mDict) {}

template<class Key>
typename MAUtil::Set<Key>::ConstIterator&
MAUtil::Set<Key>::ConstIterator::operator=(const ConstIterator& o) {
	mNode = o.mNode;
	mDict = o.mDict;
	return *this;
}

template<class Key>
const Key& MAUtil::Set<Key>::ConstIterator::operator*() const {
	MAASSERT(mNode != NULL);
	return mNode->key;
}

template<class Key>
const Key* MAUtil::Set<Key>::ConstIterator::operator->() const {
	MAASSERT(mNode != NULL);
	return &mNode->key;
}

template<class Key>
typename MAUtil::Set<Key>::ConstIterator& MAUtil::Set<Key>::ConstIterator::operator++() {
	MAASSERT(mNode != NULL);
	mNode = (DictNode*)dict_next((dict_t*)mDict, (DictNode*)mNode);
	return *this;
}

template<class Key>
typename MAUtil::Set<Key>::ConstIterator& MAUtil::Set<Key>::ConstIterator::operator--() {
	if(mNode == NULL) {
		mNode = (DictNode*)dict_last((dict_t*)mDict);
		MAASSERT(mNode != NULL);
	}
	mNode = (DictNode*)dict_prev((dict_t*)mDict, (DictNode*)mNode);
	return *this;
}

template<class Key>
bool MAUtil::Set<Key>::ConstIterator::operator==(const ConstIterator& o) const {
	return mNode == o.mNode;
}

template<class Key>
bool MAUtil::Set<Key>::ConstIterator::operator!=(const ConstIterator& o) const {
	return mNode != o.mNode;
}
