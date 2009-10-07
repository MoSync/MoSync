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

/**
* \file MTSax.cpp
* \brief MoSync Tiny SAX parser
*/

#include "MTXml.h"
#include "MTSax.h"

#include <maheap.h>
#include <mastring.h>

namespace
{

void flushAttributes(MTXSaxContext* context)
{
	// Make pointers out of the saved offsets
	for (int i = 0;  i < context->attrCount * 2;  i++)
		context->attrIndex[i] += (int) context->attrBuf;

	// Add the list end markers
	context->attrIndex[context->attrCount * 2] = NULL;
	context->attrIndex[context->attrCount * 2 + 1] = NULL;

	context->startElement(context, context->name, context->attrIndex);

	context->attrCount = 0;
	context->attrBufUsed = 0;
}

void encoding(MTXContext* context, const char* value)
{
	MTXSaxContext* outer = (MTXSaxContext*) context->userData;
	outer->encoding(outer, value);
}

void tagStart(MTXContext* context, const char* name, int length)
{
	MTXSaxContext* outer = (MTXSaxContext*) context->userData;

	if (outer->nameSize <= (int) length)
	{
		/* Add a bit extra to not fragment the heap as much */
		const int newSize = (length + 1) * 2;

		outer->name = (char*) realloc(outer->name, newSize);
		outer->nameSize = newSize;
	}

	memcpy(outer->name, name, length + 1);
}

void tagAttr(MTXContext* context, const char* attrName, const char* attrValue)
{
	MTXSaxContext* outer = (MTXSaxContext*) context->userData;

	const int attrNameLength = strlen(attrName);
	const int attrValueLength = strlen(attrValue);

	const int attrSize = attrNameLength + attrValueLength + 2;

	if (outer->attrBufSize - outer->attrBufUsed < attrSize)
	{
		// Make room for attribute name and value
		const int newSize = outer->attrBufSize + attrSize * 2;

		outer->attrBuf = (char*) realloc(outer->attrBuf, newSize);
		outer->attrBufSize = newSize;
	}

	if (outer->attrIndexSize < (outer->attrCount + 2) * 2)
	{
		// Make room for offsets to attribute name and value
		const int newSize = outer->attrCount ? outer->attrCount * 2 : 8;

		outer->attrIndex = (const char**) realloc(outer->attrIndex, newSize * sizeof(char*));
		outer->attrIndexSize = newSize;
	}

	// Save attribute name and offset to it
	strcpy(outer->attrBuf + outer->attrBufUsed, attrName);
	outer->attrIndex[outer->attrCount * 2] = (char*) outer->attrBufUsed;
	outer->attrBufUsed += attrNameLength + 1;

	// Save attribute value and offset to it
	strcpy(outer->attrBuf + outer->attrBufUsed, attrValue);
	outer->attrIndex[outer->attrCount * 2 + 1] = (char*) outer->attrBufUsed;
	outer->attrBufUsed += attrValueLength + 1;

	outer->attrCount++;
}

void tagStartEnd(MTXContext* context)
{
	MTXSaxContext* outer = (MTXSaxContext*) context->userData;
	flushAttributes(outer);
}

void tagData(MTXContext* context, const char* data, int length)
{
	MTXSaxContext* outer = (MTXSaxContext*) context->userData;
	outer->characters(outer, data, length);
}

void tagEnd(MTXContext* context, const char* name, int length)
{
	MTXSaxContext* outer = (MTXSaxContext*) context->userData;
	outer->endElement(outer, name);
}

void emptyTagEnd(MTXContext* context)
{
	MTXSaxContext* outer = (MTXSaxContext*) context->userData;
	flushAttributes(outer);
	outer->endElement(outer, outer->name);
}

void dataRemains(MTXContext* context, const char* data, int length)
{
	MTXSaxContext* outer = (MTXSaxContext*) context->userData;
	outer->dataRemains(outer, data, length);
}

void parseError(MTXContext* context)
{
	MTXSaxContext* outer = (MTXSaxContext*) context->userData;
	outer->parseError(outer);
}

unsigned char unicodeCharacter(MTXContext* context, int unicode)
{
	MTXSaxContext* outer = (MTXSaxContext*) context->userData;

	const char latin1 = outer->unicodeCharacter(outer, unicode);
	if (latin1 != '\0')
		return latin1;

	return mtxBasicUnicodeConvert(context, unicode);
}

} /* namespace */

