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

#include <string>
#include <fstream>
#include <sstream>

#include "idl-common.h"
#include "tokenizer.h"
#include "stringFunctions.h"
#include "types.h"

using namespace std;

#define TASSERT(cond) if(!(cond)) { throwException(#cond); }

static int sNextFuncNum = 1;
static Interface* sInf = NULL;
static int sNextAnonStructNum = 1;

static void processIncludes(ostream& os, istream& is, const string& inPath, int level=0);
static int findIx(const string& token, const vector<string>& ixs);

static ConstSet parseConstSet(const vector<string>& ixs, int currentIx, Group* group);
static Function parseFunction(const string& retType, size_t maxArgs, Group *group);
static Struct parseStruct(const string& type, const string& name, int currentIx, Group *group);
static Member parseAnonymousUnion(int currentIx, Group* group);
static PlainOldData parseAnonStruct(int currentIx, Group* group);
static PlainOldData parsePOD(const string& type);
static Ioctl parseIoctl(const vector<string>& ixs, Interface& inf, Group* group);
static Typedef parseTypedef(int currentIx, Group *group);
static Define parseDefine(int currentIx, Group *group);
static bool parseInterfaceStatement(Interface& inf, const vector<string>& ixs, int& currentIx, Ioctl* ioctl, Group* group=NULL);


#if 1
static int findGroup(Interface& inf, const string& id) {
	for(size_t i = 0; i < inf.groups.size(); i++) {
		if(inf.groups[i].groupId == id) {
			return i;
		}
	}
	return -1;
}

void setGroup(Statement* s, Group* group) {
	if(group) {
		s->groupId = group->groupId;
	} else {
		s->groupId = "";
	}
}

static void parseGroup(Interface& inf, const vector<string>& ixs, int& currentIx, Ioctl* ioctl, Group* parentGroup) {
	Group g;
	string id;			
	readTextToken(id);
				
	Group *group = &g;
	
	int groupIndex = findGroup(inf, id);
	
	if(groupIndex == -1) {
		group->groupId = id;
		group->comment = getComment();
		readQuotedString(group->groupPrettyName);
		//readTextToken(group->groupPrettyName);
	} else {
		group = &inf.groups[groupIndex];
		group->comment += getComment();		
	}


	doExact("{");
	while(parseInterfaceStatement(inf, ixs, currentIx, ioctl, group)) {
	}
	
	if(parentGroup) {
		if(groupIndex == -1) {
			group->isRoot = false;
			inf.groups.push_back(*group);
			groupIndex = inf.groups.size()-1;
		}
		parentGroup->groups.push_back(groupIndex);

	} else {
		if(groupIndex == -1) {
			group->isRoot = true;
			inf.groups.push_back(*group);
		}
	}
}
#endif

bool parseInterfaceStatement(Interface& inf, const vector<string>& ixs, int& currentIx, Ioctl* ioctl, Group* group) {
		string token;
		readToken(token);
		if(token == "constset") {	//constset
			inf.constSets.push_back(parseConstSet(ixs, currentIx, group));
		} else if(token == "struct" || token == "union") {	//struct or union
			string name;
			readTextToken(name);
			inf.structs.push_back(parseStruct(token, name, currentIx, group));
		} else if(token == "#define") {
			inf.defines.push_back(parseDefine(currentIx, group));
		} else if(token == "typedef") {
			inf.typedefs.push_back(parseTypedef(currentIx, group));
#if 1
		} else if(token == "group") {
			parseGroup(inf, ixs, currentIx, ioctl, group);
#endif
		} else if(token == "ioctl") {
			TASSERT(currentIx == MAIN_INTERFACE);
			if(ioctl) {
				tokenError(token);
			} else {
				inf.ioctls.push_back(parseIoctl(ixs, inf, group));
			}
		} else if(isReturnType(inf, token)) {	//function
			if(!ioctl) {
				TASSERT(currentIx == MAIN_INTERFACE);
				inf.functions.push_back(parseFunction(token, 256, group)); // 256 arguments should be enough :)
			} else {
				IoctlFunction f;
				f.f = parseFunction(token, 256, group); // 256 should be enough :)
				f.ix = currentIx;
				ioctl->functions.push_back(f);
			}
		} else if(token == "}") {
			if(group != NULL) return false;
			TASSERT(currentIx == MAIN_INTERFACE);
			if(!ioctl) doEOF();
			return false;
		} else if(token == "#if") {
			TASSERT(currentIx == MAIN_INTERFACE);	
			readTextToken(token);
			currentIx = findIx(token, ixs);
		} else if(token == "#endif") {
			TASSERT(currentIx != MAIN_INTERFACE);
			currentIx = MAIN_INTERFACE;
		} else {
			tokenError(token);
		}
		
	return true;
}

