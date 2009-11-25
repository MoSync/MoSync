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

#include <set>
#include <vector>

//todo: cleanup
#define LOGGING_ENABLED
#define CONFIG_H

#include "helpers/helpers.h"

#include "stabs_types.h"
#include "stabs_file.h"
#include "stabs.h"

using namespace std;

//static set<Type> sTypeSet;
static vector<set<Type> > sTypeSets;
static vector<std::string> sTypeFiles;

void addTypeFile(int file, const std::string& fileName) {
	if(file >= (int)sTypeSets.size()) {
		sTypeSets.resize(file + 1);
		sTypeFiles.resize(file + 1);
	}

	sTypeFiles[file] = fileName;
}

void addType(const Type& s) {
	//LOG("addType(%i,%i)\n", s.id.a, s.id.b);
	pair<set<Type>::iterator, bool> res = sTypeSets[gCurrentFile].insert(s);
	if(!res.second) {	//tuple already defined. it must be a delayed type.
		Type& t(*res.first);
		DEBUG_ASSERT(t.type->type() == TypeBase::eUnknown);
		DEBUG_ASSERT(t.id == s.id);
		t.name = s.name;
		t.type = s.type;
	}
}

const Type* stabsFindTypeByName(const std::string& name, int scope) {
	set<Type>::const_iterator itr = sTypeSets[scope].begin();
	for(;itr!=sTypeSets[scope].end(); itr++) {
		if((*itr).name == name) return &(*itr);
	}
	return NULL;
}

const TypeBase* findTypeByNameAndFileGlobal(const std::string& name, int scope) {
	const Type* type = stabsFindTypeByName(name, scope);
	if(type) return type->type;

	// okay we didn't find it in the local scope, let's
	// search through all scopes. If we get multiple hits
	// we have an error..
	for(size_t i = 0; i < sTypeSets.size(); i++) {
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
	set<Type>::const_iterator itr = sTypeSets[file].find(s);
	if(itr == sTypeSets[file].end())
		return NULL;
	return itr->type;
}
