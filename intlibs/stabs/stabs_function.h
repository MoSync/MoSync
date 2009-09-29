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

#ifndef STABS_FUNCTION_H
#define STABS_FUNCTION_H

#include <string>
#include <vector>
#include <set>

#include "stabs_symbol.h"
#include "stabs_type.h"

enum StorageClass {
	eStack, eRegister, eStatic
};

struct LocalVariable {
	LocalVariable(StorageClass sc) : storageClass(sc) {}
	const StorageClass storageClass;
	const TypeBase* dataType;
	std::string name;
};

struct StackVariable : LocalVariable {
	StackVariable() : LocalVariable(eStack) {}
	int offset;
};

struct RegisterVariable : LocalVariable {
	RegisterVariable() : LocalVariable(eRegister) {}
	int reg;
};

struct StaticLocal : LocalVariable {
	StaticLocal() : LocalVariable(eStatic) {}
	int address;
};

struct ScopedVariable {
	LocalVariable* v;
	int start, end;

	ScopedVariable() : start(-1), end(-1) {}
	bool contains(int offset) const;
};

struct Function : Symbol {
	Function() : Symbol(eFunction), type(NULL) {}
	const TypeBase* type;
	std::vector<LocalVariable*> params;
	std::vector<ScopedVariable> locals;
};

#endif	//STABS_FUNCTION_H
