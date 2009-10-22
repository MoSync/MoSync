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

#include "cmd_stack.h"
#include "expression.h"
#include "async.h"
#include "Thread.h"
#include "stabs/stabs_builtins.h"
#include "sld.h"

#include "commandInterface.h"
#include "helpers.h"

#include <set>

using namespace std; 

namespace ExpressionParser {
	void init();
	bool peekToken(Token &t);
	void nextToken(Token &t);
	bool expect(int token);
	bool accept(int token);
	bool expect(int token, Token &t);
	bool accept(int token, Token &t);

	ExpressionTreeNode* expression();
	ExpressionTreeNode* logicalOrExpression();
	ExpressionTreeNode* logicalAndExpression();
	ExpressionTreeNode* inclusiveOrExpression();
	ExpressionTreeNode* exclusiveOrExpression();
	ExpressionTreeNode* andExpression();
	ExpressionTreeNode* equalityExpression();
	ExpressionTreeNode* relationalExpression();
	ExpressionTreeNode* shiftExpression();
	ExpressionTreeNode* additiveExpression();
	ExpressionTreeNode* multiplicativeExpression();
	ExpressionTreeNode* castExpression();
	ExpressionTreeNode* unaryExpression();
	ExpressionTreeNode* postfixExpression();
	ExpressionTreeNode* primaryExpression();

	std::vector<TokenMatcher*> mTokenMatchers;
	const char *mExpr;
	std::string mExprString;
	ExpressionTree *sExpressionTree;

	bool sInitialized = false;
}

void ExpressionParser::init() {
	if(sInitialized) return;
	sInitialized = true;

	Value::initMaps();

	mTokenMatchers.push_back(new TokenNumber());
	mTokenMatchers.push_back(new TokenLiteral());
	mTokenMatchers.push_back(new TokenIdent());
	mTokenMatchers.push_back(new TokenReg());

	mTokenMatchers.push_back(new TokenFixed(TOKEN_PLUS, "+"));
	mTokenMatchers.push_back(new TokenFixed(TOKEN_MINUS, "-"));
	mTokenMatchers.push_back(new TokenFixed(TOKEN_SLASH, "/"));
	mTokenMatchers.push_back(new TokenFixed(TOKEN_STAR, "*"));
	mTokenMatchers.push_back(new TokenFixed(TOKEN_LPAREN, "("));
	mTokenMatchers.push_back(new TokenFixed(TOKEN_RPAREN, ")"));
	mTokenMatchers.push_back(new TokenFixed(TOKEN_PERCENT, "%"));

	mTokenMatchers.push_back(new TokenFixed(TOKEN_SHL, "<<"));
	mTokenMatchers.push_back(new TokenFixed(TOKEN_SHR, ">>"));
	mTokenMatchers.push_back(new TokenFixed(TOKEN_AND, "&"));
	mTokenMatchers.push_back(new TokenFixed(TOKEN_OR, "|"));
	mTokenMatchers.push_back(new TokenFixed(TOKEN_XOR, "^"));	
	mTokenMatchers.push_back(new TokenFixed(TOKEN_TILDE, "~"));

	mTokenMatchers.push_back(new TokenFixed(TOKEN_ANDAND, "&&"));
	mTokenMatchers.push_back(new TokenFixed(TOKEN_OROR, "||"));
	mTokenMatchers.push_back(new TokenFixed(TOKEN_NOT, "!"));
	mTokenMatchers.push_back(new TokenFixed(TOKEN_EQ, "=="));
	mTokenMatchers.push_back(new TokenFixed(TOKEN_NEQ, "!="));
	mTokenMatchers.push_back(new TokenFixed(TOKEN_LEQ, "<="));
	mTokenMatchers.push_back(new TokenFixed(TOKEN_GEQ, ">="));
	mTokenMatchers.push_back(new TokenFixed(TOKEN_LE, "<"));
	mTokenMatchers.push_back(new TokenFixed(TOKEN_GE, ">"));

	mTokenMatchers.push_back(new TokenFixed(TOKEN_LBRACK, "["));
	mTokenMatchers.push_back(new TokenFixed(TOKEN_RBRACK, "]"));	
	mTokenMatchers.push_back(new TokenFixed(TOKEN_DOT, "."));	
	mTokenMatchers.push_back(new TokenFixed(TOKEN_ARROW, "->"));	

	mTokenMatchers.push_back(new TokenFixed(TOKEN_UNSIGNED, "unsigned"));
	mTokenMatchers.push_back(new TokenFixed(TOKEN_SIGNED, "signed"));
	mTokenMatchers.push_back(new TokenFixed(TOKEN_STRUCT, "struct"));
	mTokenMatchers.push_back(new TokenFixed(TOKEN_ENUM, "enum"));
	mTokenMatchers.push_back(new TokenFixed(TOKEN_VOLATILE, "volatile"));
	mTokenMatchers.push_back(new TokenFixed(TOKEN_CONST, "const"));
	mTokenMatchers.push_back(new TokenFixed(TOKEN_UNION, "union"));
	mTokenMatchers.push_back(new TokenFixed(TOKEN_CLASS, "class"));
	mTokenMatchers.push_back(new TokenFixed(TOKEN_LONG, "long"));
	mTokenMatchers.push_back(new TokenFixed(TOKEN_SHORT, "short"));

	mTokenMatchers.push_back(new TokenFixed(TOKEN_SIZEOF, "sizeof"));

	mTokenMatchers.push_back(new TokenFixed(TOKEN_QUESTION, "?"));
	mTokenMatchers.push_back(new TokenFixed(TOKEN_COLON, ":"));

	mTokenMatchers.push_back(new TokenFixed(TOKEN_AT, "@"));
}

