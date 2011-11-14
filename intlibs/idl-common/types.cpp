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

#include "types.h"

#define MAX(x, y) ((x)>(y)?(x):(y))

bool isReturnType(const Interface& inf, const string& token) {
	for(size_t i=0; i<inf.typedefs.size(); i++) {
		const Typedef& t(inf.typedefs[i]);
		if(token == t.name)
			return true;
	}
	return token == "int" || token == "uint" || token == "void" || token == "double" ||
		token == "float" || token == "noreturn";
}

string directCType(const Interface& inf, const string& type) {
	if(type == "int" || type == "double" || type == "float" || type == "void" ||
		type == "char" || type == "uint" || type == "long long")
		return type;
	if(type == "noreturn")
		return "void";
	if(type == "NCString")
		return "char*";

	bool isPointer = type[type.size()-1] == '*';
	if(isPointer)
		return type;

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
	for(size_t i=0; i<inf.structs.size(); i++) {
		const Struct& s(inf.structs[i]);
		if(type == s.name)
			return s.name;
	}
	throwException("Unhandled type: " + type);
}

static bool isDecimal(const string& s, int& i) {
	char* end;
	i = strtol(s.c_str(), &end, 10);
	return (end == s.c_str() + s.size());
}

bool isArray(const Interface& inf, const string& name, int& count, string& baseName) {
	size_t pos = name.find('[');
	if(pos == string::npos)
		return false;
	size_t end = name.find(']');
	if(end != name.size()-1)
		throwException("Bad array syntax: " + name);

	baseName = name.substr(0, pos);

	string countString = name.substr(pos+1, (end - pos) - 1);
	if(isDecimal(countString, count))
		return true;
	for(size_t i=0; i<inf.constSets.size(); i++) {
		const ConstSet& cs(inf.constSets[i]);
		for(size_t j=0; j<cs.constants.size(); j++) {
			const Constant& c(cs.constants[j]);
			if((cs.name + c.name) == countString) {
				if(isDecimal(c.value, count))
					return true;
				else
					throwException("Non-integer array count: " + countString);
			}
		}
	}
	throwException("Undefined array count: " + countString);
}

size_t cTypeSize(const Interface& inf, const string& type) {
	if(type == "int" || type == "float" || type == "uint")
		return 4;
	if(type == "double" || type == "long long")
		return 8;
	if(type == "char" || type == "unsigned char")
		return 1;
	if(type == "NCString")
		return 4;

	bool isPointer = type[type.size()-1] == '*';
	if(isPointer)
		return 4;

	for(size_t i=0; i<inf.typedefs.size(); i++) {
		const Typedef& t(inf.typedefs[i]);
		if(type == t.name) {
			if(t.type[t.type.size() - 1] == '*') {	//isPointerType
				return 4;
			} else {
				return cTypeSize(inf, t.type);
			}
		}
	}
	for(size_t i=0; i<inf.structs.size(); i++) {
		const Struct& s(inf.structs[i]);
		if(type == s.name) {
			size_t size = 0;
			for(size_t j=0; j<s.members.size(); j++) {
				const Member& m(s.members[j]);
				size_t max = 0;
				for(size_t k=0; k<m.pod.size(); k++) {
					const PlainOldData& pod(m.pod[k]);
					max = MAX(max, cTypeSize(inf, pod.type));
				}
				size += max;
			}
			return size;
		}
	}
	throwException("Unhandled type: " + type);
}

string cType(const Interface& inf, const string& type) {
	for(size_t i=0; i<inf.structs.size(); i++) {
		const Struct& s(inf.structs[i]);
		if(type == s.name)
			return s.name + "*";
	}
	return directCType(inf, type);
}

const string& resolveType(const Interface& inf, const string& ctype) {
	for(size_t i=0; i<inf.typedefs.size(); i++) {
		const Typedef& t(inf.typedefs[i]);
		if(ctype == t.name) {
			return resolveType(inf, t.type);
		}
	}
	return ctype;
}

bool isPointerType(const Interface& inf, const string& type) {
	string ct = cType(inf, type);
	return ct[ct.size() - 1] == '*';
}

bool isDirectPointerType(const Interface& inf, const string& type) {
	string dct = directCType(inf, type);
	return dct[dct.size() - 1] == '*';
}

string jType(const Interface& inf, const string& type) {
	if(type == "ulong")
		return "int";
	if(type == "uint")
		return "int";
	if(type == "MAString")
		return "MAString";
	if(type == "MAWString")
		return "MAWString";
	if(type == "float")
		return "int";
	if(type == "double")
		return "long";
	if(type == "longlong")
		return "long";
	if(isPointerType(inf, type))
		return "MAAddress";
	return cType(inf, type);
}

string getSJType(const Interface& inf, const string& type, bool argIn) {
	string argType = type;
	if(!argIn)
		return "int";

	if(argType == "MAString" || argType == "MAWString")
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

	if(argType == "uint")
		return "int";

	for(size_t i=0; i<inf.structs.size(); i++) {
		if(argType == inf.structs[i].name)
			return "int";
	}

	return argType;
}
