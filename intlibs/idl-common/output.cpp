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
//#include <direct.h>
#include <iomanip>

#include <filelist/filelist.h>

#include "idl-common.h"
#include "stringFunctions.h"
#include "types.h"

#ifdef _MSC_VER
#define PFZT "Iu"
#elif defined(__GNUC__)
#define PFZT "zu"
#else
#error Unsupported compiler!
#endif

using namespace std;

#define DO_LEVEL { if(level > 0) printf("%*c", level, ' '); }
#define DUMPINT(i) { DO_LEVEL; printf("%s: %i\n", #i, i); }
#define DUMPSIZE(s) { DO_LEVEL; printf("%s: %" PFZT "\n", #s, s); }
#define DUMPSTRING(s) { DO_LEVEL; printf("%s: \"%s\"\n", #s, s.c_str()); }

static void streamStructs(ostream& stream, const vector<Struct>& structs, int ix);
static void streamIoctls(ostream& stream, const Interface& inf, int ix);
static void streamTypedefs(ostream& stream, const vector<Typedef>& typedefs, int ix);
static void streamDefines(ostream& stream, const vector<Define>& defines, int ix);
static void streamIoctlFunction(ostream& stream, const Interface& inf, const Function& f,
	const string& ioctlName);

#if 0
static void deleteCallback(const char* filename) {
	remove((string("Output/") + filename).c_str());
}

static void clearOutput() {
	int res = scanDirectory("Output/*", deleteCallback);
	if(res)
		throwException("scanDirectory");
}
#endif

void dumpInterfaces(const vector<Interface>& infs) {
	int level = 0;
	DUMPSIZE(infs.size());
	for(size_t i=0; i<infs.size(); i++) {
		const Interface& inf(infs[i]);
		DUMPSTRING(inf.name);
		level++;
		DUMPSIZE(inf.constSets.size());
		for(size_t j=0; j<inf.constSets.size(); j++) {
			const ConstSet& cs(inf.constSets[j]);
			level++;
			DUMPSTRING(cs.name);
			DUMPSIZE(cs.constants.size());
			level++;
			for(size_t k=0; k<cs.constants.size(); k++) {
				const Constant& c(cs.constants[k]);
				DO_LEVEL;
				printf("%s = %s;\n", c.name.c_str(), c.value.c_str());
			}
			level--;
			level--;
		}
		DUMPSIZE(inf.functions.size());
		level++;
		for(size_t j=0; j<inf.functions.size(); j++) {
			const Function& f(inf.functions[j]);
			DO_LEVEL;
			printf("%s %s\n", f.returnType.c_str(), f.name.c_str());
			level++;
			DUMPSIZE(f.args.size());
			if(f.args.size() != 0)
				DO_LEVEL;
			for(size_t k=0; k<f.args.size(); k++) {
				const Argument& a(f.args[k]);
				if(k != 0)
					printf(", ");
				printf("%s %s %s", a.in ? "in" : "out", a.type.c_str(), a.name.c_str());
			}
			if(f.args.size() != 0)
				printf("\n");
			level--;
		}
		level--;
		for(size_t j=0; j<inf.structs.size(); j++) {
			const Struct& s(inf.structs[j]);
			DO_LEVEL;
			printf("%s %s\n", s.type.c_str(), s.name.c_str());
			level++;
			DUMPSIZE(s.members.size());
			for(size_t k=0; k<s.members.size(); k++) {
				const Member& m(s.members[k]);
				if(m.pod.size() != 1) {
					DO_LEVEL;
					printf("union {\n");
					level++;
				}
				for(size_t l=0; l<m.pod.size(); l++) {
					const PlainOldData pod(m.pod[l]);
					DO_LEVEL;
					printf("%s %s;\n", pod.type.c_str(), pod.name.c_str());
				}
				if(m.pod.size() != 1) {
					level--;
					DO_LEVEL;
					printf("};\n");
				}
			}
			level--;
		}
		level--;
	}
}

