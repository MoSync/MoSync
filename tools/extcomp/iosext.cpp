/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <idl-common/idl-common.h>
#include <idl-common/types.h>
#include "helpers/mkdir.h"

#include "iosext.h"
#include "extcomp.h"

void writeIosStubs(string& outputDir, Interface& ext, string& iosIfName, bool headerOnly) {
	_mkdir(outputDir.c_str());

	// First some headers.
	string extensionHeaderFile = outputDir + ext.name + ".h";
	writeHeaders(extensionHeaderFile, ext, false);

	// The interface stub
	string extensionFileName = outputDir + ext.name + ".mm";
	ofstream extensionFile(extensionFileName.c_str());

	extensionFile << "#import \"" << ext.name << ".h\"\n\n";

	extensionFile << "@interface " << iosIfName <<  ": NSObject<MoSyncExtensionProtocol> { }\n\n";

	for (size_t i = 0; i < ext.functions.size(); i++) {
		Function f = ext.functions[i];
		streamObjCSignature(extensionFile, ext, f);
		extensionFile << "\n\n";
	}
	extensionFile << "@end\n\n";

	// Implementation
	extensionFile << "@implementation " << iosIfName << "\n";

	extensionFile << "-(id) initWithMoSyncContext:(MoSyncContext*)mosync\n";
	extensionFile << "{\n\treturn self;\n}\n";

	for (size_t i = 0; i < ext.functions.size(); i++) {
		Function f = ext.functions[i];
		string returnType = f.returnType;
		streamObjCSignature(extensionFile, ext, f);
		extensionFile << "\n";
		extensionFile << "{\n";
		if (returnType != "void") {
			extensionFile << "\t\treturn " << getObjCDefaultValue(returnType) << ";\n";
		}
		extensionFile << "}\n\n";
	}

	extensionFile << "@end\n";

	extensionFile.close();
}

void streamObjCSignature(ostream& out, Interface& ext, Function& f) {
	out << "- (" << f.returnType << ") " << f.name;
	for (size_t j = 0; j < f.args.size(); j++) {
		Argument arg = f.args[j];
		out << ":(";
		out << arg.type;
		out << ")";
		out << arg.name;
		if (j < f.args.size() - 1) {
			out << " ";
		}
	}
}

string getObjCDefaultValue(string& type) {
	if (type == "char" || type == "int" || type == "long" || type == "float" || type == "double") {
		return "0";
	} else {
		return "(" + type + ")null";
	}
}
