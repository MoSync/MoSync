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

/** \file Tokenizer.h
*
* Tokenizer (used by MAUtil for XPath expression evaluation)
*
* \author Patrick Broman and Niklas Nummelin
*
*/

#ifndef _SE_MSAB_MAUTIL_TOKENIZER_H_
#define _SE_MSAB_MAUTIL_TOKENIZER_H_

#include "String.h"

namespace MAUtil {

	class TokenMatcher {
	public:
		TokenMatcher(unsigned int type) : type(type) {
		}
		virtual ~TokenMatcher() {}
		virtual unsigned int match(const char* str) = 0;
		unsigned int getType() {
			return mType;
		}
	private:
		unsigned int mType;
	};

	class TokenFixed : public TokenMatcher {
	public:
		TokenFixed(unsigned int type) : TokenMatcher(type) {
		}

		TokenFixed(unsigned int type, const String& s) : TokenMatcher(type) {
			mPatterns.add(s);
		}

		unsigned int match(const char* str) {
			for(int i = 0; i < mPatterns.size(); i++) {
				if(strncmp(mPatterns[i].c_str(), str, mPatterns[i].size())==0) {
					return mPatterns[i].length();
				}
			}

			return 0;
		}
	protected:
		MAUtil::Vector<String> mPatterns;
	};

	class Token {
	public:	
		Token(unsigned int tokenType, const char* start, unsigned int length)
			: mTokenType(tokenType), mStart(start), mLength(length)
		{
		}

		unsigned int getTokenType() {
			return mTokenType;
		}

		const char *getStart() {
			return mStart;
		}

		unsigned getLength() {
			return mLength;
		}

		double toDouble() {
			return stringToDouble(toString());
		}

		int toInt() {
			return stringToInteger(toString());
		}

		String toString() {
			return String(mStart, mLength);
		}
	

	private:	
		unsigned int mTokenType;
		const char *mStart;
		unsigned int mLength;
	};



	class Tokenizer {
	public:

		Tokenizer() {
		}

		bool tokenize(const String& string, Vector<Token*>& result) {
			int position = 0;
			int length = string.length();
			const char *strPtr = string.c_str();
			while(position < length) {	
				while(strPtr[position] == ' ' || strPtr[position] == '\t') position++;
				int matchLen = 0;
				unsigned int tokenType;
				//const char *str = &strPtr[position];
				for(int i = 0; i < mTokenMatchers.size(); i++) {
					int newMatchLen;
					if((newMatchLen=mTokenMatchers[i]->match(&strPtr[position]))) {
						if(newMatchLen>matchLen) {
							matchLen = newMatchLen;
							tokenType = mTokenMatchers[i]->getType();
						}
					}
				}
				if(matchLen == 0) {
					return false;
				} else {
					Token *tok = new Token(tokenType,&strPtr[position],matchLen);
					result.add(tok);
					position+=matchLen;
				}

			}
			return true;
		}

	protected:
		Vector<TokenMatcher*> mTokenMatchers;

	};
}

#endif	//_SE_MSAB_MAUTIL_TOKENIZER_H_
