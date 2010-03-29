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

#include <fstream>

#include "tokenizer.h"
#include "stringFunctions.h"
#include "idl-common.h"

using namespace std;


static int sLine;
static istream* sStream(NULL);
static string sComment;

int getCurrentLine() {
	return sLine;
}

string getComment() {
	string temp = sComment;
	sComment.clear();
	return temp;
}

void setTokenStream(istream* is) {
	sStream = is;
	sLine = 1;
	sComment.clear();
}

void Error(const string& type, const string& str) {
	printf("Bad %s on line %i: %s\n", type.c_str(), sLine, str.c_str());
	//printf("stream.good: %i\n", sStream->good());
	throwException("token error");
}

void tokenError(const string& token) {
	Error("token", token);
}

static bool isDelimiter(char c) {
	return c==';'||c=='{'||c=='}'||c=='('||c==')'||c=='='||c==',';
}

static bool isDelimiter(const string& token) {
	if(token.size() == 1)
		return isDelimiter(token[0]);
	return false;
}

static char getCharOnLine() {
	char c = sStream->get();
	if(c == '\n')
		sLine++;
	return c;
}

static char clearWhitespace() {
	char c;
	do {
		c = getCharOnLine();
	} while(sStream->good() && isspace(c));
	return c;
}

void readLine(string& line) {
	line.clear();
	while(true) {
		char c = getCharOnLine();
		if(c == '\n')
			break;
		line += c;
	};
}

void doEOF() {
	clearWhitespace();
	if(!sStream->eof()) {
		string token;
		readToken(token);
		tokenError(token);
	}
}

void readToken(string& token) {	//should handle comments
	token.clear();
	char c = clearWhitespace();
	while(sStream->good()) {
#if 1
		while(c == '/') {  //check for comments, C/C++-style
			char d = getCharOnLine();
			if(d == '/') {  //C++style
				char e = getCharOnLine();
				bool doDoxygen = e == '/';
				if(doDoxygen)
					sComment += "///";
				do {
					c = getCharOnLine();
					if(doDoxygen)
						sComment += c;
				} while(c != '\n');
				c = clearWhitespace();
			} else if(d == '*') { //C-style or Doxygen
				char e = getCharOnLine();
				bool doDoxygen = e == '*';
				if(doDoxygen)
					sComment += "/**";
				while(true) {
					e = getCharOnLine();
					if(doDoxygen && e != '\t')
						sComment += e;
					if(e == '*') {
						e = getCharOnLine();
						if(doDoxygen)
							sComment += e;
						if(e == '/') {
							if(doDoxygen) {
								sComment += '\n';
								//printf("doEOL\n");
							}
							break;
						}	//if(e == '/')
					}	//if(e == '*')
				}	//while(true)
				c = clearWhitespace();
			} else {
				if(d == '\n')
					sLine--;
				sStream->unget();
			}	//if(d == '/')
		}	//while(c == '/')
#endif
		if(isspace(c) || isDelimiter(c)) {
			if(!token.size())
				token += c;
			break;
		}
		token += c;
		c = getCharOnLine();
	}
	if(!isDelimiter(token))
		sStream->unget();
	//printf("Token: \"%s\"\n", token.c_str());
	//if(token.size() == 0)
		//tokenError(token);
}
void readTextToken(string& token) {
	readToken(token);
	if(isDelimiter(token))
		tokenError(token);
}

void readQuotedString(string& token) {
	token.clear();
	char c = sStream->get();
	if(c!='\"') tokenError(token); 
	do {
		c = sStream->get();
		if(c == '\n')
			tokenError(token);
		else if(c=='\"') break;
		token+=c;
	} while(1);
}

void doExact(const string& aToken) {
	string token;
	readToken(token);
	if(token != aToken) {
		tokenError(token);
	}
}
