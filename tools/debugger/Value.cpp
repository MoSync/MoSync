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

#include "Value.h"
#include "operations_generated.cpp"
#include "stabs/stabs_typedefs.h"

using namespace std;

#define V_CONSTRUCTOR_ELEM_SOURCE(theType, id)\
	Value::Value(theType t) : id(t), mPrimitiveType(Builtin::e##id) { \
		mSym.symType = eVariable; \
		mSym.type = getTypeBaseFromType( Builtin::e##id); \
		mSym.address = NULL;\
	}

#define V_OPERATOR_CAST_ELEM_SOURCE(toType, id)\
	Value::operator toType () const { \
	if(getType() != TypeBase::eBuiltin && getType() != TypeBase::ePointer && getType() != TypeBase::eArray && getType() != TypeBase::eFunction) throw ParseException("Conversion error");\
		return convertToType<toType>();\
	}

TYPES(V_CONSTRUCTOR_ELEM_SOURCE)
TYPES(V_OPERATOR_CAST_ELEM_SOURCE)


TypeBase* getTypeBaseFromType(Builtin::SubType type) {
	for(int i = 0; i < snBuiltins; i++) {
		if(sBuiltins[i].type->type() == TypeBase::eBuiltin) {
			if(((Builtin*)sBuiltins[i].type)->mSubType == type) {
				return sBuiltins[i].type;
			}
		}
	}
	return NULL;
}

Value::Value() {
	mSym.symType = eNone;
	mSym.type = NULL; 
	mSym.address = NULL;
}

Value::Value(const std::string& value) {
	mSym.symType = eVariable;
	mSym.type = NULL;
	mSym.address = NULL;

	if(value[0]=='$') {
		int ri;
		if(!parseArgRegName(value.substr(1), &ri))
			throw ParseException("Couldn't parse register name");
		ASSERT_REG;
		this->Int = r.gpr[ri];
		mPrimitiveType = Builtin::eInt;
	} else if(value.find_first_of("x")!=std::string::npos) {
		this->Int = 0;
		sscanf(value.c_str(), "0x%x", &this->Int);
		mPrimitiveType = Builtin::eInt;			
	} 
	else if(value.find_first_of(".")!=std::string::npos ||
		value[value.size()-1]=='f') {
			if(value[value.size()-1]=='f') {
				this->Float = (float)atof(value.c_str());
				mPrimitiveType = Builtin::eFloat;		
			} else{
				this->Double = strtod(value.c_str(), NULL);
				mPrimitiveType = Builtin::eDouble;
			}

	}
	else {
		if(value == "true" || value == "false") {
			this->Bool = (value=="true")?true:false;
			mPrimitiveType = Builtin::eBool;
		} else {
			this->Int = atoi(value.c_str());
			mPrimitiveType = Builtin::eInt;
		}
	}
	mSym.type = getTypeBaseFromType(mPrimitiveType);
}

Value::Value(const SYM& sym) : mSym(sym), mPrimitiveType(Builtin::NUM_SUB_TYPES) {
	mSym.type = mSym.type;
	if(getType() == TypeBase::ePointer ) {
		if(!mSym.address) throw ParseException("Invalid pointer.");
		this->Int = *(const int*)mSym.address;	
		mPrimitiveType = Builtin::eInt;
	} else if(getType() == TypeBase::eArray) {
		if(!mSym.address) throw ParseException("Invalid array.");
		this->Int = (int)((const char*)mSym.address-(const char*)gMemBuf);	
		mPrimitiveType = Builtin::eInt;
	} else if(getType() == TypeBase::eEnum) {
		if(!mSym.address) throw ParseException("Invalid enum.");
		this->Int = *(const int*)mSym.address;	
		mPrimitiveType = Builtin::eInt;		
	}  else if(getType() == TypeBase::eFunction) {
		//if(!mSym.address) throw ParseException("Invalid function.");
		this->Int = (int)mSym.address;	
		mPrimitiveType = Builtin::eInt;		
	}
}

Value::Value(const TypeBase* typeBase) : mPrimitiveType(Builtin::NUM_SUB_TYPES) {
	mSym.symType = eNone;
	mSym.type = typeBase;
	mSym.address = NULL;
}

bool Value::isType() const {
	return getPrimitiveType()==Builtin::NUM_SUB_TYPES && mSym.address==NULL;
}

int Value::sizeOf() const {
	if(mSym.type) {
		return mSym.type->size();
	} else {
		throw ParseException("Sizeof not applicable on value.");
	}
}


bool Value::isPointer() const { 
	return getType()==TypeBase::ePointer; 
}

Builtin::SubType Value::getPrimitiveType() const {
	return mPrimitiveType;
}

TypeBase::Type Value::getType() const {
	return (!mSym.type) ? TypeBase::eUnknown : mSym.type->resolve()->type();
}

const SYM& Value::getSymbol() const { 
	return mSym; 
}

void Value::setSymbol(const SYM& sym) { 
	mSym = sym; 
}

void Value::setTypeBase(const TypeBase* type) {
	mSym.type = type;
}

const TypeBase* Value::getTypeBase() const {
	return mSym.type->resolve();
}


const void* Value::getDataAddress() const {
	if(getType() == TypeBase::eFunction) return (const void*)v;
	else if(mSym.address != NULL) return mSym.address;
	else return &v;
}

const TypeBase* Value::deref() const {
	if(!mSym.type) return NULL; //throw ParseException("Dereference failed.");
	if(getType() == TypeBase::eArray) {
		return ((ArrayType*)mSym.type->resolve())->mElemType;
	} else if(getType() == TypeBase::ePointer) {
		return mSym.type->deref();
	}

	return NULL;
}
