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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <direct.h>
#include <fstream>
#include <iomanip>

#include <filelist/filelist.h>

#include <idl-common/idl-common.h>
#include <idl-common/stringFunctions.h>
#include <idl-common/types.h>

using namespace std;

//******************************************************************************
// Declarations
//******************************************************************************

#define INCDIR (MOSYNCDIR + "/include/inf")

static int clear();
static int buildext(int argc, const char** argv);
static void outputExtensions(const vector<Interface>& infs);
void setupJavaRuntimes(const vector<Interface>& infs);

static string sJdkDir;
static string sJ2meDir;

//******************************************************************************
// Main
//******************************************************************************
int main(int argc, const char** argv) {
	if(argc <= 3) {
		printf(
			"Usage: %s <jdk-dir> <j2me-sdk-dir> <-clear | directories>\n"
			"\n"
			"First pass a directory containing a standard Java Development Kit."
			"Then pass a directory containing a J2ME Development Kit, with preverifier and libraries."
			"Then pass either a list of directories containing extensions,\n"
			"or the -clear flag.\n", argv[0]);
		return 1;
	}
	try {
		sJdkDir = argv[1];
		sJ2meDir = argv[2];

		if(argc == 4)
			if(strcmp(argv[3], "-clear") == 0)
				return clear();

		return buildext(argc - 3, &argv[3]);
	} catch(exception& e) {
		printf("Exception: %s\n", e.what());
		return 1;
	}
}

static int clear() {
	throw exception("clear not implemented!\n");
}

static string sPath;
static string sDir;
static void scanDirCallback(const char* filename) {
	printf("Found %s\n", filename);
	if(sPath.length() != 0)
		throwException("More than one IDL file!");
	sPath = sDir + '/' + string(filename);
}

static int buildext(int argc, const char** argv) {
	vector<Interface> infs;
	vector<string> dummy;
	for(int i=0; i<argc; i++) {
		sPath.clear();
		sDir = argv[i];
		printf("Scanning %s ...\n", sDir.c_str());
		int res = scanDirectory((sDir + "/*.idl").c_str(), scanDirCallback);
		if(res)
			throwException("scanDirectory \"" + sDir + "\" failed");
		if(sPath.length() == 0)
			throwException("Couldn't find IDL file in \"" + sDir + "\"");

		Interface inf = parseInterface(dummy, sPath);
		inf.path = argv[i];
		infs.push_back(inf);
	}
	outputExtensions(infs);
	setupJavaRuntimes(infs);
	return 0;
}

//******************************************************************************
// Helpers
//******************************************************************************

const string& getJdkDir() {
	return sJdkDir;
}

const string& getJ2meDir() {
	return sJ2meDir;
}

static void deleteCallback(const char* filename) {
	remove((string("Output/") + filename).c_str());
}

static void clearOutput() {
	int res = scanDirectory("Output/*", deleteCallback);
	if(res)
		throwException("scanDirectory");
}

//******************************************************************************
// Output
//******************************************************************************

static void outputJavaInvoke(const vector<Interface>& infs) {
	ofstream file("Output/ExtensionHandler.java");
	string declarators, initializers, destructors, invokers;

	//collect parts in separate strings
	for(size_t i=0; i<infs.size(); i++) {
		const Interface& inf(infs[i]);

		//ExtA extA;
		string className = firstUpper(inf.name) + "Extension";
		string varName = firstLower(inf.name);
		declarators += className + " " + varName + ";\n";

		//	extA = new ExtA(mSyscall);
		initializers += "	" + varName + " = new " + className + "(m);\n";

		//	extA.destruct();
		destructors += "	" + varName + ".destruct();\n";

		for(size_t j=0; j<inf.functions.size(); j++) {
			const Function& f(inf.functions[j]);
			//case 1: extA.foo(); return 0;
			//case 2: return extA.bar(a);
			//case 3: writeDouble(a, extB.fnorp(readDouble(a))); return 0;

			string args;
			int nextReg = 0;
			string regs[] = { "a", "b", "c" };
			string doubleReturnArg;

			for(size_t k=0; k<f.args.size(); k++) {
				const Argument& a(f.args[k]);
				if(nextReg > 2)
					throwException("too many arguments in function " + f.name);
				if(k != 0)
					args += ", ";
				if(a.type == "double" && a.in) {
					if(doubleReturnArg.size() == 0) {
						doubleReturnArg = regs[nextReg];
					}
					args += "mMoSync.readDouble(" + regs[nextReg++] + ")";
				} else {	//int or pointer or otherwise; it all gets passed unchanged
					args += regs[nextReg++];
				}
			}
			if(f.returnType == "double" && doubleReturnArg.size() == 0) {
				if(nextReg > 2)
					throwException("too many arguments in function " + f.name);
				doubleReturnArg = regs[nextReg++];
			}

			invokers += "case " + toString(f.number) + ": ";
			string call = varName + "." + f.name + "(" + args + ")";
			if(f.returnType == "void") {
				invokers += call + "; return 0;\n";
			} else if(f.returnType == "double") {
				invokers += "mMoSync.writeDouble(" + doubleReturnArg + ", " + call + "); return 0;\n";
			} else if(f.returnType == "int") {
				invokers += "return " + call + ";\n";
			} else {
				throwException("Unhandled return type: " + f.returnType);
			}
		}
	}

	//put the parts together
	file << "final class ExtensionHandler {\n";
	file << "MoSync mMoSync;\n";
	file << declarators << "\n";
	file << "int checksum() {\n	return 0x" << hex << setw(8) << setfill('0') <<
		calculateChecksum(infs) << ";\n}\n\n";
	file << "void init(MoSync m) {\n";
	file << "	mMoSync = m;\n";
	file << initializers << "}\n\n";
	file << "void destruct() {\n" << destructors << "}\n\n";
	file << "int invoke(int function, int a, int b, int c) {\n"
		"	switch(function) {\n" <<
		invokers <<
		"default:\n"
		"	throw new BigPhatError();\n"
		"	}\n}\n";
	file << "}\n";
}

