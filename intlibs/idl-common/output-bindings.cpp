/* Copyright (C) 2011 Mobile Sorcery AB

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

/**
 * File: output-bindings.cpp
 * Author: Mikael Kindborg
 * Summary: Generation of bindings for dynamic languages.
 * Currently generation of Lua bindings in tolua headerfile format.
 * We could also output XML or whatever format that would help
 * dynamic language developers to make bindings to MoSync syscalls.
 *
 * Instructions for Lua:
 *  Output file is in: "tools/idl2/Output/lua_maapi.pkg"
 *  Install tolua: http://www.tecgraf.puc-rio.br/~celes/tolua/
 *  Copy lua_maapi.pkg to the bin folder in the tolua install
 *  Run command: tolua -o lua_maapi.c lua_maapi.pkg
 *  Copy maapi.c to the MobileLua project
 */

#include "output-bindings.h"

#include <fstream>
#include <sstream>
#include <iomanip>

#include <filelist/filelist.h>
#include <helpers/types.h>

#include "idl-common.h"
#include "stringFunctions.h"
#include "types.h"
#include "tokenizer.h"

using namespace std;

// ****************** Lua header file generation starts here ******************

static void lua_streamHeaderFunctions(
	ostream& stream, 
	const Interface& inf);
static void lua_streamIoctlFunction(
	ostream& stream, 
	const Interface& inf, 
	const Function& f,
	const string& ioctlName);
static void lua_streamIoctls(
	ostream& stream, 
	const Interface& inf, 
	int ix);
static void lua_streamMembers(
	ostream& stream, 
	string tab, 
	const vector<Member>& members,
	const vector<Struct>& structs);
/* Not used.
static void lua_streamStructs(
	ostream& stream, 
	const vector<Struct>& structs, 
	int ix);
*/
static void lua_streamTypedefs(
	ostream& stream, 
	const vector<Typedef>& typedefs, 
	int ix);
static void lua_streamConstants(
	ostream& stream, 
	const vector<ConstSet>& constSets, 
	int ix) ;
static void lua_streamHeaderFile(
	ostream& stream, 
	const Interface& inf, 
	int ix);

/**
 * Generate header file that can be parsed by tolua.
 */
void lua_outputHeaderFile(
	const Interface& maapi,
	const vector<string>& ixs,
	const char* destinationFile) 
{
	ofstream luaHeaderFile(destinationFile);

	lua_streamHeaderFile(luaHeaderFile, maapi, MAIN_INTERFACE);

	// Include selected extensions.
	for (size_t i=0; i<ixs.size(); i++) 
	{
		// This is how we include an extension.
		// We want to include NativeUI.
		string s = ixs[i];
		if (s == "IX_WIDGET")
		{
			lua_streamHeaderFile(luaHeaderFile, maapi, i);
		}
	}
}


/**
 * Stream to a header file in a format that tolua understands.
 * http://www.tecgraf.puc-rio.br/~celes/tolua/tolua-3.2.html
 */
static void lua_streamHeaderFile(
	ostream& stream, 
	const Interface& inf, 
	int ix)
{
	lua_streamTypedefs(stream, inf.typedefs, ix);

	// We do not stream macros. They are not understood by tolua.
	//lua_streamDefines(stream, inf.defines, ix);

	lua_streamConstants(stream, inf.constSets, ix);

	// Skip structs.
	//lua_streamStructs(stream, inf.structs, ix);

	if (ix == MAIN_INTERFACE) 
	{
		lua_streamHeaderFunctions(stream, inf);
	}

	lua_streamIoctls(stream, inf, ix);
}

/**
 * Stream function declarations.
 */
static void lua_streamHeaderFunctions(
	ostream& stream, 
	const Interface& inf) 
{
	for (size_t i=0; i<inf.functions.size(); i++) 
	{
		const Function& f(inf.functions[i]);

		// We don't want this function.
		if (f.name == "maIOCtl")
			continue;

		stream << cType(inf, f.returnType);

		stream << " " << f.name << "(";

		if (f.args.size() == 0) 
		{
			stream << "void";
		}

		for (size_t j=0; j<f.args.size(); j++) 
		{
			const Argument& a(f.args[j]);
			if (j != 0)
				stream << ", ";
			if (a.in && isPointerType(inf, a.type)) 
			{
				stream << "const ";
			}
			stream << cType(inf, a.type);
			if(!isPointerType(inf, a.type) && !a.in)
				stream << "*";
			stream << " " << a.name;
		}

		if (f.isIOCtl) 
			stream << " MA_IOCTL_ELLIPSIS";

		stream << ");\n";
	}

	stream << "// End of header functions.\n";
}

