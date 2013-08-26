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
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

#include <idl-common/idl-common.h>
#include <idl-common/types.h>

#include "helpers/mkdir.h"
#include "helpers/types.h"

#include "extcomp.h"
#include "androidext.h"

#include "util.h"

#define JAVA_STRING_CLASS "Ljava/lang/String;"
#define CSTRING_CLASS "Lcom/mosync/api/CString;"
#define POINTER_CLASS "Lcom/mosync/api/Pointer;"

void generateAndroidFiles(string& outputDir, Interface& ext, string& androidPackageName, string& androidClassName) {
	string extName = ext.name;
	toDir(outputDir);
	string androidMFOut = outputDir + extName + ".xml";
	ofstream androidMFfile(androidMFOut.c_str());

	// TODO: We can use the same mechanism for native and interpreted
	// at least until we know it works satisfactorily
	//writeJNIBridge(outputDir, ext, androidPackageName);

	streamAndroidExtMF(androidMFfile, ext, androidPackageName, androidClassName);
	printf("Generated module manifest %s\n", androidMFOut.c_str());
	androidMFfile.close();
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

		int returnPointerDepth = 0;
		string returnType = extractPointerType(f.returnType, returnPointerDepth);

		out << "<arg type=\"" << returnType << "\" "
				<< "ptr=\"" << returnPointerDepth << "\" "
				<< "ret=\"true\"/>\n";

		out << "</function>\n";
	}
	out << "</module>";

}

void writeJNIBridge(string& outputDir, Interface& ext, string& androidPackageName) {
	string bridgeFileName = outputDir + "/_ext.android.c";
	ofstream bridgeFile(bridgeFileName.c_str());

	string extName = ext.name;

	bridgeFile << "// *** GENERATED FILE - Do not modify ***\n\n";
	bridgeFile << "#if defined(MOSYNC_NATIVE) && defined(__ANDROID__)\n\n";
	bridgeFile << "#include <maapi.h>\n";
	bridgeFile << "#include <jni.h>\n";
	bridgeFile << "#include \"" << extName << ".h\"\n";

	bridgeFile << "JNIEnv* g_" << extName << ";\n";
	bridgeFile << "jobject* g_This_" << extName << ";\n";

	for (size_t i = 0; i < ext.functions.size(); i++) {
		streamJNIFunctionWrapper(bridgeFile, ext, ext.functions[i], androidPackageName);
	}

	bridgeFile << "#endif";
	bridgeFile.flush();
	bridgeFile.close();
}

void streamJNIFunctionWrapper(ostream& out, Interface& ext, Function& f, string& androidPackageName) {
	// TODO: Refs to jni env and this should be global + deleted.
	streamFunctionCSignature(out, ext, f);
	out << "{\n";
	out << "jclass cls = mJNIEnv->GetObjectClass(mJThis);\n";
	out << "jmethodID methodID = mJNIEnv->GetMethodID(cls, \""
			<< f.name << "\",\"" << getJavaByteCodeSignature(ext, f, androidPackageName)
			<< "\");\n";

	stringstream preamble;
	stringstream postamble;
	stringstream call;

	bool needsReturn = isReturnType(ext, f.returnType);
	if (needsReturn) {
		call << f.returnType;
		call << " result = ";
	}

	string callType = "Object";
	if (f.returnType == "int") callType = "Int";
	if (f.returnType == "double") callType = "Double";
	if (f.returnType == "float") callType = "Float";
	if (f.returnType == "long long") callType = "Long";
	if (f.returnType == "char") callType = "Byte";
	if (f.returnType == "void") callType = "Void";

	call << "mJNIEnv->Call" << callType << "Method(";

	for (size_t i = 0; i < f.args.size(); i++) {
		if (i > 0) call << ",";
		Argument arg = f.args[i];
		string name = safeArg(arg.name);
		string bytecodeType = toAndroidType(ext, androidPackageName, arg.type, false, arg.in, true);
		string wrapperFn;
		string prefix;
		string cast;
		if (bytecodeType == "I") cast = "(jint)";
		if (bytecodeType == "D") cast = "(jdouble)";
		if (bytecodeType == "F") cast = "(jfloat)";
		if (bytecodeType == "B") cast = "(jbyte)";
		if (bytecodeType == POINTER_CLASS) wrapperFn = "createJNIPointer";
		else if (bytecodeType == CSTRING_CLASS) wrapperFn = "createCString";
		else if (bytecodeType == JAVA_STRING_CLASS) wrapperFn = "createJavaString";
		else if (bytecodeType.substr(0, 1) == "L") wrapperFn = "createStruct_" + arg.type;
		if (!wrapperFn.empty()) {
			prefix = "wr";
			preamble << "jobject " << prefix << name << " = " << wrapperFn << "(" << name << ");\n";
			postamble << "mJNIEnv->DeleteLocalRef(" << prefix << name << ");\n";
		}
		call << cast << prefix << name;
	}
	call << ");\n";
	out << preamble.str();
	out << call.str();
	out << postamble.str();

	if (needsReturn) {
		out << "return result;\n";
	}
	out << "}\n";
}