/*
bool parseIoctlStatement(Interface& inf, const vector<string>& ixs, int& currentIx, Ioctl& ioctl) {
		string token;
		readToken(token);
		if(token == "constset") {	//constset
			inf.constSets.push_back(parseConstSet(ixs, currentIx));
		} else if(token == "struct" || token == "union") {	//struct or union
			string name;
			readTextToken(name);
			inf.structs.push_back(parseStruct(token, name, currentIx));
		} else if(token == "#define") {
			inf.defines.push_back(parseDefine(currentIx));
		} else if(token == "typedef") {
			inf.typedefs.push_back(parseTypedef(currentIx));
		} else if(isReturnType(inf, token)) {	//function
			IoctlFunction f;
			f.f = parseFunction(token, 256); // 256 should be enough :)
			f.ix = currentIx;
			ioctl.functions.push_back(f);
		} else if(token == "}") {
			TASSERT(currentIx == MAIN_INTERFACE);
			return false;
		} else if(token == "#if") {
			TASSERT(currentIx == MAIN_INTERFACE);
			readTextToken(token);
			currentIx = findIx(token, ixs);
		} else if(token == "#endif") {
			TASSERT(currentIx != MAIN_INTERFACE);
			currentIx = MAIN_INTERFACE;
		} else {
			tokenError(token);
		}
		
	return true;
}
*/

Interface parseInterface(const vector<string>& ixs, const string& path) {
	ifstream stream(path.c_str());

	if(!stream.good())
		throwException("Couldn't open IDL file \"" + path + "\"");

	stringstream preprocessed;
	processIncludes(preprocessed, stream, path);
	preprocessed.seekg(0);

	ofstream pFile(("Output/" + getFilenameFromPath(path) + ".p").c_str());
	pFile << preprocessed.str();

	setTokenStream(&preprocessed);

	Interface inf;
	sInf = &inf;
	int currentIx = MAIN_INTERFACE;
	try {
		doExact("interface");
		readTextToken(inf.name);
		doExact("{");
		inf.comment = getComment();
		while(parseInterfaceStatement(inf, ixs, currentIx, NULL)) {
		}
		
		//check for dupe constsets
		for(size_t i=0; (i+1)<inf.constSets.size(); i++) {
			const ConstSet& cs1(inf.constSets[i]);
			for(size_t j=i+1; j<inf.constSets.size(); j++) {
				const ConstSet& cs2(inf.constSets[j]);
				if(cs1.name == cs2.name)
					throwException("Dupe constset: " + cs1.name);
			}
		}

	} catch(exception& e) {
		printf("error on line %i: %s\n", getCurrentLine(), e.what());
		throw e;
	}
	sInf = NULL;
	return inf;
}

static int findIx(const string& token, const vector<string>& ixs) {
	for(size_t i=0; i<ixs.size(); i++) {
		if(token == ixs[i])
			return i;
	}
	throwException(token + " not found!");
}

static void processIncludes(ostream& os, istream& is, const string& inPath, int level)
{
	char buf[1024];
	int line = 0;
	try {
		while(1) {
			is.getline(buf, sizeof(buf));
			line++;
			if(is.eof())
				return;
			if(!is.good()) {
				printf("Error reading file in ");
				throwException("Error reading file");
			}
			char incPath[1024];
			if(sscanf(buf, "#include \"%[^\"]", incPath) == 1) {
				string dir = getDirFromPath(inPath);
				string newPath = dir + "/" + incPath;
				ifstream incStream(newPath.c_str());
				if(!incStream.good()) {
					printf("Error opening file \"%s\" in\n", incPath);
					throwException("Error opening file");
				}
				processIncludes(os, incStream, incPath, level + 1);
			} else {
				os << buf << '\n';
			}
		}
	} catch(exception& e) {
		printf("%s on line %i", inPath.c_str(), line);
		if(level != 0) {
			printf(", included from");
		}
		printf("\n");
		level--;
		throw e;
	}
}

static Ioctl parseIoctl(const vector<string>& ixs, Interface& inf, Group* group)
{

	int currentIx = MAIN_INTERFACE;
	Ioctl ioctl;
	readTextToken(ioctl.name);
	doExact("{");

	{	//add ioctl function to interface
		Function f;
		f.name = ioctl.name;
		f.returnType = "longlong";
		f.comment = getComment();
		f.number = sNextFuncNum++;
		Argument a;
		a.in = true;
		a.type = "int";
		a.name = "function";
		f.args.push_back(a);
		a.name = "a";
		f.args.push_back(a);
		a.name = "b";
		f.args.push_back(a);
		a.name = "c";
		f.args.push_back(a);
		setGroup(&f, group);
		f.isIOCtl = true;

		inf.functions.push_back(f);
	}

	const int mainNextFuncNum = sNextFuncNum;	//backup
	sNextFuncNum = 1;

	while(parseInterfaceStatement(inf, ixs, currentIx, &ioctl, group)) {
	}
		
	sNextFuncNum = mainNextFuncNum;	//restore
	return ioctl;
}

