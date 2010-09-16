/*
* Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
*
*/


#include <assert.h>
#include "datatype.h"
#include "tokens.h"

// RCTypeArray

RCTypeArray::RCTypeArray()
	{
	Add( new RCType ( L_STRUCT,		"Struct",	0	));
	Add( new RCType ( L_BUF,		"Buf",		0	));
	Add( new RCType ( L_WORD,		"Word",		2	));
	Add( new RCType ( L_BYTE,		"Byte",		1	));
	Add( new RCType ( L_LONG,		"Long",		4	));
	Add( new RCType ( L_DOUBLE,		"Double",	8	));
	Add( new RCType ( L_TEXT,		"Text",		0	));
	Add( new RCType ( L_LTEXT,		"LText",	0	));

	Add( new RCType ( L_BUF8,		"Buf8",  	0	));
	Add( new RCType ( L_TEXT8,		"Text8",	0	));
	Add( new RCType ( L_LTEXT8,		"LText8",	0	));

	Add( new RCType ( L_BUF16,		"Buf16",  	0	));
	Add( new RCType ( L_TEXT16,		"Text16",	0	));
	Add( new RCType ( L_LTEXT16,	"LText16",	0	));


	Add( new RCType ( L_LINK,		"Link",		2	));
	Add( new RCType ( L_LLINK,		"LLink",	4	));
	Add( new RCType ( L_SRLINK,		"SRLink",	4	));
	}

RCTypeArray::~RCTypeArray()
	{
	DeleteAll();
	}

const String & RCTypeArray::GetName( DataType ElementTypeSought) const
	{
	RCType* pType;
	RCTypeArrayIterator next( *this);	

	while( ( pType = next()) != NULL)	
		if ( pType->GetElementType() == ElementTypeSought)
			return pType->GetName();
	
	assert(0);
	return * (new String);	// This won't actually get executed due to the assert line above.
	}

unsigned short RCTypeArray::GetSize( DataType ElementTypeSought) const
	{
	RCType * pType;
	RCTypeArrayIterator next( *this);	

	while( ( pType = next()) != NULL)	
		if ( pType->GetElementType() == ElementTypeSought)
			return pType->GetSize();
	
	assert(0);
	return 0;
	}


// RCType

RCType::RCType( DataType ElementType, const String & Name, unsigned short Size):
	iElementType(ElementType),
	iName(Name),
	iSize(Size)
	{}

RCType::~RCType()
	{}

const String &  RCType::GetName() const
	{
	return iName;
	}

DataType RCType::GetElementType() const
	{
	return iElementType;
	}

unsigned short RCType::GetSize() const
	{
	return iSize;
	}


// RCTypeArrayIterator

RCTypeArrayIterator::RCTypeArrayIterator(const RCTypeArray& r):
	ArrayIterator(r)
	{}

RCType * RCTypeArrayIterator::operator()()
	{
	return (RCType *) ArrayIterator::operator()();
	}
