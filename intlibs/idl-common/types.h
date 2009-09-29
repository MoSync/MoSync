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

#ifndef TYPES_H
#define TYPES_H

#include "idl-common.h"

inline bool isReturnType(const Interface& inf, const string& token) {
	for(size_t i=0; i<inf.typedefs.size(); i++) {
		const Typedef& t(inf.typedefs[i]);
		if(token == t.name)
			return true;
	}
	return token == "int" || token == "void" || token == "double" ||
		token == "float" || token == "noreturn";
}

inline string cType(const Interface& inf, const string& type) {
	if(type == "int" || type == "double" || type == "float" || type == "void" || type == "char")
		return type;
	if(type == "noreturn")
		return "void";
	if(type == "NCString")
		return "char*";
	for(size_t i=0; i<inf.structs.size(); i++) {
		const Struct& s(inf.structs[i]);
		if(type == s.name)
			return s.name + "*";
	}
	for(size_t i=0; i<inf.typedefs.size(); i++) {
		const Typedef& t(inf.typedefs[i]);
		if(type == t.name) {
			if(t.type[t.type.size() - 1] == '*') {	//isPointerType
				return t.type;
			} else {
				return t.name;
			}
		}
	}
	throwException("Unhandled type: " + type);
}

inline bool isPointerType(const Interface& inf, const string& type) {
	string ct = cType(inf, type);
	return ct[ct.size() - 1] == '*';
}

inline string jType(const Interface& inf, const string& type) {
	if(type == "ulong")
		return "int";
	if(type == "MAString")
		return "MAString";
	if(type == "float")
		return "int";
	if(type == "double")
		return "long";
	if(isPointerType(inf, type))
		return "MAAddress";
	return cType(inf, type);
}

//Static Java
inline string getSJType(const Interface& inf, const string& type, bool argIn) {
	string argType = type;
	if(!argIn)
		return "int";

	if(argType == "MAString")
		return "int";
	if(argType == "MAAddress")
		return "int";
	if(argType == "NCString")
		return "int";

	for(size_t i=0; i<inf.typedefs.size(); i++) {
		if(argType == inf.typedefs[i].name) {
			argType = inf.typedefs[i].type;
			break;
		}
	}

	if(argType == "noreturn")
		return "void";
	if(argType == "float")
		return "int";
	if(argType == "double")
		return "int";

	for(size_t i=0; i<inf.structs.size(); i++) {
		if(argType == inf.structs[i].name)
			return "int";
	}

	return argType;
}

#endif	//TYPES_H
