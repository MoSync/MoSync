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
#error Don't include this file directly.
#endif

//******************************************************************************
// THashFunction
//******************************************************************************

template<> hash_val_t THashFunction(const String& s) {
	return hash_fun_default(s.c_str());
}

template<> hash_val_t THashFunction(const int& data) {
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

template<class Key, class Value, class Comp>
HashMap<Key, Value, Comp>::HashNode::HashNode() {
	memset((hnode_t*)this, 0, sizeof(hnode_t));
}

template<class Key, class Value, class Comp>
HashMap<Key, Value, Comp>::HashMap(HashFunction hf, int init_bits) {
	int init_size = 1 << init_bits;
	hnode_t** table = (hnode_t**)malloc(sizeof(hnode_t*) * init_size);
	hash_init(&mHash, HASHCOUNT_T_MAX, &compare, (hash_fun_t)hf, table, init_size);
	mHash.hash_dynamic = 1;
	hash_set_allocator(&mHash, &alloc, &free, this);
}

template<class Key, class Value, class Comp>
HashMap<Key, Value, Comp>::HashMap(const HashMap& o) {
	mHash.hash_table = NULL;	//setup for clear()
	clear();
	operator=(o);
}

template<class Key, class Value, class Comp>
HashMap<Key, Value, Comp>& HashMap<Key, Value, Comp>::operator=(const HashMap& o) {
	clear();
	mHash = o.mHash;
	mHash.hash_table = (hnode_t**)malloc(sizeof(hnode_t*) * mHash.hash_nchains);
	for(hashcount_t i=0; i<mHash.hash_nchains; i++) {
		mHash.hash_table[i] = o.mHash.hash_table[i];
		if(o.mHash.hash_table[i] == NULL) {
			mHash.hash_table[i] = NULL;
		} else {
			const HashNode* oldNode = (HashNode*)o.mHash.hash_table[i];
			while(oldNode) {
				HashNode* newNode = new HashNode(*oldNode);
				hash_clone_insert(&mHash, newNode);
				oldNode = (HashNode*)oldNode->hash_next;
			}
		}
	}
	return *this;
}

template<class Key, class Value, class Comp>
HashMap<Key, Value, Comp>::~HashMap() {
	clear();
}

template<class Key, class Value, class Comp>
void HashMap<Key, Value, Comp>::clear() {
	if(mHash.hash_table) {
		hash_free_nodes_noclear(&mHash);
		::free(mHash.hash_table);
		mHash.hash_table = NULL;
	}
}

template<class Key, class Value, class Comp>
Pair<typename HashMap<Key, Value, Comp>::Iterator, bool>
HashMap<Key, Value, Comp>::insert(const Key& key, const Value& value) {
	Pair<Iterator, bool> pair = { Iterator(), false };
	HashNode* newNode = new HashNode();
	newNode->data.first = key;
	HashNode* resNode = (HashNode*)hash_insert(&mHash, newNode, &newNode->data.first);
	if(resNode == NULL) {	//success
		pair.second = true;
		hash_scan_init(&pair.first.mScan, &mHash, newNode);
	} else {	//this key was already in the table
		pair.second = false;
		hash_scan_init(&pair.first.mScan, &mHash, resNode);
	}
	return pair;
}

template<class Key, class Value, class Comp>
typename HashMap<Key, Value, Comp>::Iterator HashMap<Key, Value, Comp>::find(const Key& key) {
	Iterator itr;
	hnode_t* node = hash_lookup(&mHash, &key);
	hash_scan_init(&itr.mScan, &mHash, node);
	return itr;
}

template<class Key, class Value, class Comp>
typename HashMap<Key, Value, Comp>::ConstIterator HashMap<Key, Value, Comp>::find(const Key& key) const {
	//call the non-const function. it's safe, promise :)
	return ((HashMap*)this)->find(key);
}

template<class Key, class Value, class Comp>
bool HashMap<Key, Value, Comp>::erase(const Key& key) {
	hnode_t* node = hash_lookup(&mHash, &key);
	if(node == NULL)
		return false;
	hash_delete_free(&mHash, node);
	return true;
}

template<class Key, class Value, class Comp>
void HashMap<Key, Value, Comp>::erase(Iterator itr) {
	hash_scan_delete(&mHash, itr.mScan.hash_next);
}

template<class Key, class Value, class Comp>
size_t HashMap<Key, Value, Comp>::size() const {
	return hash_count(&mHash);
}

template<class Key, class Value, class Comp>
HashMap<Key, Value, Comp>::Iterator HashMap<Key, Value, Comp>::begin() {
	Iterator itr;
	hash_scan_begin(&itr.mScan, &mHash);
	return itr;
}
template<class Key, class Value, class Comp>
HashMap<Key, Value, Comp>::ConstIterator HashMap<Key, Value, Comp>::begin() const {
	ConstIterator itr;
	hash_scan_begin(&itr.mScan, &mHash);
	return itr;
}

template<class Key, class Value, class Comp>
HashMap<Key, Value, Comp>::Iterator HashMap<Key, Value, Comp>::end() {
	Iterator itr;
}
template<class Key, class Value, class Comp>
HashMap<Key, Value, Comp>::ConstIterator HashMap<Key, Value, Comp>::end() const;



//******************************************************************************
// Iterator
//******************************************************************************

//******************************************************************************
// ConstIterator
//******************************************************************************