ExpressionTreeNode* ExpressionParser::expression() {
	ExpressionTreeNode *a = logicalOrExpression();
	Token t;
	if(accept(TOKEN_QUESTION, t)) {
		ExpressionTreeNode *b = expression();
		if(accept(TOKEN_COLON, t)) {
			ExpressionTreeNode *c = expression();
			return new ConditionalNode(sExpressionTree, a, b, c);

		} else {
			throw ParseException("Syntax error of conditional expression");
		}
	}
	return a;
}

ExpressionTreeNode* ExpressionParser::logicalOrExpression() {
	ExpressionTreeNode* a = logicalAndExpression();
	Token t;
	if(accept(TOKEN_OROR, t)) {
		ExpressionTreeNode *b = logicalOrExpression();
		return new BinaryOpNode(sExpressionTree, t, a, b);
	}
	return a;
}

ExpressionTreeNode* ExpressionParser::logicalAndExpression() {
	ExpressionTreeNode* a = inclusiveOrExpression();
	Token t;
	if(accept(TOKEN_ANDAND, t)) {
		ExpressionTreeNode *b = logicalAndExpression();
		return new BinaryOpNode(sExpressionTree, t, a, b);
	}
	return a;
}

ExpressionTreeNode* ExpressionParser::inclusiveOrExpression() {
	ExpressionTreeNode* a = exclusiveOrExpression();
	Token t;
	if(accept(TOKEN_OR, t)) {
		ExpressionTreeNode *b = inclusiveOrExpression();
		return new BinaryOpNode(sExpressionTree, t, a, b);
	}
	return a;
}

ExpressionTreeNode* ExpressionParser::exclusiveOrExpression() {
	ExpressionTreeNode* a = andExpression();
	Token t;
	if(accept(TOKEN_XOR, t)) {
		ExpressionTreeNode *b = exclusiveOrExpression();
		return new BinaryOpNode(sExpressionTree, t, a, b);
	}
	return a;
}

ExpressionTreeNode* ExpressionParser::andExpression() {
	ExpressionTreeNode* a = equalityExpression();
	Token t;
	if(accept(TOKEN_AND, t)) {
		ExpressionTreeNode *b = andExpression();
		return new BinaryOpNode(sExpressionTree, t, a, b);
	}
	return a;
}

ExpressionTreeNode* ExpressionParser::equalityExpression() {
	ExpressionTreeNode* a = relationalExpression();
	Token t;
	if(accept(TOKEN_EQ, t) || accept(TOKEN_NEQ, t)) {
		ExpressionTreeNode *b = equalityExpression();
		return new BinaryOpNode(sExpressionTree, t, a, b);
	}
	return a;
}

ExpressionTreeNode* ExpressionParser::relationalExpression() {
	ExpressionTreeNode* a = shiftExpression();
	Token t;
	if(accept(TOKEN_LE, t) || accept(TOKEN_GE, t) || accept(TOKEN_LEQ, t) || accept(TOKEN_GEQ, t)) {
		ExpressionTreeNode *b = relationalExpression();
		return new BinaryOpNode(sExpressionTree, t, a, b);
	}
	return a;
}

