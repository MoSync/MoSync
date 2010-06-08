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

#include <idl-common/idl-common.h>
#include <idl-common/tokenizer.h>
#include <idl-common/stringFunctions.h>
#include <idl-common/types.h>

#include "helpers/mkdir.h"

#include "core.h"

#define MIN(x, y) ((x)<(y)?(x):(y))
#define MAX(x, y) ((x)>(y)?(x):(y))

using namespace std;

static vector<string> readExtensions(const char* filename);
static void outputMaapi(const vector<string>& ixs, const Interface& maapi);
static void outputCpp(const Interface& maapi);
static void outputAsmConfigLst(const Interface& maapi);
static void outputAsmConfigH(const Interface& maapi);
static void outputDllDefine(const Interface& maapi);
static void outputInvokeSyscallCpp(const Interface& maapi);
static void outputInvokeSyscallArmRecompiler(const Interface& maapi);
static void outputInvokeSyscallJava(const Interface& maapi);
static void outputSyscallStaticJava(const Interface& maapi);
static void outputSyscallStaticCpp(const Interface& maapi);
static void streamInvokeSyscall(ostream& stream, const Interface& maapi, bool java);
static void outputCoreConsts();
static void outputConsts(const string& filename, const Interface& inf, int ix);
static void outputConstSets(const Interface& maapi);

