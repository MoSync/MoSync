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
 * Parser.h
 *
 *  Created on: Mar 19, 2010
 *      Author: romain
 */

#ifndef PARSER_H_
#define PARSER_H_


#include <conprint.h>
#include <maassert.h>
#include "MAHeaders.h"
#include <MTXml/MTXml.h>
#include "Label.h"
#include "Manager.h"
#include "Layout.h"
#include "Frame.h"
#include "Edit.h"
#include <MAUtil/String.h>



/**
 * This class is an XML parser.
 * It is used to parse XML resource files
 * which are used to generate UI layouts.
 */

class Parser : public Mtx::XmlListener {
public:
	/**
	 * Constructor
	 *
	 * @param xmlResource	Handle to the XML resource file
	 * @param frame			Parent frame
	 */
	Parser(int xmlResource, Frame *frame);

	/**
	 * Destructor
	 */
	~Parser();
	/**
	* \see MTXContext::encoding
	*/
	void mtxEncoding(const char* value);
	/**
	* \see MTXContext::tagStart
	*/
	void mtxTagStart(const char* name, int len);
	/**
	* \see MTXContext::tagAttr
	*/
	void mtxTagAttr(const char* attrName, const char* attrValue);
	/**
	* \see MTXContext::tagStartEnd
	*/
	void mtxTagStartEnd();
	/**
	* \see MTXContext::tagData
	*/
	void mtxTagData(const char* data, int len);
	/**
	* \see MTXContext::tagEnd
	*/
	void mtxTagEnd(const char* name, int len);
	/**
	* \see MTXContext::parseError
	*/
	void mtxParseError();
	/**
	* \see MTXContext::emptyTagEnd
	*/
	void mtxEmptyTagEnd();
	/**
	* \see MTXContext::unicodeCharacter
	* \note This function is not pure virtual, like the others.
	* Its default implementation calls mtxBasicUnicodeConvert().
	*/
	unsigned char mtxUnicodeCharacter(int unicode);

	/**
	 * Compares a string to a known tag.
	 *
	 * @param str	Input string
	 * @param len	Length of the input string
	 * @param tag	Reference tag
	 * @return		True if the input string matches
	 * 				the reference tag, else false.
	 */
	bool compareToTag(const char *str, int len, char *tag);

private:
	Mtx::Context mContext;
	Frame *mFrame;
	Layout *l;


};

#endif /* PARSER_H_ */