ExpressionTreeNode* ExpressionParser::shiftExpression() {
	ExpressionTreeNode* a = additiveExpression();
	Token t;
	if(accept(TOKEN_SHL, t) || accept(TOKEN_SHR, t)) {
		ExpressionTreeNode *b = shiftExpression();
		return new BinaryOpNode(sExpressionTree, t, a, b);
	}
	return a;
}

ExpressionTreeNode* ExpressionParser::additiveExpression() {
	ExpressionTreeNode* a = multiplicativeExpression();
	Token t;
	while(accept(TOKEN_PLUS, t) || accept(TOKEN_MINUS, t)) {
		ExpressionTreeNode *b = additiveExpression();
		return new BinaryOpNode(sExpressionTree, t, a, b);
	}
	return a;
}

ExpressionTreeNode* ExpressionParser::multiplicativeExpression() {
	ExpressionTreeNode* a = castExpression();
	Token t;
	while(accept(TOKEN_STAR, t) || accept(TOKEN_SLASH, t) || accept(TOKEN_PERCENT, t)) {
		ExpressionTreeNode *b = multiplicativeExpression();
		return new BinaryOpNode(sExpressionTree, t, a, b);
	}
	return a;
}

std::string sTypes[] = {
	"int",
	"char",
	"double",
	"float",
};

std::set<std::string> sTypeSet = std::set<std::string>(sTypes, sTypes + (sizeof(sTypes)/sizeof(std::string)));

ExpressionTreeNode* ExpressionParser::castExpression() {
	const char *expr = mExpr;
	//return unaryExpression();

	if(accept(TOKEN_LPAREN)) {
		bool isType = false;
		while(accept(TOKEN_CONST) || accept(TOKEN_VOLATILE)) { isType = true; }

		Token t;
		std::string subType = "";
		int isSigned = 0; // 1 signed, 2 unsigned
		int howLong = 0;
		int howShort = 0;
		int numStars = 0;

		TypeBase::Type type = TypeBase::eUnknown; 
		const Type *typeInfo = NULL;

		while(1) {
			if(accept(TOKEN_CONST)) {
			}
			else if(accept(TOKEN_STRUCT)) { 
				if(type != TypeBase::eUnknown) 
					ExpressionCommon::error("Parse error"); 
				type=TypeBase::eStruct; 
			}
			else if(accept(TOKEN_UNION)) { 
				if(type != TypeBase::eUnknown) 
					ExpressionCommon::error("Parse error"); 
				type=TypeBase::eStruct; 
			}
			else if(accept(TOKEN_ENUM)) { 
				if(type != TypeBase::eUnknown) 
					ExpressionCommon::error("Parse error"); 
				type=TypeBase::eEnum; 
			}
			else if(accept(TOKEN_CLASS)) { 
				if(type != TypeBase::eUnknown) 
					ExpressionCommon::error("Parse error"); 
				type=TypeBase::eStruct;  
			}
			else if(accept(TOKEN_SIGNED, t) || accept(TOKEN_UNSIGNED, t)) {
				if(type != TypeBase::eUnknown && type != TypeBase::eBuiltin)
					ExpressionCommon::error("Parse error");
				if(isSigned != 0) 
					ExpressionCommon::error("Cannot have multiple signedness keywords");
				if(t.getTokenType() == TOKEN_SIGNED) isSigned = 1;
				else isSigned = 2;
				type = TypeBase::eBuiltin;
			}
			else if(accept(TOKEN_LONG)) {
				if(type != TypeBase::eUnknown && type != TypeBase::eBuiltin)
					ExpressionCommon::error("Parse error");

				howLong++;
				type = TypeBase::eBuiltin;
			} 			
			else if(accept(TOKEN_SHORT)) {
				if(type != TypeBase::eUnknown && type != TypeBase::eBuiltin)
					ExpressionCommon::error("Parse error");
				howShort++;
				type = TypeBase::eBuiltin;
			} 	
			else if(accept(TOKEN_IDENT, t)) {
				if(subType != "") 
					ExpressionCommon::error("Multiple types");
				subType = t.toString();
				std::set<std::string>::const_iterator iter = sTypeSet.find(t.toString());
				if(iter != sTypeSet.end()) {
					if(isSigned == 0) isSigned = 1;
					type = TypeBase::eBuiltin;
				} else {

					LineMapping lm;
					ASSERT_REG;
					if(!mapIpEx(r.pc, lm))
						ExpressionCommon::error("Could not map ip.");
					typeInfo = stabsFindTypeByName(t.toString(), lm.file + 1);

					if(typeInfo) {
						if(type != TypeBase::eUnknown) {
							if(type != typeInfo->type->type)
								ExpressionCommon::error("Invalid type specifier");
						} else {
							type = typeInfo->type->type;
						}
					}
				}
			} else if(accept(TOKEN_STAR, t)) {
				if(subType == "") break;
				numStars++;
			} else {
				break;
			}
		}

		const TypeBase *typeBase = NULL;

		if(type != TypeBase::eUnknown) {
			if(accept(TOKEN_RPAREN)) {
				if(type == TypeBase::eBuiltin) {
					if(howLong && howShort) 
						ExpressionCommon::error("Cannot have both long and short keyword");
					if(howLong >= 3) 
						ExpressionCommon::error("Too long");
					if(howShort > 1) 
						ExpressionCommon::error("Too short");
					if((howLong || howShort) && (subType=="char" || subType=="float" || subType == "double"))
						ExpressionCommon::error("Invalid keyword for type");

					if(subType == "") {
						if(isSigned != 0 || howShort || howLong) subType = "int";
					}

					std::string typeString = "";
					if(howLong) for(int i = 0; i < howLong; i++) typeString+="long ";
					else if(howShort) for(int i = 0; i < howShort; i++) typeString+="short ";
					if(isSigned == 2) typeString += "unsigned ";
					typeString += subType;


					for(int i = 0; i < snBuiltins; i++) {
						if(std::string(sBuiltins[i].name) == typeString) {
							typeBase = sBuiltins[i].type;
						}
					}
					if(typeBase == NULL) 
						ExpressionCommon::error("Missing type");
				} else {
					//const Type* typeInfo = stabsFindTypeByName(t.toString(), 0);
					if(typeInfo) {
						typeBase = typeInfo->type;
					} else
						ExpressionCommon::error("Missing type");
				}
				return new CastNode(sExpressionTree, typeBase, numStars, unaryExpression());

			} else {
				ExpressionCommon::error("Parse error");
			}
		} else {
			mExpr = expr;
		}
	}
	return unaryExpression();
}

