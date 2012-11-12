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

#include <vector>

//todo: cleanup
#define LOGGING_ENABLED
#define CONFIG_H

#include "helpers/helpers.h"

#include "stabs_typedefs.h"
#include "stabs_types.h"
#include "stabs_file.h"
#include "stabs_builtins.h"

using namespace std;

//#if 0	//unused
const char* getVisibilityString(Visibility vis) {
	switch(vis) {
		case ePrivate: return "private";
		case eProtected: return "protected";
		case ePublic: return "public";
		default: return "";
	}
}
//#endif

StringPrintFunctor::StringPrintFunctor() : mPos(0), mString(NULL), mStringSize(0) {
	resizeString(16);
}

StringPrintFunctor::~StringPrintFunctor() {
	if(mString) delete mString;
}

const char* StringPrintFunctor::getString() {
	return mString;
}

void StringPrintFunctor::resizeString(int size) {
	char *oldString = mString;
	mStringSize = size;
	mString = new char[mStringSize];
	for(int i = 0; i < mPos; i++) {
		mString[i] = oldString[i];
	}
	delete oldString;
}

void StringPrintFunctor::reset() {
	mPos = 0;
	mString[0] = 0;
}


int StringPrintFunctor::operator()(const char* fmt, ...) {
	int len = -1;
	while(len==-1) {
		if(mPos >= mStringSize)
			resizeString(mStringSize*2);
		va_list argptr;
		va_start(argptr, fmt);
		int left = mStringSize - mPos;
		len = vsnprintf(mString + mPos, left, fmt, argptr);
		if(len==-1 || len >= left) {
			resizeString(mStringSize*2);
			len = -1;
		} else {
			mPos += len;
		}
	}
	return len;
}

const TypeBase* TypeBase::deref() const {
	return NULL;
}
const TypeBase* TypeBase::resolve() const {
	return this;
}

//keep type references here until they're resolved.
vector<DelayedType*> sDelayed;

DelayedType::DelayedType(TypeReference* t) : mType(t) {
	addRef();
	sDelayed.push_back(this);
}

DelayedType::~DelayedType() {
	release();
}

void DelayedType::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const {
	mType->resolve()->printMI(pf, data, fmt);
}
void DelayedType::printTypeMI(printfPtr pf, bool complex) const {
	mType->resolve()->printTypeMI(pf, complex);
}
const TypeBase* DelayedType::deref() const {
	return mType->resolve()->deref();
}
const TypeBase* DelayedType::resolve() const {
	return mType->resolve();
}

bool DelayedType::resolveAll() {
	for(size_t i=0; i<sDelayed.size(); i++) {
		DelayedType* d(sDelayed[i]);
		const TypeBase* tb = d->resolve(); //((TypeReference*)d->mType)->resolve();
		if(tb != NULL) {
			if(d == tb) {
				TypeReference* tr = (TypeReference*)d->mType;
				StringPrintFunctor pp;
				tr->printTypeMI(pp, true);
				LOG("Could not resolve type: %s\n", pp.getString());
			}
			//FAILIF(d == tb);
			d->mType = tb;
		}
		d->release();	//in preparation for the clear().
	}
	sDelayed.clear();
	return true;
}

void TypeReference::printMI(printfPtr, const void*, PrintFormat) const {
	DEBIG_PHAT_ERROR;
}
void TypeReference::printTypeMI(printfPtr, bool complex) const {
	DEBIG_PHAT_ERROR;
}

TupleReference::TupleReference(Tuple id) : mId(id), mFile(gCurrentFile) {
}
const TypeBase* TupleReference::resolve() const {
	return findTypeByTupleAndFile(mId, mFile);
}
void TupleReference::printTypeMI(printfPtr pf, bool complex) const {
	pf("file %i, (%i,%i)", mFile, mId.a, mId.b);
}


CrossReferenceType::CrossReferenceType(Tuple id, const char* name)
: TupleReference(id), mName(name)
{}
void CrossReferenceType::printTypeMI(printfPtr pf, bool complex) const {
	pf("xs%s", mName.c_str());
}

class UnknownType : public TypeBase {
	int size() const {
		return 0;
	}

	bool isSimpleValue() const {
		return true;
	}

	Type type() const {
		return eUnknown;
	}

	void printMI(printfPtr pfptr, const void* data, PrintFormat pf) const {
		pfptr("%s", "?");
	}