void streamHash(ostream& stream, const Interface& inf) {
	stream << "/** \\brief A hash of the abstract representation of the API described in this file.\n";
	stream << " * Identifiers, declarations and definitions are included in the calculation of the hash,\n";
	stream << " * but documentation is not.\n";
	stream << " */\n";
	stream << "#define MAIDL_HASH ((int)0x" << setfill('0') << setw(8) <<
		hex << calculateChecksum(inf) << dec << ")\n\n";
}

void streamHeaderFile(ostream& stream, const Interface& inf, const vector<string>& ixs, int ix) {
	string headerName;
	if(ix == MAIN_INTERFACE)
		headerName = toupper(getFilenameFromPath(inf.name));
	else
		headerName = toupper(ixs[ix]);

	stream << "#ifndef " << headerName << "_H\n";
	stream << "#define " << headerName << "_H\n\n";

	if(ix == MAIN_INTERFACE) {
		if(inf.comment.size() != 0) {
			stream << inf.comment << "\n";
		}

		streamHash(stream, inf);

		stream << "#if defined(__GNUC__) || defined(__SYMBIAN32__)\n"
			"#define ATTRIBUTE(a, func)  func __attribute__ ((a))\n"
			"#define GCCATTRIB(a) __attribute__ ((a))\n"
			"#elif defined(_MSC_VER)\n"
			"#define ATTRIBUTE(a, func)  __declspec (a) func\n"
			"#define GCCATTRIB(a)\n"
			"#define inline __inline\n"
			"#else\n"
			"#error Unsupported compiler!\n"
			"#endif\n"
			"\n"
			"#ifndef MAPIP\n"
			"#define _HAVE_STRING_ARCH_strcmp\n"
			"#define _HAVE_STRING_ARCH_strcpy\n"
			"#include <memory.h>\n"
			"#define memset _memset\n"
			"#define memcpy _memcpy\n"
			"#include <math.h>\n"
			"#endif	//MAPIP\n"
			"\n";

		streamMoSyncDllDefines(stream);
	}

	stream << "#ifdef __cplusplus\n"
		"extern \"C\" {\n"
		"#endif\n\n";

	streamTypedefs(stream, inf.typedefs, ix);
	streamDefines(stream, inf.defines, ix);
	streamConstants(stream, inf.constSets, ix);
	streamStructs(stream, inf.structs, ix);
	if(ix == MAIN_INTERFACE) {
		streamHeaderFunctions(stream, inf, false);
	}
	streamIoctls(stream, inf, ix);

	stream << "#ifdef __cplusplus\n"
		"}\n"
		"#endif\n\n";

	if(ix == MAIN_INTERFACE) {
		stream << "#ifndef MAPIP\n"
			"#undef memset\n"
			"#undef memcpy\n"
			"#endif	//MAPIP\n"
			"\n";
	}

	stream << "#endif	//" + headerName + "_H\n";
}

void streamMoSyncDllDefines(ostream& stream) {
	stream << "#ifdef WIN32\n"
		"#define DLLEXPORT dllexport\n"
		"#define DLLIMPORT dllimport\n"
		"#elif defined(LINUX)\n"
		"#define DLLEXPORT visibility(\"default\")\n"
		"#define DLLIMPORT\n"
		"#endif\n"
		"\n"
		"#ifdef MOSYNC_DLL_EXPORT\n"
		"#define MOSYNC_API DLLEXPORT\n"
		"#elif defined(MOSYNC_DLL_IMPORT)\n"
		"#define MOSYNC_API DLLIMPORT\n"
		"#else\n"
		"#define MOSYNC_API\n"
		"#endif\n\n";
}