static void lua_streamIoctlFunction(
	ostream& stream, 
	const Interface& inf, 
	const Function& f,
	const string& ioctlName)
{
	string tempVars;

	stream << f.returnType << " " << f.name << "(";

	// If no params.
	if (f.args.size() == 0) 
	{
		stream << "void";
	}
	
	// Stream parms.
	for (size_t j=0; j<f.args.size(); j++) 
	{
		const Argument& a(f.args[j]);

		if (j != 0)
		{
			stream << ", ";
		}

		if (a.in && isPointerType(inf, a.type)) 
		{
			stream << "const ";
		}

		// Get type to use.
		string type = cType(inf, a.type);

		// Replace pointer types except char* with void*.
		// We also add a comment with the original type,
		// just for readability.
		bool addTypeComment = false;
		if (isPointerType(inf, a.type))
		{
			if (type != "char*")
			{
				// Replace type with void*.
				type = "void*";
				addTypeComment = true;
			}
		}

		// Stream the type.
		stream << type;

		// Optionally stream the original type comment.
		if (addTypeComment)
		{
			// Insert the original type in a comment.
			stream << " /* " << cType(inf, a.type) << " */";
		}

		// Possibly fix this too.
		if (!isPointerType(inf, a.type) && !a.in)
		{
			stream << "*";
		}

		stream << " " << a.name;

	}	//args
	stream << ");\n";
}

static void lua_streamIoctls(
	ostream& stream, 
	const Interface& inf, 
	int ix) 
{
	for (size_t i=0; i<inf.ioctls.size(); i++) 
	{
		const Ioctl& ioctl(inf.ioctls[i]);
		for (size_t j=0; j<ioctl.functions.size(); j++) 
		{
			const IoctlFunction& f(ioctl.functions[j]);
			if (f.ix == ix)
			{
				lua_streamIoctlFunction(stream, inf, f.f, ioctl.name);
			}
		}
	}

	if (inf.ioctls.size() > 0)
	{
		stream << "// End of IOCtls.\n";
	}
}

static void lua_streamMembers(
	ostream& stream, 
	string tab, 
	const vector<Member>& members,
	const vector<Struct>& structs)
{
	for (size_t k=0; k<members.size(); k++) 
	{
		const Member& m(members[k]);

		// Don't think unions are supported by tolua,
		// but by tolua++ it seems.
		if (m.pod.size() != 1) 
		{
			stream << tab << "union {\n";
			tab += "\t";
		}

		for (size_t l=0; l<m.pod.size(); l++) 
		{
			const PlainOldData pod(m.pod[l]);
			if (isAnonStructName(pod.type)) 
			{
				if (pod.name.empty() || pod.comment.size() > 0)
					throwException("Bad anonymous struct");

				//find struct definition
				bool found = false;
				for (size_t m2=0; m2<structs.size(); m2++) 
				{
					const Struct& as(structs[m2]);
					if (as.name == pod.type) 
					{
						stream << tab << "struct {\n";
						lua_streamMembers(stream, tab + "\t", as.members, structs);
						stream << tab << "};\n";
						found = true;
						break;
					}
				}
				if (!found)
					throwException("Anonymous struct not found");
			} 
			else 
			{
				stream << tab << pod.type << " " << pod.name << ";\n";
			}
		}	//pod

		if (m.pod.size() != 1) 
		{
			stream << "\t};\n";
		}
	}	//member
}

/* Not used.
static void lua_streamStructs(
	ostream& stream, 
	const vector<Struct>& structs, 
	int ix) 
{
	for (size_t j=0; j<structs.size(); j++) 
	{
		const Struct& s(structs[j]);
		if (s.ix == ix)
		{
			stream << "typedef " << s.type << " " << s.name << " {\n";
			lua_streamMembers(stream, "\t", s.members, structs);
			stream << "} " << s.name << ";\n";
			stream << "\n";
		}
	}

	if (structs.size() > 0)
	{
		stream << "// End of structs.\n";
	}
}
*/

static void lua_streamTypedefs(
	ostream& stream, 
	const vector<Typedef>& typedefs, 
	int ix)
{
	if (ix == MAIN_INTERFACE)
	{
		stream <<
			"typedef unsigned short wchar;\n"
			"typedef wchar wchar_t;\n"
			"typedef unsigned short ushort;\n"
			"typedef unsigned int uint;\n"
			"\n"
			;
	}

	for (size_t i=0; i<typedefs.size(); i++) 
	{
		const Typedef& t(typedefs[i]);
		if (t.ix == ix)
		{
			// We don't want long long.
			if (t.type == "long long")
				continue;

			string type = t.type;
			if (t.type == "wchar*")
			{
				type = "void*";
			}
			stream << "typedef " << type << " " << t.name << ";\n";
		}
	}

	if (typedefs.size() > 0)
	{
		stream << "// End of typedefs.\n";
	}
}

/**
 * Works ok to generate constants.
 */
static void lua_streamConstants(
	ostream& stream, 
	const vector<ConstSet>& constSets, 
	int ix) 
{
	for (size_t i=0; i<constSets.size(); i++) 
	{
		const ConstSet& cs(constSets[i]);
		for (size_t j=0; j<cs.constants.size(); j++) 
		{
			const Constant& c(cs.constants[j]);
			if (c.ix == ix)
			{
				stream << "#define " << cs.name << c.name << " " << c.value << "\n";
			}
		}
	}

	if (constSets.size() > 0)
	{
		stream << "//End of defines.\n";
	}
}
