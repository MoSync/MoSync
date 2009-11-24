#include "types.h"

bool isReturnType(const Interface& inf, const string& token) {
	for(size_t i=0; i<inf.typedefs.size(); i++) {
		const Typedef& t(inf.typedefs[i]);
		if(token == t.name)
			return true;
	}
	return token == "int" || token == "void" || token == "double" ||
		token == "float" || token == "noreturn";
}

string cType(const Interface& inf, const string& type) {
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

bool isPointerType(const Interface& inf, const string& type) {
	string ct = cType(inf, type);
	return ct[ct.size() - 1] == '*';
}

string jType(const Interface& inf, const string& type) {
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

string getSJType(const Interface& inf, const string& type, bool argIn) {
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
