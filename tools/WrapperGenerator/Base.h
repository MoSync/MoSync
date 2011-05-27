/* Copyright (C) 2010 Mobile Sorcery AB

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

#ifndef _BASE_H_
#define _BASE_H_

#include "WrapperGenerator.h"
#include "Parser.h"
#include <string>

#define WRAPPER_TYPES(m)\
	m(Namespace, "Namespace")\
	m(Function, "Function")\
	m(PointerType, "PointerType")\
	m(ReferenceType, "ReferenceType")\
	m(Typedef, "Typedef")\
	m(CvQualifiedType, "CvQualifiedType")\
	m(ArrayType, "ArrayType")\
	m(FundamentalType, "FundamentalType")\
	m(FunctionType, "FunctionType")\
	m(File, "File")\
	m(Struct, "Struct")\
	m(Field, "Field")\

class Base {
public:

#define DECLARE_ENUM(typeName, typeStr) E##typeName,

	enum BaseType {
		WRAPPER_TYPES(DECLARE_ENUM)
	};

	Base(BaseType type);
	virtual std::string toString() const;
	virtual void fromParseNode(const ParseNode& node) = 0;
	BaseType getBaseType() const;
	virtual const Base* resolveFully() const { return this; };
	
protected:
	BaseType mBaseType;
};

#endif // _BASE_H_