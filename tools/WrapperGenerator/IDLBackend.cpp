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
#include "Struct.h"
#include <algorithm>

using namespace std;

// a bit messy this function...
string IDLBackend::getIDLType(const Base* base, const Argument* argument, bool usePointer) {
	string ret = "";
	if(base->getBaseType() == Base::EFundamentalType) {
		const FundamentalType* ft = (const FundamentalType*) base;
		string name = ft->getName();
		if(argument)
			ret += "in ";
		ret += name;
	} else if(base->getBaseType() == Base::ECvQualifiedType) {
		const CvQualifiedType* qt = (const CvQualifiedType*) base;
		ret += getIDLType(qt->getType());
	} else if(base->getBaseType() == Base::EPointerType) {
		const PointerType* pt = (const PointerType*) base;
		bool isConst = false;
		if(argument) {
			if(pt->isConst()) {
				isConst = true;
				ret += "in ";
				if(argument->usesHandle() && !usePointer) {
					ret+= "MAHandle";
					return ret;
				}
			} else {
				if(argument->usesHandle() && !usePointer) {
					ret += "in MAHandle";
					return ret;
				} else {
					ret += "out ";
				}

			}
		}

		const Base* pType = pt->getType();

		const Base* resolvedType = pType->resolveFully();
		if(resolvedType->getBaseType() == Base::EFundamentalType) {
			const FundamentalType *pfType = (const FundamentalType*) resolvedType;
			if(pfType->getName() == "void") {
				ret += "MAAddress";
				return ret;
			}
		}

		if(pType->getBaseType() == Base::EFundamentalType) {
			const FundamentalType* pfType = (const FundamentalType*)pType;
			/*
			if(pfType->getName() == "char")
				ret += "MAString";
			else //if(pfType->getName() == "void")
				ret += "MAAddress";
			*/
			ret += pfType->getName();
			if(isConst) ret+="*";

		} else if(pType->getBaseType() == Base::ETypedef) {
			ret += ((const Typedef*)pType)->getName();
			//ret += "MAAddress";
			if(isConst) ret+="*";
		}  else if(pType->getBaseType() == Base::EStruct) {
			ret += ((const Struct*)pType)->getName();
			//ret += "MAAddress";
			if(isConst) ret+="*";
		} else if(pType->getBaseType() == Base::EFunctionType) {
			ret += "MAAddress";
		} else if(pType->getBaseType() == Base::EPointerType) {
			ret += "MAAddress";
		} else {
			System::error("Unsupported type! %s\n", pType->toString().c_str());
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
		ret += (argument?"in ":"") + td->getName();
	} else if(base->getBaseType() == Base::EStruct) {
		const Struct* s = (const Struct*) base;
		ret += (argument?"in struct":"struct ") + s->getName();
	} else {
		System::error("don't know how to handle type\n");
	}

	return ret;
}

void IDLBackend::emit(const BasesMap& bases, fstream& stream) {
	pair<BasesIterator, BasesIterator> typedefs = bases.equal_range("Typedef");

	std::string group = "initial";

	std::vector<const Base*> sortedTypeDefs;
	for(BasesIterator td = typedefs.first; td!=typedefs.second; td++) {
		const Typedef* t = (const Typedef*)td->second;
		//stream << "typedef " << getIDLType(t->getType(), NULL) << " " << t->getName() << ";\n";
		sortedTypeDefs.push_back(t);
	}

	std::sort(sortedTypeDefs.begin(), sortedTypeDefs.end(), BaseLocationSortPredicate);

	//for(BasesIterator td = typedefs.first; td!=typedefs.second; td++) {
	for(std::vector<const Base*>::const_iterator td = sortedTypeDefs.begin(); td!=sortedTypeDefs.end(); td++) {
		const Typedef* t = (const Typedef*)*td;

		if(t->getGroup() != group) {
			if(group!="initial") {
				stream << "#endif\n";
			}
			group = t->getGroup();
			stream << "#if IX_" << System::toUpperCase(group) << "\n";
		}
		stream << "typedef " << getIDLType(t->getType(), NULL) << " " << t->getName() << ";\n";
	}

	if(group!="initial") {
		stream << "#endif\n";
	}


	pair<BasesIterator, BasesIterator> functions = bases.equal_range("Function");

	std::vector<const Base*> sortedFunctions;
	for(BasesIterator td = functions.first; td!=functions.second; td++) {
		const Function* t = (const Function*)td->second;
		//stream << "typedef " << getIDLType(t->getType(), NULL) << " " << t->getName() << ";\n";
		sortedFunctions.push_back(t);
	}

	std::sort(sortedFunctions.begin(), sortedFunctions.end(), BaseLocationSortPredicate);

	group = "initial";
	//for(BasesIterator function = functions.first; function!=functions.second; function++) {
	for(std::vector<const Base*>::const_iterator function = sortedFunctions.begin(); function!=sortedFunctions.end(); function++) {
		const Function* func = (const Function*)*function;
		string name = func->getName();

		/**
		 * Never versions of gcc includes some builtin functions,
		 * ignore these.
		 */
		 if(name.find_first_of("__builtin") == 0)
		{
			continue;
		}

		if(func->getGroup() != group) {
			if(group!="initial") {
				stream << "#endif\n";
			}
			group = func->getGroup();
			stream << "#if IX_" << System::toUpperCase(group) << "\n";
		}

		const Base* ret = func->getReturnType();
		string returnString = ret->toString();
		bool returnsHandle = false;
		if(ret->getBaseType() == Base::EPointerType) {
#if 0
			const PointerType* pret = (const PointerType*)ret;
			const Base* target = pret->getType();
			if(target->getBaseType() == Base::ETypedef)
				target = ((const Typedef*)
#endif
			returnString = "MAHandle";
			returnsHandle = true;
		}

		const std::vector<const Argument*>& args = func->getArguments();

		stream << returnString << " " << name;
		if(func->hasHandleArguments() || returnsHandle) {
			stream << "Handle";
		}
		stream << "(";
		for(size_t i = 0; i < args.size(); i++) {
			if(args[i]->isEllipsis()) stream << "...";
			else {
				stream << getIDLType(args[i]->getType(), args[i], true);
				stream << (args[i]->getName()!=""?" ":"") << args[i]->getName();
			}
			if(i != args.size()-1) stream << ", ";
		}
		stream << ");\n";

	}

	if(group!="initial") {
		stream << "#endif\n";
	}
}
