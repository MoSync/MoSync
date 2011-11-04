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
#include <stdlib.h>
#include <string.h>
#include <iomanip>

#include <idl-common/idl-common.h>
#include <idl-common/tokenizer.h>
#include <idl-common/stringFunctions.h>
#include <idl-common/types.h>
#include <idl-common/output-bindings.h>

#include "helpers/mkdir.h"
#include "helpers/types.h"

#include "core.h"

#define MIN(x, y) ((x)<(y)?(x):(y))
#define MAX(x, y) ((x)>(y)?(x):(y))

using namespace std;

static vector<string> readExtensions(const char* filename);
static void outputMaapi(const vector<string>& ixs, const Interface& maapi);
static void outputMaapiJavascript(const vector<string>& ixs, const Interface& maapi);
static void outputMaapiCSharp(const vector<string>& ixs, const Interface& maapi);
static void outputRuntimeBuilderFiles(const Interface& maapi);
static void outputCpp(const Interface& maapi);
static void outputAsmConfigLst(const Interface& maapi);
static void outputAsmConfigH(const Interface& maapi);
static void outputDllDefine(const Interface& maapi);
static void outputInvokeSyscallCpp(const Interface& maapi);
static void outputInvokeSyscallArmRecompiler(const Interface& maapi);
static void outputInvokeSyscallJava(const Interface& maapi);
static void outputSyscallStaticJava(const Interface& maapi);
static void outputSyscallStaticCpp(const Interface& maapi);
static void outputCoreConsts();
static void outputConsts(const string& filename, const Interface& inf, int ix);
static void outputConstSets(const Interface& maapi);

static bool filesAreEqual(const char* a, const char* b) {
	ifstream as(a, ios_base::in | ios_base::binary);
	ifstream bs(b, ios_base::in | ios_base::binary);

	// compare size
	as.seekg(0, ios_base::end);
	streamoff aSize = as.tellg();
	bs.seekg(0, ios_base::end);
	streamoff bSize = bs.tellg();
	if(!(as.good() && bs.good() && (aSize == bSize)))
		return false;

	// compare content
	as.seekg(0);
	bs.seekg(0);
	static const size_t BUFSIZE = 64*1024;	//arbitrary
	char aBuf[BUFSIZE], bBuf[BUFSIZE];
	ssize_t pos = 0;
	while(pos < aSize) {
		size_t len = MIN(BUFSIZE, (size_t)(aSize - pos));
		as.read(aBuf, len);
		bs.read(bBuf, len);
		if(!(as.good() && bs.good())) {
			printf("Warning: file read error.\n");
			return false;
		}
		if(memcmp(aBuf, bBuf, len) != 0)
			return false;
		pos += len;
	}
	return true;
}

#ifdef WIN32
// change slashes
static void u2d(string& path) {
	for(size_t i=0; i<path.size(); i++) {
		if(path[i] == '/')
			path[i] = '\\';
	}
}
#endif

static void copy(string src, string dst) {
	// check if the file is different. if not, don't copy.
	size_t off = src.find_last_of('/');
	if(off == string::npos)
		off = 0;
	else
		off++;
	string dstFile = dst + src.substr(off);
	if(filesAreEqual(src.c_str(), dstFile.c_str())) {
		printf("Files are equal: %s => %s\n", src.c_str(), dstFile.c_str());
		return;
	}

#ifdef WIN32
	u2d(src);
	u2d(dst);
	runCommand("copy /Y " + src + " " + dst);
#else
	runCommand("cp " + string(src) + " " + string(dst));
#endif
}

/**
 * This is where the idl file is parsed and output files are generated.
 */
int main() {
	try {
		// Generated files goes to this folder, is then copied to
		// target folders.
		_mkdir("Output");

		// Read and parse the extensions and idl files.
		vector<string> ixs = readExtensions("extensions.h");
		Interface maapi = parseInterface(ixs, "maapi.idl");

		// Generate files for the MoSync API.
		outputMaapi(ixs, maapi);
		outputMaapiJavascript(ixs, maapi);
		outputMaapiCSharp(ixs, maapi);

		// Generate files used when building the runtimes.
		outputRuntimeBuilderFiles(maapi);

		// Generate core constants.
		outputCoreConsts();

		// Generate headefile suitable for use with the tolua binding library.
		// See comment in output-bindings.h for notes on how to use thsi file
		// with tolua.
		lua_outputHeaderFile(maapi, ixs, "Output/lua_maapi.h");

		// Create directory for include files.
		// TODO: Document how this directory is used.
		// Is this the incldue directory in the MoSync installation?
		// Do we copy files there in this program?
		_mkdir((MOSYNCDIR + "/include").c_str());

		// Create the new generated folder for java files.
		_mkdir("../../runtimes/java/Shared/generated");

		copy("Output/maapi.h", "../../libs/MAStd/");
		copy("maapi_defs.h", "../../libs/MAStd/");
		copy("Output/maapi.h", "../../libs/newlib/libc/sys/mosync/");
		copy("maapi_defs.h", "../../libs/newlib/libc/sys/mosync/");

		// Create directory for binary files
		_mkdir((MOSYNCDIR + "/bin").c_str());
		copy("Output/asm_config.lst", "" + MOSYNCDIR + "/bin/");

		copy("Output/invoke_syscall_java.h", "../../runtimes/java/Shared/generated/");
		copy("Output/syscall_static_java.h", "../../runtimes/java/Shared/generated/");
		copy("Output/core_consts.h", "../../runtimes/java/Shared/generated/");
		copy("Output/MAAPI_consts.h", "../../runtimes/java/Shared/generated/");

		// Create folder for generated java files in the Android runtime.
		_mkdir("../../runtimes/java/platforms/androidJNI/AndroidProject/src/com/mosync/internal/generated");

		// Copy generated Android file.
		copy("Output/MAAPI_consts.java",
			"../../runtimes/java/platforms/androidJNI/AndroidProject/src/com/mosync/internal/generated/");

		// Copy windows phone file.
		copy("Output/maapi.cs", "../../runtimes/csharp/windowsphone/mosync/mosyncRuntime/Source/");

		copy("Output/cpp_defs.h", "../../intlibs/helpers/");
		copy("Output/cpp_maapi.h", "../../intlibs/helpers/");
		copy("maapi_defs.h", "../../intlibs/helpers/");

		copy("Output/invoke_syscall_cpp.h", "../../runtimes/cpp/core/");
		copy("Output/syscall_static_cpp.h", "../../runtimes/cpp/platforms/iphone/Classes/");
		copy("Output/invoke_syscall_arm_recompiler.h", "../../runtimes/cpp/core/");
		copy("Output/asm_config.h", "../../intlibs/helpers/");

		copy("Output/mosynclib.def", "../../runtimes/cpp/platforms/sdl/mosynclib/");

		//copy("Output/Syscall.java ../../testPrograms/");

		copy("Output/constSets.h", "../../intlibs/helpers/");

		for(size_t i=0; i<ixs.size(); i++) {
			copy("Output/CPP_" + ixs[i] + ".h", "../../intlibs/helpers/");
			copy("Output/" + ixs[i] + ".h", "../../libs/MAStd/");
			copy("Output/" + ixs[i] + ".h", "../../libs/newlib/libc/sys/mosync/");
			copy("Output/" + ixs[i] + "_CONSTS.h", "../../runtimes/java/Shared/generated/");

			// Copy generated Android files.
			copy("Output/" + ixs[i] + ".java",
				"../../runtimes/java/platforms/androidJNI/AndroidProject/src/com/mosync/internal/generated/");
		}

		return 0;
	} catch(exception& e) {
		printf("Exception: %s\n", e.what());
		return 1;
	}
}