ExpressionTreeNode* ExpressionParser::unaryExpression() {
	Token t;
	if(accept(TOKEN_SIZEOF)) {
		return unaryExpression();
	} 
	else if(
		accept(TOKEN_AND, t) ||
		accept(TOKEN_STAR, t) ||
		accept(TOKEN_PLUS, t) ||
		accept(TOKEN_MINUS, t) ||
		accept(TOKEN_TILDE, t) ||
		accept(TOKEN_NOT, t)) {
			ExpressionTreeNode *a = castExpression();
			if(	t.getTokenType() == TOKEN_PLUS ||
				t.getTokenType() == TOKEN_MINUS ||
				t.getTokenType() == TOKEN_TILDE ||
				t.getTokenType() == TOKEN_NOT) {
					return new UnaryOpNode(sExpressionTree, t, a);
			} else {
				if(t.getTokenType() == TOKEN_STAR) {
					return new DerefNode(sExpressionTree, a);
				} else if(t.getTokenType() == TOKEN_AND) {
					return new RefNode(sExpressionTree, a);
				}
				throw ParseException("Unsupported operation");
			}
	} else {
		return postfixExpression();	
	}

	return NULL;
}

ExpressionTreeNode* ExpressionParser::postfixExpression() {
	ExpressionTreeNode* child = primaryExpression();
	Token t;
	while(1) {
		if(accept(TOKEN_DOT, t)) {
			if(accept(TOKEN_IDENT, t)) {
				ExpressionTreeNode *dot = new DotNode(sExpressionTree, t.toString(), child);
				child = dot;
			} else {
				throw ParseException("Operator must be followed by an identifier");
			}
		} else if(accept(TOKEN_ARROW, t)) {
			if(accept(TOKEN_IDENT, t)) {
				ExpressionTreeNode *deref = new DerefNode(sExpressionTree, child);
				ExpressionTreeNode *dot = new DotNode(sExpressionTree, t.toString(), deref);
				child = dot;
			} else {
				throw ParseException("Operator must be followed by an identifier");
			}
		} else if(accept(TOKEN_LBRACK, t)) {
			ExpressionTreeNode *expr = expression();
			ExpressionTreeNode *indexNode = new IndexNode(sExpressionTree, child, expr);
			child = indexNode; 
			if(!accept(TOKEN_RBRACK))
				throw ParseException("Missing right bracket.");
		}
		else
		{
			return child;
		}
	}
}

