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

using namespace MAUtil;
namespace TagNameSpace{
	char *layoutTag="Layout";
	char *labelTag="Label";
	char *buttonTag="Button";
	char *editTag="Edit";
}

/**
 * Constructor
 */
Parser::Parser(int xmlResource, Frame *frame) {
	mFrame=frame;
	mContext.init(NULL, this);
	int len = maGetDataSize(xmlResource);
	char buf[len];
	maReadData(xmlResource, buf, 0, len);
	maWriteLog(buf, len);
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

	if(compareToTag(name, len, TagNameSpace::layoutTag)) {
		l = new Layout(mFrame);
	}

	if(compareToTag(name, len, TagNameSpace::labelTag)) {
		Label *lab = new Label("huhuhu");
		Manager::Instance().addWidget(lab);
		l->addWidget(lab);

	}

	if(compareToTag(name, len, TagNameSpace::editTag)) {
		Edit *edit = new Edit("huhuhu");
		Manager::Instance().addWidget(edit);
		l->addWidget(edit);

	}
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
	if(compareToTag(name, len, TagNameSpace::layoutTag)) {
		l->build();
	}
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
/**
 * Compares a string to a known tag.
 *
 * @param str	Input string
 * @param len	Length of the input string
 * @param tag	Reference tag
 * @return		True if the input string matches
 * 				the reference tag, else false.
 */
bool Parser::compareToTag(const char *str, int len, char *tag) {
	char s[len+1];
	int i;
	for(i=0 ; i<len+1 ; i++) {
		s[i]=str[i];
	}


	bool same = true;
	for(i=0; i<len+1; i++) {
		if(s[i]!=tag[i]) same=false;
	}
	return same;
}


