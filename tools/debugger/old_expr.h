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

#ifndef OLD_EXPR_H
#define OLD_EXPR_H

#include <string>
#include "stabs/stabs_symbol.h"
#include "stabs/stabs_type.h"
#include "stabs/stabs_function.h"

struct SYM {
	SymbolType symType;
	const TypeBase* type;

	const void* address;
	StorageClass storageClass; // only valid if adress exist.
};

typedef void (*EECallback)(const char* type, const char* value, bool simpleType);
typedef void (*SeeCallback)(const SYM& sym);

//Attempts to evaluate \a expr in a given frame.
//\a frameAddr may be <0, in which case the Current Frame is used.
//Fails if there is no current frame.
//Calls the callback on success.
//Calls error() on failure.
/*
void stackEvaluateExpression(const std::string& expr, int frameAddr,
	EECallback, bool complex);
*/

void locate_symbol(const std::string& name, SeeCallback cb);

bool isLocalGlobalOrStatic(const std::string& name);


#endif
