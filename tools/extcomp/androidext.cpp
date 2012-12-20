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
#include "helpers/types.h"

#include "extcomp.h"
#include "androidext.h"

void streamAndroidExtMF(ostream& out, Interface& ext, string& androidPackageName, string& androidSimpleClassName) {
	streamXMLComment(out, ext.comment);
	out << "<module name=\"" << ext.name
			<< "\" hash=\"";
	streamExtHashValue(out, ext);
	out	<< "\" class=\"" << androidPackageName  << "." << androidSimpleClassName << "\">\n";

	for (size_t i = 0; i < ext.typedefs.size(); i++) {
		Typedef t = ext.typedefs[i];
		streamXMLComment(out, t.comment);
		out << "<typedef type=\"" << t.type << "\" name=\"" << t.name << "\"/>\n";
	}

	for (size_t i = 0; i < ext.structs.size(); i++) {
		Struct s = ext.structs[i];
		streamXMLComment(out, s.comment);
		out << "<struct type=\"" << s.type << "\" name=\"" << s.name << "\" "
			<< "class=\"" << androidPackageName << ".types." << s.name << "\">\n";
		for (size_t j = 0; j < s.members.size(); j++) {
			Member member = s.members[j];
			if (member.pod.size() == 1) {
				string name = member.pod[0].name;
				string type = member.pod[0].type;
				int pointerDepth = 0;
				string pointerType = extractPointerType(type, pointerDepth);
				streamXMLComment(out, member.pod[0].comment);
				out << "<member type=\"" << pointerType << "\" name=\"" << name << "\" ptr=\"" << pointerDepth << "\"/>\n";
			} else {
				// TODO: Unsupported!!
			}

		}
		out << "</struct>\n";
	}
	for (size_t i = 0; i < ext.functions.size(); i++) {
		Function f = ext.functions[i];
		streamXMLComment(out, f.comment);
		out << "<function name=\"" << f.name << "\">\n";
		for (size_t j = 0; j < f.args.size(); j++) {
			Argument arg = f.args[j];
			int pointerDepth = 0;
			string pointerType = extractPointerType(arg.type, pointerDepth);
			out << "<arg type=\"" << pointerType << "\" "
					<< "ptr=\"" << pointerDepth << "\" "
					<< "in=\"" << (arg.in ? "true" : "false") << "\" "
					<< "out=\"" << (arg.in ? "false" : "true") << "\"/>\n";
		}
		out << "</function>\n";
	}
	out << "</module>";

}

void streamAndroidStubs(string& outputDir, Interface& ext, string& androidPackageName) {
	_mkdir(outputDir.c_str());

	// The interface stub
	string extensionFileName = outputDir + "Extension.java";
	ofstream extensionFile(extensionFileName.c_str());

	extensionFile << "import com.mosync.api.*;\n";
	extensionFile << "import " << androidPackageName << ".types.*;\n\n";

	extensionFile << "public class Extension {\n\n";

	extensionFile << "\tpublic void initialize(MoSyncContext context) {\n\t}\n\n";

	for (size_t i = 0; i < ext.functions.size(); i++) {
		Function f = ext.functions[i];
		string returnType = toAndroidType(ext, f.returnType, false);
		extensionFile << "\tpublic " << returnType << " " << f.name << "(";
		for (size_t j = 0; j < f.args.size(); j++) {
			if (j > 0) {
				extensionFile << ", ";
			}
			Argument arg = f.args[j];
			extensionFile << toAndroidType(ext, arg.type, false);
			extensionFile << " ";
			extensionFile << arg.name;
		}
		extensionFile << ") {\n\t\t/* TODO: Replace this with an implementation */\n";
		if (returnType != "void") {
			extensionFile << "\t\treturn " << getAndroidDefaultValue(returnType) << ";\n";
		}
		extensionFile << "\t}\n\n";
	}

	extensionFile << "}\n";

	extensionFile.close();

	// The structs
	string typesDir = outputDir + "types/";
	_mkdir(typesDir.c_str());

	for (size_t i = 0; i < ext.structs.size(); i++) {
		Struct s = ext.structs[i];
		string structFileName = typesDir + s.name + ".java";
		ofstream structFile(structFileName.c_str());

		// Package
		structFile << "package " << androidPackageName << ".types;\n\n";

		// Import(s)
		structFile << "import com.mosync.api.*;\n\n";

		// Class declaration and fields
		structFile << "public class " << s.name << " extends Struct {\n\n";

		for (size_t j = 0; j < s.members.size(); j++) {
			Member m = s.members[j];
			string type = m.pod[0].type;
			string androidType = toAndroidType(ext, type, false);
			structFile << "\tpublic " << androidType << " " << m.pod[0].name << ";\n";
			int pointerDepth = 0;
			string pointerType = extractPointerType(type, pointerDepth);
			structFile << "\tprivate final static Marshaller __" << m.pod[0].name << " = _m(\"" << ext.name << "\", \"" << pointerType << "\", " << pointerDepth << ");\n\n";
		}

		// (Un)Marshalling
		size_t size = 0;
		structFile << "\n\tpublic Struct unmarshal(byte[] data, int offset) {\n";
		structFile << "\t\t" << s.name << " s = new " << s.name << "();\n";
		for (size_t j = 0; j < s.members.size(); j++) {
			Member m = s.members[j];
			string ctype = m.pod[0].type;
			string name = m.pod[0].name;
			string cast = toAndroidType(ext, ctype, true);
			structFile << "\t\ts." << name << " = (" << cast << ")__" << name << ".unmarshal(data, offset + " << size << ");\n";
			size += cTypeSize(ext, ctype);
		}
		structFile << "\t\treturn s;\n";
		structFile << "\t}\n\n";

		structFile << "\tpublic int size() { return " << size << "; }\n\n";

		structFile << "}\n";

		structFile.close();
	}
}

string toAndroidType(Interface& ext, string& ctype, bool autoBox) {
	int ptrDepth = 0;
	string extractedType = extractPointerType(ctype, ptrDepth);
	if (ptrDepth > 0) {
		if (extractedType == "char") {
			ptrDepth--;
			extractedType = "NCString";
		}
		string prefix = "";
		string suffix = "";
		for (int i = 0; i < ptrDepth; i++) {
			prefix.append("Pointer<");
			suffix.append(">");
		}
		return prefix + toAndroidType(ext, extractedType, autoBox) + suffix;
	} else {
		extractedType = resolveTypedef(ext, extractedType);
		if (extractedType == "NCString") {
			return "String";
		}
		if (autoBox) {
			if (extractedType == "int") return "Integer";
			if (extractedType == "char") return "Character";
			if (extractedType == "double") return "Double";
			if (extractedType == "float") return "Float";
		} else if (extractedType == "int" || extractedType == "char" ||
			extractedType == "double" || extractedType == "float") {
			return extractedType;
		}

		// The rest are structs
		return extractedType;
	}
}

string getAndroidDefaultValue(string& type) {
	if (type == "char" || type == "int" || type == "long" || type == "float" || type == "double") {
		return "0";
	} else {
		return "null";
	}
}