	//if complex && !isSimpleValue, then members will be printed.
	//multiple lines will be used for structs.
	//enums will be printed on single lines.
	void printTypeMI(printfPtr pf, bool complex) const {
		pf("%s", "UnknownType");
	}
};

const TypeBase* CrossReferenceType::resolve() const {
	const TypeBase* tb = findTypeByNameAndFileGlobal(mName, mFile);
	//const TypeBase* tb = findTypeByNameAndTupleAndFileGlobal(mName, mId, mFile);
	if(!tb) {
		return new UnknownType();
	}
	return tb;
}

PointerType::PointerType(const TypeBase* target) : mTarget(target) {
}
void PointerType::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const {
//	pf("0x%x", *(int*)data);
	printPrimitiveByFormat<int>(pf, data, "%u", fmt, TypeBase::eHexadecimal);
}

void PointerType::printTypeMI(printfPtr pf, bool complex) const {
	if(mTarget->type() == eArray) {
		ArrayType* arrayType = (ArrayType*)mTarget->resolve();
		arrayType->mElemType->printTypeMI(pf, complex);
		pf(" (*)[%i]", arrayType->mLength);
		return;
	}

	mTarget->printTypeMI(pf, complex);
	pf(" *");
}
const TypeBase* PointerType::deref() const {
	return mTarget;
}

PointerToMemberType::PointerToMemberType(const StructType* c, const TypeBase* m)
: mClass(c), mMember(m)
{
}
void PointerToMemberType::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const {
//	pf("0x%x", *(int*)data);
	printPrimitiveByFormat<int>(pf, data, "%u", fmt, TypeBase::eHexadecimal);
}
void PointerToMemberType::printTypeMI(printfPtr pf, bool complex) const {
	mMember->printTypeMI(pf, false);
	pf(" %s::*", mClass->mName.c_str());
}

FunctionType::FunctionType(const TypeBase* returnType) : mReturnType(returnType) {
}
void FunctionType::addParam(const TypeBase* p) {
	mParams.push_back(p);
}
void FunctionType::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const {
	//relies on data being a MoSync function pointer.
	pf("0x%x", (int)(size_t)data);
}
void FunctionType::printTypeMI(printfPtr pf, bool complex) const {
	mReturnType->printTypeMI(pf, false);
	pf("(");
	for(size_t i=0; i<mParams.size(); i++) {
		mParams[i]->printTypeMI(pf, false);
	}
	pf(")");
}

StructType::StructType(int s, const std::string& name) : mName(name), mSize(s) {
}

void StructType::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const {
	pf("{");
	const byte* bp = (byte*)data;

	for(size_t i=0; i<mBases.size(); i++) {
		pf("<%s> = ", ((StructType*)mBases[i].type->resolve())->mName.c_str());
		((StructType*)mBases[i].type->resolve())->printMI(pf, ((char*)data)+mBases[i].offset, fmt);
		pf(", ");
	}

	//TODO: add static data members
	for(size_t i=0; i<mDataMembers.size(); i++) {
		if(i != 0)
			pf(", ");
		const DataMember& m(mDataMembers[i]);
		if(m.name.size() > 0)
			pf("%s = ", m.name.c_str());
		DEBUG_ASSERT(m.offsetBits % 8 == 0);
		m.type->printMI(pf, bp + m.offsetBits / 8, fmt);
	}
	pf("}");
}
void StructType::printTypeMI(printfPtr pf, bool complex) const {
#if 0
	pf("{");
	for(size_t i=0; i<mDataMembers.size(); i++) {
		if(i != 0)
			pf(", ");
		const DataMember& m(mDataMembers[i]);
		m.type->printTypeMI(pf);
	}
	pf("}");
#endif
	if(complex) {
		pf("struct %s", mName.c_str());

		if(mBases.size()) pf(" : ");

		for(size_t i=0; i<mBases.size(); i++) {
			const TypeBase* tb = mBases[i].type->resolve();
			const StructType* st = (StructType*)tb->resolve();
			pf("public %s", st->mName.c_str());
			if(i!=mBases.size()-1) pf(", ");
		}

		pf(" {\n");
		for(size_t i=0; i<mDataMembers.size(); i++) {
			const TypeBase* tb = mDataMembers[i].type->resolve();

			if(tb->type() == ePointer) {
#if 0
				if(mName == "Widget") {
					int a = 2;
				}
#endif
				const PointerType* pt = (const PointerType*)tb;
				const TypeBase *t = pt->mTarget->resolve();
				if(t && t->type() == eBuiltin) {
					const Builtin* bi = (const Builtin*)t;
					if(bi->mSubType == Builtin::eVTablePtr) continue;
				}
			}


			//original GDB ptype uses "    ".
			//any good parser should be able to handle a tab instead.
			pf("\t");
			mDataMembers[i].type->printTypeMI(pf, false);
			if(mDataMembers[i].name.length() > 0) {
				pf(" %s", mDataMembers[i].name.c_str());
			}
			pf(";\n");
		}

		//TODO: add static data members

		pf("}");
	} else {
		pf("%s", mName.c_str());
	}
}

