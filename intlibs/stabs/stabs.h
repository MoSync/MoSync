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

#ifndef STABS_H
#define STABS_H

#include "stabs_function.h"
#include "stabs_static.h"

bool loadStabs(const char* sld, const char* stabs);
bool stabsIsLoaded();

void stabsEnumerateSymbols(void (*callback)(const Symbol*));
const Type* stabsFindTypeByName(const std::string& name, int scope);

//note: separate address spaces.
const Function* stabsGetFunctionByAddress(int address);
const StaticVariable* stabsGetVariableByAddress(int address);

const Symbol* stabsGetSymbolByScopeAndName(int scope, const std::string& name);
const Symbol* stabsGetSymbolGlobal(const std::string& name);

#ifndef WIN32
inline int iscsym(int c) { return (isalnum(c) || ((c) == '_')); }
#endif

#endif	//STABS_H
