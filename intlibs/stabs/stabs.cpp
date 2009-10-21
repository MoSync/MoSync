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
#include <string>
#include <stack>

//todo: cleanup
#define LOGGING_ENABLED
#define CONFIG_H

#include "helpers/helpers.h"

#include "sld.h"

#include "stabs.h"
#include "stabs_parse.h"
#include "stabs_function.h"
#include "stabs_types.h"
#include "stabs_file.h"
#include "stabs_symbols.h"
#include "stabs_static.h"
#include "stabs_typedefs.h"

using namespace std;

static bool parseStabLine(string& line);

static bool parseBIncl(Tuple t, char* text);
static bool parseFun(Tuple t, char* text);
static bool parseSo(Tuple t, char* text);
static bool parseSol(Tuple t, char* text);
static bool parsePSym(Tuple t, char* text);
static bool parseRSym(Tuple t, char* text);
static bool parseSTSym(Tuple t, char* text);
static bool parseGSym(Tuple t, char* text);
static bool parseFile(Tuple t, char* text);
static bool parseThunk(Tuple t, char* text);
static bool parseLbrac(Tuple t);
static bool parseRbrac(Tuple t);
static bool parseEIncl(Tuple t);

static void checkLastFunctionIntegrity();

int gCurrentFile;
Function* gLastFunction = NULL;

static bool sLoaded = false;
static stack<int> sBracStack;

void addFile(int file,const std::string& fileName) {
	addTypeFile(file, fileName);
	addSymbolFile(file, fileName);
}

bool loadStabs(const char* sld, const char* stabs) {
	gCurrentFile = -1;
	TEST(loadSLD(sld));
	ifstream in(stabs);
	string line;
	while(in.good()) {
		getline(in, line);
		if(!in.good())
			break;
		TEST(parseStabLine(line));
	}
	gCurrentFile = -1;
	return sLoaded = in.eof();
}

bool stabsIsLoaded() {
	return sLoaded;
}

void clearStabs() {
	DEBIG_PHAT_ERROR;
}

static bool parseStabLine(string& line) {
	FAILIF(line.length() < 4);
	if(line[0] == '/')	//ignore this line
		return true;

	size_t firstSpace = line.find_first_of(' ');
	FAILIF(firstSpace == line.npos);
	Tuple t;

	if(line[0] == '#') {	//numeric stab. has no string. TODO.
		int n = -1;
		int res = sscanf(line.c_str() + firstSpace, " %i %i%n", &t.a, &t.b, &n);
		FAILIF(res != 2);
		FAILIF(n < 0);

		if(strncmp(line.c_str(), "#N_LBRAC", firstSpace) == 0) {
			return parseLbrac(t);
		} else if(strncmp(line.c_str(), "#N_RBRAC", firstSpace) == 0) {
			return parseRbrac(t);
		} else if(strncmp(line.c_str(), "#N_EINCL", firstSpace) == 0) {
			return parseEIncl(t);
		} else {
			FAIL;
		}
	}
	int n = -1;
	int res = sscanf(line.c_str() + firstSpace, " %i %i %n", &t.a, &t.b, &n);
	FAILIF(res != 2);
	FAILIF(n < 0);
	int textOffset = firstSpace + n;
	char* text = (char*)line.c_str() + textOffset;
	FAILIF(text[0] != '\'');
	FAILIF(line[line.length()-1] != '\'');
	line[line.length()-1] = 0;
	text++;
	if(strncmp(line.c_str(), "N_LSYM", firstSpace) == 0) {
		return parseLSym(t, text);
	} else if(strncmp(line.c_str(), "N_BINCL", firstSpace) == 0) {
		return parseBIncl(t, text);
	} else if(strncmp(line.c_str(), "N_FUN", firstSpace) == 0) {
		return parseFun(t, text);
	} else if(strncmp(line.c_str(), "N_SO", firstSpace) == 0) {
		return parseSo(t, text);
	} else if(strncmp(line.c_str(), "N_SOL", firstSpace) == 0) {
		return parseSol(t, text);
	} else if(strncmp(line.c_str(), "N_PSYM", firstSpace) == 0) {
		return parsePSym(t, text);
	} else if(strncmp(line.c_str(), "N_RSYM", firstSpace) == 0) {
		return parseRSym(t, text);
	} else if(strncmp(line.c_str(), "N_STSYM", firstSpace) == 0) {
		return parseSTSym(t, text);
	} else if(strncmp(line.c_str(), "N_LCSYM", firstSpace) == 0) {
		return parseSTSym(t, text);
	} else if(strncmp(line.c_str(), "N_GSYM", firstSpace) == 0) {
		return parseGSym(t, text);
	} else if(strncmp(line.c_str(), "N_FILE", firstSpace) == 0) {
		return parseFile(t, text);
	} else if(strncmp(line.c_str(), "N_THUNK", firstSpace) == 0) {
		return parseThunk(t, text);
	} else {
		FAIL;
	}
}