static void outputHeader(const Interface& inf) {
	ofstream file((INCDIR + "/" + inf.name + ".h").c_str());
	vector<string> dummy;
	streamHeaderFile(file, inf, dummy, MAIN_INTERFACE);
}

static void outputConstants(const Interface& inf) {
	ofstream file(("Output/MX_" + toupper(inf.name) + "_CONSTS.h").c_str());

	file << "#ifndef MX_" + toupper(inf.name) + "_CONSTS_H\n";
	file << "#define MX_" + toupper(inf.name) + "_CONSTS_H\n\n";

	streamConstants(file, inf.constSets, MAIN_INTERFACE);

	file << "#endif	//MX_" + toupper(inf.name) + "_CONSTS_H\n";
}

static void outputExtensions(const vector<Interface>& infs) {
	_mkdir(INCDIR.c_str());
	_mkdir("Output");
	clearOutput();

	outputJavaInvoke(infs);
	
	for(size_t i=0; i<infs.size(); i++) {
		outputHeader(infs[i]);
		outputConstants(infs[i]);
	}
}

//streams C function declarations.
void streamHeaderFunctions(ostream& stream, const Interface& inf) {
	for(size_t i=0; i<inf.functions.size(); i++) {
		const Function& f(inf.functions[i]);

		stream << f.comment;

		string invokeArgs;
		string doubleRetVar;
		stream << "inline " << f.returnType << " " << f.name << "(";
		for(size_t j=0; j<f.args.size(); j++) {
			const Argument& a(f.args[j]);
			if(j != 0)
				stream << ", ";
			invokeArgs += ", ";
			if(a.in && isPointerType(inf, a.type)) {
				stream << "const ";
			}
			stream << cType(inf, a.type);
			if(!isPointerType(inf, a.type) && !a.in)
				stream << "*";
			stream << " " << a.name;
			if(isPointerType(inf, a.type) || !a.in) {
				invokeArgs += "(int)";
			}
			if(a.type == "double" && a.in) {
				invokeArgs += "(int)&";
				if(doubleRetVar.size() == 0)
				doubleRetVar = a.name;
			}
			invokeArgs += a.name;
		}	//args
		stream << ") {\n	";

		int usedArgs = f.args.size();
		if(f.returnType == "double" && doubleRetVar.size() == 0) {
			if(usedArgs >= 3)
				throwException("Too many arguments used in function " + f.name);
			doubleRetVar = "_temp";
			stream << "double " << doubleRetVar << ";\n	";
			invokeArgs += ", (int)&_temp";
			usedArgs++;
		}
		for(size_t j=usedArgs; j<3; j++) {
			invokeArgs += ", 0";
		}

		string invoke = "maInvokeExtension(" + toString(f.number) + invokeArgs + ");";
		if(f.returnType == "double") {
			stream << invoke + "\n	return " << doubleRetVar << ";\n";
		} else {
			if(f.returnType != "void") {
				stream << "return ";
			}
			stream << invoke + "\n";
		}
		stream << "}\n\n";
	}	//function
}