extern "C" {

void mtxSaxStart(MTXSaxContext* context)
{
	/* Set up callback function pointers */
	context->inner.encoding = encoding;
	context->inner.tagStart = tagStart;
	context->inner.tagAttr = tagAttr;
	context->inner.tagStartEnd = tagStartEnd;
	context->inner.tagData = tagData;
	context->inner.tagEnd = tagEnd;
	context->inner.emptyTagEnd = emptyTagEnd;
	context->inner.dataRemains = dataRemains;
	context->inner.parseError = parseError;
	context->inner.unicodeCharacter = unicodeCharacter;

	mtxStart(&(context->inner));
	context->inner.userData = context;
	context->name = NULL;
	context->nameSize = 0;
	context->attrBuf = NULL;
	context->attrBufSize = 0;
	context->attrBufUsed = 0;
	context->attrCount = 0;

	/* The attribute index always needs room at least for a NULL pair */
	context->attrIndexSize = 2;
	context->attrIndex = (const char**) malloc(context->attrIndexSize * sizeof(char*));
}

bool mtxSaxFeed(MTXSaxContext* context, char* data)
{
	return !!mtxFeed(&(context->inner), data);
}

bool mtxSaxFeedProcess(MTXSaxContext* context, char* data)
{
	return !!mtxFeedProcess(&(context->inner), data);
}

void mtxSaxStop(MTXSaxContext* context)
{
	mtxStop(&(context->inner));

	free(context->name);
	context->name = NULL;
	context->nameSize = 0;

	free(context->attrBuf);
	context->attrBuf = NULL;
	context->attrBufSize = 0;

	free(context->attrIndex);
	context->attrIndex = NULL;
	context->attrIndexSize = 0;

	context->attrCount = 0;
}

} /*extern "C"*/

namespace Mtx
{

SaxContext::SaxContext(void):
	mSaxListener(NULL),
	mMtxListener(NULL)
{
	mContext.userData = this;

	mContext.encoding = encoding;
	mContext.startElement = startElement;
	mContext.endElement = endElement;
	mContext.characters = characters;
	mContext.unicodeCharacter = unicodeCharacter;
	mContext.parseError = parseError;
}

SaxContext::~SaxContext(void)
{
	if (mSaxListener)
		mtxSaxStop(&mContext);
}

void SaxContext::start(SaxListener& newSaxListener, MtxListener& newMtxListener)
{
	mSaxListener = &newSaxListener;
	mMtxListener = &newMtxListener;
	mtxSaxStart(&mContext);
}

bool SaxContext::feed(char* data)
{
	return mtxSaxFeed(&mContext, data);
}

bool SaxContext::feedProcess(char* data)
{
	return mtxSaxFeedProcess(&mContext, data);
}

void SaxContext::stop(void)
{
	mSaxListener = NULL;
	mMtxListener = NULL;
	mtxSaxStop(&mContext);
}

bool SaxContext::isStarted(void) const
{
	return mSaxListener != NULL;
}

void SaxContext::encoding(MTXSaxContext* context, const char* value)
{
	SaxContext* outer = (SaxContext*) context->userData;
	outer->mSaxListener->mtxEncoding(value);
}

void SaxContext::startElement(MTXSaxContext* context, const char* name, const char** attributes)
{
	SaxContext* outer = (SaxContext*) context->userData;
	outer->mSaxListener->mtxStartElement(name, attributes);
}

void SaxContext::endElement(MTXSaxContext* context, const char* name)
{
	SaxContext* outer = (SaxContext*) context->userData;
	outer->mSaxListener->mtxEndElement(name);
}

void SaxContext::characters(MTXSaxContext* context, const char* data, int length)
{
	SaxContext* outer = (SaxContext*) context->userData;
	outer->mSaxListener->mtxCharacters(data, length);
}

unsigned char SaxContext::unicodeCharacter(MTXSaxContext* context, int character)
{
	SaxContext* outer = (SaxContext*) context->userData;
	return outer->mSaxListener->mtxUnicodeCharacter(character);
}

void SaxContext::dataRemains(MTXSaxContext* context, const char* data, int length)
{
	SaxContext* outer = (SaxContext*) context->userData;
	outer->mMtxListener->mtxDataRemains(data, length);
}

void SaxContext::parseError(MTXSaxContext* context)
{
	SaxContext* outer = (SaxContext*) context->userData;
	outer->mSaxListener->mtxParseError();
}

} /* namespace Mtx */

