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
			extensionFile << arg.name;
			if (j < f.args.size() - 1) {
				extensionFile << ", ";
			}
			if (!arg.in) outParamCount++;
		}
		if (outParamCount || isReturnType(ext, f.returnType)) {
			if (f.args.size()) {
				extensionFile << ", ";
			}
			extensionFile << "fnCallback";
		}
		extensionFile << ") {\n";

		extensionFile << "\tmosync.bridge.send([\n";
		extensionFile << "\t\t\"Extension\",\n";
		extensionFile << "\t\t\"" << extname << "\",\n";
		extensionFile << "\t\t" << i << ",\n";
		extensionFile << "\t\t\"";
		map<string, int> structIdMap;
		vector<string> typeDescs;
		vector<string> argTypes;

		for (size_t j = 0; j < f.args.size(); j++) {
			Argument arg = f.args[j];
			argTypes.push_back(arg.type);
		}
		string argTypeDesc = getJSTypeDesc(ext, argTypes, structIdMap, typeDescs);
		extensionFile << argTypeDesc << "\"";

		for (size_t j = 0; j < typeDescs.size(); j++) {
			extensionFile << ",\n\t\t\"";
			extensionFile << typeDescs[j] << "\"";
		}

		extensionFile << "\n\t]);\n";
		extensionFile << "}\n\n";
	}

	extensionFile.close();
}

string getJSTypeDesc(Interface& ext, vector<string>& types, map<string, int>& structIdMap, vector<string>& typeDescs) {
	string typeDesc;
	for (size_t i = 0; i < types.size(); i++) {
		string t = types[i];
		printf("TPYE: %s\n", t.c_str());
		int ptrDepth = 0;
		string type = extractPointerType(t, ptrDepth);
		type = resolveTypedef(ext, type);
		if ("int" == type) {
			typeDesc.append("I");
		} else if ("long long" == type) {
			typeDesc.append("L");
		} else if ("char" == type) {
			typeDesc.append("B");
		} else if ("float" == type) {
			typeDesc.append("F");
		} else if ("double" == type) {
			typeDesc.append("D");
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
						string typeDesc = getJSTypeDesc(ext, structTypes, structIdMap, typeDescs);
						typeDescs.push_back(typeDesc);
					}
				}
			}
			int id = structIdMap[type];
			ostringstream result;
			result << id << ";";
			typeDesc.append(result.str());
		}
		for (int i = 0; i < ptrDepth; i++) {
			typeDesc.append("*");
		}
	}

	return typeDesc;
}
