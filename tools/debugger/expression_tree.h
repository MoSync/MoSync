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

#ifndef _SYNTAX_TREE_H_
#define _SYNTAX_TREE_H_

#include "expression.h"
#include <string>
#include "old_expr.h"
#include "stabs/stabs_typedefs.h"
#include "helpers/RefCounted.h"

class ExpressionTree;

class ExpressionTreeNode : public RefCounted {
public:
	ExpressionTreeNode(ExpressionTree *tree);
	virtual ~ExpressionTreeNode() {}

	virtual Value evaluate() = 0;
protected:
	ExpressionTree *mTree;
};

class TerminalNode : public ExpressionTreeNode {
public:
	enum Type {
		IS_SYM,
		IS_TOKEN
	};

	TerminalNode(ExpressionTree *tree, const Token& token);
	TerminalNode(ExpressionTree *tree, const SYM& sym);

	Value evaluate();
protected:
	Type mType;

	// what's the problem with the constructor (can't create union?)
	//union {
	Token mToken;
	SYM mSym;
	//};
};

class TypeNode : public ExpressionTreeNode {
public:
	TypeNode(ExpressionTree *tree, const TypeBase *typeBase, int numStars);
	virtual ~TypeNode();
	Value evaluate();
protected:
	const TypeBase* mTypeBase;
	std::vector<TypeBase*> mPointerTypes;
	int mNumStars;
};

class CastNode : public ExpressionTreeNode {
public:
	CastNode(ExpressionTree *tree, ExpressionTreeNode *child, ExpressionTreeNode *type);
	virtual ~CastNode();
	Value evaluate();
protected:
	ExpressionTreeNode *mChild;
	ExpressionTreeNode *mType;
};

class BinaryOpNode : public ExpressionTreeNode {
public:
	BinaryOpNode(ExpressionTree *tree, const Token& t, ExpressionTreeNode* child1, ExpressionTreeNode *child2);
	virtual ~BinaryOpNode();
	Value evaluate();
protected:
	Token mToken;
	ExpressionTreeNode *mChild1, *mChild2;
};


class UnaryOpNode : public ExpressionTreeNode {
public:
	UnaryOpNode(ExpressionTree *tree, const Token& t, ExpressionTreeNode* child);
	virtual ~UnaryOpNode();
	Value evaluate();
protected:
	Token mToken;
	ExpressionTreeNode *mChild;
};

class DerefNode : public ExpressionTreeNode {
public:
	DerefNode(ExpressionTree *tree, ExpressionTreeNode* child);
	virtual ~DerefNode();
	Value evaluate();
protected:
	ExpressionTreeNode *mChild;
};

class RefNode : public ExpressionTreeNode {
public:
	RefNode(ExpressionTree *tree, ExpressionTreeNode* child);
	virtual ~RefNode();
	Value evaluate();
protected:
	ExpressionTreeNode *mChild;
	TypeBase* mPtrTypeBase;
};

class IndexNode : public ExpressionTreeNode {
public:
	IndexNode(ExpressionTree *tree, ExpressionTreeNode* child, ExpressionTreeNode* index);
	virtual ~IndexNode();
	Value evaluate();
protected:
	ExpressionTreeNode *mChild;
	ExpressionTreeNode *mIndex;
};

class ConditionalNode : public ExpressionTreeNode {
public:
	ConditionalNode(ExpressionTree *tree, ExpressionTreeNode* a, ExpressionTreeNode* b, ExpressionTreeNode *c);
	virtual ~ConditionalNode();
	Value evaluate();
protected:
	ExpressionTreeNode *mA, *mB, *mC;
};

class DotNode : public ExpressionTreeNode {
public:
	DotNode(ExpressionTree *tree, std::string ident, ExpressionTreeNode* child);
	virtual ~DotNode();
	Value evaluate();


	// I need to use these on another place.. maybe should be put in stabs instead..
	struct SearchResult {
		bool found;
		int offsetBits, sizeBits;
		const TypeBase *type;
		Visibility visibility;
	};

	// returns offset
	static void recursiveSearch(const std::string& ident, const StructType *s, SearchResult *res, int offset=0);
protected:
	std::string mIdent;
	ExpressionTreeNode *mChild;
};

class ArrayNode : public ExpressionTreeNode {
public:
	ArrayNode(ExpressionTree *tree, std::string length, ExpressionTreeNode *child);
	virtual ~ArrayNode();
	Value evaluate();
protected:
	std::string mLength;
	ExpressionTreeNode *mChild;
	TypeBase *mCachedType;
};

#endif
