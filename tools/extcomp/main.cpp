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

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <iomanip>
#include <vector>

#include <idl-common/idl-common.h>
#include <idl-common/tokenizer.h>
#include <idl-common/stringFunctions.h>
#include <idl-common/types.h>
#include <idl-common/output-bindings.h>

#include "helpers/mkdir.h"
#include "helpers/types.h"

using namespace std;

void streamFunctionCSignature(ostream&, Interface&, Function&);
void streamFunctionWrapper(ostream&, Interface&, Function&, bool);
string resolveTypedef(Interface&, string&);
void streamAndroidExtMF(ostream&, Interface&, string&, string&);
void streamAndroidStubs(string&, Interface&, string&);
void streamExtensionManifest(string&, string&);
string toAndroidType(Interface&, string&, bool);
void streamExtHashValue(ostream&, Interface&);
void streamXMLComment(ostream&, string& comment);
string getModHandle(Interface& ext);
string getFnIxDefine(Function& f);
string getFnIxHandle(Interface& f);
string getModHashDefine(Interface& ext);
string extractPointerType(string type, int& ptrDepth);

int main(int argc, const char** argv) {
	try {
		// TODO: Better args handling!
		string extDir = argv[1];
		string extName = argv[2];
		string androidPackageName = argv[3];
		string androidClassName = argv[4];

		// This one is empty for extensions.
		vector<string> ixs;
		ixs.push_back(extName);

		string inputIdl = extDir + "/" + extName + ".idl";
		printf("Parsing %s for extension %s.\n", inputIdl.c_str(), extName.c_str());
		chdir(extDir.c_str());
		Interface ext = parseInterface(ixs, inputIdl);
		ext.name = extName;

		string headerOut = extName + ".h";
		ofstream headerfile(headerOut.c_str());

		string sourceOut = "_" + extName + ".c";
		ofstream sourcefile(sourceOut.c_str());

		string modulePrefix = "EXT_MODULE_";
		string extHashConst = getModHashDefine(ext);
		string uExtName = toupper(string(extName));
		string extDef = modulePrefix + uExtName + "_H";

		headerfile << "#ifndef " << extDef << "\n";
		headerfile << "#define " << extDef << "\n\n";
		headerfile << "#define " << extHashConst << " ((int)0x";
		streamExtHashValue(headerfile, ext);
		headerfile << ")\n\n";

		sourcefile << "// *** GENERATED FILE - Do not modify ***\n\n";
		sourcefile << "#include <maapi.h>\n";
		sourcefile << "#include \"" << extName << ".h\"\n";
		sourcefile << "static MAExtensionModule " << getModHandle(ext) << " = 0;\n";
		sourcefile << "static MAExtensionFunction " << getFnIxHandle(ext) << "[" << ext.functions.size() << "];\n";

		for (size_t i = 0; i < ext.typedefs.size(); i++) {
			Typedef t = ext.typedefs[i];
			headerfile << "typedef " << t.name << " " << t.type << ";\n\n";
		}

		for (size_t i = 0; i < ext.structs.size(); i++) {
			Struct s = ext.structs[i];
			headerfile << "typedef struct {\n";
			for (size_t j = 0; j < s.members.size(); j++) {
				Member m = s.members[j];
				headerfile << "\t" << m.pod[0].type << " " << m.pod[0].name << ";\n";
			}
			headerfile << "} " << s.name << ";\n\n";
		}

		for (size_t i = 0; i < ext.functions.size(); i++) {
			Function f = ext.functions[i];
			headerfile << "#define " << getFnIxDefine(f) << " " << i << "\n";
			streamFunctionCSignature(headerfile, ext, f);
			headerfile << ";\n\n";

			streamFunctionWrapper(sourcefile, ext, f, i == 0);
		}
		headerfile << "#endif //" << extDef << "\n";

		headerfile.close();
		sourcefile.close();

		string androidOut = extDir + "/android/";
		_mkdir(androidOut.c_str());
		string androidManifestOut = androidOut + "assets/";
		_mkdir(androidManifestOut.c_str());
		string androidMFOut = androidManifestOut + extName + ".xml";
		ofstream androidMFfile(androidMFOut.c_str());

		streamAndroidExtMF(androidMFfile, ext, androidPackageName, androidClassName);
		androidMFfile.close();

		string stubsDir = androidOut + "stubs/";
		streamAndroidStubs(stubsDir, ext, androidPackageName);

		streamExtensionManifest(extDir, extName);
	} catch (exception e) {
		printf("Failed: %s\n", e.what());
		return 1;
	}

	return 0;
}

