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

#ifndef IDL_COMMON_H
#define IDL_COMMON_H

#include <vector>
#include <exception>
#include <string>
#include "helpers/attribute.h"

using namespace std;

#ifndef MIN
#define MIN(x, y) ((x)<(y)?(x):(y))
#endif

#ifndef MAX
#define MAX(x, y) ((x)>(y)?(x):(y))
#endif

struct Group;
struct Statement {
	string comment;
	string groupId;
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
	int ix;
};

struct Define : public Statement {
	string value;
	int ix;
};

struct Constant : public Statement {
	string type;
	string name, value;
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
	int ix;	//internal extension
};

struct Interface {
	string comment;
	string path;	//used by extensions
	string name;
	vector<Typedef> typedefs;
	vector<Define> defines;
	vector<ConstSet> constSets;
	vector<Function> functions;
	vector<Struct> structs;
	vector<Ioctl> ioctls;	//internal IDL only
	vector<Group> groups;
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

void streamConstants(ostream& stream, const vector<ConstSet>& constSets, int ix);
void streamIoctlDefines(ostream& stream, const Interface& ioctls, const string& headerName, int ix, bool java);
void streamIoctlFunction(ostream& stream, const Interface& inf, const Function& f,
	const string& ioctlName, int fnOffset = 0);
void streamIoctlFunction(ostream& stream, const Interface& inf, const Function& f,
	const string& ioctlName, const string& fnName);
void streamCppDefs(ostream& stream, const Interface& inf, int ix, const string& headerName);

void streamInvokeSyscall(ostream&, const Interface&, bool java, int argOffset = 0);
void streamHeaderFunctions(ostream& stream, const Interface& inf, bool syscall);

std::string getCSharpType(const Interface& maapi, const std::string& maapiType, bool in);
void outputCSharpSyscallArg(ostream& maapiFile, int i, const string& coreobject);
void outputCSharpSyscallNativeArg(ostream& maapiFile, int i, const string& coreobject);
void outputCSharpIoctlArg(ostream& maapiFile, int i, const string& coreobject);
void outputCSharpSyscallArgTyped(ostream& maapiFile, int& i, const Argument& a, const Interface& maapi, const string& coreobject);
void outputCSharpIoctlArgTyped(ostream& maapiFile, int& argindex, const Argument& a, const Interface& maapi, const string& coreobject);
size_t streamCSharpOffsets(ostream& stream, const Interface& inf, const Struct& s, size_t offset, int indent);
void streamCSharpConstants(ostream& maapiFile, const Interface& maapi);
void streamCSharpFunctionDelegates(ostream& maapiFile, const Interface& maapi);
void outputCSharpIoctlArgTypedWP8(ostream& maapiFile, int& i, const Argument& a, const Interface& maapi, const string& coreObject);
void outputCSharpIoctlArgWP8(ostream& maapiFile, int i, const string& coreobject);
void outputCSharpSyscallNativeArgWP8(ostream& maapiFile, int i, const string& coreObject);
void streamCSharpFunctionDelegatesWP8(ostream& maapiFile, const Interface& maapi);
void streamIoctlFunctionInvokeCSharp(ostream& maapiFile, const Function& f, const Interface& maapi, const std::string& object="mIoctls.",  const std::string& coreObject="mCore.");
void streamManagedCppDelegatesWP8(ostream& gluecodeFile, const Interface& maapi);
void streamManagedCppDelegatesSettersWP8(ostream& gluecodeFile, const Interface& maapi);
void streamManagedCppDelegateInstancesWP8(ostream& gluecodeFile, const Interface& maapi);
void streamManagedCppSyscallImplWP8(ostream& gluecodeFile, const Interface& maapi);
void streamManagedCppDelegatesSettersImplWP8(ostream& gluecodeFile, const Interface& maapi);
void streamCSharpMachinePartialCodeWP8(ostream& gluecodeFile, const Interface& maapi);

//implemented by the user
void streamInvokePrefix(ostream&, const Function&);

void ATTRIBUTE(noreturn, throwException(const string& msg));
void runCommand(const string& cmd);
const string& getJdkDir();
const string& getJ2meDir();
void flushStream(ostream&);

bool isAnonStructName(const string& name);

inline const char *validateString(const char *str) {
	if(!str) throwException("No MOSYNCDIR exists");
	return str;
}
#define MOSYNCDIR string(validateString(getenv("MOSYNCDIR")))

#endif	//IDL_COMMON_H
