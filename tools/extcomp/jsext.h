/* Copyright (C) 2013 Mobile Sorcery AB

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

#ifndef JSEXT_H_
#define JSEXT_H_

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <map>

#include <idl-common/idl-common.h>
#include <idl-common/types.h>

using namespace std;

void writeJSBridge(string&, Interface&);
string getJSTypeDesc(Interface& ext, vector<string>& names, vector<string>& types, vector<bool>* dirs, map<string, int>& structIdMap, vector<string>& typedescs, bool argList);
string getJSDefaultValue(string scalarType);
void generateArrayMarshalling(ostream& extensionFile, Interface& ext, string& arrayName, string& arrayType, int ptrDepth);
void generateStructMarshalling(ostream& extensionFile, Interface& ext, string& varName, string& varType);
void generateMarshalling(ostream& extensionFile, Interface& ext, string& varName, string& varType);

#endif /* JSEXT_H_ */
