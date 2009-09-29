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

typedef set<Symbol*, symbol_p_address_less> SymbolSet;
typedef set<Symbol*, symbol_p_scope_name_less> SymbolScopeSet;

static SymbolSet sVariableSet, sFunctionSet;
static SymbolScopeSet sSymbolScopeSet;

void addSymbolFile(int file) {
}

static void addSymbol(SymbolSet& ss, Symbol* s) {
	//LOG("addSymbol(%i,%s) 0x%x\n", s->fileScope, s->name.c_str(), s->address);
	DEBUG_ASSERT(s->fileScope >= 0);
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