void StructType::addDataMember(const DataMember& m) {
	mDataMembers.push_back(m);
}

void StructType::addMethod(const Method& m) {
	mMethods.push_back(m);
}

void StructType::addBaseClass(const BaseClass& bc) {
	mBases.push_back(bc);
}

void StructType::addStaticDataMember(const StaticDataMember& sm) {
	mStaticDataMembers.push_back(sm);
}

const std::vector<DataMember>& StructType::getDataMembers() const {
	return mDataMembers;
}

const std::vector<Method>& StructType::getMethods() const {
	return mMethods;
}

const std::vector<BaseClass>& StructType::getBases() const {
	return mBases;
}

const std::vector<StaticDataMember>& StructType::getStaticDataMembers() const {
	return mStaticDataMembers;
}

RangeType::RangeType(int min, int max) : mMin(min), mMax(max) {
}

void RangeType::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const {
	DEBIG_PHAT_ERROR;
}
void RangeType::printTypeMI(printfPtr pf, bool complex) const {
	DEBIG_PHAT_ERROR;
}

ArrayType::ArrayType(int len, const TypeBase* elemType, const TypeBase* rangeType)
: mLength(len), mElemType(elemType), mRangeType(rangeType)
{
}
void ArrayType::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const {
	pf("{");
	const byte* bp = (byte*)data;
	for(int i=0; i<mLength; i++) {
		if(i != 0)
			pf(", ");
		mElemType->printMI(pf, bp + i * mElemType->size(), fmt);
	}
	pf("}");
}
void ArrayType::printTypeMI(printfPtr pf, bool complex) const {
	mElemType->printTypeMI(pf, complex);
	pf(" [%i]", mLength);
}
int ArrayType::size() const {
	return mLength * mElemType->size();
}

ConstType::ConstType(const TypeBase* target) : mTarget(target) {
}
void ConstType::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const {
	mTarget->printMI(pf, data, fmt);
}
void ConstType::printTypeMI(printfPtr pf, bool complex) const {
	pf("const ");
	mTarget->printTypeMI(pf, complex);
}
const TypeBase* ConstType::deref() const {
	return mTarget->deref();
}
int ConstType::size() const {
	return mTarget->size();
}
bool ConstType::isSimpleValue() const {
	return mTarget->isSimpleValue();
}

EnumType::EnumType(const string& name) : mName(name) {
}
void EnumType::addMember(const EnumMember& em) {
	mMembers.push_back(em);
}
void EnumType::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const {
	int value = *(int*)data;
	//maybe mMembers should be a map<int, string> instead, for speed.
	for(size_t i=0; i<mMembers.size(); i++) {
		if(mMembers[i].value == value) {
			pf("%s", mMembers[i].name.c_str());
			return;
		}
	}
	//invalid value. no need to throw an error, I think.
	//todo: check what other debuggers do.
	pf("%i", value);
}
void EnumType::printTypeMI(printfPtr pf, bool complex) const {
	if(complex)
		pf("enum ");
	pf("%s", mName.c_str());
	if(complex) {
		//todo: print members, specifying values for member that had values specified in source,
		//name only for the other members.
		// added by niklas, don't know if this is right though.
		pf("{\n");
		for(size_t i=0; i<mMembers.size(); i++) {
			pf("%s = %d", mMembers[i].name.c_str(), mMembers[i].value);
			pf((i!=mMembers.size()-1)?(",\n"):("\n"));
		}
		pf("}");
	}
}



#if 0
void StructCrossReferenceType::printMI(printfPtr, const void*, TypeBase::PrintFormat pf) const {
	DEBIG_PHAT_ERROR;
}

void EnumCrossReferenceType::printMI(printfPtr, const void*) const {
	DEBIG_PHAT_ERROR;
}
#endif