static bool filesAreEqual(const char* a, const char* b) {
	ifstream as(a, ios_base::in | ios_base::binary);
	ifstream bs(b, ios_base::in | ios_base::binary);

	// compare size
	as.seekg(0, ios_base::end);
	size_t aSize = as.tellg();
	bs.seekg(0, ios_base::end);
	size_t bSize = bs.tellg();
	if(!(as.good() && bs.good() && (aSize == bSize)))
		return false;

	// compare content
	as.seekg(0);
	bs.seekg(0);
	static const size_t BUFSIZE = 64*1024;	//arbitrary
	char aBuf[BUFSIZE], bBuf[BUFSIZE];
	size_t pos = 0;
	while(pos < aSize) {
		size_t len = MIN(BUFSIZE, aSize - pos);
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

// change slashes
static void u2d(string& path) {
	for(size_t i=0; i<path.size(); i++) {
		if(path[i] == '/')
			path[i] = '\\';
	}
}

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

int main() {
	try {
		_mkdir("Output");
		vector<string> ixs = readExtensions("extensions.h");
		Interface maapi = parseInterface(ixs, "maapi.idl");
		outputMaapi(ixs, maapi);
		outputCoreConsts();

		_mkdir((MOSYNCDIR + "/include").c_str());

		// create the new generated folder for java files
		_mkdir("..\\..\\runtimes\\java\\shared\\generated");

		copy("Output/maapi.h", "../../libs/MAStd/");
		copy("Output/maapi.h", "../../libs/newlib/libc/sys/mosync/");
		copy("Output/asm_config.lst", "" + MOSYNCDIR + "/bin/");

		copy("Output/invoke_syscall_java.h", "../../runtimes/java/Shared/generated/");
		copy("Output/syscall_static_java.h", "../../runtimes/java/Shared/generated/");
		copy("Output/core_consts.h", "../../runtimes/java/Shared/generated/");
		copy("Output/MAAPI_consts.h", "../../runtimes/java/Shared/generated/");

		copy("Output/cpp_defs.h", "../../intlibs/helpers/");
		copy("Output/cpp_maapi.h", "../../intlibs/helpers/");

		copy("Output/invoke_syscall_cpp.h", "../../runtimes/cpp/core/");
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
		}

		return 0;
	} catch(exception& e) {
		printf("Exception: %s\n", e.what());
		return 1;
	}
}

static void outputMaapi(const vector<string>& ixs, const Interface& maapi) {
	{ ofstream file("Output/cpp_defs.h"); streamCppDefsFile(file, maapi, ixs, -1); }
	outputConsts("Output/MAAPI_consts.h", maapi, -1);
	{ ofstream file("Output/maapi.h"); streamHeaderFile(file, maapi, ixs, -1); }

	for(size_t i=0; i<ixs.size(); i++) {
		{
			ofstream file(("Output/CPP_" + toupper(ixs[i]) + ".h").c_str());
			streamCppDefsFile(file, maapi, ixs, i);
		}
		outputConsts("Output/" + toupper(ixs[i]) + "_CONSTS.h", maapi, i);
		{
			ofstream file(("Output/" + toupper(ixs[i]) + ".h").c_str());
			streamHeaderFile(file, maapi, ixs, i);
		}
	}

	outputCpp(maapi);
	outputAsmConfigLst(maapi);
	outputAsmConfigH(maapi);
	outputDllDefine(maapi);
	outputInvokeSyscallCpp(maapi);
	outputInvokeSyscallArmRecompiler(maapi);
	outputInvokeSyscallJava(maapi);
	outputSyscallStaticJava(maapi);
	outputSyscallStaticCpp(maapi);
	outputConstSets(maapi);
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

static void outputConsts(const string& filename, const Interface& inf, int ix) {
	ofstream stream(filename.c_str());
	string def = toDef(getFilenameFromPath(filename));
	stream << "#ifndef " << def << "\n";
	stream << "#define " << def << "\n\n";

	if(ix == MAIN_INTERFACE) {
		streamHash(stream, inf);
	}

	streamConstants(stream, inf.constSets, ix);
	streamIoctlDefines(stream, inf.ioctls, ix);

	stream << "#endif\t//" << def << "\n";
}

//streams C function declarations.
void streamHeaderFunctions(ostream& stream, const Interface& inf, bool syscall) {
	for(size_t i=0; i<inf.functions.size(); i++) {
		const Function& f(inf.functions[i]);
		stream << f.comment;

		if(syscall)
			stream << "SYSCALL(";
		stream << cType(inf, f.returnType);
		if(syscall)
			stream << ", ";
		if(f.returnType == "noreturn")
			stream << " ATTRIBUTE(noreturn,";
		stream << " " << f.name << "(";
		if(f.args.size() == 0) {
			stream << "void";
		}
		for(size_t j=0; j<f.args.size(); j++) {
			const Argument& a(f.args[j]);
			if(j != 0)
				stream << ", ";
			if(a.in && isPointerType(inf, a.type)) {
				stream << "const ";
			}
			stream << cType(inf, a.type);
			if(!isPointerType(inf, a.type) && !a.in)
				stream << "*";
			stream << " " << a.name;
		}
		if(f.returnType == "noreturn")
			stream << ")";
		if(syscall)
			stream << ")";
		stream << ");\n\n";
	}
	stream << "\n";
}


static void outputCpp(const Interface& maapi) {
	ofstream stream("Output/cpp_maapi.h");

	stream << "#ifndef CPP_MAAPI_H\n";
	stream << "#define CPP_MAAPI_H\n\n";

	stream << "#if defined(__cplusplus) && !defined(__SYMBIAN32__)\n"
		"extern \"C\" {\n"
		"#endif\n\n";

	stream << "#if defined(__GNUC__) || defined(__SYMBIAN32__)\n"
		"#define ATTRIBUTE(a, func)  func __attribute__ ((a))\n"
		"#elif defined(_MSC_VER)\n"
		"#define ATTRIBUTE(a, func)  __declspec (a) func\n"
		"#else\n"
		"#error Unsupported compiler!\n"
		"#endif\n\n";

	streamMoSyncDllDefines(stream);

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

static void streamInvokeSyscall(ostream& stream, const Interface& maapi, bool java) {
	for(size_t i=0; i<maapi.functions.size(); i++) {
		const Function& f(maapi.functions[i]);
		stream << "case " << f.number << ":\n"
		"{\n"
		"\tLOGSC(\"\\t" << f.name << "(\");\n";
		int ireg = 0;
		for(size_t j=0; j<f.args.size(); j++) {
			const Argument& a(f.args[j]);
			if(j != 0)
				stream << "\tLOGSC(\", \");\n";
			string argType, convType;
			if(java) {
				if((a.type == "MAString" || a.type == "MAWString") && !a.in)
					argType = "MAAddress";
				else
					argType = jType(maapi, a.type);
				convType = argType;
			} else {
				argType = cType(maapi, a.type);
				convType = a.type;
			}
			stream << "\t" << argType << " " << a.name << " = _SYSCALL_CONVERT_" << convType;
			if((argType == "double" || argType == "long") && java) {
				stream << "(REG_i" << ireg << ");\n";
			} else {
				stream << "(REG(REG_i" << ireg << "));\n";
			}
			if(argType == "double" || argType == "long")
				ireg += 2;
			else
				ireg += 1;
		}
		stream << "\t";
		if(f.returnType != "void" && f.returnType != "noreturn") {
			string retType;
			if(java) {
				if(f.returnType == "MAString" || f.returnType == "MAWString")
					retType = "Address";
				else
					retType = jType(maapi, f.returnType);
			} else {
				retType = cType(maapi, f.returnType);
			}
			stream << retType << " res = ";
		}
		stream << "CALL_SYSCALL(" << f.name << ")(";
		for(size_t j=0; j<f.args.size(); j++) {
			const Argument& a(f.args[j]);
			if(j != 0)
				stream << ", ";
			stream << a.name;
		}
		stream << ");\n";
		if(f.returnType != "void" && f.returnType != "noreturn") {
			stream << "\t_SYSCALL_HANDLERES_" << f.returnType << "\n";
		} else {
			stream << "\t_SYSCALL_HANDLERES_void\n";
		}
		stream << "\treturn;\n"
			"}\n";
	}
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
