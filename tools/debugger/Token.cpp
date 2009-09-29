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

#include "Token.h"

using namespace std;

TokenMatcher::TokenMatcher(unsigned int type) : type(type) {
}

TokenMatcher::~TokenMatcher() {}

unsigned int TokenMatcher::getType() {
	return type;
}


TokenFixed::TokenFixed(unsigned int type) : TokenMatcher(type) {
}

TokenFixed::TokenFixed(unsigned int type, const char *s) : TokenMatcher(type) {
	patterns.push_back(s);
}

unsigned int TokenFixed::match(const char* str) {
	for(unsigned int i = 0; i < patterns.size(); i++) {
		int len = strlen(patterns[i]);
		if(strncmp(patterns[i], str, len)==0) {
			return len;
		}
	}

	return 0;
}

void TokenFixed::addPattern(const char* str) {
	patterns.push_back(str);
}

Token::Token() : tokenType(0), start(0), length(0) {
}

Token::Token(unsigned int tokenType, const char* start, unsigned int length)
: tokenType(tokenType),	start(start), length(length)
{
}

unsigned int Token::getTokenType() const {
	return tokenType;
}

const char *Token::getStart() const {
	return start;
}

unsigned int Token::getLength() const {
	return length;
}

double Token::toDouble() const {
	char temp[16];
	memcpy(temp, start, length);
	temp[length] = 0;
	return atof(temp);
}

int Token::toInt() const {
	char temp[16];
	memcpy(temp, start, length);
	temp[length] = 0;
	return atoi(temp);
}

string Token::toString() const {
	return string(start, length);
}

TokenLiteral::TokenLiteral() : TokenMatcher(TOKEN_LITERAL) {
}

unsigned int TokenLiteral::match(const char* str) {
	const char* initialStr = str;
	char firstType;
	if((*str)!='\"' && (*str)!='\'') return 0;
	firstType = *str;
	str++;
	while(*str != firstType) {
		str++;
	}
	str++;
	return (unsigned int)(str-initialStr);
}

bool TokenNumber::isNum(char c) {
	if(c>='0' && c<='9') return true;
	else return false;
}

bool TokenNumber::isHex(char c) {
	return isNum(c)||(c>='a'&&c<='f')||(c>='A'&&c<='F');
}

TokenNumber::TokenNumber() : TokenMatcher(TOKEN_NUMBER) {
}

unsigned int TokenNumber::match(const char* str) {
	const char* initialStr = str;

	if(!isNum(*str)) {
		return 0;
	}
	if(*str=='0') {
		str++;
		if(*str && *str=='x') {
			str++;
			if(!isHex(*str)) {
				return 0;
			}
			while(*str && (isHex(*str))) {
				str++;
			}
			return (unsigned int)(str-initialStr);
		}
	}
	else str++;

	while(*str && isNum(*str)) {
		str++;
	}

	if((*str)=='.') {
		str++;

		if((*str)&&(*str=='f')) {
			str++; 
			return (unsigned int)(str-initialStr);;
		}

		if(!isNum(*str)) {
			return 0;
		}
		while(*str && isNum(*str)) {
			str++;
		}

		if(*str && *str=='f') str++;
	}
	return (unsigned int)(str-initialStr);
}

bool TokenIdent::isValidChar(char c) {
	if((c>='a' && c<='z') || (c>='A' && c<='Z') || (c>='0' && c<='9') || (c=='_') ) return true;
	else return false;
}

bool TokenIdent::isValidFirstChar(char c) {
	if((c>='a' && c<='z') || (c>='A' && c<='Z') || (c=='_') || (c=='$')) return true;
	else return false;
}

TokenIdent::TokenIdent() : TokenMatcher(TOKEN_IDENT) {
}

unsigned int TokenIdent::match(const char* str) {
	const char* initialStr = str;
	if(!isValidFirstChar(*str))	return 0;
	str++;
	while(isValidChar(*str)) {
		str++;
	}
	return (unsigned int)(str-initialStr);
}


TokenReg::TokenReg() : TokenMatcher(TOKEN_REG) {
}

bool TokenReg::isValidChar(char c) {
	if((c>='a' && c<='z') || (c>='A' && c<='Z') || (c>='0' && c<='9') || (c=='_') ) return true;
	else return false;
}

unsigned int TokenReg::match(const char* str) {
	const char* initialStr = str;
	if(*str != '$')	return 0;
	str++;
	while(isValidChar(*str)) {
		str++;
	}
	return (unsigned int)(str-initialStr);
}
