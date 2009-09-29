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

#include <maassert.h>
#include <mactype.h>
#include <mavsprintf.h>
#include <mastdlib.h>

#include "MTXml.h"

//******************************************************************************
// Declarations
//******************************************************************************

enum ProcessErrorCode {
	//PEC_SUCCESS = -1,
	PEC_INVALID = -2,
	PEC_INCOMPLETE = -3,
	PEC_OUTSIDE = -4	//unicode value outside Latin-1
};

struct ATTRIBUTE {
	char* namePtr;
	char* valuePtr;
};

//processes UTF-8 of the data if the CONTEXT says the document uses that encoding.
//also processes standard entities if(ent).
//return <0 on error.
//if any data was successfully processed, return the length of it.
//remainsLen = 0;
//if any data remains, set remainsLen to the length of it and set sCurPtr to the start of it.
template<bool ent> static int proc(char* data, int* remainsLen);

//does processing, fires parseError on remain, returns length or PEC error.
template<bool ent> static int procComplete(char* data);

//returns a value >0 representing the reference on success.
//returns <0 on failure.
//*pnBytes is valid on success and PEC_INCOMPLETE.
static int parseRef(char* ref, int* pnBytes);

//returns the converted character, or a PEC code.
//*pnBytes is valid on success and PEC_INCOMPLETE.
static int convertUtf8ToLatin1(const char* utf8, int* pnBytes);

template<bool process> static void parse();
static bool skipWhiteSpace();	//returns true if a non-whitespace character is found.
template<bool process> static void parseOutsideTag();
template<bool process> static void parseTagStart();
template<bool process> static void parseInsideTag();
template<bool process> static void parseCDATA();
static void parseSpecialTag();
static void parseComment();

//sets at least one pointer to NULL on failure or incompleteness.
//calls fireParseError() and fireDataRemains() as appropriate.
//sets pointers on success, and null-terminates the strings.
static void parseAttribute(ATTRIBUTE* a);

//could be processed for UTF-8, but we just ignore non-prolog PIs now, so it'd be no use.
static void parseProcessingInstruction();

//scans for a character
//returns the index of the character matching \a c
//returns <0 if not found until end-of-buffer
//updates sCurPtr to point to the end of buffer, in that case.
static int find(char c);


//for *String functions:
//on match, returns 0 and sets sCurPtr to point to the character succeeding \a str.
//on partial match with end-of-buffer, returns the length of the partial match.
//sets sCurPtr to point to the beginning of the match, in that case.
//returns <0 if no match, set sCurPtr to point to the end of buffer.

//compares sCurPtr with a string
//returns <0 if sCurPtr doesn't match str right away
static int matchString(const char* str);

//returns <0 if not found until end-of-buffer
static int findString(const char* str);


//assumes sCurPtr points to the beginning of an XML Name
//scans for the end of that Name
//returns the index of the character succeeding the Name
//returns 0 if no end was found. updates sCurPtr to point to the end of data in that case.
//returns <0 if there's no data available.
static int findNameEnd();	//tag or attr names

static void fireEncoding(char* name);
template<bool process> static void fireTagStart(char* name, int len);
template<bool process> static void fireTagAttr(char* name, char* value);
template<bool process> static void fireTagEnd(char* name, int len);
template<bool utf8, bool ent> static void fireTagData(char* data, int len);
static void fireEmptyTagEnd();
static void fireParseError();
static void fireDataRemains(int len);	//reports len bytes starting at sCurPtr
static void fireDataRemains(char* resetPtr);	//reports (sCurPtr - resetPtr) bytes starting at resetPtr

enum State {
	EStart, EOutsideTag, EInsideTag, ETagStart, ECDATA, EComment, EProcessingInstruction
};

#define DECLARE_STATIC(type, name) static type s##name;
#define LOAD_CONTEXT(type, name) s##name = sContext->i##name;
#define STORE_CONTEXT(type, name) sContext->i##name = s##name;

CONTEXT_INTERNAL_VARIABLES(DECLARE_STATIC);

