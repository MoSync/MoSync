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

#include "expression_tree.h"
#include "stabs/stabs_builtins.h"
#include "cmd_stack.h"

using namespace std;

ExpressionTreeNode::ExpressionTreeNode(ExpressionTree *tree) : mTree(tree) {
}

CastNode::CastNode(ExpressionTree *tree, const TypeBase *typeBase, int numStars, ExpressionTreeNode *child) : 
ExpressionTreeNode(tree), mChild(child), mNumStars(numStars) {
	const TypeBase *target = typeBase;
	for(int i = 0; i < mNumStars; i++) {
		TypeBase *newTarget = new PointerType(target);
		mPointerTypes.push_back(newTarget);
		target = newTarget;
	}
	mTypeBase = target;

	mChild->addRef();
}

CastNode::~CastNode() {
	for(int i = 0; i < mPointerTypes.size(); i++) delete mPointerTypes[i];
	mChild->deleteRef();
}

#define CAST_ELEM(type, id) case Builtin::##e##id: return Value((type)a);
Value CastNode::evaluate() {
	Value a = mChild->evaluate();

	if(mTypeBase->type == TypeBase::eBuiltin) {
		switch(((Builtin*)mTypeBase)->type) {
			TYPES(CAST_ELEM)
				default: throw ParseException("Invalid cast");
		}
	} else if(mTypeBase->type == TypeBase::ePointer || mTypeBase->type == TypeBase::eEnum || mTypeBase->type == TypeBase::eFunction) {
		Value ret = Value((int)a);
		SYM newSym;
		newSym.address = NULL;
		newSym.symType = eVariable;
		newSym.type = mTypeBase;
		ret.setSymbol(newSym);
		return ret;
	} else {
		throw ParseException("Unsupported cast.");
	}
}

TerminalNode::TerminalNode(ExpressionTree *tree, const Token& t) : ExpressionTreeNode(tree), mToken(t) {
}

#define CAST_BUILTIN(name, id) case Builtin::e##id##: v = Value(*((name*)symbol.address)); break;

Value getValueFromSymbol(const SYM& symbol) {
	if(symbol.type->type==TypeBase::eBuiltin) {
		Builtin* builtin = (Builtin*)symbol.type;
		Value v;
		switch(builtin->mSubType) {
			BUILTINS(CAST_BUILTIN, CAST_BUILTIN)
			default:
				throw ParseException("Unknown type.");
		}
		SYM newSym = symbol;
		if(newSym.storageClass==eRegister)
			newSym.address = NULL;
		v.setSymbol(newSym);
		return v;
		
	} else {
		return Value(symbol);
	}
}

Value TerminalNode::evaluate() {
	if(mToken.getTokenType() == TOKEN_IDENT) {
		const SYM& symbol = mTree->getSymbol(mToken.toString());	
		return getValueFromSymbol(symbol);
	}
	else if(mToken.getTokenType() == TOKEN_NUMBER || mToken.getTokenType() == TOKEN_REG)
		return Value(mToken.toString());
	else throw ParseException("Invalid value or identifier");
}

BinaryOpNode::BinaryOpNode(ExpressionTree *tree, const Token& t, ExpressionTreeNode* child1, ExpressionTreeNode *child2) :
ExpressionTreeNode(tree), mToken(t), mChild1(child1), mChild2(child2) {
	mChild1->addRef();
	mChild2->addRef();
}

BinaryOpNode::~BinaryOpNode() {
	mChild1->deleteRef();
	mChild2->deleteRef();
}

Value BinaryOpNode::evaluate() {
	Value a = mChild1->evaluate();
	Value b = mChild2->evaluate();	

	SYM aSym = a.getSymbol();
	const SYM &bSym = b.getSymbol();

	const TypeBase *derefA = a.deref();
	const TypeBase *derefB = b.deref();

	if(derefA && derefB && derefA!=derefB)
		throw ParseException("Cannot do operation between two pointers of different types");

	bool aIsPointer = derefA!=NULL;
	bool bIsPointer = derefB!=NULL;

	if(aIsPointer && !bIsPointer) {
		b=b*Value((int)derefA->size);
	}

	a = a.doBinaryOperation(b, mToken);

	if(aIsPointer) {
		a.setTypeBase(aSym.type);
	} else if(bIsPointer) {
		a.setTypeBase(bSym.type);
	}

	return a;
}

UnaryOpNode::UnaryOpNode(ExpressionTree *tree, const Token& t, ExpressionTreeNode* child) : 
ExpressionTreeNode(tree), mToken(t), mChild(child) {
	mChild->addRef();
}