static vector<string> readExtensions(const char* filename) {
	vector<string> ixs;
	ifstream file(filename);
	setTokenStream(&file);
	string token;
	for(;;) {
		readToken(token);
		if(!file.good())
			break;
		if(token != "#define")
			tokenError(token);
		readTextToken(token);
		if(!file.good())
			Error("syntax", "Unexpected end of file");
		ixs.push_back(token);
	}
	return ixs;
}

/**
 * Generate files for the MoSync API.
 */
static void outputMaapi(const vector<string>& ixs, const Interface& maapi) {
	// Generate files for the main API.
	{
		ofstream ccpDefsFile("Output/cpp_defs.h");
		streamCppDefsFile(ccpDefsFile, maapi, ixs, MAIN_INTERFACE);

		outputConsts("Output/MAAPI_consts.h", maapi, MAIN_INTERFACE);

		// This is where maapi.h is generated.
		ofstream headerFile("Output/maapi.h");
		streamHeaderFile(headerFile, maapi, ixs, MAIN_INTERFACE);

		// Generate Java definition file for the main API (used by
		// the Android runtime).
		string className = "MAAPI_consts";
		ofstream javaFile(("Output/" + className + ".java").c_str());
		streamJavaDefinitionFile(javaFile, className, maapi, MAIN_INTERFACE);
	}

	// Generate files for API extensions.
	for (size_t i=0; i<ixs.size(); i++) {
		ofstream ccpDefsFile(("Output/CPP_" + toupper(ixs[i]) + ".h").c_str());
		streamCppDefsFile(ccpDefsFile, maapi, ixs, i);

		outputConsts("Output/" + toupper(ixs[i]) + "_CONSTS.h", maapi, i);

		ofstream headerFile(("Output/" + toupper(ixs[i]) + ".h").c_str());
		streamHeaderFile(headerFile, maapi, ixs, i);

		// Generate Java definition file for extension (used by
		// the Android runtime).
		string className = toupper(ixs[i]);
		ofstream javaFile(("Output/" + className + ".java").c_str());
		streamJavaDefinitionFile(javaFile, className, maapi, i);
	}
}


static bool isKeyInConstsets(const Interface& maapi, const string& str) {
	for(size_t j=0; j<maapi.constSets.size(); j++) {
		const ConstSet& cs(maapi.constSets[j]);
		for(size_t k=0; k<cs.constants.size(); k++) {
			const Constant& c(cs.constants[k]);
			if(str == (cs.name + c.name))
				return true;
		}
	}
	return false;
}

static void outputJavascriptSyscallArg(ofstream& maapiFile, int i) {
	if(i < 4) {
		maapiFile << "this.regs[Reg.i" << i << "]";
	} else {
		maapiFile << "this.getStackValue(" << ((i-4)<<2) << ")";
	}
}

static void outputJavascriptIoctlArg(ofstream& maapiFile, int i) {
	if(i < 4) {
		maapiFile << "i" << (i);
	} else {
		maapiFile << "this.getStackValue(" << ((i-4)<<2) << ")";
	}
}

