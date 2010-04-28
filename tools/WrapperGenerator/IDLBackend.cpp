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

#include "IDLBackend.h"

using namespace std;

// a bit messy this function...
string IDLBackend::getIDLType(const Base* base, bool isArgument) {
	string ret = "";
	if(base->getBaseType() == Base::EFundamentalType) {
		const FundamentalType* ft = (const FundamentalType*) base;
		string name = ft->getName();
		if(isArgument)
			ret += "in ";
		ret += name;
	} else if(base->getBaseType() == Base::ECvQualifiedType) {
		const CvQualifiedType* qt = (const CvQualifiedType*) base;
		ret += getIDLType(qt->getType());
	} else if(base->getBaseType() == Base::EPointerType) {
		const PointerType* pt = (const PointerType*) base;
		if(isArgument) {
			if(pt->isConst()) {
				ret += "in ";
			} else {
				//ret += "out ";
				ret += "in MAHandle"; 
				return ret;
			}
		}

		const Base* pType = pt->getType();
		if(pType->getBaseType() == Base::EFundamentalType) {
			const FundamentalType* pfType = (const FundamentalType*)pType;
			if(pfType->getName() == "char")
				ret += "MAString";
			else //if(pfType->getName() == "void") 
				ret += "MAAddress";
			/*
			else
				ret += getIDLType(pt->getType(), false) + "*";
			*/
		} else {
			ret += "MAAddress";
		}
		/*
		else if(pType->getBaseType() == Base::EFunctionType){
			ret += "int"; // function address. We should probably be able to handle them...
		} else {
			ret += getIDLType(pt->getType(), false) + "*";
		}
		*/

	} else if(base->getBaseType() == Base::ETypedef) {
		const Typedef* td = (const Typedef*) base;
		ret += (isArgument?"in ":"") + td->getName();
	} else if(base->getBaseType() == Base::EStruct) {
		const Struct* s = (const Struct*) base;
		ret += (isArgument?"in struct":"struct ") + s->getName();
	} else {
		System::error("don't know how to handle type\n");
	}

	return ret;
}

void IDLBackend::emit(const BasesMap& bases, fstream& stream) {
	pair<BasesIterator, BasesIterator> typedefs = bases.equal_range("Typedef");
	for(BasesIterator td = typedefs.first; td!=typedefs.second; td++) {
		const Typedef* t = (const Typedef*)td->second;
		stream << "typedef " << getIDLType(t->getType(), false) << " " << t->getName() << ";\n";

	}

	pair<BasesIterator, BasesIterator> functions = bases.equal_range("Function");

	for(BasesIterator function = functions.first; function!=functions.second; function++) {
		const Function* func = (const Function*)function->second;
		string name = func->getName();
		const Base* ret = func->getReturnType();
		string returnString = ret->toString();
		if(ret->getBaseType() == Base::EPointerType) {
			const PointerType* pret = (const PointerType*)ret;
			
			/*
			const Base* target = pret->getType();
			if(target->getBaseType() == Base::ETypedef)
				target = ((const Typedef*)
			*/
			returnString = "MAHandle";
		}

		stream << returnString << " " << name << "(";
		const std::vector<const Argument*>& args = func->getArguments();
		for(int i = 0; i < args.size(); i++) {
			if(args[i]->isEllipsis()) stream << "...";
			else {
				stream << getIDLType(args[i]->getType());
				stream << (args[i]->getName()!=""?" ":"") << args[i]->getName();
			}
			if(i != args.size()-1) stream << ", ";
		}

		stream << ");\n";
	}

}	