void streamExtensionManifest(string& extDir, string& extName) {
	string manifestOut = extDir + "/extension.mf";
	ofstream manifest(manifestOut.c_str());
	manifest << "name = " << extName << "\n";
	manifest.close();
}

string getFnIxDefine(Function &f) {
	string uName = toupper(string(f.name));
	return string("FN_IX_") + uName;
}

string getFnIxHandle(Interface& ext) {
	string uName = toupper(string(ext.name));
	return string("FN_IX_") + uName;
}

string getModHashDefine(Interface& ext) {
	string uExtName = toupper(string(ext.name));
	string extHashConst = uExtName + "_IDL_HASH";
	return extHashConst;
}

void streamFunctionCSignature(ostream& out, Interface& ext, Function& f) {
	out << cType(ext, f.returnType);
	out << " ";
	out << f.name;
	out << "(";
	for (size_t j = 0; j < f.args.size(); j++) {
		if (j > 0) {
			out << ", ";
		}
		Argument arg = f.args[j];
		out << cType(ext, arg.type);
		out << " ";
		out << "_" << arg.name;
	}
	out << ")";
}

string getModHandle(Interface& ext) {
	string modHandle = "mod_handle_" + ext.name;
	return modHandle;
}

void streamFunctionWrapper(ostream& out, Interface& ext, Function& f, bool modHandle) {
	streamFunctionCSignature(out, ext, f);
	int numargs = f.args.size();
	out << " {\n";
	out << "    int i, result;\n";
	out << "    int passedArgs[" << numargs << "];\n";
	for (int i = 0; i < numargs; i++) {
		string name = "_" + f.args[i].name;
		// For simplicity we just always pass the pointer
		string arg = ("(&" + name + ")");
		out << "    passedArgs[" << i << "] = (int) " << arg << ";\n";
	}
	out << "    if (!" << getModHandle(ext) << ") {\n";
	out << "         " << getModHandle(ext) << " = maExtensionModuleLoad(\"" << ext.name << "\", ";
	out << getModHashDefine(ext) << ");\n";
	out << "         for (i = 0; i < " << ext.functions.size() << "; i++) {\n";
	out << "             " << getFnIxHandle(ext) << "[i] = maExtensionFunctionLoad(" << getModHandle(ext) << ", i);\n";
	out << "         }\n";
	out << "    }\n";
	out << "    result = maExtensionFunctionInvoke2(";
	out << getFnIxHandle(ext) << "[" << getFnIxDefine(f) << "], " << numargs << ", (int) passedArgs/*, 0dummy*/);\n";
	if (strcmp("void", cType(ext, f.returnType).c_str())) {
		out << "    return result;\n";
	}
	out << "}\n";

}

void streamExtHashValue(ostream& out, Interface& ext) {
	out << setfill('0') << setw(8) << hex << calculateChecksum(ext) << dec;
}

string resolveTypedef(Interface& ext, string& typedefName) {
	for (size_t i = 0; i < ext.typedefs.size(); i++) {
		Typedef t = ext.typedefs[i];
		if (t.type == typedefName) {
			return resolveTypedef(ext, t.name);
		}
	}
	return typedefName;
}

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
	// TODO: The actual interface.
	_mkdir(outputDir.c_str());
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
			structFile << "\t\t" << name << " = (" << cast << ")__" << name << ".unmarshal(data, offset + " << size << ");\n";
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

void streamXMLComment(ostream& out, string& comment) {
	if (!comment.find("-->") != string::npos && !comment.empty()) {
		// We just skip comments that could mess the xml up;
		// has no semantic meaning anyway!
		out << "<!-- " << comment << " -->\n";
	}
}
string extractPointerType(string type, int& ptrDepth) {
	size_t pos = 0;
	size_t arrayType = string::npos;
	while (pos != string::npos) {
		pos++;
		ptrDepth++;
		pos = type.find_first_of("*", pos);
		if (arrayType == string::npos) {
			arrayType = pos;
		}
	}
	ptrDepth--;

	return arrayType == string::npos ? type : type.substr(0, arrayType);
}

void streamInvokePrefix(ostream& stream, const Function& f) {
	// Must have this to compile.
}