static void outputMaapiJavascript(const vector<string>& ixs, const Interface& maapi) {
	ofstream maapiFile("Output/maapi.js");

	maapiFile << "MoSyncGenerated = {};\n\n";

	// generate hash
	maapiFile << "MoSyncHash = " << "0x" << setfill('0') << setw(8) << hex << calculateChecksum(maapi) << dec << ";\n\n";

	// generate constant table.
	maapiFile << "MoSyncConstants = {\n";
	for(size_t j=0; j<maapi.constSets.size(); j++) {
		const ConstSet& cs(maapi.constSets[j]);
		for(size_t k=0; k<cs.constants.size(); k++) {
			const Constant& c(cs.constants[k]);
			//printf("%s = %s;\n", c.name.c_str(), c.value.c_str());
			bool isKey = isKeyInConstsets(maapi, c.value);
			string keyString = (isKey)?"this.":"";

			maapiFile << "\t" << cs.name << c.name.c_str() << ": " << keyString << c.value.c_str();
			if(j==maapi.constSets.size()-1 && k == cs.constants.size()-1)
				maapiFile << "\n";
			else
				maapiFile << ",\n";
		}
	}
	maapiFile << "};\n";

	// generate struct wrappers (todo)

	// generate invoke syscall
	maapiFile << "MoSyncGenerated.invokeSyscall = function(id) {\n";
	maapiFile << "\tswitch(id) {\n";
	for(size_t j=0; j<maapi.functions.size(); j++) {
		const Function& f(maapi.functions[j]);

		maapiFile << "\t\tcase " << f.number << ":\n";

		maapiFile << "\t\t\tret = this.Syscalls." << f.name << "(";

		int i = 0;
		for(size_t k=0; k<f.args.size(); k++) {
			const Argument& a(f.args[k]);
			if(k != 0)
				maapiFile << ", ";
			if(a.type == "double") {
				//maapiFile << "this.regs[Reg.i" << i << "], ";
				outputJavascriptSyscallArg(maapiFile, i);
				maapiFile << ", ";
				i++;
//				maapiFile << "this.regs[Reg.i" << i << "] ";
				outputJavascriptSyscallArg(maapiFile, i);
				i++;
			} else {
				//maapiFile << "this.regs[Reg.i" << i << "] ";
				outputJavascriptSyscallArg(maapiFile, i);
				i++;
			}
		}
		maapiFile << ");\n";

		if(f.returnType == "double") {
			maapiFile << "\t\t\tthis.regs[Reg.r14] = ret.hi;\n";
			maapiFile << "\t\t\tthis.regs[Reg.r15] = ret.lo;\n";
		} else {
			maapiFile << "\t\t\tthis.regs[Reg.r14] = (ret&0xffffffff);\n";
		}

		maapiFile << "\t\tbreak;\n";
	}
	maapiFile << "\t};\n";
	maapiFile << "};\n";

	// generate ioctl invoke
	const vector<Ioctl>& ioctls = maapi.ioctls;
	maapiFile << "MoSyncGenerated.maIOCtl = function(id, i1, i2, i3) {\n";
	maapiFile << "\tswitch(id) {\n";
	for(size_t i=0; i<ioctls.size(); i++) {
		const Ioctl& ioctl(ioctls[i]);
		for(size_t j=0; j<ioctl.functions.size(); j++) {
			const Function& f(ioctl.functions[j].f);
			int l = 1;
			maapiFile << "\t\tcase " << f.number << ":\n";
			maapiFile << "\t\t\tif(this." << f.name << " == undefined)\n";
			maapiFile << "\t\t\t\treturn MoSyncConstants.IOCTL_UNAVAILABLE;\n";
			maapiFile << "\t\t\treturn this." << f.name << "(";
			for(size_t k=0; k<f.args.size(); k++) {
				const Argument& a(f.args[k]);
				if(k != 0)
					maapiFile << ", ";
				if(a.type == "double") {
//					maapiFile << "i" << l << ", ";
					outputJavascriptIoctlArg(maapiFile, l);
					maapiFile << ", ";
					l++;
//					maapiFile << "i" << l << "";
					outputJavascriptIoctlArg(maapiFile, l);
					l++;
				} else {
//					maapiFile << "i" << l << "";
					outputJavascriptIoctlArg(maapiFile, l);
					l++;
				}
			}
			maapiFile << ");\n";
		}
	}
	maapiFile << "\t};\n";
	maapiFile << "};\n";
}


static std::string getCSharpType(const Interface& maapi, const std::string& maapiType, bool in) {
	std::string resolvedMaapiType = resolveType(maapi, maapiType);
	if(resolvedMaapiType == "unsigned char" || resolvedMaapiType == "unsigned int" || resolvedMaapiType == "unsigned long"
		|| resolvedMaapiType == "unsigned short" || resolvedMaapiType == "long int")
		resolvedMaapiType = "int";

	if(in == false)
		resolvedMaapiType += "*";

	std::string returnType = jType(maapi, resolvedMaapiType);

	if(resolvedMaapiType == "double")
		returnType = "double";
	else if(resolvedMaapiType == "float")
		returnType = "float";
	else if(resolvedMaapiType == "long long")
		returnType = "long";

	if(returnType == "noreturn")
		returnType = "void";
	else if(returnType == "MAString" || returnType == "MAWString")
		//returnType = "String";
		returnType = "int";
	else if(returnType == "MAExtent")
		returnType = "int";
	else if(returnType == "MAAddress")
		returnType = "int";
	else if(returnType == "MAHandle")
		returnType = "int";
	return returnType;
}

static void outputCSharpSyscallArg(ofstream& maapiFile, int i) {
	if(i < 4) {
		maapiFile << "mCore.GetRegisterValue(CoreInterpreted.Reg.I" << i << ")";
	} else {
		maapiFile << "mCore.GetStackValue(" << ((i-4)<<2) << ")";
	}
}

static void outputCSharpSyscallNativeArg(ofstream& maapiFile, int i) {
	if(i < 4) {
		maapiFile << "i" << i;
	} else {
		maapiFile << "mCore.GetStackValue(" << ((i-4)<<2) << ")";
	}
}

static void outputCSharpIoctlArg(ofstream& maapiFile, int i) {
	if(i < 3) {
		maapiFile << (char)('a'+i);
	} else {
		maapiFile << "mCore.GetStackValue(" << ((i-3)<<2) << ")";
	}
}