static Struct parseStruct(const string& type, const string& name, int currentIx, Group *group) {
	Struct s;
	s.type = type;
	s.ix = currentIx;
	s.name = name;
	s.comment = getComment();

	setGroup(&s, group);

	doExact("{");
	while(true) {
		string token;
		readToken(token);
		if(token == "}")
			break;
		Member m;
		if(token == "union") {
			if(type == "union")
				throwException("unions may not be nested");
			m = parseAnonymousUnion(currentIx, group);
		} else {
			m.pod.push_back(parsePOD(token));
		}
		s.members.push_back(m);
	}
	return s;
}

static PlainOldData parsePOD(const string& type) {
	PlainOldData pod;
	pod.type = type;
	readTextToken(pod.name);
	doExact(";");
	pod.comment = getComment();
	return pod;
}

static Member parseAnonymousUnion(int currentIx, Group* group) {
	Member m;
	doExact("{");
	while(true) {
		string token;
		readToken(token);
		if(token == "}")
			break;
		if(token == "struct") {
			m.pod.push_back(parseAnonStruct(currentIx, group));
		} else {
			m.pod.push_back(parsePOD(token));
		}
	}
	return m;
}

static PlainOldData parseAnonStruct(int currentIx, Group *group) {
	char buf[32];
	sprintf(buf, "_AS%05i", sNextAnonStructNum);
	sNextAnonStructNum++;
	Struct s = parseStruct("struct", buf, currentIx, group);
	sInf->structs.push_back(s);
	PlainOldData pod;
	pod.type = s.name;
	readToken(pod.name);
	doExact(";");
	return pod;
}

static ConstSet parseConstSet(const vector<string>& ixs, int currentIx, Group *group) {
	ConstSet cs;
	
	//doExact("int");
	string constSetType;
	readToken(constSetType);
	if(constSetType != "int" && constSetType != "MAString") 
		tokenError(constSetType);
		
	readTextToken(cs.name);

	doExact("{");
	while(true) {
		string token;
		readToken(token);
		if(token == "}") {
			break;
		} else if(token == "#if") {
			TASSERT(currentIx == MAIN_INTERFACE);
			readTextToken(token);
			currentIx = findIx(token, ixs);
			continue;
		} else if(token == "#endif") {
			TASSERT(currentIx != MAIN_INTERFACE);
			currentIx = MAIN_INTERFACE;
			continue;
		}
		Constant c;
		c.ix = currentIx;
		c.name = token;
		c.type = constSetType;
		
		setGroup(&c, group);

		doExact("=");
		readTextToken(c.value);
		doExact(";");
		c.comment = getComment();
		cs.constants.push_back(c);
	}
	return cs;
}

static void readRange(string &range) {
	doExact("(");
	readQuotedString(range);
	doExact(")");
}

static Function parseFunction(const string& retType, size_t maxArgs, Group *group) {
	Function f;
	f.returnType = retType;
	f.number = sNextFuncNum++;
	f.isIOCtl = false;

	setGroup(&f, group);

	readTextToken(f.name);
	doExact("(");
	string token;
	readToken(token);
	while(true) {
		Argument a;
		if(token == ")") {
			break;
		}
		if(f.args.size() != 0) {
			if(token != ",")
				tokenError(token);
			readTextToken(token);
		}
		if(token == "in") {
			a.in = true;
		} else if(token == "out") {
			a.in = false;
		} else {
			tokenError(token);
		}
		readTextToken(a.type);
		readTextToken(a.name);
		readToken(token);
		if(token == "range") {
			readRange(a.range);
			readToken(token);
		}
		if(!a.in && a.type == "MAString" && a.range.empty()) {
			Error("logic", "Argument of type \"out MAString\" lacks range");
		}
		f.args.push_back(a);
	}
	
	readToken(token);
	if(token == "range") {
		readRange(f.returnTypeRange);
		readToken(token);
	}
	
	if(token != ";") {
		tokenError(token);
	}

	f.comment = getComment();
	if(f.args.size() > maxArgs) {
		Error("function", "Too many arguments");
	}
	return f;
}

static Typedef parseTypedef(int currentIx, Group *group) {
	Typedef t;
	t.ix = currentIx;
	t.comment = getComment();

	setGroup(&t, group);

	string last;
	while(true) {
		string token;
		readToken(token);
		if(token == ";")
			break;
		if(last.size() != 0) {
			if(t.type.size() != 0)
				t.type += " ";
			t.type += last;
		}
		last = token;
	}
	t.name = last;
	if(t.type.size() == 0 || t.name.size() == 0)
		throwException("Syntax error");
	return t;
}

static Define parseDefine(int currentIx, Group *group) {
	Define d;
	d.ix = currentIx;
	d.comment = getComment();

	setGroup(&d, group);

	readLine(d.value);
	return d;
}
