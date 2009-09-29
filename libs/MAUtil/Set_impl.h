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

#ifndef SET_H
#error Don't include this file directly.
#endif

//******************************************************************************
// Set
//******************************************************************************

template<class Key, class Comp>
int Set<Key, Comp>::compare(const void* a, const void* b) {
	return Comp::Compare(*(const Key*)a, *(const Key*)b);
}

template<class Key, class Comp>
Set<Key, Comp>::Set() {
	dict_init(&mDict, DICTCOUNT_T_MAX, &compare);
	dict_set_allocator(&mDict, &alloc, &free, this);
}

template<class Key, class Comp>
Set<Key, Comp>::~Set() {
	clear();
}

template<class Key, class Comp>
void Set<Key, Comp>::clear() {
	dict_free_nodes(&mDict);
}

template<class Key, class Comp>
Pair<bool, class Set<Key, Comp>::Iterator> Set<Key, Comp>::insert(const Key& key) {
	Pair<bool, Iterator> pair = { false, Iterator(&mDict) };
	DictNode* newNode = new DictNode;
	dnode_init(newNode, NULL);
	newNode->key = key;
	DictNode* res = (DictNode*)dict_insert(&mDict, newNode, &newNode->key);
	if(res != NULL) {	//insert->dupe
		delete newNode;
		pair.first = false;
		pair.second.mNode = res;
	} else {	//insert successful
		pair.first = true;
		pair.second.mNode = newNode;
	}
	return pair;
}

template<class Key, class Comp>
typename Set<Key, Comp>::Iterator Set<Key, Comp>::begin() {
	Iterator itr(&mDict);
	itr.mNode = (DictNode*)dict_first(&mDict);
	return itr;
}

template<class Key, class Comp>
typename Set<Key, Comp>::Iterator Set<Key, Comp>::end() {
	Iterator itr(&mDict);
	itr.mNode = NULL;
	return itr;
}

template<class Key, class Comp>
size_t Set<Key, Comp>::size() const {
	return dict_count(&mDict);
}

template<class Key, class Comp>
bool Set<Key, Comp>::erase(const Key& key) {
	dnode_t* node = dict_lookup(&mDict, &key);
	if(node == NULL)
		return false;
	dict_delete_free(&mDict, node);
	return true;
}

template<class Key, class Comp>
typename Set<Key, Comp>::Iterator Set<Key, Comp>::find(const Key& key) {
	Iterator itr(&mDict);
	itr.mNode = (DictNode*)dict_lookup(&mDict, &key);
	return itr;
}

//******************************************************************************
// Iterator
//******************************************************************************

template<class Key, class Comp>
Set<Key, Comp>::Iterator::Iterator(dict_t* dict) : mNode(NULL), mDict(dict) {}

template<class Key, class Comp>
Set<Key, Comp>::Iterator::Iterator(const Iterator& o) : mNode(o.mNode), mDict(o.mDict) {}

template<class Key, class Comp>
typename Set<Key, Comp>::Iterator&
Set<Key, Comp>::Iterator::operator=(const typename Set<Key, Comp>::Iterator& o) {
	mNode = o.mNode;
	mDict = o.mDict;
	return *this;
}

template<class Key, class Comp>
Key& Set<Key, Comp>::Iterator::operator*() {
	MAASSERT(mNode != NULL);
	return mNode->key;
}

template<class Key, class Comp>
Key* Set<Key, Comp>::Iterator::operator->() {
	MAASSERT(mNode != NULL);
	return &mNode->key;
}

template<class Key, class Comp>
typename Set<Key, Comp>::Iterator& Set<Key, Comp>::Iterator::operator++() {
	MAASSERT(mNode != NULL);
	mNode = (DictNode*)dict_next(mDict, mNode);
	return *this;
}

template<class Key, class Comp>
typename Set<Key, Comp>::Iterator& Set<Key, Comp>::Iterator::operator--() {
	if(mNode == NULL) {
		mNode = (DictNode*)dict_last(mDict);
		MAASSERT(mNode != NULL);
	}
	mNode = (DictNode*)dict_prev(mDict, mNode);
	return *this;
}

template<class Key, class Comp>
bool Set<Key, Comp>::Iterator::operator==(const Iterator& o) const {
	return mNode == o.mNode;
}

template<class Key, class Comp>
bool Set<Key, Comp>::Iterator::operator!=(const Iterator& o) const {
	return mNode != o.mNode;
}