static void outputCSharpSyscallArgTyped(ofstream& maapiFile, int& i, const Argument& a,
	const Interface& maapi)
{
	std::string argType = getCSharpType(maapi, a.type, a.in);
	if(argType == "double") {
		maapiFile << "MoSync.Util.ConvertToDouble(";
		outputCSharpSyscallArg(maapiFile, i);
		maapiFile << ", ";
		i++;
		outputCSharpSyscallArg(maapiFile, i);
		i++;
		maapiFile << ")";
	}
	else if(argType == "float") {
		maapiFile << "MoSync.Util.ConvertToFloat(";
		outputCSharpSyscallArg(maapiFile, i);
		i++;
		maapiFile << ")";
	} else {
		outputCSharpSyscallArg(maapiFile, i);
		i++;
	}
}

static void outputCSharpIoctlArgTyped(ofstream& maapiFile, int& argindex, const Argument& a,
	const Interface& maapi)
{
	std::string argType = getCSharpType(maapi, a.type, a.in);
	if(argType == "double") {
		maapiFile << "MoSync.Util.ConvertToDouble(";
		outputCSharpIoctlArg(maapiFile, argindex);
		maapiFile << ", ";
		argindex++;
		outputCSharpIoctlArg(maapiFile, argindex);
		argindex++;
		maapiFile << ")";
	}
	else if(argType == "float") {
		maapiFile << "MoSync.Util.ConvertToFloat(";
		outputCSharpIoctlArg(maapiFile, argindex);
		argindex++;
		maapiFile << ")";
	} else {
		outputCSharpIoctlArg(maapiFile, argindex);
		argindex++;
	}
}