static char* sCurPtr;
static MTXContext* sContext = NULL;
static bool sThereIsData;
static char* sLastBeginPtr;
static char* sFirstPtr;
static bool sStop;


//******************************************************************************
// Entities
//******************************************************************************
struct ENTITY {
	const char* name;

	//could be byte instead of int on IA32, but since PIP-E requires word alignment,
	//we wouldn't save any space and would lose performance on Java machines.
	int value;
};

//This is the Latin-1 subset of a combined HTML4.01/XML1.0 predefined entity set.
static const ENTITY sEntities[] = {
	{ "quot", 0x22 },
	{ "amp", 0x26 },
	{ "apos", 0x27 },
	{ "lt", 0x3C },
	{ "gt", 0x3E },
	{ "nbsp", 0xA0 },
	{ "iexcl", 0xA1 },
	{ "cent", 0xA2 },
	{ "pound", 0xA3 },
	{ "curren", 0xA4 },
	{ "yen", 0xA5 },
	{ "brvbar", 0xA6 },
	{ "sect", 0xA7 },
	{ "uml", 0xA8 },
	{ "copy", 0xA9 },
	{ "ordf", 0xAA },
	{ "laquo", 0xAB },
	{ "not", 0xAC },
	{ "shy", 0xAD },
	{ "reg", 0xAE },
	{ "macr", 0xAF },
	{ "deg", 0xB0 },
	{ "plusmn", 0xB1 },
	{ "sup2", 0xB2 },
	{ "sup3", 0xB3 },
	{ "acute", 0xB4 },
	{ "micro", 0xB5 },
	{ "para", 0xB6 },
	{ "middot", 0xB7 },
	{ "cedil", 0xB8 },
	{ "sup1", 0xB9 },
	{ "ordm", 0xBA },
	{ "raquo", 0xBB },
	{ "frac14", 0xBC },
	{ "frac12", 0xBD },
	{ "frac34", 0xBE },
	{ "iquest", 0xBF },
	{ "Agrave", 0xC0 },
	{ "Aacute", 0xC1 },
	{ "Acirc", 0xC2 },
	{ "Atilde", 0xC3 },
	{ "Auml", 0xC4 },
	{ "Aring", 0xC5 },
	{ "AElig", 0xC6 },
	{ "Ccedil", 0xC7 },
	{ "Egrave", 0xC8 },
	{ "Eacute", 0xC9 },
	{ "Ecirc", 0xCA },
	{ "Euml", 0xCB },
	{ "Igrave", 0xCC },
	{ "Iacute", 0xCD },
	{ "Icirc", 0xCE },
	{ "Iuml", 0xCF },
	{ "ETH", 0xD0 },
	{ "Ntilde", 0xD1 },
	{ "Ograve", 0xD2 },
	{ "Oacute", 0xD3 },
	{ "Ocirc", 0xD4 },
	{ "Otilde", 0xD5 },
	{ "Ouml", 0xD6 },
	{ "times", 0xD7 },
	{ "Oslash", 0xD8 },
	{ "Ugrave", 0xD9 },
	{ "Uacute", 0xDA },
	{ "Ucirc", 0xDB },
	{ "Uuml", 0xDC },
	{ "Yacute", 0xDD },
	{ "THORN", 0xDE },
	{ "szlig", 0xDF },
	{ "agrave", 0xE0 },
	{ "aacute", 0xE1 },
	{ "acirc", 0xE2 },
	{ "atilde", 0xE3 },
	{ "auml", 0xE4 },
	{ "aring", 0xE5 },
	{ "aelig", 0xE6 },
	{ "ccedil", 0xE7 },
	{ "egrave", 0xE8 },
	{ "eacute", 0xE9 },
	{ "ecirc", 0xEA },
	{ "euml", 0xEB },
	{ "igrave", 0xEC },
	{ "iacute", 0xED },
	{ "icirc", 0xEE },
	{ "iuml", 0xEF },
	{ "eth", 0xF0 },
	{ "ntilde", 0xF1 },
	{ "ograve", 0xF2 },
	{ "oacute", 0xF3 },
	{ "ocirc", 0xF4 },
	{ "otilde", 0xF5 },
	{ "ouml", 0xF6 },
	{ "divide", 0xF7 },
	{ "oslash", 0xF8 },
	{ "ugrave", 0xF9 },
	{ "uacute", 0xFA },
	{ "ucirc", 0xFB },
	{ "uuml", 0xFC },
	{ "yacute", 0xFD },
	{ "thorn", 0xFE },
	{ "yuml", 0xFF }
};