static void streamMembers(ostream& stream, string tab, const vector<Member>& members,
	const vector<Struct>& structs)
{
	bool lastPodHadComment = false;
	for(size_t k=0; k<members.size(); k++) {
		const Member& m(members[k]);
		if(m.pod.size() != 1) {
			stream << tab << "union {\n";
			tab += "\t";
		}
		for(size_t l=0; l<m.pod.size(); l++) {
			const PlainOldData pod(m.pod[l]);
			if(lastPodHadComment) {
				stream << "\n";
			}
			if(pod.comment.size() > 0) {
				lastPodHadComment = true;
				//format multi-line comments to take the tabulation into account.
				size_t lastIdx = 0;
				while(true) {
					size_t idx = pod.comment.find_first_of('\n', lastIdx);
					if(idx == string::npos)
						break;
					stream << tab << pod.comment.substr(lastIdx, idx - lastIdx) << "\n";
					lastIdx = idx + 1;
				}
			}	//comment
			if(isAnonStructName(pod.type)) {
				if(pod.name.size() > 0 || pod.comment.size() > 0)
					throwException("Bad anonymous struct");
				//find struct definition
				bool found = false;
				for(size_t m2=0; m2<structs.size(); m2++) {
					const Struct& as(structs[m2]);
					if(as.name == pod.type) {
						//print it
						stream << tab << "struct {\n";
						streamMembers(stream, tab + "\t", as.members, structs);
						stream << tab << "};\n";
						found = true;
						break;
					}
				}
				if(!found)
					throwException("Anonymous struct not found");
			} else {
				stream << tab << pod.type << " " << pod.name << ";\n";
			}
		}	//pod
		if(m.pod.size() != 1) {
			stream << "\t};\n";
		}
	}	//member
}

static void streamStructs(ostream& stream, const vector<Struct>& structs, int ix) {
	for(size_t j=0; j<structs.size(); j++) {
		const Struct& s(structs[j]);
		if(s.ix != ix || isAnonStructName(s.name))
			continue;
		if(s.comment.size() > 0)
			stream << s.comment;
		stream << "typedef " << s.type << " " << s.name << " {\n";
		streamMembers(stream, "\t", s.members, structs);
		stream << "} " << s.name << ";\n\n";
	}	//struct
}

void streamConstants(ostream& stream, const vector<ConstSet>& constSets, int ix) {
	for(size_t i=0; i<constSets.size(); i++) {
		const ConstSet& cs(constSets[i]);
		bool anyStreamed = false;
		for(size_t j=0; j<cs.constants.size(); j++) {
			const Constant& c(cs.constants[j]);
			if(c.ix != ix)
				continue;

			if(anyStreamed && (c.comment.size() != 0 || cs.constants[j-1].comment.size() != 0))
				stream << "\n";
			stream << c.comment;
			stream << "#define " << cs.name << c.name << " " << c.value << "\n";
			anyStreamed = true;
		}
		if(anyStreamed)
			stream << "\n";
	}
}

static void streamTypedefs(ostream& stream, const vector<Typedef>& typedefs, int ix) {
	bool anyStreamed = false;
	for(size_t i=0; i<typedefs.size(); i++) {
		const Typedef& t(typedefs[i]);
		if(t.ix != ix)
			continue;
		if(anyStreamed && (t.comment.size() != 0 || typedefs[i-1].comment.size() != 0))
			stream << "\n";
		stream << t.comment;
		stream << "typedef " << t.type << " " << t.name << ";\n";
		anyStreamed = true;
	}
	if(anyStreamed)
		stream << "\n";
}

static void streamDefines(ostream& stream, const vector<Define>& defines, int ix) {
	bool anyStreamed = false;
	for(size_t i=0; i<defines.size(); i++) {
		const Define& d(defines[i]);
		if(d.ix != ix)
			continue;
		if(anyStreamed && (d.comment.size() != 0 || defines[i-1].comment.size() != 0))
			stream << "\n";
		stream << d.comment;
		stream << "#define " << d.value << "\n";
		anyStreamed = true;
	}
	if(anyStreamed)
		stream << "\n";
}

