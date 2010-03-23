/* Copyright (C) 2010 MoSync AB

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
/*
 * Parser.cpp
 *
 *  Created on: Mar 19, 2010
 *      Author: romain
 */

#include "Parser.h"


/**
 * Constructor
 */
Parser::Parser(int xmlResource, Frame *frame) {
	mFrame=frame;
	mContext.init(NULL, this);
	int len = maGetDataSize(xmlResource);
	char buf[len];
	maReadData(xmlResource, buf, 0, len);
	mContext.feed(buf);
}

/**
 * Destructor
 */
Parser::~Parser() {

}

/**
* \see MTXContext::encoding
*/
void Parser::mtxEncoding(const char* value) {

}
/**
* \see MTXContext::tagStart
*/
void Parser::mtxTagStart(const char* name, int len) {
	//printf("s %i: \"%s\"\n", len, name);
	Label *lab = new Label("hahahah");
	Manager::Instance().addWidget(lab);
	Layout *l = new Layout(mFrame);
	l->addWidget(lab);
	l->build();
}
/**
* \see MTXContext::tagAttr
*/
void Parser::mtxTagAttr(const char* attrName, const char* attrValue) {

}
/**
* \see MTXContext::tagStartEnd
*/
void Parser::mtxTagStartEnd() {

}
/**
* \see MTXContext::tagData
*/
void Parser::mtxTagData(const char* data, int len) {

}
/**
* \see MTXContext::tagEnd
*/
void Parser::mtxTagEnd(const char* name, int len) {

}
/**
* \see MTXContext::parseError
*/
void Parser::mtxParseError() {

}
/**
* \see MTXContext::emptyTagEnd
*/
void Parser::mtxEmptyTagEnd() {

}
/**
* \see MTXContext::unicodeCharacter
* \note This function is not pure virtual, like the others.
* Its default implementation calls mtxBasicUnicodeConvert().
*/
unsigned char Parser::mtxUnicodeCharacter(int unicode) {

}