static const int sNEntities = sizeof(sEntities) / sizeof(ENTITY);

//******************************************************************************
// Functions
//******************************************************************************

extern "C" int mtxProcess(MTXContext* context, char* data) {
	//save old context
	if(sContext != NULL) {
		CONTEXT_INTERNAL_VARIABLES(STORE_CONTEXT);
	}
	MTXContext* oldContext = sContext;

	//load our context
	sContext = context;
	CONTEXT_INTERNAL_VARIABLES(LOAD_CONTEXT);

	//process data
	int remainLen;
	int res = proc<true>(data, &remainLen);
	if(remainLen > 0) {
		res = PEC_INCOMPLETE;
	}

	//restore old context, disregard changed wrought by proc().
	sContext = oldContext;
	if(sContext != NULL) {
		CONTEXT_INTERNAL_VARIABLES(LOAD_CONTEXT);
	}
	return res;
}

extern "C" void mtxStart(MTXContext* context) {
	context->iState = EStart;
	context->iUtf8 = TRUE;
}

template<bool process> static int feed(MTXContext* context, char* data) {
	ASSERT_MSG(sContext == NULL, "MTXml cannot be called recursively");
	sContext = context;
	sCurPtr = data;
	sThereIsData = true;
	sStop = false;
	CONTEXT_INTERNAL_VARIABLES(LOAD_CONTEXT);
	sFirstPtr = data;
	do {
		sLastBeginPtr = sCurPtr;
		//lprintfln("parse %i \"%s\"\n", sState, sCurPtr);
		parse<process>();
	} while(sThereIsData && !sStop);
	if(!sStop) {
		CONTEXT_INTERNAL_VARIABLES(STORE_CONTEXT);
	}
	sContext = NULL;
	return sStop;
}

extern "C" int mtxFeed(MTXContext* context, char* data) {
	return feed<false>(context, data);
}

extern "C" int mtxFeedProcess(MTXContext* context, char* data) {
	return feed<true>(context, data);
}

extern "C" void mtxStop(MTXContext* context) {
	if(sContext == context)
		sStop = true;
}

static void fireDataRemains(int len) {
	if(sStop)
		return;
	sThereIsData = false;
	sContext->dataRemains(sContext, sCurPtr, len);
}
static void fireDataRemains(char* resetPtr) {
	char* endPtr = sCurPtr;
	sCurPtr = resetPtr;
	fireDataRemains(endPtr - sCurPtr);
}

static void fireTagStartEnd() {
	if(sStop)
		return;
	sContext->tagStartEnd(sContext);
}

static void fireParseError() {
	if(sStop)
		return;
	sThereIsData = false;
	sContext->parseError(sContext);
}

static void fireEncoding(char* name) {
	if(sStop)
		return;
	sContext->encoding(sContext, name);
}

template<bool process> static void fireTagStart(char* name, int len) {
	if(sStop)
		return;
	if(process) {
		len = procComplete<false>(name);
		if(len <= 0) {
			return;
		}
	}
	sContext->tagStart(sContext, name, len);
}

template<bool process> static void fireTagAttr(char* name, char* value) {
	if(sStop)
		return;
	if(process) {
		int len = procComplete<false>(name);
		if(len <= 0) {
			return;
		}
		len = procComplete<true>(value);
		if(len < 0) {
			return;
		}
	}
	sContext->tagAttr(sContext, name, value);
}

template<bool process> static void fireTagEnd(char* name, int len) {
	if(sStop)
		return;
	if(process) {
		len = procComplete<false>(name);
		if(len <= 0) {
			return;
		}
	}
	sContext->tagEnd(sContext, name, len);
}

