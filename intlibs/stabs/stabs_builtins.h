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

#ifndef STABS_BUILTINS_H
#define STABS_BUILTINS_H

#include "stabs_types.h"

struct BUILTINdecl {
	const char* name;
	TypeBase* type;
};

extern BUILTINdecl sBuiltins[];
extern const int snBuiltins;

#include "stabs_builtins_generated.h"

#define ENUM_BUILTIN_FIRST(name, id) e##id
#define ENUM_BUILTIN(name, id) ,e##id

class Builtin : public TypeBase {
public:
	enum SubType {
		BUILTINS(ENUM_BUILTIN_FIRST, ENUM_BUILTIN)
		BROKEN_BUILTINS(ENUM_BUILTIN),
		eBool, eVTablePtr,
		NUM_SUB_TYPES
	};

	Builtin(const char* name, int s, SubType st) :
		TypeBase(s, true, eBuiltin), mName(name), mSubType(st) {}
	const TypeBase* resolve() const { return this; }
	void printTypeMI(printfPtr p, bool complex) const { p("%s", mName); }
public:
	const char* const mName;
	const SubType mSubType;
};

#endif	//STABS_BUILTINS_H