ExpressionTreeNode* ExpressionParser::primaryExpression() {
	Token t;
	if(accept(TOKEN_IDENT, t) || accept(TOKEN_NUMBER, t) || accept(TOKEN_REG, t)) {
		ExpressionTreeNode* terminalNode =  new TerminalNode(sExpressionTree, t);

		if(t.getTokenType() == TOKEN_IDENT) {
			sExpressionTree->addSymbol(t.toString());
			if(accept(TOKEN_AT)) {
				if(accept(TOKEN_NUMBER, t)) {
					return new ArrayNode(sExpressionTree, t.toString(), terminalNode);
				} else {
					ExpressionCommon::error("Missing length to @ operator");
				}
			}
		}
		return terminalNode;
	} else if(accept(TOKEN_LPAREN, t)) {
		ExpressionTreeNode* ret = expression();
		expect(TOKEN_RPAREN);

		if(accept(TOKEN_AT)) {
			if(accept(TOKEN_NUMBER, t)) {
				return new ArrayNode(sExpressionTree, t.toString(), ret);
			} else {
				ExpressionCommon::error("Missing length to @ operator");
			}
		}
		return ret;
	}
	ExpressionCommon::error("Parse error");
	return NULL;
}

/*
ExpressionParser::~ExpressionParser() {
}
*/

bool ExpressionParser::peekToken(Token &output) {
	const char *str = mExpr;
	while(*str && *str > 0 && *str <= 32) str++;
	if(!*str) return false;
	int matchLen = 0;
	unsigned int tokenType;
	//const char *str = &strPtr[position];
	for(unsigned int i = 0; i < mTokenMatchers.size(); i++) {
		int newMatchLen;
		if((newMatchLen=mTokenMatchers[i]->match(str))) {
			if(newMatchLen>=matchLen) {
				matchLen = newMatchLen;
				tokenType = mTokenMatchers[i]->getType();
			}
		}
	}
	if(matchLen == 0) {
		output = Token(0, mExpr, 1);
		ExpressionCommon::error("Invalid token.");
		return false;
	} else {
		output = Token(tokenType,str,matchLen);
		return true;
	}
}

void ExpressionParser::nextToken(Token &token) {
	mExpr = token.getStart()+token.getLength();
}

bool ExpressionParser::expect(int token, Token &t) {
	if(accept(token, t)) {
		return true;
	}

	ExpressionCommon::error("Unexpected token");
	return false;
}

bool ExpressionParser::expect(int token) {
	if(accept(token)) {
		return true;
	}
	ExpressionCommon::error("Unexpected token");
	return false;
}

bool ExpressionParser::accept(int tokenId, Token &token) {
	if(!peekToken(token)) {
		return false;
	}
	if(tokenId == token.getTokenType()) {
		nextToken(token);
		return true;
	} else {
		return false;
	}
}

bool ExpressionParser::accept(int tokenId) {
	Token token;
	return accept(tokenId, token);
}

void ExpressionCommon::error(const char *msg) {
	throw ParseException(msg);
}

ExpressionTree::ExpressionTree(const char *expression) : mRoot(NULL), mExpression(expression) {
}

ExpressionTree::~ExpressionTree() {
	if(mRoot) delete mRoot;
}

const char* ExpressionTree::getExpression() {
	return mExpression.c_str();
}

Value ExpressionTree::evaluate() {
	if(mRoot)
		return mRoot->evaluate();
	throw ParseException("Expression tree == NULL.");
}

void ExpressionTree::addSymbol(const string& name) {
	SYM empty;
	empty.address = NULL;
	empty.symType = eNone;
	empty.type = NULL;
	setSymbol(name, empty);
}

void ExpressionTree::setSymbol(const string& name, const SYM& sym) {
	mSymbols[name] = sym;
}

const SYM& ExpressionTree::getSymbol(const string& name) {
	map<string, SYM>::const_iterator i = mSymbols.find(name);
	if(i == mSymbols.end()) ExpressionCommon::error("Undefined symbol");
	return i->second;
}

std::map<string, SYM>& ExpressionTree::getSymbols() {
	return mSymbols;
}