string getJavaByteCodeSignature(Interface& ext, Function& f, string& androidPackageName) {
	string result = "(";
	for (size_t i = 0; i < f.args.size(); i++) {
		Argument arg = f.args[i];
		string ctype = arg.type;
		result += toAndroidType(ext, androidPackageName, ctype, false, arg.in, true);
	}
	result += ")";
	result += toAndroidType(ext, androidPackageName, f.returnType, false, false, true);
	return result;
}

string toJavaClassName(string& name) {
	string result(name);
	result[0] = toupper(result[0]);
	return result;
}

void writeExtensionClass(string& extensionFileName, Interface& ext, string& androidPackageName, bool interface) {
	ofstream extensionFile(extensionFileName.c_str());

	string name = toJavaClassName(ext.name);

	extensionFile << "package " << androidPackageName;
	if (interface) {
		extensionFile << ".interfaces";
	}
	extensionFile << ";\n\n";

	extensionFile << "import com.mosync.api.*;\n";
	if (!ext.structs.empty()) {
		extensionFile << "import " << androidPackageName << ".types.*;\n";
	}

	if (interface) {
		extensionFile << "public interface I" << name << " {\n\n";
		extensionFile << "\tpublic final static String MODULE_NAME = \"" << ext.name << "\";\n\n";
		extensionFile << "\tpublic final static int MODULE_HASH = 0x";
		streamExtHashValue(extensionFile, ext);
		extensionFile << ";\n\n";

		for (size_t i = 0; i < ext.constSets.size(); i++) {
			ConstSet cs = ext.constSets[i];
			string name = cs.name;
			for (size_t j = 0; j < cs.constants.size(); j++) {
				Constant c = cs.constants[j];
				extensionFile << "\tpublic final static " << toAndroidType(ext, androidPackageName, c.type, false, true, false) << " " << name << c.name << " = " << c.value << ";\n";
			}
			extensionFile << "\n";
		}
	} else {
		extensionFile << "import " << androidPackageName << ".interfaces.I" << name << ";\n\n";
		extensionFile << "public class " << name << " extends MoSyncExtension implements I" << name << " {\n\n";
		extensionFile << "\tpublic void onLoad(MoSyncContext context) {\n\t\tsuper.onLoad(context);\n\t}\n\n";
		extensionFile << "\tpublic void onUnload(MoSyncContext context) {\n\t\tsuper.onUnload(context);\n\t}\n\n";
	}

	for (size_t i = 0; i < ext.functions.size(); i++) {
		Function f = ext.functions[i];
		string returnType = toAndroidType(ext, androidPackageName, f.returnType, false, false, false);
		extensionFile << "\tpublic " << returnType << " " << f.name << "(";
		for (size_t j = 0; j < f.args.size(); j++) {
			if (j > 0) {
				extensionFile << ", ";
			}
			Argument arg = f.args[j];
			extensionFile << toAndroidType(ext, androidPackageName, arg.type, false, arg.in, false);
			extensionFile << " ";
			extensionFile << arg.name;
		}
		extensionFile << ")";
		if (interface) {
			extensionFile << ";\n\n";
		} else {
			extensionFile << " {\n\t\t/* TODO: Replace this with an implementation */\n";
			if (isReturnType(ext, f.returnType)) {
				extensionFile << "\t\treturn " << getAndroidDefaultValue(returnType) << ";\n";
			}
			extensionFile << "\t}\n\n";
		}
	}

	extensionFile << "}\n";

	extensionFile.close();
}

