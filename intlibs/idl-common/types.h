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

#ifndef TYPES_H
#define TYPES_H

#include "idl-common.h"

bool isReturnType(const Interface& inf, const string& token);
string cType(const Interface& inf, const string& type);
string directCType(const Interface& inf, const string& type);
const string& resolveType(const Interface& inf, const string& ctype);

bool isPointerType(const Interface& inf, const string& type);
bool isDirectPointerType(const Interface& inf, const string& type);

string jType(const Interface& inf, const string& type);
string getSJType(const Interface& inf, const string& type, bool argIn);	//Static Java

#endif	//TYPES_H