void streamIoctlDefines(ostream& stream, const vector<Ioctl>& ioctls, int ix) {
	for(size_t i=0; i<ioctls.size(); i++) {
		const Ioctl& ioctl(ioctls[i]);
		bool anyStreamed = false;
		for(size_t j=0; j<ioctl.functions.size(); j++) {
			const IoctlFunction& f(ioctl.functions[j]);
			if(f.ix != ix)
				continue;

			stream << "#define " << ioctl.name << "_" << f.f.name << " " << f.f.number << "\n";
			anyStreamed = true;
		}
		if(anyStreamed)
			stream << "\n";
	}
}

static void streamIoctls(ostream& stream, const Interface& inf, int ix) {
	for(size_t i=0; i<inf.ioctls.size(); i++) {
		const Ioctl& ioctl(inf.ioctls[i]);
		for(size_t j=0; j<ioctl.functions.size(); j++) {
			const IoctlFunction& f(ioctl.functions[j]);
			if(f.ix != ix)
				continue;
			streamIoctlFunction(stream, inf, f.f, ioctl.name);
		}
	}
}

static void streamIoctlFunction(ostream& stream, const Interface& inf, const Function& f,
	const string& ioctlName)
{
	stream << f.comment;

	string invokeArgs;
	string doubleRetVar;
	stream << "static inline " << f.returnType << " " << f.name << "(";
	if(f.args.size() == 0) {
		stream << "void";
	}
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

	string invoke = ioctlName + "(" + toString(f.number) + invokeArgs + ");";
	if(f.returnType == "double") {
		stream << invoke + "\n	return " << doubleRetVar << ";\n";
	} else {
		if(f.returnType != "void") {
			stream << "return ";
		}
		stream << invoke + "\n";
	}
	stream << "}\n\n";
}

void streamCppDefsFile(ostream& stream, const Interface& inf, const vector<string>& ixs, int ix) {
	string headerName;
	if(ix == MAIN_INTERFACE)
		headerName = toupper(getFilenameFromPath(inf.name));
	else
		headerName = toupper(ixs[ix]);

	stream << "#ifndef " << headerName << "_DEFS_H\n";
	stream << "#define " << headerName << "_DEFS_H\n\n";

	streamHash(stream, inf);

	streamTypedefs(stream, inf.typedefs, ix);
	streamDefines(stream, inf.defines, ix);
	streamConstants(stream, inf.constSets, ix);
	streamStructs(stream, inf.structs, ix);
	streamIoctlDefines(stream, inf.ioctls, ix);

	stream << "#endif\t//" + headerName + "_DEFS_H\n";
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


#if (defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__))) || defined(__WATCOMC__) || \
	defined(_MSC_VER) || defined (__BORLANDC__) || defined (__TURBOC__)
#define get16bits(d) (*((const uint16 *) (d)))
#endif

#if !defined (get16bits)
#define get16bits(d) ((((uint32)(((const uint8 *)(d))[1])) << 8)\
	+(uint32)(((const uint8 *)(d))[0]) )
#endif

//Thanks to Paul Hsieh.
static uint32 SuperFastHash(const char * data, int len) {
	uint32 hash = len, tmp;
	int rem;

	if (len <= 0 || data == NULL) return 0;

	rem = len & 3;
	len >>= 2;

	/* Main loop */
	for (; len > 0; len--) {
		hash += (*((const uint16*)(data)));
		hash  += get16bits (data);
		tmp    = (get16bits (data+2) << 11) ^ hash;
		hash   = (hash << 16) ^ tmp;
		data  += 2*sizeof (uint16);
		hash  += hash >> 11;
	}

	/* MAHandle end cases */
	switch (rem) {
	case 3: hash += get16bits (data);
		hash ^= hash << 16;
		hash ^= data[sizeof (uint16)] << 18;
		hash += hash >> 11;
		break;
	case 2: hash += get16bits (data);
		hash ^= hash << 11;
		hash += hash >> 17;
		break;
	case 1: hash += *data;
		hash ^= hash << 10;
		hash += hash >> 1;
	}

	/* Force "avalanching" of final 127 bits */
	hash ^= hash << 3;
	hash += hash >> 5;
	hash ^= hash << 4;
	hash += hash >> 17;
	hash ^= hash << 25;
	hash += hash >> 6;

	return hash;
}

