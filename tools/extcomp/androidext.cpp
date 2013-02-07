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

		int returnPointerDepth = 0;
		string returnType = extractPointerType(f.returnType, returnPointerDepth);

		out << "<arg type=\"" << returnType << "\" "
				<< "ptr=\"" << returnPointerDepth << "\" "
				<< "ret=\"true\"/>\n";

		out << "</function>\n";
	}
	out << "</module>";

}

void writeAndroidStubs(string& outputDir, Interface& ext, string& androidPackageName) {
	_mkdir(outputDir.c_str());

	// The interface stub
	string extensionFileName = outputDir + "Extension.java";
	ofstream extensionFile(extensionFileName.c_str());

	extensionFile << "package " << androidPackageName << ";\n\n";

	extensionFile << "import com.mosync.api.*;\n";
	extensionFile << "import " << androidPackageName << ".types.*;\n\n";

	extensionFile << "public class Extension extends MoSyncExtension {\n\n";

	for (size_t i = 0; i < ext.constSets.size(); i++) {
		ConstSet cs = ext.constSets[i];
		string name = cs.name;
		for (size_t j = 0; j < cs.constants.size(); j++) {
			Constant c = cs.constants[j];
			extensionFile << "\tpublic final static " << toAndroidType(ext, c.type, false, true) << " " << name << c.name << " = " << c.value << ";\n";
		}
		extensionFile << "\n";
	}

	extensionFile << "\t/*public void onLoad(MoSyncContext context) {\n\t}*/\n\n";
	extensionFile << "\t/*public void onUnload(MoSyncContext context) {\n\t}*/\n\n";

	for (size_t i = 0; i < ext.functions.size(); i++) {
		Function f = ext.functions[i];
		string returnType = toAndroidType(ext, f.returnType, false, false);
		extensionFile << "\tpublic " << returnType << " " << f.name << "(";
		for (size_t j = 0; j < f.args.size(); j++) {
			if (j > 0) {
				extensionFile << ", ";
			}
			Argument arg = f.args[j];
			extensionFile << toAndroidType(ext, arg.type, false, arg.in);
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
	if (!ext.structs.empty()) {
		_mkdir(typesDir.c_str());
	}

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
			string androidType = toAndroidType(ext, type, false, false);
			structFile << "\tpublic " << androidType << " " << m.pod[0].name << ";\n";
			int pointerDepth = 0;
			string pointerType = extractPointerType(type, pointerDepth);
			structFile << "\tprivate final static Marshaller __" << m.pod[0].name << " = _m(\"" << ext.name << "\", \"" << pointerType << "\", " << pointerDepth << ");\n\n";
		}

		// Unmarshalling
		size_t size = 0;
		structFile << "\n\tpublic Struct unmarshal(byte[] data, int offset) {\n";
		structFile << "\t\t" << s.name << " s = new " << s.name << "();\n";
		for (size_t j = 0; j < s.members.size(); j++) {
			Member m = s.members[j];
			string ctype = m.pod[0].type;
			string name = m.pod[0].name;
			string cast = toAndroidType(ext, ctype, true, false);
			int padding = getPadding(ext, ctype);
			structFile << "\t\ts." << name << " = (" << cast << ")__" << name << ".unmarshal(data, offset + " << (size + padding) << ");\n";
			size += cTypeAlignedSize(ext, ctype);
		}
		structFile << "\t\treturn s;\n";
		structFile << "\t}\n\n";

		// Marshalling
		size = 0; // <-- Reset size to 0.
		structFile << "\tpublic void marshal(Object struct, byte[] data, int offset) {\n";
		structFile << "\t\t" << s.name << " s = (" << s.name << ") struct;\n";
		for (size_t j = 0; j < s.members.size(); j++) {
			Member m = s.members[j];
			string ctype = m.pod[0].type;
			string name = m.pod[0].name;
			string cast = toAndroidType(ext, ctype, true, false);
			int padding = getPadding(ext, ctype);
			structFile << "\t\t__" << name << ".marshal(s." << name << ", data, offset + " << (size + padding) << ");\n";
			size += cTypeAlignedSize(ext, ctype);
		}
		structFile << "\t}\n\n";

		structFile << "\tpublic int size() { return " << size << "; }\n\n";

		structFile << "}\n";

		structFile.close();
	}
}

string toAndroidType(Interface& ext, string& ctype, bool autoBox, bool constant) {
	int ptrDepth = 0;
	string extractedType = extractPointerType(ctype, ptrDepth);
	if (ptrDepth > 0) {
		string prefix = "";
		string suffix = "";
		for (int i = 0; i < ptrDepth; i++) {
			prefix.append("Pointer<");
			suffix.append(">");
		}
		return prefix + toAndroidType(ext, extractedType, true, constant) + suffix;
	} else {
		extractedType = resolveTypedef(ext, extractedType);
		if (extractedType == "NCString" || extractedType == "MAString") {
			return constant ? "String" : "CString";
		}
		if (autoBox) {
			if (extractedType == "int") return "Integer";
			if (extractedType == "char") return "Byte";
			if (extractedType == "double") return "Double";
			if (extractedType == "float") return "Float";
			if (extractedType == "long long") return "Long";
		} else if (extractedType == "int" ||
			extractedType == "double" ||
			extractedType == "float") {
			return extractedType;
		} else if (extractedType == "long long") {
			return "long";
		} else if (extractedType == "char") {
			return "byte";
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
