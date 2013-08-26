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
		if(c == '\n' || c == '\r')
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
	char c =  clearWhitespace(); //sStream->get();
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