template<class T> uint32 templateHash(const T& v);

template<> uint32 templateHash<string>(const string& v) {
	return SuperFastHash(v.c_str(), v.length());
}
#ifdef __x86_64__	//in this case, uint32 is different from size_t
template<> uint32 templateHash<uint32>(const uint32& v) {
	return hash32shift(v);
}
#endif
template<> uint32 templateHash<int>(const int& v) {
	return hash32shift(v);
}
template<> uint32 templateHash<size_t>(const size_t& v) {
	return hash32shift(v);
}

//NOTE: order of things is taken into account.
//EXAMPLE: swapping argument order will not yield the same hash.

#define AH(value) sum = hash32shift(sum + templateHash(value))

static uint32 calculateFunctionChecksum(const Function& f) {
	uint32 sum = 0;
	AH(f.number);
	AH(f.returnType);
	AH(f.name);
	AH(f.args.size());
	for(size_t k=0; k<f.args.size(); k++) {
		const Argument& a(f.args[k]);
		AH(a.in ? (1 << k) : (32 << k));
		AH(a.type);
		AH(a.name);
	}
	return sum;
}

uint32 calculateChecksum(const vector<Interface>& infs) {
	uint32 sum = 0;
	AH(infs.size());
	for(size_t i=0; i<infs.size(); i++) {
		AH(calculateChecksum(infs[i]));
	}
	return sum;
}

uint32 calculateChecksum(const Interface& inf) {
	uint32 sum = 0;
	AH(inf.name);

	AH(inf.typedefs.size());
	for(size_t j=0; j<inf.typedefs.size(); j++) {
		const Typedef& t(inf.typedefs[j]);
		AH(t.type);
		AH(t.name);
		AH(t.ix);
	}	//typedefs

	AH(inf.defines.size());
	for(size_t j=0; j<inf.defines.size(); j++) {
		const Define& d(inf.defines[j]);
		AH(d.value);
		AH(d.ix);
	}	//defines

	AH(inf.constSets.size());
	for(size_t j=0; j<inf.constSets.size(); j++) {
		const ConstSet& cs(inf.constSets[j]);
		AH(cs.name);
		AH(cs.constants.size());
		for(size_t k=0; k<cs.constants.size(); k++) {
			const Constant& c(cs.constants[k]);
			AH(c.name);
			AH(c.value);
			AH(c.ix);
		}
	}	//constSets

	AH(inf.functions.size());
	for(size_t j=0; j<inf.functions.size(); j++) {
		const Function& f(inf.functions[j]);
		AH(calculateFunctionChecksum(f));
	}	//functions

	AH(inf.structs.size());
	for(size_t j=0; j<inf.structs.size(); j++) {
		const Struct& s(inf.structs[j]);
		AH(s.type);
		AH(s.name);
		AH(s.members.size());
		AH(s.ix);
		for(size_t k=0; k<s.members.size(); k++) {
			const Member& m(s.members[k]);
			AH(m.pod.size());
			for(size_t l=0; l<m.pod.size(); l++) {
				const PlainOldData& pod(m.pod[l]);
				AH(pod.type);
				AH(pod.name);
			}
		}
	}	//structs

	AH(inf.ioctls.size());
	for(size_t j=0; j<inf.ioctls.size(); j++) {
		const Ioctl& ioctl(inf.ioctls[j]);
		AH(ioctl.name);
		AH(ioctl.functions.size());
		for(size_t k=0; k<ioctl.functions.size(); k++) {
			const IoctlFunction& f(ioctl.functions[k]);
			AH(f.ix);
			AH(calculateFunctionChecksum(f.f));
		}
	}	//ioctls

	return sum;
}