static ExpressionTree *sExpressionTree;
static Value sReturnValue((int)0);
static map<string, SYM>::iterator sSymbolIter;
static MoSyncSemaphore sSemaphore;
static ExpressionCallback sCallback;
static bool sComplex;
static std::string sErrorStr;

int evaluateThread(void* data) {
	DebuggerEvent *evnt = new DebuggerEvent;
	evnt->err = NULL;
	try {
		sReturnValue = sExpressionTree->evaluate();

		const TypeBase* deref = NULL;	
		if(sReturnValue.getType()==TypeBase::eArray) {
			deref = (const ArrayType*)sReturnValue.getSymbol().type;
		} else if(sReturnValue.getType()==TypeBase::ePointer) {
			deref = sReturnValue.getSymbol().type->deref()->resolve();
			int addr = (int)sReturnValue;
			if(addr<=0 || addr>gMemSize) {
				deref = NULL;
				sErrorStr = "Invalid pointer.";
			}
		}

		if(deref)
			ExpressionCommon::loadMemory((int)sReturnValue, deref->size);

	} catch(ParseException& e) {
		sErrorStr = e.what();
		evnt->err = sErrorStr.c_str();
	}

	evnt->type = DebuggerEvent::eExpressionEvaluated;
	evnt->erv = &sReturnValue;
	evnt->ecb = sCallback;
	putEvent(evnt);
	return 0;
}

void loadSymbol();

void symbolLoaded(const SYM& sym) {
	sExpressionTree->setSymbol(sSymbolIter->first, sym);
	sSymbolIter++;
	loadSymbol();
}

void errorCallback() {
	//TODO: set error state, so thread knows not to continue?
	sSemaphore.post();
	setErrorCallback(NULL);
}

void loadSymbol() {
	if(sSymbolIter == sExpressionTree->getSymbols().end()) {
		setErrorCallback(errorCallback);
		MoSyncThread thread;
		thread.start(evaluateThread, NULL);
	} else {
		locate_symbol(sSymbolIter->first, symbolLoaded);
	}
}

void memoryLoaded() {
	sSemaphore.post();
}

void ExpressionCommon::loadMemory(int addr, int len) {
	DebuggerEvent *evnt = new DebuggerEvent;
	evnt->type = DebuggerEvent::eReadMemory;
	evnt->src = addr;
	evnt->len = len;
	evnt->rmcb = memoryLoaded;
	putEvent(evnt);
	sSemaphore.wait();
}

void stackLoaded() {
	sSymbolIter = sExpressionTree->getSymbols().begin();
	loadSymbol();
}

void ExpressionTree::setRoot(ExpressionTreeNode *root) {
	mRoot = root;
}

ExpressionTreeNode* ExpressionTree::getRoot() {
	return mRoot;
}

ExpressionTree* ExpressionParser::parse(const char *expr) {
	init();
	sExpressionTree = new ExpressionTree(expr);
	mExpr = sExpressionTree->getExpression();
	sExpressionTree->setRoot(expression());
	return sExpressionTree;
}

void stackEvaluateExpression(const string& args, int frameAddr, ExpressionCallback callback) {
	CHECK_STABS;

	if(frameAddr >= 0) {
		//todo: search the stack for a matching frame.
		// if no matching frame is found, the address can be considered faulty.
		error("Don't know how to handle specific frame address");
		return;
	}

	sCallback = callback;

	ExpressionTree *tree;
	try {
		tree = ExpressionParser::parse(args.c_str());
	} 
	catch(ParseException& e) {
		error("%s", e.what());
		return;
	}

	sExpressionTree = tree;

	loadStack(stackLoaded);
}

void stackEvaluateExpressionTree(ExpressionTree *tree, int frameAddr, ExpressionCallback callback) {
	CHECK_STABS;

	if(frameAddr >= 0) {
		//todo: search the stack for a matching frame.
		// if no matching frame is found, the address can be considered faulty.
		error("Don't know how to handle specific frame address");
		return;
	}

	sCallback = callback;
	sExpressionTree = tree;

	loadStack(stackLoaded);
}

std::string getType(const TypeBase *tb, bool complex) {
	StringPrintFunctor spf;
	tb->printTypeMI(spf, complex);
	return spf.getString();
}

std::string getValue(const TypeBase* tb, const void* addr, TypeBase::PrintFormat fmt) {
	StringPrintFunctor spf;
	tb->printMI(spf, addr, fmt);
	return spf.getString();
}
