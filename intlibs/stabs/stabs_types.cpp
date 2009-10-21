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
	DEBUG_ASSERT(res.second);
}

const Type* stabsFindTypeByName(const std::string& name, int scope) {
	set<Type>::const_iterator itr = sTypeSets[scope].begin();
	for(;itr!=sTypeSets[scope].end(); itr++) {
		if((*itr).name == name) return &(*itr);
	}
	return NULL;
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