void writeAndroidStubs(string& outputDir, Interface& ext, string& androidPackageName, bool generateImplStub) {
	_mkdir(outputDir.c_str());

	string packageOutputDir = outputDir;
	vector<string> fragments;
	split(fragments, androidPackageName, ".");
	for (size_t i = 0; i < fragments.size(); i++) {
		packageOutputDir = packageOutputDir + "/" + fragments[i];
		_mkdir(packageOutputDir.c_str());
	}
	toDir(packageOutputDir);

	// The interface stub
	if (generateImplStub) {
		string extensionFileName = packageOutputDir + toJavaClassName(ext.name) + ".java";
		writeExtensionClass(extensionFileName, ext, androidPackageName, false);
	}

	string name = toJavaClassName(ext.name);
	string interfaceDir = packageOutputDir + "/interfaces/";
	_mkdir(interfaceDir.c_str());
	string interfaceFileName = interfaceDir + "I" + name + ".java";
	writeExtensionClass(interfaceFileName, ext, androidPackageName, true);

	printf("Generated Android classes for %s in directory %s\n", ext.name.c_str(), outputDir.c_str());

	// The structs
	string typesDir = packageOutputDir + "types/";
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
			string androidType = toAndroidType(ext, androidPackageName, type, false, false, false);
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
			string cast = toAndroidType(ext, androidPackageName, ctype, true, false, false);
			int padding = getPadding(ext, ctype, size);
			structFile << "\t\ts." << name << " = (" << cast << ")__" << name << ".unmarshal(data, offset + " << (size + padding) << ");\n";
			size += cTypeAlignedSize(ext, ctype) + padding;
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
			string cast = toAndroidType(ext, androidPackageName, ctype, true, false, false);
			int padding = getPadding(ext, ctype, size);
			structFile << "\t\t__" << name << ".marshal(s." << name << ", data, offset + " << (size + padding) << ");\n";
			size += cTypeAlignedSize(ext, ctype) + padding;
		}
		structFile << "\t}\n\n";

		structFile << "\tpublic int size() { return " << cTypeAlignedSize(ext, s.name) << "; }\n\n";

		structFile << "}\n";

		structFile.close();
	}
}

string toAndroidType(Interface& ext, string& androidPackageName, string& ctype, bool autoBox, bool constant, bool bytecode) {
	int ptrDepth = 0;
	string extractedType = extractPointerType(ctype, ptrDepth);
	if (ptrDepth > 0) {
		if (bytecode) {
			return POINTER_CLASS;
		} else {
			string prefix = "";
			string suffix = "";
			for (int i = 0; i < ptrDepth; i++) {
				prefix.append("Pointer<");
				suffix.append(">");
			}
			return prefix + toAndroidType(ext, androidPackageName, extractedType, true, constant, bytecode) + suffix;
		}
	} else {
		extractedType = resolveTypedef(ext, extractedType);
		if (extractedType == "NCString" || extractedType == "MAString") {
			if (bytecode) {
				return constant ? JAVA_STRING_CLASS : CSTRING_CLASS;
			} else {
				return constant ? "String" : "CString";
			}
		}
		if (autoBox) {
			if (extractedType == "int") return "Integer";
			if (extractedType == "char") return "Byte";
			if (extractedType == "double") return "Double";
			if (extractedType == "float") return "Float";
			if (extractedType == "long long") return "Long";
		} else {
			if (extractedType == "int") return (bytecode ? "I" : "int");
			if (extractedType == "double") return (bytecode ? "D" : "double");
			if (extractedType == "float") return (bytecode ? "F" : "float");
			if (extractedType == "void") return (bytecode ? "V" : "void");
			if (extractedType == "long long") return (bytecode ? "J" : "long");
			if (extractedType == "char") return (bytecode ? "B" : "byte");
			// S - short, Z - boolean
		}

		// The rest are structs
		return bytecode ?
				getBytecodeStructClass(androidPackageName, extractedType) :
				extractedType;
	}
}

string getBytecodeStructClass(string& package, string& type) {
	string result = "L" + package + ".types." + type + ";";
	::replace(result.begin(), result.end(), '.', '/');
	return result;
}

string getAndroidDefaultValue(string& type) {
	if (type == "char" || type == "int" || type == "long" || type == "float" || type == "double") {
		return "0";
	} else {
		return "null";
	}
}