static void outputMaapiCSharp(const vector<string>& ixs, const Interface& maapi) {
	ofstream maapiFile("Output/maapi.cs");

	maapiFile << "using System;\n";
	maapiFile << "namespace MoSync {\n";

	// generate constant table.
	maapiFile << "public class Constants {\n";

	// generate hash
	maapiFile << "\tpublic const uint MoSyncHash = " << "0x" << setfill('0') << setw(8) << hex << calculateChecksum(maapi) << dec << ";\n";

	for(size_t j=0; j<maapi.constSets.size(); j++) {
		const ConstSet& cs(maapi.constSets[j]);
		for(size_t k=0; k<cs.constants.size(); k++) {
			const Constant& c(cs.constants[k]);
			//printf("%s = %s;\n", c.name.c_str(), c.value.c_str());

			std::string type = "int";
			if(c.type == "MAString")
				type = "String";
			maapiFile << "\tpublic const " << type << " " << cs.name << c.name.c_str() << " = " << c.value.c_str();
			maapiFile << ";\n";
		}
	}
	maapiFile << "}\n\n";

	// generate struct wrappers (todo)

	// generate syscall delegate declarations
	maapiFile << "public class Syscalls {\n";
	for(size_t j=0; j<maapi.functions.size(); j++) {
		const Function& f(maapi.functions[j]);

		//maapiFile << "\t\tcase " << f.number << ":\n";
		std::string returnType = getCSharpType(maapi, f.returnType, true);
		maapiFile << "\tpublic delegate " << returnType << " Delegate_" << f.name << "(";
		for(size_t k=0; k<f.args.size(); k++) {
			const Argument& a(f.args[k]);
			if(k != 0)
				maapiFile << ", ";

			std::string argType = getCSharpType(maapi, a.type, a.in);
			maapiFile << argType << " _" << a.name;
		}
		maapiFile << ");\n";
		maapiFile << "\tpublic Delegate_" << f.name << " " << f.name << " = null;\n";
	}
	maapiFile << "}\n\n";

	// generate syscall invoker
	maapiFile << "public class SyscallInvoker {\n\n";
	maapiFile << "\tprivate CoreInterpreted mCore;\n";
	maapiFile << "\tprivate Syscalls mSyscalls;\n\n";
	maapiFile << "\tpublic SyscallInvoker(CoreInterpreted core, Syscalls syscalls) {\n";
	maapiFile << "\t\tmCore = core;\n";
	maapiFile << "\t\tmSyscalls = syscalls;\n";
	maapiFile << "\t}\n\n";

	maapiFile << "\tpublic void InvokeSyscall(int id) {\n";
	maapiFile << "\t\tswitch(id) {\n";
	for(size_t j=0; j<maapi.functions.size(); j++) {
		const Function& f(maapi.functions[j]);

		maapiFile << "\t\t\tcase " << f.number << ":\n";

		std::string returnType = getCSharpType(maapi, f.returnType, true);
		maapiFile << "\t\t\t";
		if(returnType != "noreturn" && returnType != "void")
			maapiFile << "mCore.SetReturnValue(";

		maapiFile << "mSyscalls." << f.name << "(";

		int i = 0;
		for(size_t k=0; k<f.args.size(); k++) {
			const Argument& a(f.args[k]);
			if(k != 0)
				maapiFile << ", ";
			outputCSharpSyscallArgTyped(maapiFile, i, a, maapi);
		}
		maapiFile << ")";

		if(returnType != "noreturn" && returnType != "void")
			maapiFile << ")";
		maapiFile << ";\n";

		// SYSCALL_LOG
		if(f.name != "maIOCtl") {
		maapiFile << "#if SYSCALL_LOG\n";
		maapiFile << "\t\t\tUtil.Log(\""<<f.name<<"(\"+\n";
		i = 0;
		for(size_t k=0; k<f.args.size(); k++) {
			const Argument& a(f.args[k]);
			maapiFile << "\t\t\t\t";
			if(k != 0)
				maapiFile << "\",\"+";
			outputCSharpSyscallArgTyped(maapiFile, i, a, maapi);
			maapiFile << "+\n";
		}
		maapiFile << "\t\t\t\t\")";
		if(returnType != "noreturn" && returnType != "void")
			maapiFile << ": \"+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+\"";
		maapiFile << "\\n\");\n";
		maapiFile << "#endif\n";
		}
		// END LOG

		maapiFile << "\t\t\tbreak;\n";
	}
	maapiFile << "\t\t}\n";
	maapiFile << "\t}\n";
	maapiFile << "}\n";
	// end SyscallInvoker

	// generate CoreNativeBase
	// CoreNative should implement this.
	maapiFile << "public class CoreNativeSyscallInvoker\n";
	maapiFile << "{\n";
	maapiFile << "\tprotected CoreNative mCore;\n";
	maapiFile << "\tprotected Syscalls mSyscalls;\n";
	maapiFile << "\tpublic CoreNativeSyscallInvoker(CoreNative core, Syscalls syscalls)\n";
	maapiFile << "\t{\n";
	maapiFile << "\t\tmCore = core;\n";
	maapiFile << "\t\tmSyscalls = syscalls;\n";
	maapiFile << "\t}\n\n";

	for(size_t j=0; j<maapi.functions.size(); j++) {
		const Function& f(maapi.functions[j]);
		std::string returnType = getCSharpType(maapi, f.returnType, true);

		maapiFile << "\tpublic ";

		if(returnType != "noreturn" && returnType != "void") {
			maapiFile << "int ";
		} else {
			maapiFile << "void ";
		}

		maapiFile << f.name << "(";

		int i = 0;
		for(size_t k=0; k<f.args.size(); k++) {
			const Argument& a(f.args[k]);
			std::string argType = getCSharpType(maapi, a.type, a.in);
			if(k!=0)
				maapiFile << ", ";
			if(argType == "double")
			{
				i+=2;
				if(i > 4)
					break;
				maapiFile << "int " << "i" << (i-2) << ", ";
				maapiFile << "int " << "i" << (i-1);
			}
			else
			{
				i+=1;
				if(i > 4)
					break;
				maapiFile << "int " << "i" << (i-1);
			}
		};

		maapiFile << ")\n";
		maapiFile << "\t{\n";

		maapiFile << "\t\t";

		if(returnType != "noreturn" && returnType != "void") {
			maapiFile << "return mCore.SetReturnValue(";
		}

		maapiFile << "mSyscalls." << f.name << "(";

		i = 0;
		for(size_t k=0; k<f.args.size(); k++) {
			const Argument& a(f.args[k]);
			std::string argType = getCSharpType(maapi, a.type, a.in);
			if(k != 0)
				maapiFile << ", ";
			if(argType == "double") {
				maapiFile << "MoSync.Util.ConvertToDouble(";
				outputCSharpSyscallNativeArg(maapiFile, i);
				maapiFile << ", ";
				i++;
				outputCSharpSyscallNativeArg(maapiFile, i);
				i++;
				maapiFile << ")";
			}
			else if(argType == "float") {
				maapiFile << "MoSync.Util.ConvertToFloat(";
				outputCSharpSyscallNativeArg(maapiFile, i);
				i++;
				maapiFile << ")";
			} else {
				outputCSharpSyscallNativeArg(maapiFile, i);
				i++;
			}
		}

		maapiFile << ")";
		if(returnType != "noreturn" && returnType != "void") {
			maapiFile << ")";
		}
		maapiFile << ";\n";

		maapiFile << "\t}\n";
	}
	maapiFile << "}\n";
	// end NativeCoreSyscallInvoker

	// generate ioctl delegate declarations
	const vector<Ioctl>& ioctls = maapi.ioctls;

	maapiFile << "public class Ioctls {\n";
	for(size_t i=0; i<ioctls.size(); i++) {
		const Ioctl& ioctl(ioctls[i]);
		for(size_t j=0; j<ioctl.functions.size(); j++) {
			const IoctlFunction& ifunc(ioctl.functions[j]);
			const Function& f(ifunc.f);

			std::string returnType = "long";//getCSharpType(maapi, f.returnType);
			maapiFile << "\tpublic delegate " << returnType << " Delegate_" << f.name << "(";
			for(size_t k=0; k<f.args.size(); k++) {
				const Argument& a(f.args[k]);
				if(k != 0)
					maapiFile << ", ";

				std::string argType = getCSharpType(maapi, a.type, a.in);
				maapiFile << argType << " _" << a.name;
			}
			maapiFile << ");\n";
			maapiFile << "\tpublic Delegate_" << f.name << " " << f.name << " = null;\n";
		}
	}
	maapiFile << "}\n\n";

	// generate ioctl invoker

	maapiFile << "public class IoctlInvoker {\n\n";
	maapiFile << "\tprivate Core mCore;\n";
	maapiFile << "\tprivate Ioctls mIoctls;\n\n";
	maapiFile << "\tpublic IoctlInvoker(Core core, Ioctls ioctls) {\n";
	maapiFile << "\t\tmCore = core;\n";
	maapiFile << "\t\tmIoctls = ioctls;\n";
	maapiFile << "\t}\n\n";

	maapiFile << "\tpublic long InvokeIoctl(int id, int a, int b, int c) {\n";
	maapiFile << "\t\tlong result;\n";
	maapiFile << "\t\tswitch(id) {\n";
	for(size_t i=0; i<ioctls.size(); i++) {
		const Ioctl& ioctl(ioctls[i]);
		for(size_t j=0; j<ioctl.functions.size(); j++) {
			const IoctlFunction& ifunc(ioctl.functions[j]);
			const Function& f(ifunc.f);

			maapiFile << "\t\t\tcase " << f.number << ":\n";

			maapiFile << "\t\t\tif(mIoctls." << f.name << " == null)\n";
			maapiFile << "\t\t\t\tresult = MoSync.Constants.IOCTL_UNAVAILABLE;\n";
			maapiFile << "\t\t\telse\n";
			maapiFile << "\t\t\tresult = mIoctls." << f.name << "(";

			int argindex = 0;
			for(size_t k=0; k<f.args.size(); k++) {
				const Argument& a(f.args[k]);
				std::string argType = getCSharpType(maapi, a.type, a.in);
				if(k != 0)
					maapiFile << ", ";
				outputCSharpIoctlArgTyped(maapiFile, argindex, a, maapi);
			}
			maapiFile << ");\n";

			// SYSCALL_LOG
			if(f.name != "maWriteLog") {
			maapiFile << "#if SYSCALL_LOG\n";
			maapiFile << "\t\t\tUtil.Log(\""<<f.name<<"(\"+\n";
			argindex = 0;
			for(size_t k=0; k<f.args.size(); k++) {
				const Argument& a(f.args[k]);
				maapiFile << "\t\t\t\t";
				if(k != 0)
					maapiFile << "\",\"+";
				outputCSharpIoctlArgTyped(maapiFile, argindex, a, maapi);
				maapiFile << "+\n";
			}
			maapiFile << "\t\t\t\t\"): \"+result+\"\\n\");\n";
			maapiFile << "#endif\n";
			}
			// END LOG

			maapiFile << "\t\treturn result;\n";
		}
	}
	maapiFile << "\t\t}\n";
	maapiFile << "\t\treturn MoSync.Constants.IOCTL_UNAVAILABLE;\n";
	maapiFile << "\t}\n";
	maapiFile << "}\n\n";

	// output struct info as offsets etc.
	for(size_t i = 0; i < maapi.structs.size(); i++)
	{
		const Struct& s(maapi.structs[i]);
		for(size_t j = 0; j < s.members.size(); j++)
		{
			const Member& m(s.members[j]);
			if(m.pod.size() == 1) // POD
			{

			}
			else // Anonymous union
			{
			}
		}
	};

	maapiFile << "} // namespace MoSync\n";
}

