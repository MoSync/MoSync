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
#include <helpers/types.h>

#include "idl-common.h"
#include "stringFunctions.h"
#include "types.h"
#include "tokenizer.h"

#include <string.h>

using namespace std;

#define DO_LEVEL { if(level > 0) printf("%*c", level, ' '); }
#define DUMPINT(i) { DO_LEVEL; printf("%s: %i\n", #i, i); }
#define DUMPSIZE(s) { DO_LEVEL; printf("%s: %" PFZT "\n", #s, s); }
#define DUMPSTRING(s) { DO_LEVEL; printf("%s: \"%s\"\n", #s, s.c_str()); }

static void streamStructs(ostream& stream, const Interface& inf, int ix, bool runtime);
static void streamIoctls(ostream& stream, const Interface& inf, int ix);
static void streamTypedefs(ostream& stream, const vector<Typedef>& typedefs, int ix, bool runtime);
static void streamDefines(ostream& stream, const vector<Define>& defines, int ix);
static void streamJavaConstants(
	ostream& stream,
	const vector<ConstSet>&
	constSets,
	int ix);
void streamGroups(ostream& stream, const vector<Group>& groups, const vector<int>& children, int ix, int level=0);
void streamGroups(ostream& stream, const vector<Group>& groups, int ix);

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


void streamGroups(ostream& stream, const vector<Group>& groups, const vector<int>& children, int ix, int level) {
	string tabString = "";
	for(int i = 0; i < level; i++) tabString += "\t";

	for(size_t i = 0; i < children.size(); i++) {
		int index = children[i];
		stream << groups[index].comment;
		if(groups[index].groups.size() > 0) {
			stream << "/** @defgroup " << groups[index].groupId << " " << groups[index].groupPrettyName << "\n";
			stream << "* @{\n";
			stream << "*/\n";

			streamGroups(stream, groups, groups[index].groups, ix, level+1);
			stream << "/** @} */ // end of " << groups[index].groupId  << "\n";
		} else {
			stream << "/** @defgroup " << groups[index].groupId << " " << groups[index].groupPrettyName << " */\n";
		}
	}
}

void streamGroups(ostream& stream, const vector<Group>& groups, int ix) {
	if(ix != MAIN_INTERFACE) return;

	vector<int> rootGroups;
	for(size_t i = 0; i < groups.size(); i++) {
		if(groups[i].isRoot) {
			rootGroups.push_back(i);
		}
	}

	streamGroups(stream, groups, rootGroups, ix);

	stream << "\n";
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

		stream << "#include \"maapi_defs.h\"\n"
			"\n"
			"#ifndef MAPIP\n"
			"#define _HAVE_STRING_ARCH_strcmp\n"
			"#define _HAVE_STRING_ARCH_strcpy\n"
			"#include <memory.h>\n"
			"#define memset _memset\n"
			"#define memcpy _memcpy\n"
			"#include <math.h>\n"
			"#endif	//MAPIP\n"
			"\n"
			;
	}

	stream << "#ifdef __cplusplus\n"
		"extern \"C\" {\n"
		"#endif\n\n";

	streamGroups(stream, inf.groups, ix);
	streamTypedefs(stream, inf.typedefs, ix, false);
	streamDefines(stream, inf.defines, ix);
	streamConstants(stream, inf.constSets, ix);
	streamStructs(stream, inf, ix, false);
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

/**
 * Streams C function declarations.
 */
void streamHeaderFunctions(ostream& stream, const Interface& inf, bool syscall) {
	for(size_t i=0; i<inf.functions.size(); i++) {
		const Function& f(inf.functions[i]);
		stream << f.comment;
		if(f.groupId != "")
			stream << "/** @ingroup " << f.groupId << " */\n";

		if(!syscall && f.returnType != "noreturn")
			stream <<
				"#if defined(__arm__) && !defined(MAPIP) && !defined(MOSYNC_NATIVE)\n"
				"inline\n"
				"#endif\n";
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

		if(f.isIOCtl)
			stream << " MA_IOCTL_ELLIPSIS";

		if(f.returnType == "noreturn")
			stream << ")";
		if(syscall)
			stream << ")";
		stream << ");\n\n";
	}
	stream << "\n";
}

/**
 * Generate the content of a .java class file for the main MoSync API
 * or the given interface.
 * @param stream The output stream.
 * @param className Name of the class.
 * @param apiData The parsed API data.
 * @param ix The id of the extension to generate definitions for.
 * Also used to specify if definitions for the main API is to be generated.
 */
void streamJavaDefinitionFile(
	ostream& stream,
	const string& className,
	const Interface& apiData,
	int ix)
{
	stream << "package com.mosync.internal.generated;\n\n";
	stream << "public class " << className << "\n";
	stream << "{\n";

	streamJavaConstants(stream, apiData.constSets, ix);

	stream << "}\n";
}

/**
 * Generate constants for a Java definition class file.
 * @param stream The output stream.
 * @param constSets Constant definitions.
 * @param ix The id of the extension to generate definitions for.
 * Also used to specify if definitions for the main API is to be generated.
 */
