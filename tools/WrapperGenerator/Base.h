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

class Location;

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
	virtual const Base* resolveFully() const { return this; }
	virtual const Location* getLocation() const { return NULL; }

	void setGroup(const std::string& group);
	const std::string& getGroup() const;
protected:
	std::string mGroup;
	BaseType mBaseType;
};

bool BaseLocationSortPredicate(const Base* d1, const Base* d2);

#endif // _BASE_H_
