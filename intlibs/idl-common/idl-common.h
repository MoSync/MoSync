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

#ifndef IDL_COMMON_H
#define IDL_COMMON_H

#include <vector>
#include <exception>
#include <string>
#include "helpers/attribute.h"

using namespace std;

struct Group;
struct Statement {
	string comment;
	int groupId;
};

struct Group {
	string groupPrettyName;
	string groupId;
	string comment;
	bool isRoot;
	vector<int> groups;
};

struct Typedef : public Statement {
	string type, name;
	//string comment;
	int ix;
};

struct Define : public Statement {
	string value;
	//string comment;
	int ix;
};

struct Constant : public Statement {
	string type;
	string name, value;
	//string comment;
	int ix;
};

struct ConstSet {
	string name;
	vector<Constant> constants;
};

struct Argument {
	bool in;
	string type;
	string name;
	string range; // if available
};

struct Function : public Statement {
	string returnType;
	string returnTypeRange; // if available

	string name;
	vector<Argument> args;
	//string comment;
	int number;
	bool isIOCtl;
};

struct IoctlFunction {
	Function f;
	int ix;
};

struct Ioctl {
	string name;
	vector<IoctlFunction> functions;
};

struct PlainOldData {
	string type, name;
	string comment;
};

//enum MemberType { PLAIN_OLD_DATA, ANONYMOUS_UNION };

struct Member {
	//MemberType type;
	vector<PlainOldData> pod;	//if size() == 1, then type == POD, else AU.
};

struct Struct : public Statement {
	string type, name;
	vector<Member> members;
	//string comment;
	int ix;	//internal extension
};

struct Interface : public Statement {
	string path;	//used by extensions
	string name;
	vector<Typedef> typedefs;
	vector<Define> defines;
	vector<ConstSet> constSets;
	vector<Function> functions;
	vector<Struct> structs;
	vector<Ioctl> ioctls;	//internal IDL only
	vector<Group> groups;
	//string comment;
};

typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;

#define MAIN_INTERFACE (-1)

//doesn't set Interface.path
Interface parseInterface(const vector<string>& ixs, const string& filename);

void dumpInterfaces(const vector<Interface>& infs);
uint32 calculateChecksum(const vector<Interface>& infs);
uint32 calculateChecksum(const Interface& inf);

void streamHash(ostream& stream, const Interface& inf);

void streamHeaderFile(ostream& stream, const Interface& inf, const vector<string>& ixs, int ix);
void streamCppDefsFile(ostream& stream, const Interface& inf, const vector<string>& ixs, int ix);
void streamJavaDefinitionFile(
	ostream& stream, 
	const string& className,
	const Interface& apiData,
	int ix);

void streamConstants(ostream& stream, const string& interfaceName, const vector<ConstSet>& constSets, int ix);
void streamIoctlDefines(ostream& stream, const Interface& ioctls, const string& headerName, int ix, bool java);
//void streamIoctlFunction(ostream& stream, const Interface& inf, const Function& f,
//	const string& ioctlName);

//implemented by the user
void streamHeaderFunctions(ostream& stream, const Interface& inf, bool syscall);

void ATTRIBUTE(noreturn, throwException(const string& msg));
void runCommand(const string& cmd);
const string& getJdkDir();
const string& getJ2meDir();

bool isAnonStructName(const string& name);

inline const char *validateString(const char *str) {
	if(!str) throwException("No MOSYNCDIR exists");
	return str;
}
#define MOSYNCDIR string(validateString(getenv("MOSYNCDIR")))

#endif	//IDL_COMMON_H
