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

#include "Token.h"
#include <stdlib.h>
#include <string.h>

using namespace std;

TokenMatcher::TokenMatcher(unsigned int type) : mType(type) {
}

TokenMatcher::~TokenMatcher() {}

unsigned int TokenMatcher::getType() {
	return mType;
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

Token::Token() : mTokenType(0), mStart(0), mLength(0) {
}

Token::Token(unsigned int tokenType, const char* start, unsigned int length)
: mTokenType(tokenType), mStart(start), mLength(length)
{
}

unsigned int Token::getTokenType() const {
	return mTokenType;
}

const char *Token::getStart() const {
	return mStart;
}

unsigned int Token::getLength() const {
	return mLength;
}

double Token::toDouble() const {
	char temp[16];
	memcpy(temp, mStart, mLength);
	temp[mLength] = 0;
	return atof(temp);
}

int Token::toInt() const {
	char temp[16];
	memcpy(temp, mStart, mLength);
	temp[mLength] = 0;
	return atoi(temp);
}

string Token::toString() const {
	return string(mStart, mLength);
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
	if((c>='a' && c<='z') || (c>='A' && c<='Z') || (c>='0' && c<='9') || (c=='_') || (c=='$') ) return true;
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