template<bool utf8, bool ent> static void fireTagData(char* data, int len) {
	if(sStop)
		return;
	if(utf8) {
		len = procComplete<ent>(data);
		if(len <= 0) {
			return;
		}
	}
	sContext->tagData(sContext, data, len);
}

static void fireEmptyTagEnd() {
	if(sStop)
		return;
	sContext->emptyTagEnd(sContext);
}

template<bool process> static void parse() {
	switch(sState) {
	case EStart:
		if(!skipWhiteSpace())
			return;
		sState = EOutsideTag;
		break;
	case EOutsideTag:
		//handle data or tag-start
		parseOutsideTag<process>();
		break;
	case ETagStart:
		parseTagStart<process>();
		break;
	case EInsideTag:
		//handle attribute or tag-end	(not end-tag)
		parseInsideTag<process>();
		break;
	case ECDATA:
		//handle CDATA or CDATA-end
		parseCDATA<process>();
		break;
	case EComment:
		parseComment();
		break;
	case EProcessingInstruction:
		parseProcessingInstruction();
		break;
	default:
		PANIC_MESSAGE("unknown state");
	}
}

template<bool process> static void parseOutsideTag() {
	char* start = sCurPtr;
	int index = find('<');
	if(index > 0) {
		sCurPtr[index] = 0;
		fireTagData<process, process>(sCurPtr, index);
		sCurPtr += index;
	} else if(index < 0) {
		int len = sCurPtr - start;
		if(len > 0) {
#if 0	//not optimal
			if(process && (refIndex = findIncompleteReference(start)) >= 0) {
				start[refIndex] = 0;
				fireTagData<process, process>(start, refIndex);
				start[refIndex] = '&';
				fireDataRemains(start + refIndex);
#else
			if(process) {
#if 0
				int refIndex = proc<true>(start);
				if(refIndex >= 0) {	//partial reference
					if(refIndex > 0) {
						start[refIndex] = 0;
						fireTagData<false, false>(start, refIndex);
						start[refIndex] = '&';
					}
					fireDataRemains(start + refIndex);
				} else if(refIndex == PEC_SUCCESS) {
					fireTagData<false, false>(start, refIndex);
				} else {
				}
#else
				//usage prototype
				int remainsLen;
				int res = proc<true>(start, &remainsLen);
				if(res > 0)	//we got some proper data to report
					fireTagData<false, false>(start, res);
				if(res < 0) {
					fireParseError();
				} else if(remainsLen)
					fireDataRemains(remainsLen);
#endif	//0
			} else {	//(process)
				fireTagData<process, process>(start, len);
			}
#endif	//0
		}	//(len > 0)
		sThereIsData = false;
		return;
	}
	sCurPtr++;
	sState = ETagStart;
}

template<bool process> static void parseTagStart() {
	bool endTag;
	int index;
	switch(*sCurPtr) {
	case '!':
		sCurPtr++;
		parseSpecialTag();
		return;
	case '?':
		sCurPtr++;
		sState = EProcessingInstruction;
		return;
	case '/':
		sCurPtr++;
		endTag = true;
		break;
	default:
		endTag = false;
	}
	if(endTag && *sCurPtr == 0) {	//hack-fix
		fireDataRemains(sCurPtr - 1);
		return;
	}
	index = findNameEnd();
	if(index == 0)
		fireDataRemains(sLastBeginPtr);
	if(index <= 0)
		return;
	if(endTag) {
		char* namePtr = sCurPtr;
		sCurPtr += index;
		if(!skipWhiteSpace())
			return;
		if(*sCurPtr != '>') {
			fireParseError();
			return;
		}
		sCurPtr++;
		namePtr[index] = 0;
		fireTagEnd<process>(namePtr, index);
		sState = EOutsideTag;
	} else {
		if(sCurPtr[index] == '>') {
			sState = EOutsideTag;
		} else {
			sState = EInsideTag;
			if(sCurPtr[index] == '/')
				endTag = true;
		}
		sCurPtr[index] = 0;
		fireTagStart<process>(sCurPtr, index);
		if(sState == EOutsideTag) {
			fireTagStartEnd();
		}
		sCurPtr += index;
		if(endTag) {
			*sCurPtr = '/';
		} else {
			sCurPtr++;
		}
	}
}

template<bool process> static void parseInsideTag() {
	if(!skipWhiteSpace())
		return;
	if(*sCurPtr == '/') {
		sCurPtr++;
		if(*sCurPtr == 0) {
			fireDataRemains(sCurPtr - 1);
			return;
		}
		if(*sCurPtr != '>') {
			fireParseError();
			return;
		}
		fireEmptyTagEnd();
	}
	if(*sCurPtr == '>') {
		sState = EOutsideTag;
		fireTagStartEnd();
		sCurPtr++;
		return;
	}
	ATTRIBUTE a;
	parseAttribute(&a);
	if(a.namePtr && a.valuePtr)
		fireTagAttr<process>(a.namePtr, a.valuePtr);
}

static void parseAttribute(ATTRIBUTE* a) {
	a->namePtr = NULL;
	a->valuePtr = NULL;
	//attribute
	int attrNameLen = findNameEnd();
	if(attrNameLen == 0)
		fireDataRemains(sLastBeginPtr);
	if(attrNameLen <= 0)
		return;
	a->namePtr = sCurPtr;
	sCurPtr += attrNameLen;
	if(!skipWhiteSpace()) {
		fireDataRemains(a->namePtr);
		return;
	}
	if(*sCurPtr != '=') {
		fireParseError();
		return;
	}
	sCurPtr++;
	if(!skipWhiteSpace()) {
		fireDataRemains(a->namePtr);
		return;
	}
	if(*sCurPtr != '\'' && *sCurPtr != '\"') {
		fireParseError();
		return;
	}
	char terminator = *sCurPtr++;
	a->valuePtr = sCurPtr;
	int attrValueLen = find(terminator);
	if(attrValueLen < 0) {
		fireDataRemains(a->namePtr);
		a->valuePtr = NULL;
		return;
	}
	a->namePtr[attrNameLen] = 0;
	a->valuePtr[attrValueLen] = 0;
	sCurPtr += attrValueLen + 1;
}

static void parseSpecialTag() {
	int res = matchString("[CDATA[");
	if(res == 0) {
		sState = ECDATA;
		return;
	} else if(res > 0) {	//partial match, then end-of-buffer
		sCurPtr += res;
		fireDataRemains(sLastBeginPtr);
		return;
	}
	res = matchString("--");
	if(res == 0) {
		sState = EComment;
		return;
	} else if(res > 0) {	//partial match, then end-of-buffer
		sCurPtr += res;
		fireDataRemains(sLastBeginPtr);
		return;
	}
	if(*sCurPtr == 0) {
		fireDataRemains(sLastBeginPtr);
		return;
	}
	fireParseError();
}

template<bool process> static void parseCDATA() {
	int res = findString("]]>");
	if(res == 0) {
		//strlen("]]>") == 3
		char* endPtr = sCurPtr - 3;
		if(endPtr - sLastBeginPtr != 0) {
			*endPtr = 0;
//#error
			fireTagData<process, false>(sLastBeginPtr, endPtr - sLastBeginPtr);
		}
		sState = EOutsideTag;
	} else if(res > 0) {	//partial match, then end-of-buffer
		fireDataRemains(res);
	} else {	//res < 0
		if(sCurPtr - sLastBeginPtr != 0) {
			fireTagData<process, false>(sLastBeginPtr, sCurPtr - sLastBeginPtr);
		}
		sThereIsData = false;
	}
}

static void parseProcessingInstruction() {
	char* start = sCurPtr;
	int res = findString("?>");
	if(res == 0) {	//found it
		char* end = sCurPtr;
		if(tolower(start[0]) == 'x' && tolower(start[1]) == 'm' && tolower(start[2]) == 'l' &&
			isspace(start[3]))
		{
			//xmlDecl
			sCurPtr = start + 3;
			while(1) {
				skipWhiteSpace();
				if(*sCurPtr == '?') {
					sCurPtr = end;
					break;
				}
				ATTRIBUTE a;
				parseAttribute(&a);
				if(a.namePtr == NULL || a.valuePtr == NULL) {
					fireParseError();
					return;
				}
				if(strcmp(a.namePtr, "encoding") == 0) {
					if(stricmp(a.valuePtr, "UTF-8") != 0) {
						sUtf8 = FALSE;
					}
					fireEncoding(a.valuePtr);
				}
			}
		}
		sState = EOutsideTag;
	} else if(res > 0) {	//partial match, then end-of-buffer
		sCurPtr += res;
		fireDataRemains(start);
	} else if(*start != 0) {	//res < 0
		fireDataRemains(start);
	} else {
		sThereIsData = false;
	}
}

static void parseComment() {
	int res = findString("-->");
	if(res == 0) {	//found it
		sState = EOutsideTag;
	} else if(res > 0) {	//partial match, then end-of-buffer
		fireDataRemains(res);
	} else {	//res < 0
		sThereIsData = false;
	}
}

static int find(char c) {
	int i=0;
	while(sCurPtr[i] != 0) {
		if(sCurPtr[i] == c)
			return i;
		i++;
	}
	sCurPtr += i;
	return -1;
}

static int matchString(const char* str) {
	int i = 0;
	int partialStart = -1;
	int index = 0;
	while(sCurPtr[index] != 0) {
		if(str[i] == 0) {
			sCurPtr += index;
			return 0;
		}
		if(sCurPtr[index] != str[i])
			break;
		if(i == 0)
			partialStart = index;
		index++;
		i++;
	}
	if(partialStart > 0)
		sCurPtr += partialStart;
	if(partialStart < 0)
		return partialStart;
	else
		return index - partialStart;
}

static int findString(const char* str) {
	int i = 0;
	int lastPartialStart = -1;
	int index = 0;
	while(sCurPtr[index] != 0) {
		if(str[i] == 0) {
			sCurPtr += index;
			return 0;
		}
		if(sCurPtr[index] != str[i]) {
			if(lastPartialStart >= 0) {
				i = 0;
				//go back to character directly after start of last partial match.
				index = lastPartialStart + 1;
				lastPartialStart = -1;
			} else {
				index++;
			}
			continue;
		}
		if(i == 0)
			lastPartialStart = index;
		index++;
		i++;
	}
	if(lastPartialStart > 0)
		sCurPtr += lastPartialStart;
	if(lastPartialStart < 0) {
		sCurPtr += index;
		return lastPartialStart;
	}
	return index - lastPartialStart;
}

static int findNameEnd() {
	int index = -1;
	int i=0;
	while(sCurPtr[i] != 0) {
		byte c = sCurPtr[i];
		if(isspace(c) || c == '>' || c == '/' || c == '=' || c == '[') {
			index = i;
			break;
		}
		i++;
	}

	if(index < 0) {
		if(i == 0) {
			if(sCurPtr[i] != 0)
				fireParseError();
			else
				sThereIsData = false;
		} else {
			sCurPtr += i;
			return 0;
		}
	}
	return index;
}

static bool skipWhiteSpace() {
	while(*sCurPtr != 0) {
		if(!isspace((byte)*sCurPtr))
			return true;
		sCurPtr++;
	}
	sThereIsData = false;
	return false;
}

#if 0	//copy from above
int remainsLen;
int res = proc<true>(start, &remainsLen);
if(res > 0)	//we got some proper data to report
	fireTagData<false, false>(start, res);
if(res < 0) {
	fireParseError();
} else if(remainsLen)
	fireDataRemains(remainsLen);

//processes UTF-8 of the data if the CONTEXT says the document uses that encoding.
//also processes standard entities if(ent).
//return <0 on error.
//if any data was successfully processed, return the length of it.
//remainsLen = 0;
//if any data remains, set remainsLen to the length of it and set sCurPtr to the start of it.
#endif
//for some functions, remaining data will mean error.
//remainsLen is partially used as "refLen" and "utf8Len" inside this function.
template<bool ent> static int proc(char* data, int* remainsLen) {
	char* dst = data;
	char* src = data;
	*remainsLen = 0;
	while(*src != 0) {
		int res = 0;
		if(ent && *src == '&') {	//reference
			src++;
			res = parseRef(src, remainsLen);
			if(res == PEC_INCOMPLETE) {
				sCurPtr = src - 1;
				break;
			} else if(res < 0)
				return res;
		} else if(sUtf8) {	//normal character
			res = convertUtf8ToLatin1(src, remainsLen);
			if(res == PEC_INCOMPLETE) {
				sCurPtr = src;
				break;
			} else if(res < 0)
				return res;
		}
		if(res != 0) {
			*(dst++) = (char)res;
			src += *remainsLen;
			*remainsLen = 0;
		} else {
			*(dst++) = *(src++);
		}
	}
	*dst = 0;
	return dst - data;
}

template<bool ent> static int procComplete(char* data) {
	int remainLen;
	int res = proc<ent>(data, &remainLen);
	if(remainLen > 0) {
		res = PEC_INCOMPLETE;
	}
	if(res < 0) {
		fireParseError();
	}
	return res;
}

static int parseRef(char* ref, int* pnBytes) {
	char* end = strchr(ref, ';');
	if(end == NULL)
		return PEC_INCOMPLETE;
	*pnBytes = (end - ref) + 1;
	*end = 0;
	if(ref[0] == '#') {	//character reference
		if(ref[1] == 'x') {	//hexadecimal
			return axtoi(ref + 2);
		} else {	//decimal
			return atoi(ref + 1);
		}
	} else {	//standard reference
		for(int i=0; i<sNEntities; i++) {
			if(strcmp(sEntities[i].name, ref) == 0) {
				return sEntities[i].value;
			}
		}
		return PEC_INVALID;
	}
}

static int convertUtf8ToLatin1(const char* utf8, int* pnBytes) {
	char b = utf8[0];
	if(b & 0x80) {
		int nBytes = 0, unicode, i;
		do {
			nBytes++;
			b <<= 1;
		} while(b & 0x80);
		if(nBytes > 4) {
			//printf("CU8b: %i", nBytes);
			return PEC_INVALID;
		}
		unicode = (b >> nBytes) << ((nBytes-1) * 6);
		for(i=0; i<nBytes-1; i++) {
			if(utf8[i+1] == 0) {
				*pnBytes = i+1;
				return PEC_INCOMPLETE;
			}
			unicode |= (utf8[i+1] & 0x3F) << ((nBytes-2-i) * 6);
		}
		if(unicode > 0xFF) {
			//0xFEFF is ZERO WIDTH NO-BREAK SPACE, an encoding signature.
			if(unicode == 0xFEFF) {
				unicode = ' ';	//not a proper translation, but it shouldn't cause any problems.
			} else {
				//printf("CU8u: %i", unicode);
				unicode = sContext->unicodeCharacter(sContext, unicode);
				if(unicode == 0)
					return PEC_OUTSIDE;
			}
		}
		*pnBytes = nBytes;
		return unicode;
	} else {
		*pnBytes = 1;
		return b;
	}
}

// see the official Unicode standards document
// http://www.unicode.org/Public/5.1.0/charts/
extern "C" unsigned char mtxBasicUnicodeConvert(MTXContext*, int unicode) {
	unsigned char result;
	switch(unicode) {
	case 0x201C:	//LEFT DOUBLE QUOTATION MARK
	case 0x201D:	//RIGHT DOUBLE QUOTATION MARK
	case 0x201E:	//DOUBLE LOW-9 QUOTATION MARK
	case 0x201F:	//DOUBLE HIGH-REVERSED-9 QUOTATION MARK
		result = '\"';	//QUOTATION MARK
		break;
	case 0x2018:	//LEFT SINGLE QUOTATION MARK
	case 0x2019:	//RIGHT SINGLE QUOTATION MARK
	case 0x201A:	//SINGLE LOW-9 QUOTATION MARK
	case 0x201B:	//SINGLE HIGH-REVERSED-9 QUOTATION MARK
		result = '\'';
		break;
	case 0x2010:	//HYPHEN
	case 0x2011:	//NON-BREAKING HYPHEN
	case 0x2012:	//FIGURE DASH
	case 0x2013:	//EN DASH
	case 0x2014:	//EM DASH
	case 0x2015:	//HORIZONTAL BAR
		result = '-';	//HYPHEN-MINUS
		break;
	case 0x2000:	//EN QUAD
	case 0x2001:	//EM QUAD
	case 0x2002:	//EN SPACE
	case 0x2003:	//EM SPACE
	case 0x2004:	//THREE-PER-EM SPACE
	case 0x2005:	//FOUR-PER-EM SPACE
	case 0x2006:	//SIX-PER-EM SPACE
	case 0x2007:	//FIGURE SPACE
	case 0x2008:	//PUCTUATION SPACE
	case 0x2009:	//THIN SPACE
	case 0x200A:	//HAIR SPACE
		result = ' ';	//SPACE
		break;
	case 0x2022:	//BULLET
		result = 0xB7;	//MIDDLE DOT
		break;
	case 0x20AC:	//EURO SIGN
		result = 0x80;
		break;
	case 0x2122:	//TRADE MARK SIGN
		result = 0x99;
		break;
	default:
		result = 0;
	}
	return result;
}

//******************************************************************************
// C++ wrapper
//******************************************************************************
namespace Mtx {
	static MtxListener* sMtx;
	static XmlListener* sXml;

	static void encoding(MTXContext*, const char* name) {
		sXml->mtxEncoding(name);
	}
	static void tagStart(MTXContext*, const char* name, int len) {
		sXml->mtxTagStart(name, len);
	}
	static void tagAttr(MTXContext*, const char* attrName, const char* attrValue) {
		sXml->mtxTagAttr(attrName, attrValue);
	}
	static void tagStartEnd(MTXContext*) {
		sXml->mtxTagStartEnd();
	}
	static void tagData(MTXContext*, const char* data, int len) {
		sXml->mtxTagData(data, len);
	}
	static void tagEnd(MTXContext*, const char* name, int len) {
		sXml->mtxTagEnd(name, len);
	}
	static void emptyTagEnd(MTXContext*) {
		sXml->mtxEmptyTagEnd();
	}
	static void parseError(MTXContext*) {
		sXml->mtxParseError();
	}
	static unsigned char unicodeCharacter(MTXContext*, int unicode) {
		return sXml->mtxUnicodeCharacter(unicode);
	}

	static void dataRemains(MTXContext*, const char* data, int len) {
		sMtx->mtxDataRemains(data, len);
	}

	unsigned char XmlListener::mtxUnicodeCharacter(int unicode) {
		return mtxBasicUnicodeConvert(NULL, unicode);
	}

	void Context::init(MtxListener* mtx, XmlListener* xml) {
		mContext.encoding = encoding;
		mContext.tagStart = tagStart;
		mContext.tagStartEnd = tagStartEnd;
		mContext.tagAttr = tagAttr;
		mContext.tagData = tagData;
		mContext.tagEnd = tagEnd;
		mContext.emptyTagEnd = emptyTagEnd;
		mContext.dataRemains = dataRemains;
		mContext.parseError = parseError;
		mContext.unicodeCharacter = unicodeCharacter;

		//mContext.userData = this;	//not used

		mMtx = mtx;
		mXml = xml;

		mtxStart(&mContext);
	}

	bool Context::feed(char* data) {
		sMtx = mMtx;
		sXml = mXml;
		return !!mtxFeed(&mContext, data);
	}

	bool Context::feedProcess(char* data) {
		sMtx = mMtx;
		sXml = mXml;
		return !!mtxFeedProcess(&mContext, data);
	}

	void Context::stop() {
		mtxStop(&mContext);
	}

	int Context::process(char* data) {
		sXml = mXml;
		return mtxProcess(&mContext, data);
	}
}