static void streamJavaConstants(
	ostream& stream,
	const vector<ConstSet>& constSets,
	int ix)
{
	for (size_t i=0; i<constSets.size(); i++)
	{
		const ConstSet& cs(constSets[i]);
		bool anyStreamed = false;
		for (size_t j=0; j<cs.constants.size(); j++)
		{
			const Constant& c(cs.constants[j]);
			if (c.ix != ix)
			{
				continue;
			}

			if (anyStreamed &&
				(c.comment.size() != 0
				|| cs.constants[j-1].comment.size() != 0))
			{
				stream << "\n";
			}

			string type = "int ";
			if(c.type == "MAString")
				type = "String ";

			stream << c.comment;
			stream
				<< "\tpublic static final "
				<< type
				<< cs.name
				<< c.name
				<< " = "
				<< c.value
				<< ";\n";
			anyStreamed = true;
		}
		if (anyStreamed)
		{
			stream << "\n";
		}
	}
}

static void streamMembers(ostream& stream, string tab, const vector<Member>& members,
	const Interface& inf, bool runtime, bool native)
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
				if(pod.name.empty() || pod.comment.size() > 0)
					throwException("Bad anonymous struct");
				//find struct definition
				bool found = false;
				for(size_t m2=0; m2<inf.structs.size(); m2++) {
					const Struct& as(inf.structs[m2]);
					if(as.name == pod.type) {
						//print it
						if(runtime) {
							stream << "#ifdef SYMBIAN\n";
							stream << tab << pod.type << " " << pod.name << ";\n";
							stream << "#else\n";
						}
						stream << tab << "struct {\n";
						streamMembers(stream, tab + "\t", as.members, inf, runtime, native);
						stream << tab << "};\n";
						if(runtime) {
							stream << "#endif\n";
						}
						found = true;
						break;
					}
				}
				if(!found)
					throwException("Anonymous struct not found");
			} else {
				stream << tab;
				if(runtime && !native && isDirectPointerType(inf, pod.type)) {
					stream << "MAAddress";
				} else {
					stream << pod.type;
				}
				stream << " " << pod.name << ";\n";
			}
		}	//pod
		if(m.pod.size() != 1) {
			stream << "\t};\n";
		}
	}	//member
}

static void streamStruct(ostream& stream, const Struct& s, const string& name,
	const Interface& inf, int ix, bool runtime, bool native)
{
	stream << "typedef " << s.type;
	if(!native) {
		stream << "\n"
			"#ifdef __GNUC__\n"
			"\t__attribute((packed,aligned(4)))\n"
			"#elif defined(_MSC_VER)\n"
			"#pragma pack(push, 4)\n"
			"#elif !defined(SYMBIAN)\n"
			"#error Unsupported compiler!\n"
			"#endif\n"
		;
	}
	stream << " " << name << " {\n";
	streamMembers(stream, "\t", s.members, inf, runtime, native);
	stream << "} " << name << ";\n";
	if(!native) {
		stream <<
			"#ifdef _MSC_VER\n"
			"#pragma pack(pop)\n"
			"#endif\n"
		;
	}
}

static void streamStructs(ostream& stream, const Interface& inf, int ix, bool runtime) {
	for(size_t j=0; j<inf.structs.size(); j++) {
		const Struct& s(inf.structs[j]);
		if(s.ix != ix)
			continue;
		if(isAnonStructName(s.name)) {
			if(!runtime)
				continue;
			stream << "#ifdef SYMBIAN\n";
		}
		if(s.comment.size() > 0)
			stream << s.comment;
		if(s.groupId != "")
			stream << "/** @ingroup " << s.groupId << " */\n";
		streamStruct(stream, s, s.name, inf, ix, runtime, false);
		if(runtime) {
			streamStruct(stream, s, s.name + "Native", inf, ix, true, true);
		}
		if(isAnonStructName(s.name)) {
			stream << "#endif\n";
		}
		stream << "\n";
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
			anyStreamed = true;
		}

		anyStreamed = false;
		for(size_t j=0; j<cs.constants.size(); j++) {
			const Constant& c(cs.constants[j]);
			if(c.ix != ix)
				continue;

			if(anyStreamed && (c.comment.size() != 0 || cs.constants[j-1].comment.size() != 0))
				stream << "\n";

			stream << c.comment;
			if(c.groupId != "")
				stream << "/** @ingroup " << c.groupId << " */\n";
			stream << "#define " << cs.name << c.name << " " << c.value << "\n";

			anyStreamed = true;
		}
	}
}

