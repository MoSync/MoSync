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
	virtual Value evaluate() = 0;
protected:
	ExpressionTree *mTree;
};

class TerminalNode : public ExpressionTreeNode {
public:
	TerminalNode(ExpressionTree *tree, const Token& token);
	Value evaluate();
protected:
	Token mToken;
};

class TypeNode : public ExpressionTreeNode {
public:
	TypeNode(ExpressionTree *tree, const TypeBase *typeBase, int numStars);
	~TypeNode();
	Value evaluate();
protected:
	const TypeBase* mTypeBase;
	std::vector<TypeBase*> mPointerTypes;
	int mNumStars;
};

class CastNode : public ExpressionTreeNode {
public:
	CastNode(ExpressionTree *tree, ExpressionTreeNode *child, ExpressionTreeNode *type);
	~CastNode();
	Value evaluate();
protected:
	ExpressionTreeNode *mChild;
	ExpressionTreeNode *mType;
};

class BinaryOpNode : public ExpressionTreeNode {
public:
	BinaryOpNode(ExpressionTree *tree, const Token& t, ExpressionTreeNode* child1, ExpressionTreeNode *child2);
	~BinaryOpNode();
	Value evaluate();
protected:
	Token mToken;
	ExpressionTreeNode *mChild1, *mChild2;
};


class UnaryOpNode : public ExpressionTreeNode {
public:
	UnaryOpNode(ExpressionTree *tree, const Token& t, ExpressionTreeNode* child);
	~UnaryOpNode();
	Value evaluate();
protected:
	Token mToken;
	ExpressionTreeNode *mChild;
};

class DerefNode : public ExpressionTreeNode {
public:
	DerefNode(ExpressionTree *tree, ExpressionTreeNode* child);
	~DerefNode();
	Value evaluate();
protected:
	ExpressionTreeNode *mChild;
};

class RefNode : public ExpressionTreeNode {
public:
	RefNode(ExpressionTree *tree, ExpressionTreeNode* child);
	~RefNode();
	Value evaluate();
protected:
	ExpressionTreeNode *mChild;
	TypeBase* mPtrTypeBase;
};

class IndexNode : public ExpressionTreeNode {
public:
	IndexNode(ExpressionTree *tree, ExpressionTreeNode* child, ExpressionTreeNode* index);
	~IndexNode();
	Value evaluate();
protected:
	ExpressionTreeNode *mChild;
	ExpressionTreeNode *mIndex;
};

class ConditionalNode : public ExpressionTreeNode {
public:
	ConditionalNode(ExpressionTree *tree, ExpressionTreeNode* a, ExpressionTreeNode* b, ExpressionTreeNode *c);
	~ConditionalNode();
	Value evaluate();
protected:
	ExpressionTreeNode *mA, *mB, *mC;
};

class DotNode : public ExpressionTreeNode {
public:
	DotNode(ExpressionTree *tree, std::string ident, ExpressionTreeNode* child);
	~DotNode();
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
	~ArrayNode();
	Value evaluate();
protected:
	std::string mLength;
	ExpressionTreeNode *mChild;
	TypeBase *mCachedType;
};

#endif