static bool parseFile(Tuple t, char* text) {
	t.a++;
	//LOG("\nnew file: %i %s\n", t.a, text);
	gCurrentFile = t.a;

	addFile(gCurrentFile, text);
	return true;
}

static bool parseBIncl(Tuple t, char* text) {
	return true;	//ignored for now
}

static bool parseEIncl(Tuple t) {
	return true;	//ignored for now
}

//grammar for symbols, including functions:
// symbol = stabType ' ' lineNumber ' ' address ' ' string
//the lineNumber is for the C source file in which the symbol is defined.
//the address is in MoSync memory. Code section for functions, data section for variables.
static bool parseFun(Tuple t, char* text) {
	FAILIF(!sBracStack.empty());
	Function* pf = new Function;
	Function& f(*pf);
	f.lineNumber = t.a;
	f.address = t.b;

	char* colon = strchr(text, ':');
	TEST(colon);
	*colon = 0;
	f.name = text;

	char* type = colon + 1;
	switch(*type) {
	case 'F':
		f.fileScope = 0;
		break;
	case 'f':
		f.fileScope = gCurrentFile;
		break;
	default:
		FAIL;
	}
	/*char* end = scanTuple(type + 1, &t);
	TEST(end);
	FAILIF(*end != 0);
	TEST(f.returnType = findTypeByTuple(t));*/
	char* retType = type + 1;
	const TypeBase* tb;
	TEST(tb = subParseType(&retType, Tuple(), string()));
	pf->type = new FunctionType(tb);
	addFunction(pf);
	checkLastFunctionIntegrity();
	gLastFunction = pf;
	return true;
}
static bool parseThunk(Tuple t, char* text) {
	FAILIF(!sBracStack.empty());
	Function* pf = new Function;
	Function& f(*pf);
	f.lineNumber = -t.a;
	f.address = -(gCurrentFile*10000 + t.a);
	f.fileScope = gCurrentFile;
	f.name = "thunk";	//incorrect; we don't get the proper name from pipe-tool.
	f.name += t.a;
	f.name += text;
	f.type = NULL;	//dangerous; might crash
	addFunction(pf);
	checkLastFunctionIntegrity();
	gLastFunction = pf;
	return true;
}

static bool parseSo(Tuple t, char* text) {
	FAILIF(!sBracStack.empty());
	checkLastFunctionIntegrity();
	//this marks the end of a file.
	//todo: resolve delayed types
	return true;	//ignore
}
static bool parseSol(Tuple t, char* text) {
	return true;	//ignore
}

