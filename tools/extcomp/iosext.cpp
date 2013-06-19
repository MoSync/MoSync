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
