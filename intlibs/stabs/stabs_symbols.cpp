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

#include <set>

//todo: cleanup
#define LOGGING_ENABLED
#define CONFIG_H

#include "helpers/helpers.h"

#include "stabs.h"
#include "stabs_symbols.h"
#include "stabs_file.h"

using namespace std;

struct symbol_p_address_less
	: public binary_function<Symbol*, Symbol*, bool>
{	// functor for operator<
	bool operator()(const Symbol*const& _Left, const Symbol*const& _Right) const
	{	// apply operator< to operands
		return (_Left->address < _Right->address);
	}
};

struct symbol_p_scope_name_less
	: public binary_function<Symbol*, Symbol*, bool>
{	// functor for operator<
	bool operator()(const Symbol*const& _Left, const Symbol*const& _Right) const
	{	// apply operator< to operands
		if(_Left->fileScope != _Right->fileScope)
			return _Left->fileScope < _Right->fileScope;
		else
			return _Left->name < _Right->name;
	}
};

struct symbol_p_name_less
	: public binary_function<Symbol*, Symbol*, bool>
{	// functor for operator<
	bool operator()(const Symbol*const& _Left, const Symbol*const& _Right) const
	{	// apply operator< to operands
		return _Left->name < _Right->name;
	}
};

typedef set<Symbol*, symbol_p_address_less> SymbolSet;
typedef set<Symbol*, symbol_p_scope_name_less> SymbolScopeSet;
typedef set<Symbol*, symbol_p_name_less> SymbolNameSet;

static SymbolSet sVariableSet, sFunctionSet;
static SymbolScopeSet sSymbolScopeSet;
static SymbolNameSet sSymbolGlobalSet;

void addSymbolFile(int file, const std::string& fileName) {
}

static void addSymbol(SymbolSet& ss, Symbol* s) {
	//LOG("addSymbol(%i,%s) 0x%x\n", s->fileScope, s->name.c_str(), s->address);
	DEBUG_ASSERT(s->fileScope > 0);
	pair<SymbolSet::iterator, bool> res = ss.insert(s);
	if(!res.second) {
		//if the existing symbol isn't identical to the new, then error.
		const Symbol* f = *res.first;
		if(s->type != f->type ||
			s->address != f->address ||
			s->lineNumber != f->lineNumber ||
			s->fileScope != f->fileScope ||
			s->name != f->name)
		{
			DEBIG_PHAT_ERROR;
		}
		return;
	}
	pair<SymbolScopeSet::iterator, bool> sres = sSymbolScopeSet.insert(s);
	DEBUG_ASSERT(sres.second);
	if(s->global) {
		pair<SymbolNameSet::iterator, bool> nres = sSymbolGlobalSet.insert(s);
		DEBUG_ASSERT(nres.second);
	}
}

void addFunction(Symbol* s) {
	DEBUG_ASSERT(s->type == eFunction);
	addSymbol(sFunctionSet, s);
}

void addVariable(Symbol* s) {
	DEBUG_ASSERT(s->type == eVariable);
	addSymbol(sVariableSet, s);
}

const Symbol* stabsGetSymbolByScopeAndName(int scope, const std::string& name) {
	Symbol s(eNone);
	s.fileScope = scope;
	s.name = name;
	SymbolScopeSet::const_iterator itr = sSymbolScopeSet.find(&s);
	if(itr == sSymbolScopeSet.end())
		return NULL;
	return *itr;
}

static const Symbol* getSymbolByAddress(SymbolSet& ss, int address) {
	Symbol s(eNone);
	s.address = address;
	SymbolSet::const_iterator itr = ss.find(&s);
	if(itr == ss.end())
		return NULL;
	return *itr;
}

struct Function;

const Function* stabsGetFunctionByAddress(int address) {
	const Symbol* s = getSymbolByAddress(sFunctionSet, address);
	if(!s)
		return NULL;
	DEBUG_ASSERT(s->type == eFunction);
	return (Function*)s;
}

struct StaticVariable;

const StaticVariable* stabsGetVariableByAddress(int address) {
	const Symbol* s = getSymbolByAddress(sVariableSet, address);
	if(!s)
		return NULL;
	DEBUG_ASSERT(s->type == eVariable);
	return (StaticVariable*)s;
}

const Symbol* stabsGetSymbolGlobal(const std::string& name) {
	Symbol s(eNone);
	s.name = name;
	SymbolNameSet::const_iterator itr = sSymbolGlobalSet.find(&s);
	if(itr == sSymbolGlobalSet.end())
		return NULL;
	return *itr;
}