//grammar:
// 'N_PSYM ' lineNumber ' ' offset ' ' text
// text := name ':' paramType typeDeclaration
// paramType := ('p' | 'P')
static bool parsePSym(Tuple t, char* text) {
	Function* f = gLastFunction;
	TEST(f);
	StackVariable* p = new StackVariable;

	char* colon = strchr(text, ':');
	TEST(colon);
	*colon = 0;
	p->name = text;

	char* type = colon + 1;
	switch(*type) {
	case 'p':	//on the stack?
		p->offset = t.b;
		break;
	case 'P':
		//break;
	default:
		FAIL;
	}
	type++;
	TEST(p->dataType = subParseType(&type, Tuple(), p->name));

	f->params.push_back(p);
	return true;
}
static bool parseRSym(Tuple t, char* text) {
	Function* f = gLastFunction;
	if(!f)	//occasionally
		return true;

	char* colon = strchr(text, ':');
	TEST(colon);
	*colon = 0;

	char* type = colon + 1;

	RegisterVariable* p = new RegisterVariable;
	p->name = text;
	char* pType = type + 1;
	TEST(p->dataType = subParseType(&pType, Tuple(), string()));
	p->reg = t.b;

	switch(*type) {
	case 'P':	//register parameter
		f->params.push_back(p);
		break;
	case 'r':	//register local variable
		{
			ScopedVariable scv;
			scv.v = p;
			f->locals.push_back(scv);
		}
		break;
	case 'a':	//register reference parameter
		p->dataType = new ReferenceType(p->dataType);
		f->params.push_back(p);
		break;
	default:
		FAIL;
	}
	return true;
}
static bool parseSTSym(Tuple t, char* text) {
	char* colon = strchr(text, ':');
	TEST(colon);
	*colon = 0;

	char* type = colon + 1;

	char* pType = type + 1;
	const TypeBase* dataType = subParseType(&pType, Tuple(), string());

	switch(*type) {
	case 'S':
		{
			StaticVariable* var = new StaticVariable;
			var->fileScope = gCurrentFile;
			var->name = text;
			var->address = t.b;
			var->dataType = dataType;
			addVariable(var);
		}
		break;
	case 'V':
		{
			StaticLocal* var = new StaticLocal;
			var->name = text;
			var->address = t.b;
			var->dataType = dataType;
			Function* f = gLastFunction;
			TEST(f);
			ScopedVariable scv;
			scv.v = var;
			f->locals.push_back(scv);
		}
		break;
	default:
		FAIL;
	}
	return true;
}
static bool parseGSym(Tuple t, char* text) {
	char* colon = strchr(text, ':');
	TEST(colon);
	*colon = 0;

	char* type = colon + 1;
	DEBUG_ASSERT(*type == 'G');

	char* pType = type + 1;
	const TypeBase* dataType = subParseType(&pType, Tuple(), string());

	StaticVariable* var = new StaticVariable;
	var->name = text;
	var->address = mapVariable(text, gCurrentFile - 1);
	FAILIF(var->address < 0);
	var->dataType = dataType;
	var->fileScope = 0;
	var->lineNumber = t.a;
	addVariable(var);
	return true;
}

static bool parseLbrac(Tuple t) {
	size_t i=gLastFunction->locals.size()-1;
	for(; i<gLastFunction->locals.size(); i--) {
		ScopedVariable& sv(gLastFunction->locals[i]);
		if(sv.start >= 0)
			break;
		sv.start = t.b;
	}
	i++;
	sBracStack.push(i);
	return true;
}

static bool parseRbrac(Tuple t) {
	FAILIF(sBracStack.empty());
	for(size_t i=sBracStack.top(); i<gLastFunction->locals.size(); i++) {
		ScopedVariable& sv(gLastFunction->locals[i]);
		sv.end = t.b;
	}
	sBracStack.pop();
	return true;
}

char* scanTuple(char* str, Tuple* t) {
	int len;
	char c;
	int res = sscanf(str, "(%i,%i%c%n", &t->a, &t->b, &c, &len);
	FAILIF(res != 3);
	FAILIF(c != ')');
	return str + len;
}

bool ScopedVariable::contains(int offset) const {
	DEBUG_ASSERT(offset >= 0);
	DEBUG_ASSERT(start >= 0);
	DEBUG_ASSERT(end >= 0);
	DEBUG_ASSERT(end >= start);
	return offset >= start && offset <= end;
}

static void checkLastFunctionIntegrity() {
	if(gLastFunction == NULL)
		return;
	for(size_t i=0; i<gLastFunction->locals.size(); i++) {
		gLastFunction->locals[i].contains(0);
	}
}