/**
 * Generate files used when building the runtimes.
 */
static void outputRuntimeBuilderFiles(const Interface& maapi) {
	outputCpp(maapi);
	outputAsmConfigLst(maapi);
	outputAsmConfigH(maapi);
	outputDllDefine(maapi);
	outputInvokeSyscallCpp(maapi);
	outputInvokeSyscallArmRecompiler(maapi);
	outputInvokeSyscallJava(maapi);
//	outputInvokeSyscallJavascript(maapi);
	outputSyscallStaticJava(maapi);
	outputSyscallStaticCpp(maapi);
	outputConstSets(maapi);
}

static void outputConsts(const string& filename, const Interface& inf, int ix) {
	ofstream stream(filename.c_str());
	string def = toDef(getFilenameFromPath(filename));
	stream << "#ifndef " << def << "\n";
	stream << "#define " << def << "\n\n";

	if(ix == MAIN_INTERFACE) {
		streamHash(stream, inf);
	}

	streamConstants(stream, inf.constSets, ix);
	streamIoctlDefines(stream, inf, def, ix, true);

	stream << "#endif\t//" << def << "\n";
}

static void outputCpp(const Interface& maapi) {
	ofstream stream("Output/cpp_maapi.h");

	stream << "#ifndef CPP_MAAPI_H\n";
	stream << "#define CPP_MAAPI_H\n\n";

	stream << "#if defined(__cplusplus) && !defined(__SYMBIAN32__)\n"
		"extern \"C\" {\n"
		"#endif\n\n";

	stream << "#include \"maapi_defs.h\"\n\n";

	streamHeaderFunctions(stream, maapi, true);

	stream << "#if defined(__cplusplus) && !defined(__SYMBIAN32__)\n"
		"}\n"
		"#endif\n\n";

	stream << "#endif\t//CPP_MAAPI_H\n";
}

static void outputAsmConfigLst(const Interface& maapi) {
	ofstream stream("Output/asm_config.lst");

	for(size_t i=0; i<maapi.functions.size(); i++) {
		const Function& f(maapi.functions[i]);
		string type;
		if(f.returnType == "void" || f.returnType == "double")
			type = f.returnType;
		else if(f.returnType == "noreturn")
			type = "void";
		else
			type = "int";
		int nargs = 0;
		for(size_t j=0; j<f.args.size(); j++) {
			const Argument& a(f.args[j]);
			if(a.type == "double")
				nargs += 2;
			else
				nargs += 1;
		}
		stream << ".syscall _" << f.name << " " << f.number << ", " << nargs << ", " <<
			type << "\n";
	}
}

static void outputAsmConfigH(const Interface& maapi) {
	ofstream stream("Output/asm_config.h");
//#define SYSCALLS(macro, arg1, arg, argD)
//	macro(0, MAAddress, _memset, arg1(dst) arg(val) arg(size), argD(out, MAAddress, dst)argD(in, int, val)argD(in, int, size))
	stream << "#define SYSCALLS(m, arg1, arg, argD)\\\n";

	for(size_t i=0; i<maapi.functions.size(); i++) {
		const Function& f(maapi.functions[i]);
		stream << "\tm(" << f.number << ", " << f.returnType << ", " << f.name << ", ";
		for(size_t j=0; j<f.args.size(); j++) {
			const Argument& a(f.args[j]);
			stream << (j == 0 ? "arg1" : "arg") << "(" << a.name << ")";
		}
		stream << ",";
		for(size_t j=0; j<f.args.size(); j++) {
			const Argument& a(f.args[j]);
			stream << " argD(" << (a.in ? "in" : "out") << ", " << a.type << ", " << a.name << ")";
		}
		stream << ")\\\n";
	}
	stream << "\n";
}