static void streamTypedefs(ostream& stream, const vector<Typedef>& typedefs, int ix, bool runtime) {
	if(ix == MAIN_INTERFACE)
		stream <<
		"#ifndef _WCHAR_DEFINED\n"
		"#define _WCHAR_DEFINED\n"
		"#ifdef MAPIP\n"
		"#ifdef __cplusplus\n"
		"typedef wchar_t wchar;\n"
		"#else\n"
		"typedef unsigned short wchar;\n"
		"typedef wchar wchar_t;\n"
		"#endif	//__cplusplus\n"
		"#else	//MAPIP\n"
		"#if defined(MOSYNC_NATIVE)\n"
		"#include <stddef.h>\n"
		"typedef wchar_t wchar;\n"
		"#else\n"
		"typedef unsigned short wchar;\n"
		"#endif // MOSYNC_NATIVE\n"
		"#endif	//MAPIP\n"
		"#endif	//_WCHAR_DEFINED\n"
		"\n"
		"#ifndef _SYSV_TYPES_DEFINED\n"
		"#define _SYSV_TYPES_DEFINED\n"
		"typedef unsigned short ushort;\n"
		"typedef unsigned int uint;\n"
		"#endif	//_SYSV_TYPES_DEFINED\n"
		"\n"
		;
	bool anyStreamed = false;
	for(size_t i=0; i<typedefs.size(); i++) {
		const Typedef& t(typedefs[i]);
		if(t.ix != ix)
			continue;
		if(anyStreamed && (t.comment.size() != 0 || typedefs[i-1].comment.size() != 0))
			stream << "\n";
		stream << t.comment;
		if(t.groupId != "")
			stream << "/** @ingroup " << t.groupId << " */\n";
		stream << "typedef ";
		if(runtime && t.name == "MAAddress") {
			stream << "unsigned int";
		} else {
			stream << t.type;
		}
		stream << " " << t.name << ";\n";
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
		if(d.groupId != "")
			stream << "/** @ingroup " << d.groupId << " */\n";
		stream << "#define " << d.value << "\n";
		anyStreamed = true;
	}
	if(anyStreamed)
		stream << "\n";
}

static void streamIoctlInputParam(ostream& stream, int k, bool java) {
	if(k < 3) {
		stream << (char)('a'+k);
	} else {
		if (!java) {
			stream << "ARG_NO_" << (k + 1);
		} else {
			stream << "mCore.";
			stream << "GetValidatedStackValue(" << ((k-3)<<2) << ")";
		}
	}
}

// IS C Token Character
static bool isctc(int c) {
	return isalnum(c) || c == '_';
}

static void streamArgRange(ostream& stream, const vector<Argument>& args,
	const string& range)
{
	stream << ", ";
	// tokenize range expression.
	// for each token, compare to args.
	// on match, add "_".
	//arg.range;
	size_t pos = 0;
	while(pos < range.length()) {
		size_t beg = pos;
		bool cToken = isctc(range[beg]);
		size_t end = beg + 1;
		while(end < range.length()) {
			if(isctc(range[end]) != cToken)
				break;
			end++;
		}
		string token = range.substr(beg, end - beg);
		pos = end;

		// we now have a token.
		bool match = false;
		for(size_t i=0; i<args.size(); i++) {
			if(token == args[i].name) {
				match = true;
				stream << "_" << token;
				break;
			}
		}
		if(!match)
			stream << token;
	}
	stream << ")";
}

static void streamIoctlArg(ostream& stream, const vector<Argument>& args,
	const Argument& arg, const Interface& inf, size_t inK, bool java)
{
	string ctype = (java ? jType : cType)(inf, arg.type);
	const string& resolvedType = resolveType(inf, ctype);
	bool isPointer = isPointerType(inf, arg.type) || !arg.in;
	bool isString = arg.type == "MAString" || arg.type == "MAWString";
	bool isWideString = arg.type == "MAWString";
	bool isDouble = resolvedType == "double" && arg.in;
	bool isFloat = resolvedType == "float" && arg.in;

	if(!java) {
		if(arg.type == "MAAddress") {
			ctype = "void*";
			isPointer = true;
		}

		if(isPointer || isString)
			ctype = (arg.in ? "const " : "") + ctype;
		if(!isPointerType(inf, arg.type) && !arg.in)
			ctype += '*';
	} else if(!arg.in) {
		ctype = "MAAddress";
	}

	string localName = "_" + arg.name;
	string dvName = localName+"_dv";

	if(isDouble) {
		stream << "MA_DV "<<dvName<<"; \\\n";
		stream << dvName<<".MA_DV_HI = ";
		streamIoctlInputParam(stream, inK, java);
		stream << "; \\\n";
		inK++;
		stream << dvName<<".MA_DV_LO = ";
		streamIoctlInputParam(stream, inK, java);
		stream << "; \\\n";
	}
	if(isFloat) {
		stream << "MA_FV "<<dvName<<"; \\\n";
		stream << dvName<<".i = ";
		streamIoctlInputParam(stream, inK, java);
		stream << "; \\\n";
	}
	stream << ctype << " " << localName << " = ";

	if(isDouble) {
		stream << dvName << ".d";
	} else if(isFloat) {
		stream << dvName << ".f";
	} else {
		if(isString && arg.in)
			stream << (isWideString ? "GVWS(" : "GVS(");
		else if(isPointer && !java) {
			if(arg.range.empty()) {
				stream << "GVMR(";
			} else {
				stream << "(" << arg.type << ") SYSCALL_THIS->GetValidatedMemRange(";
			}
		} else if(ctype != "int")
			stream << "(" << ctype << ")";

		streamIoctlInputParam(stream, inK, java);

		if(isString && arg.in)
			stream << ")";
		else if(isPointer && !java) {
			if(arg.range.empty()) {
				string gvmrType;
				size_t m1 = arg.type.size()-1;
				if(arg.type[m1] == '*') {
					gvmrType = arg.type.substr(0, m1);
				} else {
					gvmrType = arg.type;
				}
				stream << ", " << gvmrType << ")";
			} else {
				streamArgRange(stream, args, arg.range);
			}
		}
	}

	stream << "; \\\n";
}