Value UnaryOpNode::evaluate() {
	Value a = mChild->evaluate();
	return a.doUnaryOperation(mToken);
}

UnaryOpNode::~UnaryOpNode() {
	mChild->deleteRef();
}

DerefNode::DerefNode(ExpressionTree *tree, ExpressionTreeNode* child) : 
ExpressionTreeNode(tree), mChild(child) {
	mChild->addRef();
}

DerefNode::~DerefNode() {
	mChild->deleteRef();
}

Value DerefNode::evaluate() {
	Value a = mChild->evaluate();

	if(a.isPointer() == false) throw ParseException("Dereferenceing non-pointer type");

	const SYM& sym = a.getSymbol();
	const TypeBase* deref = sym.type->deref()->resolve();
	if(!deref) throw ParseException("Invalid pointer");

	int addr = (int)a;
	ExpressionCommon::loadMemory(addr, deref->size);

	SYM newSym;
	newSym.address = &gMemBuf[addr];
	if(deref->type == TypeBase::eFunction)
		newSym.symType = eFunction;
	else newSym.symType = eVariable;
	newSym.type = deref;

	return getValueFromSymbol(newSym);
}

RefNode::RefNode(ExpressionTree *tree, ExpressionTreeNode* child) : 
ExpressionTreeNode(tree), mChild(child), mPtrTypeBase(NULL) {
	mChild->addRef();
}

RefNode::~RefNode() {
	mChild->deleteRef();
	if(mPtrTypeBase) delete mPtrTypeBase;
}

Value RefNode::evaluate() {
	Value a = mChild->evaluate();
	const SYM& sym = a.getSymbol();
	
	if(a.getType() == TypeBase::eFunction) {
		return a;
	} 
	else if(a.getType() == TypeBase::eArray) {
		if(mPtrTypeBase != NULL) delete mPtrTypeBase;
		mPtrTypeBase = new PointerType(((ArrayType*)sym.type)->mElemType);
		SYM newSym;
		int addr = (int)a;
		newSym.address = NULL;
		newSym.symType = eVariable;
		newSym.type = mPtrTypeBase;
		Value ret = Value(addr);
		ret.setSymbol(newSym);
		return ret;
	}

	if(sym.type && sym.address) {
		if(mPtrTypeBase != NULL) delete mPtrTypeBase;
		mPtrTypeBase = new PointerType(sym.type);
		SYM newSym;
		newSym.address = NULL;
		newSym.symType = eVariable;
		newSym.type = mPtrTypeBase;
		Value ret = Value((int)((const char*)sym.address-(const char*)gMemBuf));
		ret.setSymbol(newSym);
		return ret;
	} else {
		throw ParseException("Non referrable type");
	}
}


IndexNode::IndexNode(ExpressionTree *tree, ExpressionTreeNode* child, ExpressionTreeNode* index) : 
ExpressionTreeNode(tree), mChild(child), mIndex(index) {
	mChild->addRef();
	mIndex->addRef();
}

IndexNode::~IndexNode() {
	mChild->deleteRef();
	mIndex->deleteRef();
}

Value IndexNode::evaluate() {
	Value ptr = mChild->evaluate();
	Value idx = mIndex->evaluate();

	if(idx.getType() != TypeBase::eBuiltin && idx.getType() != TypeBase::eEnum)
		throw ParseException("Not a valid index.");

	const SYM& sym = ptr.getSymbol();
	TypeBase::Type ptrType = ptr.getType();
	const TypeBase* deref = NULL;
	int index = (int)idx;
	
	if(ptrType == TypeBase::ePointer) {
		deref = sym.type->deref()->resolve();
	} else if (ptrType == TypeBase::eArray) {
		ArrayType *arrayType = (ArrayType*) sym.type;
		deref = arrayType->mElemType->resolve();
		if(index<0 || index>=arrayType->mLength)
			throw ParseException("Index out of bounds.");
	} else {
		throw ParseException("Non-indexable type");
	}
	if(!deref) throw ParseException("Non-indexable type");

	int addr = (int)ptr+index*deref->size;
	ExpressionCommon::loadMemory(addr, deref->size);

	SYM newSym;
	newSym.address = &gMemBuf[addr];
	if(deref->type == TypeBase::eFunction)
		newSym.symType = eFunction;
	else newSym.symType = eVariable;
	newSym.type = deref;

	return getValueFromSymbol(newSym);
}