static void outputDllDefine(const Interface& maapi) {
	ofstream stream("Output/mosynclib.def");

	stream << "EXPORTS\n";
	for(size_t i=0; i<maapi.functions.size(); i++) {
		const Function& f(maapi.functions[i]);
		if(f.name.substr(0, 2) == "ma")
			stream << "\t" << f.name << " @" << f.number << "\n";
	}
	stream << "\tmosyncLibMain @" << maapi.functions.size() + 1 << "\n";
}

static void outputArmRecompilerParameterPassing(int amountOfRegistersForArguments, int startRegister,
	ostream &stream, const Interface& maapi, const Function& f, bool stackPass)
{
	int ireg;
	int totalSize = 0;
	for(size_t j=0; j<f.args.size(); j++) {
		const Argument& a(f.args[j]);
		int argSize = 1;
		string argType = cType(maapi, a.type);
		if(argType == "double" || argType == "long")
			argSize = 2;
		totalSize+=argSize;
	}

	if(stackPass)
		ireg = totalSize;
	else
		ireg = 0;

	int areg = startRegister;
	int argIndex = 0;
	for(int j=f.args.size()-1; j>=0; j--) {
		const Argument& a(f.args[argIndex++]);
		int argSize = 1;
		string argType = cType(maapi, a.type);
		if(argType == "double" || argType == "long") {
			argSize = 2;
		}

		int curIndex;
		if(stackPass)
			curIndex = ireg-argSize;
		else
			curIndex = ireg+argSize;

		int stackSize = 0;
		int regSize = argSize;

		if(stackPass) {
			stackSize = argSize;
			if(curIndex<amountOfRegistersForArguments)
			{
				stackSize-=amountOfRegistersForArguments-curIndex;
				curIndex = amountOfRegistersForArguments;
			}

			for(int i = 0; i < stackSize; i++) {
				stream << "\tassm.SUB_imm8(AA::SP, AA::SP, 4);\n";
				char num[16];
				//_itoa((curIndex+i), num, 10);
				sprintf(num, "%i", (curIndex+i));
				string reg = string("loadRegister(REG_i") + num + ", AA::R0)";
				if(a.in == false || isPointerType(maapi, a.type)) {
					stream << "\tassm.ADD(AA::R0, " << reg << ", MEMORY_ADDR);\n";
					reg = "AA::R0";
				}

				stream << "\tassm.STR(" << reg << ", 0, AA::SP);\n";
			}
		} else {
			regSize = argSize;
			if(curIndex>amountOfRegistersForArguments)
			{
				regSize-=curIndex-amountOfRegistersForArguments;
			}

			for(int i = 0; i < regSize; i++) {

				if(a.in == false || isPointerType(maapi, a.type)) {
					stream << "\tassm.ADD(AA::R" << (areg+i) <<", loadRegister(REG_i" << (ireg+i) << ", AA::R" << (areg+i) << ", false) , MEMORY_ADDR);\n";
				} else {
					stream << "\tloadRegister(REG_i" << (ireg+i) << ", AA::R" << (areg+i) << ", true);\n";
				}
			}
		}


		if(stackPass)
			ireg -= argSize;
		else
			ireg += argSize;


		areg += argSize;
	}
}

static void outputInvokeSyscallArmRecompiler(const Interface& maapi) {
	ofstream stream("Output/invoke_syscall_arm_recompiler.h");

	for(size_t i=0; i<maapi.functions.size(); i++) {
		const Function& f(maapi.functions[i]);
		stream << "case " << f.number << ":\n" << "{\n";

/*
		outputArmRecompilerParameterPassing(3, 1, stream, maapi, f, true);
		outputArmRecompilerParameterPassing(3, 1, stream, maapi, f, false);
		stream << "\temitFuncCall((int)&((void (*)())" << f.name << "), (int)gSyscall);\n";
*/
		//stream << "\tGEN_BREAKPOINT;\n";



		stream << "#ifdef SYMBIAN\n";
		outputArmRecompilerParameterPassing(3, 1, stream, maapi, f, true);
		outputArmRecompilerParameterPassing(3, 1, stream, maapi, f, false);
		stream << "\temitFuncCall(FUNC_CAST(Syscall::" << f.name << "), (int)gSyscall);\n";
		stream << "#else\n";
		outputArmRecompilerParameterPassing(4, 0, stream, maapi, f, true);
		outputArmRecompilerParameterPassing(4, 0, stream, maapi, f, false);
		stream << "\temitFuncCall((int)&((void (*)())" << f.name << "), (int)gSyscall);\n";
		stream << "#endif\n";

		if(f.returnType != "void" && f.returnType != "noreturn") {
			stream << "\tsaveRegister(REG_r14, AA::R0);\n";
			if(f.returnType == "double" || f.returnType == "long") {
				stream << "\tsaveRegister(REG_r15, AA::R1);\n";
			}
		}

		stream << "\tbreak;\n"
			"}\n";
	}
}

static void outputInvokeSyscallCpp(const Interface& maapi) {
	ofstream stream("Output/invoke_syscall_cpp.h");
	streamInvokeSyscall(stream, maapi, false);
}

void streamInvokePrefix(ostream& stream, const Function& f) {
	stream << "case " << f.number << ":\n";
}

static void outputInvokeSyscallJava(const Interface& maapi) {
	ofstream stream("Output/invoke_syscall_java.h");
	streamInvokeSyscall(stream, maapi, true);
}

