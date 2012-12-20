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
#include <iomanip>
#include <vector>

#include <idl-common/idl-common.h>
#include <idl-common/tokenizer.h>
#include <idl-common/stringFunctions.h>
#include <idl-common/types.h>
#include <idl-common/output-bindings.h>

#include "helpers/mkdir.h"
#include "helpers/types.h"

#include "extcomp.h"
#include "androidext.h"


using namespace std;

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