ConditionalNode::ConditionalNode(ExpressionTree *tree, ExpressionTreeNode* a, ExpressionTreeNode* b, ExpressionTreeNode *c) : 
ExpressionTreeNode(tree), a(a), b(b), c(c) {
	a->addRef();
	b->addRef();
	c->addRef();
}

ConditionalNode::~ConditionalNode() {
	a->deleteRef();
	b->deleteRef();
	c->deleteRef();
}

Value ConditionalNode::evaluate() {
	Value x = a->evaluate();
	Value y = b->evaluate();
	Value z = c->evaluate();

	return ((bool)x)?y:z;
}

DotNode::DotNode(ExpressionTree *tree, std::string ident, ExpressionTreeNode* child) : 
ExpressionTreeNode(tree), mIdent(ident), mChild(child) {
	mChild->addRef();
}

DotNode::~DotNode() {
	mChild->deleteRef();
}

Value DotNode::evaluate() {
	Value a = mChild->evaluate();

	const SYM& sym = a.getSymbol();
	if(!sym.type || sym.type->type != TypeBase::eStruct) throw ParseException("Left operand must be of type struct");

	StructType *s = (StructType*)sym.type;
	std::string ident = mIdent;
	SearchResult res;
	recursiveSearch(ident, s, &res);

	if(res.found) {
		if((res.offsetBits&0x7)!=0)
			throw ParseException("Doesn't support bitfields just yet...");
		int addr = (int)sym.address;
		addr+=res.offsetBits>>3;
		//mParser->loadMemory(addr, res.sizeBits>>3);

		SYM sym;
		sym.address = (const void*)addr;
		sym.storageClass = eStack;

		if(res.type->type == TypeBase::eFunction)
			sym.symType = eFunction;
		else sym.symType = eVariable;
		sym.type = res.type;

		return getValueFromSymbol(sym);
	}

	throw ParseException("Data member not found");
	return Value(0);
}

void DotNode::recursiveSearch(const std::string& ident, StructType *s, SearchResult *res, int offset) {
	const std::vector<DataMember>& dataMembers = s->getDataMembers();
	const std::vector<Method>& methods = s->getMethods();
	const std::vector<BaseClass>& bases = s->getBases();
	const std::vector<StaticDataMember>& staticDataMembers = 
		s->getStaticDataMembers();

	res->found = false;

	for(size_t i = 0; i < dataMembers.size(); i++) {
		if(dataMembers[i].name == ident) {
			res->offsetBits = dataMembers[i].offsetBits + (offset<<3);
			res->sizeBits = dataMembers[i].sizeBits;
			res->type = dataMembers[i].type;
			res->visibility = dataMembers[i].visibility;
			res->found = true;
			return;
		}
	}

	/*
	// do methods and this later..
	for(int i = 0; i < staticDataMembers.size(); i++) {
	if(staticDataMembers[i].name == ident) {
	staticDataMembers[i].
	}
	}
	*/

	for(size_t i = 0; i < bases.size(); i++) {
		//if(bases[i].visibility != ePrivate) {
		recursiveSearch(ident, (StructType*)bases[i].type, res, bases[i].offset);
		if(res->found == true) return;
		//}
	}
}

ArrayNode::ArrayNode(ExpressionTree *tree, string length, ExpressionTreeNode *child) : 
ExpressionTreeNode(tree), mLength(length), mChild(child), mCachedType(NULL) {
	mChild->addRef();
}

ArrayNode::~ArrayNode() {
	mChild->deleteRef();
	if(mCachedType) {
		delete mCachedType->deref();
		delete mCachedType;
	}
}

Value ArrayNode::evaluate() {
	Value a = mChild->evaluate();
	const TypeBase *deref = NULL;
	if(!(deref = a.deref())) throw ParseException("Left value not of pointer type");
	Value b = Value(mLength);
	if(b.getPrimitiveType() == Builtin::eFloat ||
		b.getPrimitiveType() == Builtin::eDouble ||
		b.getPrimitiveType() == Builtin::eBool) {
		throw ParseException("Right value not of integer type.");
	}

	if(mCachedType) delete mCachedType;
	mCachedType = 
		new PointerType(
		new ArrayType((int)b, deref, getTypeBaseFromType(Builtin::eInt))
	);

	ExpressionCommon::loadMemory((int)a, (int)b);

	SYM newSym;
	newSym.symType = eVariable;
	newSym.type = mCachedType;
	newSym.address = &gMemBuf[(int)a];

	Value v((int)a);
	v.setSymbol(newSym);
	return v;
}