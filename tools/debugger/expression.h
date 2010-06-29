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

#ifndef _EXPRESSION_H_
#define _EXPRESSION_H_

#include <vector>
#include <string>
#include <map>
#include "ParseException.h"
#include "Token.h"
#include "Value.h"
#include "expression_tree.h"
#include "old_expr.h"

class ExpressionTreeNode;
class ExpressionTree;

/*
#define EXP_ERRORS(m)\
	m(MISSING_SYMBOL, "Missing symbol")\
*/

namespace ExpressionCommon {
	void loadMemory(int addr, int len);
	void error(const char *msg) GCCATTRIB(noreturn);
};

namespace ExpressionParser {
	ExpressionTree* parse(const char *expr);
};

class ExpressionTree {
public:
	ExpressionTree(const char *expression);
	~ExpressionTree();

	Value evaluate();

	void addSymbol(const std::string& name);
	void setSymbol(const std::string& name, const SYM& sym);
	const SYM& getSymbol(const std::string& name);
	bool hasSymbol(const std::string& name) const;
	std::map<std::string, SYM>& getSymbols();
	void setRoot(ExpressionTreeNode *root);
	ExpressionTreeNode* getRoot();
	const char *getExpression();

private:
	ExpressionTreeNode* mRoot;
	std::string mExpression;
	std::map<std::string, SYM> mSymbols;
};

// err!=NULL on error, describing the error.
typedef void (*ExpressionCallback)(const Value* value, const char *err);
void stackEvaluateExpression(const std::string& expr, int frameAddr, ExpressionCallback cb);
void stackEvaluateExpressionTree(ExpressionTree *tree, int frameAddr, ExpressionCallback cb);
std::string getType(const TypeBase* tb, bool complex);
std::string getValue(const TypeBase* tb, const void* addr, TypeBase::PrintFormat fmt);

int getFileScope(unsigned int pc);
const TypeBase* findTypeByNameAndPC(const std::string& t);

#endif /* _EXPRESSION_H_ */