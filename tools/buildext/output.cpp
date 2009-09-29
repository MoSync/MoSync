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
#include <sstream>
#include <direct.h>
#include <hash_map>
#include <iomanip>

#include <filelist/filelist.h>

#include "buildext.h"
#include "stringFunctions.h"
#include "types.h"

using namespace std;

#if 1
#define DO_LEVEL { if(level > 0) printf("%*c", level, ' '); }
#define DUMPINT(i) { DO_LEVEL; printf("%s: %i\n", #i, i); }
#define DUMPSTRING(s) { DO_LEVEL; printf("%s: \"%s\"\n", #s, s.c_str()); }
#endif

#define INCDIR (MOSYNCDIR + "/include/ext")

static void outputJavaInvoke(const vector<ParsedExtension>& exts);
static void outputHeader(const ParsedExtension& ext);
static void streamConstants(ostream& stream, const vector<ConstSet>& constSets);
static void streamStructs(ostream& stream, const vector<Struct>& structs);
static void streamFunctions(ostream& stream, const ParsedExtension& ext);
static void outputConstants(const ParsedExtension& ext);

static void outputJavaInvoke(const vector<ParsedExtension>& exts) {
	ofstream file("Output/ExtensionHandler.java");
	string declarators, initializers, destructors, invokers;

	//collect parts in separate strings
	for(size_t i=0; i<exts.size(); i++) {
		const ParsedExtension& ext(exts[i]);

		//ExtA extA;
		string className = firstUpper(ext.name) + "Extension";
		string varName = firstLower(ext.name);
		declarators += className + " " + varName + ";\n";

		//	extA = new ExtA(mSyscall);
		initializers += "	" + varName + " = new " + className + "(m);\n";

		//	extA.destruct();
		destructors += "	" + varName + ".destruct();\n";

		for(size_t j=0; j<ext.functions.size(); j++) {
			const Function& f(ext.functions[j]);
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
		calculateChecksum(exts) << ";\n}\n\n";
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

static void outputHeader(const ParsedExtension& ext) {
	ofstream file((INCDIR + "/" + ext.name + ".h").c_str());

	file << "#ifndef " << toupper(ext.name) << "_H\n";
	file << "#define " << toupper(ext.name) << "_H\n\n";

	if(ext.comment.size() != 0) {
		file << ext.comment << "\n\n";
	}

	streamConstants(file, ext.constSets);
	streamStructs(file, ext.structs);
	streamFunctions(file, ext);

	file << "#endif	//" + toupper(ext.name) + "_H\n";
}

static void streamStructs(ostream& stream, const vector<Struct>& structs) {
	for(size_t j=0; j<structs.size(); j++) {
		const Struct& s(structs[j]);
		bool lastPodHadComment = false;
		if(s.comment.size() > 0)
			stream << s.comment;
		stream << "typedef " << s.type << " " << s.name << " {\n";
		for(size_t k=0; k<s.members.size(); k++) {
			const Member& m(s.members[k]);
			if(m.pod.size() != 1) {
				stream << "\tunion {\n";
			}
			for(size_t l=0; l<m.pod.size(); l++) {
				const PlainOldData pod(m.pod[l]);
				string tab = (m.pod.size() != 1) ? "\t\t" : "\t";
				if(lastPodHadComment) {
					stream << "\n";
				}
				if(pod.comment.size() > 0) {
					lastPodHadComment = true;
					string formattedComment;
					formattedComment.reserve(pod.comment.size() * 2);
					size_t lastIdx = 0;
					while(true) {
						size_t idx = pod.comment.find_first_of('\n', lastIdx);
						if(idx == string::npos)
							break;
						formattedComment += tab;
						formattedComment += pod.comment.substr(lastIdx, idx - lastIdx);
						formattedComment += "\n";
						lastIdx = idx + 1;
					}
					stream << formattedComment;
				}	//comment
				stream << tab << pod.type << " " << pod.name << ";\n";
			}	//pod
			if(m.pod.size() != 1) {
				stream << "\t};\n";
			}
		}	//member
		stream << "} " << s.name << ";\n\n";
	}	//struct
}

static void streamFunctions(ostream& stream, const ParsedExtension& ext) {
	for(size_t i=0; i<ext.functions.size(); i++) {
		const Function& f(ext.functions[i]);

		stream << f.comment;

		string invokeArgs;
		string doubleRetVar;
		stream << "inline " << f.returnType << " " << f.name << "(";
		for(size_t j=0; j<f.args.size(); j++) {
			const Argument& a(f.args[j]);
			if(j != 0)
				stream << ", ";
			invokeArgs += ", ";
			if(a.in && isPointerType(ext, a.type)) {
				stream << "const ";
			}
			stream << cType(ext, a.type);
			if(!isPointerType(ext, a.type) && !a.in)
				stream << "*";
			stream << " " << a.name;
			if(isPointerType(ext, a.type) || !a.in) {
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

static void outputConstants(const ParsedExtension& ext) {
	ofstream file(("Output/MX_" + toupper(ext.name) + "_CONSTS.h").c_str());

	file << "#ifndef MX_" + toupper(ext.name) + "_CONSTS_H\n";
	file << "#define MX_" + toupper(ext.name) + "_CONSTS_H\n\n";

	streamConstants(file, ext.constSets);

	file << "#endif	//MX_" + toupper(ext.name) + "_CONSTS_H\n";
}

static void streamConstants(ostream& stream, const vector<ConstSet>& constSets) {
	for(size_t i=0; i<constSets.size(); i++) {
		const ConstSet& cs(constSets[i]);
		for(size_t j=0; j<cs.constants.size(); j++) {
			const Constant& c(cs.constants[j]);

			stream << c.comment;
			stream << "#define " << cs.name << c.name << " " << c.value << "\n";
			if(c.comment.size() != 0) {
				stream << "\n";
			}
		}
		stream << "\n";
	}
}

static uint32 hash32shift(uint32 key) {
	key = key + ~(key << 15); // key = key - (key << 15) - 1;
	key = key ^ (key >> 10);
	key = key + (key << 3);
	key = key ^ (key >> 6);
	key = key * 16389; // key = (key + (key << 2)) + (key << 14);
	key = key ^ (key >> 16);
	return key;
}

uint32 calculateChecksum(const vector<ParsedExtension>& exts) {
	uint32 sum = 0;
	sum = hash32shift(exts.size());
	for(size_t i=0; i<exts.size(); i++) {
		const ParsedExtension& ext(exts[i]);
		sum += stdext::hash_value(ext.name);

		sum += ext.constSets.size();
		sum = hash32shift(sum);
		for(size_t j=0; j<ext.constSets.size(); j++) {
			const ConstSet& cs(ext.constSets[j]);
			sum += stdext::hash_value(cs.name);
			sum += cs.constants.size();
			sum = hash32shift(sum);
			for(size_t k=0; k<cs.constants.size(); k++) {
				const Constant& c(cs.constants[k]);
				sum += stdext::hash_value(c.name);
				sum += stdext::hash_value(c.value);
			}
		}

		sum += ext.functions.size();
		sum = hash32shift(sum);
		for(size_t j=0; j<ext.functions.size(); j++) {
			const Function& f(ext.functions[j]);
			sum += f.number;
			sum = hash32shift(sum);
			sum += stdext::hash_value(f.returnType);
			sum += stdext::hash_value(f.name);
			sum += f.args.size();
			sum = hash32shift(sum);
			for(size_t k=0; k<f.args.size(); k++) {
				const Argument& a(f.args[k]);
				sum += ((a.in ? (1 << k) : 32 << k));
				sum += stdext::hash_value(a.type);
				sum += stdext::hash_value(a.name);
			}
		}

		sum += ext.structs.size();
		sum = hash32shift(sum);
		for(size_t j=0; j<ext.structs.size(); j++) {
			const Struct& s(ext.structs[j]);
			sum += stdext::hash_value(s.type);
			sum += stdext::hash_value(s.name);
			sum += s.members.size();
			sum = hash32shift(sum);
			for(size_t k=0; k<s.members.size(); k++) {
				const Member& m(s.members[k]);
				sum += m.pod.size();
				sum = hash32shift(sum);
				for(size_t l=0; l<m.pod.size(); l++) {
					const PlainOldData& pod(m.pod[l]);
					sum += stdext::hash_value(pod.type);
					sum += stdext::hash_value(pod.name);
				}
			}
		}
	}
	return sum;
}
