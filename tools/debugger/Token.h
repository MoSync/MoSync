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

#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <vector>
#include <string>

enum eTokenType {
	// primary-expression
	TOKEN_NUMBER,
	TOKEN_LITERAL,
	TOKEN_IDENT,
	TOKEN_REG,

	// postfix-expression
	TOKEN_LBRACK,
	TOKEN_RBRACK,
	TOKEN_DOT,
	TOKEN_ARROW,

	// arithmetic
	TOKEN_LPAREN,
	TOKEN_RPAREN,
	TOKEN_PLUS,
	TOKEN_MINUS,
	TOKEN_SLASH,
	TOKEN_STAR,
	TOKEN_PERCENT,

	// bitwise
	TOKEN_SHL,
	TOKEN_SHR,
	TOKEN_AND,
	TOKEN_XOR,
	TOKEN_OR,
	TOKEN_TILDE,

	// boolean
	TOKEN_OROR,
	TOKEN_ANDAND,
	TOKEN_NOT,
	TOKEN_EQ,
	TOKEN_NEQ,
	TOKEN_LEQ,
	TOKEN_GEQ,
	TOKEN_LE,
	TOKEN_GE,

	// keywords
	TOKEN_SIGNED,
	TOKEN_UNSIGNED,
	TOKEN_STRUCT,
	TOKEN_ENUM,
	TOKEN_VOLATILE,
	TOKEN_CONST,
	TOKEN_UNION,
	TOKEN_CLASS,
	TOKEN_LONG,
	TOKEN_SHORT,

	TOKEN_SIZEOF,

	// misc
	TOKEN_QUESTION,
	TOKEN_COLON,

	// GDB specific
	TOKEN_AT,
	
	_TOKEN_END
};

class TokenMatcher {
public:
	TokenMatcher(unsigned int type);
	virtual ~TokenMatcher();
	virtual unsigned int match(const char* str) = 0;
	unsigned int getType();
private:
	unsigned int mType;
};

class TokenFixed : public TokenMatcher {
public:
	TokenFixed(unsigned int type);
	TokenFixed(unsigned int type, const char *s);
	unsigned int match(const char* str);
	void addPattern(const char *str);
protected:
	std::vector<const char*> patterns;
};

class TokenLiteral : public TokenMatcher {
public:
	TokenLiteral();
	unsigned int match(const char* str);
};

class TokenNumber : public TokenMatcher {
private:
	bool isNum(char c);
	bool isHex(char c);

public:
	TokenNumber();
	unsigned int match(const char* str);
};

class TokenIdent : public TokenMatcher {
private:
	bool isValidChar(char c);
	bool isValidFirstChar(char c);

public:
	TokenIdent();
	unsigned int match(const char* str);
};

class TokenReg : public TokenMatcher {
private:
	bool isValidChar(char c);

public:
	TokenReg();
	unsigned int match(const char* str);
};

class Token {
public:
	Token();
	Token(unsigned int tokenType, const char* start, unsigned int length);
	unsigned int getTokenType() const;
	const char *getStart() const;
	unsigned getLength() const;
	double toDouble() const;
	int toInt() const;
	std::string toString() const;
private:
	unsigned int mTokenType;
	const char *mStart;
	unsigned int mLength;
};

#endif // _TOKEN_H_
