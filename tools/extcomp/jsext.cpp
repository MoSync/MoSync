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

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <idl-common/idl-common.h>
#include <idl-common/types.h>

#include "jsext.h"
#include "extcomp.h"

using namespace std;

void writeJSBridge(string& outputfile, Interface& ext) {
	ofstream extensionFile(outputfile.c_str());

	string extname = ext.name;

	extensionFile << "var " << extname << " = {};\n\n";

	for (size_t i = 0; i < ext.constSets.size(); i++) {
		ConstSet cs = ext.constSets[i];
		string name = cs.name;
		for (size_t j = 0; j < cs.constants.size(); j++) {
			Constant c = cs.constants[j];
			extensionFile << extname << "." << name << c.name << " = " << c.value << ";\n";
		}
		extensionFile << "\n";
	}

	for (size_t i = 0; i < ext.functions.size(); i++) {
		Function f = ext.functions[i];
		int outParamCount = 0;
		extensionFile << extname << "." << f.name << " = function(";
		for (size_t j = 0; j < f.args.size(); j++) {
			Argument arg = f.args[j];
			extensionFile << "_" + arg.name;
			if (j < f.args.size() - 1) {
				extensionFile << ", ";
			}
			if (!arg.in) outParamCount++;
		}
		if (f.args.size()) {
			extensionFile << ", ";
		}
		extensionFile << "fnc";
		extensionFile << ") {\n";

		extensionFile << "var args = [";
		extensionFile << "\"Extension\",";
		extensionFile << "\"" << extname << "\",";
		extensionFile << i << "];\n";

		map<string, int> structIdMap;
		vector<string> typeDescs;
		vector<string> argTypes;
		vector<bool> argDirs;

		for (size_t j = 0; j < f.args.size(); j++) {
			Argument arg = f.args[j];
			string argName = "_" + arg.name;
			generateMarshalling(extensionFile, ext, argName, arg.type);
		}
		string resultVar = isReturnType(ext, f.returnType) ? "r.result" : "null";
		string outVar = outParamCount ? "r.out" : "null";
		extensionFile << "mosync.bridge.send(args, function(r) { fnc(" << resultVar << "," << outVar <<  ")});};\n";

		// Self-executing initializer.
		extensionFile << "(function() {\n";
		extensionFile << "var initArgs = [";
		extensionFile << "\"Extension\",";
		extensionFile << "\"*" << extname << "\",";
		streamExtHashValue(extensionFile, ext);
		extensionFile << "," << i << "];\n";

		extensionFile << "initArgs.push(\"";
		for (size_t j = 0; j < f.args.size(); j++) {
			Argument arg = f.args[j];
			argTypes.push_back(arg.type);
			argDirs.push_back(!arg.in);
		}
		argTypes.push_back(f.returnType);
		argDirs.push_back(true);

		string argTypeDesc = getJSTypeDesc(ext, argTypes, &argDirs, structIdMap, typeDescs);
		extensionFile << argTypeDesc << "\");";

		for (size_t j = 0; j < typeDescs.size(); j++) {
			extensionFile << "initArgs.push(\"" << typeDescs[j] << "\");\n";
		}

		extensionFile << "initArgs.push(\"-\");\n";
		extensionFile << "mosync.bridge.send(initArgs);\n";
		extensionFile << "}());\n";

	}

	extensionFile.close();
}

void generateArrayMarshalling(ostream& extensionFile, Interface& ext, string& arrayName, string& arrayType, int ptrDepth) {
	if (ptrDepth <= 0) {
		return;
	}
	string subArrayVar = "A" + arrayName;
	replace(subArrayVar.begin(), subArrayVar.end(), '.', '_');
	int throwAway;
	string scalarType = extractPointerType(arrayType, throwAway);
	extensionFile << "if (Array.isArray(" << arrayName << ")) {\n";
	extensionFile << "args.push(" << arrayName << ".length);\n";
	extensionFile << "for (var i" << ptrDepth << " = 0; i" << ptrDepth << " < " << arrayName << ".length; i" << ptrDepth << "++) {\n";
	extensionFile << "var " << subArrayVar << " = " << arrayName << "[i" << ptrDepth << "];\n";
	generateArrayMarshalling(extensionFile, ext, subArrayVar, arrayType, ptrDepth - 1);
	if (ptrDepth == 1) {
		generateMarshalling(extensionFile, ext, subArrayVar, scalarType);
	}
	extensionFile << "}";
	extensionFile << "} else {\n";
	extensionFile << "args.push(1);\n";
	generateArrayMarshalling(extensionFile, ext, subArrayVar, arrayType, ptrDepth - 1);
	if (ptrDepth == 1) {
		generateMarshalling(extensionFile, ext, subArrayVar, scalarType);
	}
	extensionFile << "}\n";
}

void generateStructMarshalling(ostream& extensionFile, Interface& ext, string& varName, string& structType) {
	Struct* s = getStruct(ext, structType);
	if (s) {
		for (size_t i = 0; i < s->members.size(); i++) {
			Member m = s->members[i];
			string name = m.pod[0].name;
			string fieldAccess = varName + "." + name;
			generateMarshalling(extensionFile, ext, fieldAccess, m.pod[0].type);
			extensionFile << "\n";
		}

	}
}

void generateMarshalling(ostream& extensionFile, Interface& ext, string& name, string& type) {
	int ptrDepth = 0;
	string scalarType = extractPointerType(type, ptrDepth);
	bool stringType = scalarType == "char" && ptrDepth == 1;
	if (ptrDepth > 0 && !stringType) {
		generateArrayMarshalling(extensionFile, ext, name, type, ptrDepth);
	} else if (getStruct(ext, type)) {
		generateStructMarshalling(extensionFile, ext, name, type);
	} else {
		// Primitive
		extensionFile << "args.push(" << name << ");";
	}
}

string getJSTypeDesc(Interface& ext, vector<string>& types, vector<bool>* dirs, map<string, int>& structIdMap, vector<string>& typeDescs) {
	string typeDesc;
	for (size_t i = 0; i < types.size(); i++) {
		string t = types[i];
		bool out = dirs && (*dirs)[i];
		int ptrDepth = 0;
		string type = extractPointerType(t, ptrDepth);
		type = resolveTypedef(ext, type);
		if ("int" == type) {
			typeDesc.append(out ? "i" : "I");
		} else if ("long long" == type) {
			typeDesc.append(out ? "l" : "L");
		} else if ("char" == type) {
			typeDesc.append(out ? "b" : "B");
		} else if ("float" == type) {
			typeDesc.append(out ? "f" : "F");
		} else if ("double" == type) {
			typeDesc.append(out ? "d" : "D");
		} else if ("void" == type) {
			// Only for return values, always out.
			typeDesc.append("v");
		} else {
			if (structIdMap.find(type) == structIdMap.end()) {
				structIdMap[type] = typeDescs.size();
				for (size_t j = 0; j < ext.structs.size(); j++) {
					Struct s = ext.structs[j];
					if (s.name == type) {
						vector<string> structTypes;
						for (size_t j = 0; j < s.members.size(); j++) {
							Member m = s.members[j];
							structTypes.push_back(m.pod[0].type);
						}
						string typeDesc = getJSTypeDesc(ext, structTypes, NULL, structIdMap, typeDescs);
						typeDescs.push_back(typeDesc);
					}
				}
			}
			int id = structIdMap[type];
			ostringstream result;
			result << id << (out ? "?" : "!");
			typeDesc.append(result.str());
		}
		for (int i = 0; i < ptrDepth; i++) {
			typeDesc.append("*");
		}
	}

	return typeDesc;
}