static void streamCaselist(ostream& stream, const Ioctl& ioctl, const string& headerName,
	int ix, bool syscall)
{
	stream << "#define " << ioctl.name << "_" << headerName <<
		(syscall ? "_syscall" : "") << "_caselist \\\n";
	for(size_t j=0; j<ioctl.functions.size(); j++) {
		const IoctlFunction& f(ioctl.functions[j]);
		if(f.ix != ix)
			continue;
		if(syscall)
			stream << "maIOCtl_syscall_case(" << f.f.name << ") \\\n";
		else
			stream << ioctl.name << "_" << f.f.name << "_case(" << f.f.name << ") \\\n";
	}
	stream << "\n";
}

void streamIoctlDefines(ostream& stream, const Interface& inf, const string& headerName,
	int ix, bool java)
{
	const vector<Ioctl>& ioctls = inf.ioctls;
	for(size_t i=0; i<ioctls.size(); i++) {
		const Ioctl& ioctl(ioctls[i]);
		bool anyStreamed = false;

		streamCaselist(stream, ioctl, headerName, ix, false);
		streamCaselist(stream, ioctl, headerName, ix, true);

		for (int k = 3; !java && k < 10; k++) {
			int argNo = k + 1;
			stream << "#undef ARG_NO_" << argNo << "\n";
			stream << "#define ARG_NO_" << argNo << " ";
			stream << "SYSCALL_THIS->GetValidatedStackValue(" << ((k-3)<<2) << " VSV_ARGPTR_USE)\n";
		}
		for(size_t j=0; j<ioctl.functions.size(); j++) {
			const IoctlFunction& f(ioctl.functions[j]);
			if(f.ix != ix)
				continue;

			stream << "#define " << ioctl.name << "_" << f.f.name << " " << f.f.number << "\n";

			stream << "#define " << ioctl.name << "_" << f.f.name << "_case(func) \\\n";
			stream << "case " << f.f.number << ": \\\n";
			stream << "{ \\\n";
			for(size_t k = 0, inK = 0; k < f.f.args.size(); k++, inK++) {
				const Argument& arg(f.f.args[k]);
				if(arg.range.empty())
					streamIoctlArg(stream, f.f.args, arg, inf, inK, java);
			}
			for(size_t k = 0, inK = 0; k < f.f.args.size(); k++, inK++) {
				const Argument& arg(f.f.args[k]);
				if(!arg.range.empty())
					streamIoctlArg(stream, f.f.args, arg, inf, inK, java);
			}

			string resolvedReturnType = resolveType(inf, f.f.returnType);
			bool returnDouble = resolvedReturnType == "double";
			bool returnFloat = resolvedReturnType == "float";
			bool returnVoid = resolvedReturnType == "void";
			if(returnDouble) {
				stream << "MA_DV result; \\\n";
				stream << "result.d = func(";
			} else if(returnFloat) {
				stream << "MA_FV result; \\\n";
				stream << "result.f = func(";
			} else if(returnVoid) {
				stream << "func(";
			} else {
				stream << "return func(";
			}
			for(size_t k = 0; k < f.f.args.size(); k++) {
				if(k!=0)
					stream << ", ";
				stream << "_" << f.f.args[k].name;
			}
			stream << "); \\\n";
			if(returnDouble) {
				stream << "return result.ll; \\\n";
			} else if(returnFloat) {
				stream << "return result.i; \\\n";
			} else if(returnVoid) {
				stream << "return 0; \\\n";
			}
			stream << "} \\\n";
			stream << "\n";

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
void streamIoctlFunction(ostream& stream, const Interface& inf, const Function& f,
	const string& ioctlName, int fnOffset)
{
	streamIoctlFunction(stream, inf, f, ioctlName, toString(fnOffset + f.number));
}

void streamIoctlFunction(ostream& stream, const Interface& inf, const Function& f,
	const string& ioctlName, const string& fnName)
{
	stream << f.comment;
	if(f.groupId != "")
		stream << "/** @ingroup " << f.groupId << " */\n";

	string invokeArgs;
	string tempVars;
	int usedArgs = f.args.size();
	stream << "IOCTLDEF " << f.returnType << " " << f.name << "(";
	if(f.args.size() == 0) {
		stream << "void";
	}

	string varDeclarations;
	if(f.returnType == "double") {
		varDeclarations += "\tMA_DV _result;\n";
	}
	else if(f.returnType == "float") {
		varDeclarations += "\tMA_FV _result;\n";
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

		string resolved = resolveType(inf, a.type);

		if(resolved == "double" && a.in) {
			string tvn = "_" + a.name;
			//tempVars += "\tMA_DV " + tvn + ";\n";
			varDeclarations += "\tMA_DV " + tvn + ";\n";
			tempVars += "\t" + tvn + ".d = " + a.name + ";\n";
			invokeArgs += tvn + ".hi, " + tvn + ".lo";
			usedArgs++;
		} else if(resolved == "float" && a.in) {
			string tvn = "_" + a.name;
			//tempVars += "\tMA_FV " + tvn + ";\n";
			varDeclarations += "\tMA_FV " + tvn + ";\n";
			tempVars += "\t" + tvn + ".f = " + a.name + ";\n";
			invokeArgs += tvn + ".i";
		} else {
			invokeArgs += a.name;
		}
	}	//args
	stream << ") {\n";

	stream << varDeclarations;
	stream << tempVars;

	for(size_t j=usedArgs; j<3; j++) {
		invokeArgs += ", 0";
	}
	string invoke = ioctlName + "(" + fnName /*toString(fnOffset + f.number)*/ + invokeArgs + ");";
	if(f.returnType == "double") {
		//stream << "\tMA_DV _result;\n";
		stream << "\t_result.ll = " + invoke + "\n";
		stream << "\treturn _result.d;\n";
	} else if(f.returnType == "float") {
		//stream << "\tMA_FV _result;\n";
		stream << "\t_result.i = (int)" + invoke + "\n";
		stream << "\treturn _result.f;\n";
	} else {
		stream << "\t";

		if(f.returnType != "void") {
			stream << "return ";
		}
		if(f.returnType != "longlong")
			stream << "(" << f.returnType << ") ";

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

	streamCppDefs(stream, inf, ix, headerName);

	stream << "#endif\t//" + headerName + "_DEFS_H\n";
}

void streamCppDefs(ostream& stream, const Interface& inf, int ix, const string& headerName) {
	stream << "#ifndef DONT_WANT_" << headerName << "_TYPEDEFS\n";
	streamTypedefs(stream, inf.typedefs, ix, true);
	stream << "#endif\n";

	streamDefines(stream, inf.defines, ix);
	streamConstants(stream, inf.constSets, ix);
	streamStructs(stream, inf, ix, true);
	streamIoctlDefines(stream, inf, headerName, ix, false);
}

void streamInvokeSyscall(ostream& stream, const Interface& maapi, bool java, int argOffset) {
	for(size_t i=0; i<maapi.functions.size(); i++) {
		const Function& f(maapi.functions[i]);
		streamInvokePrefix(stream, f);
		stream << "{\n"
		"\tLOGSC(\"\\t" << f.name << "(\");\n";
		int ireg = argOffset;
		int stack_ireg = 0;
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

			int sizeOfArgType = 1;
			if(argType == "double" || argType == "long")
				sizeOfArgType = 2;

			stream << "\t" << argType << " " << a.name << " = _SYSCALL_CONVERT_" << convType;
			if(ireg+sizeOfArgType>4) {
				if(java) {
					stream << "(RINT(REG(REG_sp)+" << (stack_ireg<<2) << ")";
					if(argType == "double" || argType == "long") {
						stream << ", RINT(REG(REG_sp)+" << ((stack_ireg+1)<<2) << ")";
					}
					stream << ")";
				}
				else
					stream << "(MEM(" << argType << ", REG(REG_sp)+" << (stack_ireg<<2) << ")";

				stream << ");\n";
				stack_ireg += sizeOfArgType;
				continue;
			}
			else if((argType == "double" || argType == "long") && java) {
				//stream << "(REG_i" << ireg << ");\n";
				stream << "(REG(REG_i" << ireg << "), REG(REG_i" << ireg << "+1));\n";
			} else {
				stream << "(REG(REG_i" << ireg << "));\n";
			}
			ireg += sizeOfArgType;
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

std::string getCSharpType(const Interface& maapi, const std::string& maapiType, bool in) {
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

void outputCSharpSyscallArg(ostream& maapiFile, int i, const std::string& coreObject) {
	if(i < 4) {
		maapiFile << coreObject << "GetRegisterValue(CoreInterpreted.Reg.I" << i << ")";
	} else {
		maapiFile << coreObject << "GetStackValue(" << ((i-4)<<2) << ")";
	}
}

void outputCSharpSyscallNativeArg(ostream& maapiFile, int i, const string& coreObject) {
	if(i < 4) {
		maapiFile << "i" << i;
	} else {
		maapiFile << coreObject << "GetStackValue(" << ((i-4)<<2) << ")";
	}
}

void outputCSharpSyscallNativeArgWP8(ostream& maapiFile, int i, const string& coreObject) {
	if(i < 4) {
		maapiFile << "i" << i;
	} else {
		maapiFile << coreObject << "ExtractArgs( args, " << ((i-4)<<2) << ")";
	}
}

void outputCSharpIoctlArg(ostream& maapiFile, int i, const std::string& coreObject) {
	if(i < 3) {
		maapiFile << (char)('a'+i);
	} else {
		maapiFile << coreObject << "GetStackValue(" << ((i-3)<<2) << ")";
	}
}

void outputCSharpIoctlArgWP8(ostream& maapiFile, int i, const std::string& coreObject) {
	if(i < 3) {
		maapiFile << (char)('a'+i);
	} else {
		maapiFile << coreObject << "ExtractArgs( args, " << ((i-3)<<2) << ")";
	}
}

void outputCSharpSyscallArgTyped(ostream& maapiFile, int& i, const Argument& a,
	const Interface& maapi, const std::string& coreObject)
{
	std::string argType = getCSharpType(maapi, a.type, a.in);
	if(argType == "double") {
		maapiFile << "MoSync.Util.ConvertToDouble(";
		outputCSharpSyscallArg(maapiFile, i, coreObject);
		maapiFile << ", ";
		i++;
		outputCSharpSyscallArg(maapiFile, i, coreObject);
		i++;
		maapiFile << ")";
	}
	else if(argType == "float") {
		maapiFile << "MoSync.Util.ConvertToFloat(";
		outputCSharpSyscallArg(maapiFile, i, coreObject);
		i++;
		maapiFile << ")";
	} else {
		outputCSharpSyscallArg(maapiFile, i, coreObject);
		i++;
	}
}

void outputCSharpIoctlArgTyped(ostream& maapiFile, int& argindex, const Argument& a,
	const Interface& maapi, const std::string& coreObject)
{
	std::string argType = getCSharpType(maapi, a.type, a.in);
	if(argType == "double") {
		maapiFile << "MoSync.Util.ConvertToDouble(";
		outputCSharpIoctlArg(maapiFile, argindex, coreObject);
		maapiFile << ", ";
		argindex++;
		outputCSharpIoctlArg(maapiFile, argindex, coreObject);
		argindex++;
		maapiFile << ")";
	}
	else if(argType == "float") {
		maapiFile << "MoSync.Util.ConvertToFloat(";
		outputCSharpIoctlArg(maapiFile, argindex, coreObject);
		argindex++;
		maapiFile << ")";
	} else {
		outputCSharpIoctlArg(maapiFile, argindex, coreObject);
		argindex++;
	}
}

void outputCSharpIoctlArgTypedWP8(ostream& maapiFile, int& argindex, const Argument& a,
	const Interface& maapi, const std::string& coreObject)
{
	std::string argType = getCSharpType(maapi, a.type, a.in);
	if(argType == "double") {
		maapiFile << "MoSync.Util.ConvertToDouble(";
		outputCSharpIoctlArgWP8(maapiFile, argindex, coreObject);
		maapiFile << ", ";
		argindex++;
		outputCSharpIoctlArgWP8(maapiFile, argindex, coreObject);
		argindex++;
		maapiFile << ")";
	}
	else if(argType == "float") {
		maapiFile << "MoSync.Util.ConvertToFloat(";
		outputCSharpIoctlArgWP8(maapiFile, argindex, coreObject);
		argindex++;
		maapiFile << ")";
	} else {
		outputCSharpIoctlArgWP8(maapiFile, argindex, coreObject);
		argindex++;
	}
}

static const Struct* findStruct(const Interface& inf, const string& name) {
	for(size_t i=0; i<inf.structs.size(); i++) {
		const Struct& s(inf.structs[i]);
		if(s.name == name)
			return &s;
	}
	return NULL;
}

static void streamIndent(ostream& stream, int indent) {
	for(int i=0; i<indent; i++) {
		stream << "\t";
	}
}

static const char* escapeCSharp(const string& name) {
	static const char* cSharpKeywords[] = {
		"abstract_",
		"as_",
		"base_",
		"bool_",
		"break_",
		"byte_",
		"case_",
		"catch_",
		"char_",
		"checked_",
		"class_",
		"const_",
		"continue_",
		"decimal_",
		"default_",
		"delegate_",
		"do_",
		"double_",
		"else_",
		"enum_",
		"event_",
		"explicit_",
		"extern_",
		"false_",
		"finally_",
		"fixed_",
		"float_",
		"for_",
		"foreach_",
		"goto_",
		"if_",
		"implicit_",
		"in_",
		"int_",
		"interface_",
		"internal_",
		"is_",
		"lock_",
		"long_",
		"namespace_",
		"new_",
		"null_",
		"object_",
		"operator_",
		"out_",
		"override_",
		"params_",
		"private_",
		"protected_",
		"public_",
		"readonly_",
		"ref_",
		"return_",
		"sbyte_",
		"sealed_",
		"short_",
		"sizeof_",
		"stackalloc_",
		"static_",
		"string_",
		"struct_",
		"switch_",
		"this_",
		"throw_",
		"true_",
		"try_",
		"typeof_",
		"uint_",
		"ulong_",
		"unchecked_",
		"unsafe_",
		"ushort_",
		"using_",
		"virtual_",
		"void_",
		"volatile_",
		"while_",
	};
	for(size_t i=0; i<(sizeof(cSharpKeywords)/sizeof(char*)); i++) {
		const char* key = cSharpKeywords[i];
		if(strncmp(key, name.c_str(), MAX(strlen(key)-1, name.size())) == 0)
			return key;
	}
	return name.c_str();
}

size_t streamCSharpOffsets(ostream& stream, const Interface& inf,
	const Struct& s, size_t offset, int indent)
{
	size_t structSize = 0;
	for(size_t j=0; j<s.members.size(); j++) {
		const Member& m(s.members[j]);
		size_t max = 0;
		for(size_t k=0; k<m.pod.size(); k++) {
			const PlainOldData& pod(m.pod[k]);
			size_t memberSize;
			const Struct* subStruct = findStruct(inf, pod.type);
			if(isAnonStructName(pod.type)) {
				if(!subStruct)
					throwException("Struct not found: " + pod.type);
				memberSize = streamCSharpOffsets(stream, inf, *subStruct, offset, indent);
				max = MAX(max, memberSize);
				continue;
			}

			memberSize = cTypeSize(inf, pod.type);

			int count;
			string baseName;
			bool array = isArray(inf, pod.name, count, baseName);
			if(array) {
				memberSize *= count;
			} else {
				baseName = pod.name;
			}

			if(subStruct && !array) {
				streamIndent(stream, indent);
				stream << "public class "<<escapeCSharp(baseName)<<" {\n";
				memberSize = streamCSharpOffsets(stream, inf, *subStruct, offset, indent+1);
				streamIndent(stream, indent);
				stream << "}\n";
			} else {
				streamIndent(stream, indent);
				stream << "public const int "<<escapeCSharp(baseName)<< " = " <<offset<< ";\n";
			}
			max = MAX(max, memberSize);
		}
		offset += max;
		structSize += max;
	}
	return structSize;
}

void streamCSharpConstants(ostream& maapiFile, const Interface& maapi)
{
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
}

void streamCSharpFunctionDelegates(ostream& maapiFile, const Interface& maapi)
{
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
}

void streamCSharpFunctionDelegatesWP8(ostream& maapiFile, const Interface& maapi)
{
	for(size_t j=0; j<maapi.functions.size(); j++) {
		const Function& f(maapi.functions[j]);

		//maapiFile << "\t\tcase " << f.number << ":\n";

		maapiFile << "\tpublic MosyncLibrary.Delegate_" << f.name << " " << f.name << " = null;\n";
	}
}

void streamManagedCppDelegatesWP8(ostream& gluecodeFile, const Interface& maapi)
{
	for(size_t j=0; j<maapi.functions.size(); j++) {
		const Function& f(maapi.functions[j]);

		//gluecodeFile << "\t\tcase " << f.number << ":\n";

		std::string returnType;

		if(f.returnType.compare("longlong") == 0)
		{
			returnType.assign("longlong");
		}
		else
			returnType = getCSharpType(maapi, f.returnType, true);


		gluecodeFile << "\tpublic delegate " << returnType << " Delegate_" << f.name << "(";
		for(size_t k=0; k<f.args.size(); k++) {
			const Argument& a(f.args[k]);
			if(k != 0)
				gluecodeFile << ", ";

			std::string argType = getCSharpType(maapi, a.type, a.in);
			gluecodeFile << argType << " _" << a.name;
		}

		if(f.name.compare("maIOCtl") == 0 || f.name.compare("maExtensionFunctionInvoke")  == 0)
		{
			gluecodeFile << ", int _args";
		}

		gluecodeFile  << ");\n";
	}
}

void streamManagedCppDelegateInstancesWP8(ostream& gluecodeFile, const Interface& maapi)
{
	for(size_t j=0; j<maapi.functions.size(); j++) {
		const Function& f(maapi.functions[j]);

		gluecodeFile << "\tstatic Delegate_" << f.name << "^ _" << f.name << " = nullptr;\n";
	}
}

void streamManagedCppDelegatesSettersWP8(ostream& gluecodeFile, const Interface& maapi)
{
	for(size_t j=0; j<maapi.functions.size(); j++) {
		const Function& f(maapi.functions[j]);

		gluecodeFile << "\t\tstatic void Set_" << f.name << "(" << "Delegate_" << f.name << "^ del);\n";
	}
}

void streamManagedCppDelegatesSettersImplWP8(ostream& gluecodeFile, const Interface& maapi)
{
	for(size_t j=0; j<maapi.functions.size(); j++) {
		const Function& f(maapi.functions[j]);

		gluecodeFile << "void WindowsPhoneRuntimeComponent::Set_" << f.name << "(" << "Delegate_" << f.name << "^ del)";
		gluecodeFile << "\n{\n" << "\tMosyncLibrary::_" << f.name << " = del;\n}\n\n";
	}
}

void streamManagedCppSyscallImplWP8(ostream& gluecodeFile, const Interface& maapi)
{
	bool noReturn, maIoctlEllipsis;
	for(size_t j=0; j<maapi.functions.size(); j++) {
		const Function& f(maapi.functions[j]);

		noReturn = false;
		maIoctlEllipsis = false;

		std::string returnType = cType(maapi, f.returnType);

		//gluecodeFile << "\t\tcase " << f.number << ":\n";

		gluecodeFile << returnType << " " << f.name << "(";

		if(returnType.compare("void") == 0)
		{
			noReturn = true;
		}

		for(size_t k=0; k < f.args.size(); k++) {
			const Argument& a(f.args[k]);
			if(k != 0)
				gluecodeFile << ", ";

			//std::string argType = getCSharpType(maapi, a.type, a.in);
			if(a.in && isPointerType(maapi, a.type))
			{
				gluecodeFile << "const ";
			}

			gluecodeFile << cType(maapi, a.type);
			gluecodeFile << " _" << a.name;
		}

		//For maIOCtl and maExtensionFunctionInvoke we have additional arguments
		//If  new syscalls like these ones are added this code needs to be update
		if(f.name.compare("maIOCtl") == 0 || f.name.compare("maExtensionFunctionInvoke") == 0)
		{
			gluecodeFile  << " MA_IOCTL_ELLIPSIS)\n";
			maIoctlEllipsis = true;
		}
		else gluecodeFile  << ")\n";

		//function body
		gluecodeFile << "{\n";

		if(noReturn)
		{
			gluecodeFile << "\tif(nullptr != MosyncLibrary::_" << f.name << ")\n";
			gluecodeFile << "\t{\n";

			if(maIoctlEllipsis)
			{
				gluecodeFile << "\t\tva_list args;\n";
				const Argument& a(f.args[f.args.size() - 1]);
				gluecodeFile << "\t\tva_start(args, _" << a.name << ");\n";
			}

			gluecodeFile << "\t\t MosyncLibrary::_" << f.name << "(";

			for(size_t k = 0; k < f.args.size(); k++) {
			const Argument& a(f.args[k]);
			if(k != 0)
				gluecodeFile << ", ";

				//std::string argType = getCSharpType(maapi, a.type, a.in);
				if(a.type.compare("int") != 0)
					gluecodeFile << "(int)_" << a.name;
				else gluecodeFile << "_" << a.name;
			}

			gluecodeFile << ");\n";

			if(maIoctlEllipsis)
			{
				gluecodeFile << "\t\tva_end(args);\n";
			}
			gluecodeFile << "\t}\n";
		}
		else
		{
			gluecodeFile << "\tif(nullptr != MosyncLibrary::_" << f.name << ")\n";
			gluecodeFile << "\t{\n";

			if(maIoctlEllipsis)
			{
				gluecodeFile << "\t\tva_list args;\n";
				gluecodeFile << "\t\tint returnVal = 0;\n";
				const Argument& a(f.args[f.args.size() - 1]);
				gluecodeFile << "\t\tva_start(args, _" << a.name << ");\n";
			}

			if(returnType.find("*") != std::string::npos)
				gluecodeFile << "\t\treturn (" << returnType << ") MosyncLibrary::_" << f.name << "(";
			else
				gluecodeFile << "\t\treturn MosyncLibrary::_" << f.name << "(";

			for(size_t k = 0; k < f.args.size(); k++)
			{
				const Argument& a(f.args[k]);
				if(k != 0)
					gluecodeFile << ", ";

					//std::string argType = getCSharpType(maapi, a.type, a.in);
					if(cType(maapi, a.type).compare("int") != 0 &&
					   cType(maapi, a.type).compare("float") != 0 &&
					   cType(maapi, a.type).compare("double") != 0)
						gluecodeFile << "(int)_" << a.name;
					else gluecodeFile << "_" << a.name;
			}

			if(maIoctlEllipsis)
			{
				gluecodeFile << ", (int)args);\n";
				gluecodeFile << "\t\tva_end(args);\n\n";
				gluecodeFile << "\t\treturn returnVal;\n";
			}
			else gluecodeFile << ");\n";

			gluecodeFile << "\t}\n";
			if(returnType.find("*") != std::string::npos)
				gluecodeFile << "\telse return NULL;\n";
			else gluecodeFile << "\telse return IOCTL_UNAVAILABLE;\n";
		}

		gluecodeFile << "}\n\n";
	}
}

void streamCSharpMachinePartialCodeWP8(ostream& gluecodeFile, const Interface& maapi)
{
	for(size_t j=0; j<maapi.functions.size(); j++) {
		const Function& f(maapi.functions[j]);

		gluecodeFile << "\t\t\tMosyncLibrary.WindowsPhoneRuntimeComponent.Set_" << f.name << "(";
		gluecodeFile << "mosyncMachine.GetRuntime().GetSyscalls()." << f.name << ");\n";
	}
}

void streamIoctlFunctionInvokeCSharp(ostream& maapiFile, const Function& f, const Interface& maapi, const std::string& object, const std::string& coreObject)
{
	maapiFile << "\t\t\tcase " << f.number << ":\n";

	maapiFile << "\t\t\tif(" << object << f.name << " == null)\n";
	maapiFile << "\t\t\t\tresult = MoSync.Constants.IOCTL_UNAVAILABLE;\n";
	maapiFile << "\t\t\telse\n";
	maapiFile << "\t\t\t\tresult = " << object << f.name << "(";

	int argindex = 0;
	for(size_t k=0; k<f.args.size(); k++) {
		const Argument& a(f.args[k]);
		std::string argType = getCSharpType(maapi, a.type, a.in);
		if(k != 0)
			maapiFile << ", ";
		outputCSharpIoctlArgTyped(maapiFile, argindex, a, maapi, coreObject);
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
			outputCSharpIoctlArgTyped(maapiFile, argindex, a, maapi, coreObject);
			maapiFile << "+\n";
		}
		maapiFile << "\t\t\t\t\"): \"+result+\"\\n\");\n";
		maapiFile << "#endif\n";
	}
	// END LOG

	maapiFile << "\t\t\treturn result;\n";
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

	/* Handle end cases */
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
#if (defined(__x86_64__) || defined(DARWIN))	//in this case, uint32 is different from size_t
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
		AH(a.in ? k : 0);
		AH(a.in ? 0 : k);
		AH(a.type);
		AH(a.name);
		AH(a.range);
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
			AH(c.type);
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

void flushStream(ostream& stream) {
	stream.flush();
	if(!stream.good()) {
		throwException("Stream error!\n");
	}
}
