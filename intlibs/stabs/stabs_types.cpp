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

#include <vector>

//todo: cleanup
#define LOGGING_ENABLED
#define CONFIG_H

#include "helpers/helpers.h"
#include "helpers/hash_set.h"

#include "stabs_types.h"
#include "stabs_file.h"
#include "stabs.h"

using namespace std;

//struct type_name_less : public binary_function<Type, Type, bool> {
class type_name_hash {
private:
	hash_compare<string> mHC;
public:
	enum {	//arbitrary
		bucket_size = 4,
		min_buckets = 20000,
	};
	size_t operator()(const Type* key) const {
		return mHC(key->name);
	}
	bool operator()(const Type* left, const Type* right) const {
		return (left->name < right->name);
	}
};

struct type_tuple_hash {
private:
	hash_compare<int> mHC;
public:
	enum {
		bucket_size = 4,
		min_buckets = 20000,
	};
	size_t operator()(const Type* key) const {
		return mHC(mHC(key->id.a) + key->id.b);
	}
	bool operator()(const Type* left, const Type* right) const {
		if(left->id.a < right->id.a)
			return true;
		else if(left->id.a == right->id.a)
			return left->id.b < right->id.b;
		else
			return false;
	}
};

#ifdef HAVE_TR1
struct type_name_equal {
	bool operator()(const Type* left, const Type* right) const {
		return left->name == right->name;
	}
};
struct type_tuple_equal {
	bool operator()(const Type* left, const Type* right) const {
		return left->id == right->id;
	}
};
#define TNE ,type_name_equal
#define TTE ,type_tuple_equal
#else
#define TNE
#define TTE
#endif

typedef hash_set<Type*, type_name_hash TNE> TypeNameSet;
typedef hash_set<Type*, type_tuple_hash TTE> TypeTupleSet;

static vector<TypeTupleSet*> sTypeTupleSets;
static vector<TypeNameSet*> sTypeNameSets;
static vector<std::string> sTypeFiles;

void addTypeFile(int file, const std::string& fileName) {
	if(file >= (int)sTypeTupleSets.size()) {
		sTypeTupleSets.resize(file + 1);
		sTypeNameSets.resize(file + 1);
		sTypeFiles.resize(file + 1);

		sTypeTupleSets[file] = new TypeTupleSet;
		sTypeNameSets[file] = new TypeNameSet;
	}

	sTypeFiles[file] = fileName;
}

void addType(const Type& cs) {
	Type* s = new Type(cs);
	//LOG("addType %i (%i,%i)\n", gCurrentFile, s->id.a, s->id.b);
	pair<TypeTupleSet::iterator, bool> res = sTypeTupleSets[gCurrentFile]->insert(s);
	if(res.second) {
		sTypeNameSets[gCurrentFile]->insert(s);
	} else {	//tuple already defined. it must be a delayed type.
		//LOG("dupe\n");
		Type* t = *res.first;
		//DEBUG_ASSERT(t->type->type() == TypeBase::eUnknown);
		if(t->type->type() != TypeBase::eUnknown) {
			LOG("weird dupe: (%i,%i) %s\n", t->id.a, t->id.b, t->name.c_str());
			delete s;
			return;
		}
		DEBUG_ASSERT(t->id == s->id);
		t->name = s->name;
		t->type = s->type;
		TypeTupleSet::iterator temp = res.first;
		temp++;
		sTypeTupleSets[gCurrentFile]->erase(res.first);
		sTypeTupleSets[gCurrentFile]->insert(temp, t);
		sTypeNameSets[gCurrentFile]->erase(s);
		sTypeNameSets[gCurrentFile]->insert(t);
	}
}

const Type* stabsFindTypeByName(const std::string& name, int scope) {
	if((size_t)scope>=sTypeTupleSets.size() || scope<0) return NULL;
#if 0
	set<Type>::const_iterator itr = sTypeTupleSets[scope].begin();
	for(;itr!=sTypeTupleSets[scope].end(); itr++) {
		if((*itr).name == name) return &(*itr);
	}
	return NULL;
#endif
	if(!sTypeNameSets[scope])
		return NULL;
	Type t;
	t.name = name;
	TypeNameSet::const_iterator itr = sTypeNameSets[scope]->find(&t);
	if(itr == sTypeNameSets[scope]->end())
		return NULL;
	else
		return *itr;
}

const TypeBase* findTypeByNameAndFileGlobal(const std::string& name, int scope) {
	const Type* type = stabsFindTypeByName(name, scope);
	if(type) return type->type;

	// okay we didn't find it in the local scope, let's
	// search through all scopes. If we get multiple hits
	// we have an error..
	for(size_t i = 0; i < sTypeTupleSets.size(); i++) {
		//LOG("Searching through type set of %s for type definition of %s\n", sTypeFiles[i].c_str(), name.c_str());
		const Type* type2 = stabsFindTypeByName(name, i);
		if(type2)  {
			//LOG("Found one!\n");
			if(type) {
				LOG("Multiple instances of type %s found.\n", name.c_str());
				//let's return one of the instances, for now.
				return type->type;
			}
			type = type2;
		}
	}

	if(!type) return NULL;
	return type->type;
}

const TypeBase* findTypeByNameAndTupleAndFileGlobal(const std::string& name, Tuple tuple, int scope) {
	const TypeBase* t = findTypeByTupleAndFile(tuple, scope);
	if(t) return t;

	return findTypeByNameAndFileGlobal(name, scope);
}

const TypeBase* findTypeByTuple(Tuple t) {
	return findTypeByTupleAndFile(t, gCurrentFile);
}

const TypeBase* findTypeByTupleAndFile(Tuple t, int file) {
	Type s;
 	s.id = t;
	TypeTupleSet::const_iterator itr = sTypeTupleSets[file]->find(&s);
	if(itr == sTypeTupleSets[file]->end())
		return NULL;
	return (*itr)->type;
}
