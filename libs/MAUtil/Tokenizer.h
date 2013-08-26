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

/** \file Tokenizer.h
*
* \brief Tokenizer used by MAUtil for XPath expression evaluation
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