static void outputSyscallStaticJava(const Interface& maapi) {
	ofstream stream("Output/syscall_static_java.h");
	for(size_t i=0; i<maapi.functions.size(); i++) {
		const Function& f(maapi.functions[i]);
		bool staticConversionNeeded = false;
		bool arguments = false;
		stringstream staticJavaStream;
		stringstream staticJavaInvokeStream;

		staticJavaStream << getSJType(maapi, f.returnType, true)<<" "<<f.name<<" (";

		if(f.returnType == "double")
			staticConversionNeeded = true;

		for(size_t j=0; j<f.args.size(); j++) {
			const Argument& a(f.args[j]);
			if(a.type == "MAString" || a.type == "MAWString" || a.type == "double")
				staticConversionNeeded = arguments = true;
			if(a.type == "NCString")
				staticConversionNeeded = false;

			if(j > 0) {
				staticJavaStream << ", ";
				staticJavaInvokeStream << ", ";
			}

			string sjType = getSJType(maapi, a.type, a.in);
			string jPracticalArgType = (a.in ? a.type : "MAAddress");

			staticJavaStream << sjType << " " << a.name;
			if(a.type == "double") {
				staticJavaStream << "1, "<<sjType<<" "<<a.name<<"2";
				staticJavaInvokeStream << "_SYSCALL_CONVERT_long("<<a.name<<"1, "<<a.name<<"2)";
			} else {
				if(jPracticalArgType == "MAString" || jPracticalArgType == "MAWString") {
					staticJavaInvokeStream << "_SYSCALL_CONVERT_"<<jPracticalArgType<<
						"("<<a.name<<")";
				} else {
					staticJavaInvokeStream << a.name;
				}
			}
		}
		staticJavaStream << ") throws Exception {";

		if(staticConversionNeeded) {
			stream << staticJavaStream.str()<<"\n	";
			const char* arg = arguments ? "" : "_";
			if(f.returnType == "double") {
				stream <<
					"long res = "<<arg<<f.name<<"("<<staticJavaInvokeStream.str()<<");\n"
					"	mCore.mStaticCode.__dbl_high = (int)(res >> 32);\n"
					"	return (int)res;\n"
					"}\n";
			} else {
				if(f.returnType != "void" && f.returnType != "noreturn") {
					stream << "return ";
				}
				stream << f.name<<"("<<staticJavaInvokeStream.str()<<");\n"
					"}\n";
			}
		}
	}
}

static void outputSyscallStaticCpp(const Interface& maapi) {
	ofstream stream("Output/syscall_static_cpp.h");
	stream << "#include <helpers/maapi_defs.h>\n";
	stream << "#include \"converters.h\"\n\n";

	for(size_t i=0; i<maapi.functions.size(); i++) {
		const Function& f(maapi.functions[i]);
		stringstream staticCppStream;
		stringstream staticCppInvokeStream;
		stringstream staticCppInitStream;


		staticCppStream << "inline " << getSJType(maapi, f.returnType, true)<<" SYSCALL("<<f.name<<") (";

		for(size_t j=0; j<f.args.size(); j++) {
			const Argument& a(f.args[j]);

			if(j > 0) {
				staticCppStream << ", ";
				staticCppInvokeStream << ", ";
			}

			string sjType = getSJType(maapi, a.type, a.in);
			string ctype = cType(maapi, a.type);

			staticCppStream << "int" << " " << a.name;
			if(a.type == "double") {
				staticCppStream << "1, "<<sjType<<" "<<a.name<<"2";

				staticCppInvokeStream << "convertDoubleArg(" << a.name << "1, " << a.name << "2" << ")";//a.name << "_dv" << ".d";


			} else if(isPointerType(maapi, a.type )) {
				staticCppInvokeStream << "convertPointerArg<"<<ctype<<">("<<a.name<<")";
			} else {
				staticCppInvokeStream << "convertSingleArg<"<<ctype<<">("<<a.name<<")";
			}
		}
		staticCppStream << ") {";

		stream << staticCppStream.str() <<"\n";

		stream << staticCppInitStream.str();

		if(f.returnType != "void" && f.returnType != "noreturn") {
			stream << "	int ret;\n";
			string returnType = cType(maapi, f.returnType);
			if(isPointerType(maapi, f.returnType)) {
				returnType = "void*";
			}
			stream << "	convertRet<" << returnType << ">(ret, __dbl_high, (" << returnType << ") SYSCALL_IMPL(" << f.name<<")("<<staticCppInvokeStream.str()<<"));\n";
			stream << "	return ret;\n";
			stream << "}\n";
		}
		else stream << "	SYSCALL_IMPL(" << f.name<<")("<<staticCppInvokeStream.str()<<");\n}\n";
	}
}

static void outputCoreConsts() {
#define DO(id) { file << "#define "<<#id<<" "<<i<<"\n"; i++; }
	int i=0;
	ofstream file("Output/core_consts.h");
	INSTRUCTIONS(DO);
	file <<"\n";
	i = 0;
	REGISTERS(DO);
	file <<"\n";
}

static void outputConstSets(const Interface& maapi) {
	ofstream stream("Output/constSets.h");
	stream <<
		"#ifndef CONSTSETS_H\n"
		"#define CONSTSETS_H\n"
		"\n";
	for(size_t i=0; i<maapi.constSets.size(); i++) {
		const ConstSet& cs(maapi.constSets[i]);
		stream <<
			"#define CONSTSET_"<<cs.name<<"(f, m) \\\n";
		bool first = true;
		for(size_t j=0; j<cs.constants.size(); j++) {
			const Constant& c(cs.constants[j]);
			stream << "\t";
			if(first) {
				stream << "f";
				first = false;
			} else {
				stream << "m";
			}
			stream << "("<<c.name<<") \\\n";
		}
		stream << "\n";
	}
	stream <<
		"#endif	//CONSTSETS_H\n"
		"\n";
}
