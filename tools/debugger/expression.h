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
	void ATTRIBUTE(noreturn, error(const char *msg));
};

namespace ExpressionParser {
	ExpressionTree* parse(const char *expr);
};

class ExpressionTree {
public:
	ExpressionTree();
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
void stackEvaluateExpressionTree(ExpressionTree *tree, int frameAddr, ExpressionCallback cb, bool parse=true);
std::string getType(const TypeBase* tb, bool complex);
std::string getValue(const TypeBase* tb, const void* addr, TypeBase::PrintFormat fmt);

int getFileScope(unsigned int pc);
const TypeBase* findTypeByNameAndPC(const std::string& t);

#endif /* _EXPRESSION_H_ */
