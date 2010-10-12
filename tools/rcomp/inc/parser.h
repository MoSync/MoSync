/*
* Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef PARSER_H_
#define PARSER_H_

// Extra types for YACC parser.

struct RlsValue;

enum TRlsCardinality { ERlsCardinalitySingle, ERlsCardinalityMultiple };
enum TRlsType { ERlsString, ERlsString8,
	ERlsByte, ERlsWord, ERlsLong, ERlsDouble,
	// The ...Char types are normal types declared as character literals
	ERlsByteChar, ERlsWordChar, ERlsLongChar,
	// rls_string declaring a character is no longer allowed: included for
	// backwards compatibility.
	ERlsStringChar };

// rls item length and cardinality.
struct TRlsQualifiers
	{
	TRlsCardinality iCardinality;
	unsigned long iMaxLength;
	};

// Value type when the value might have come from an rls item.
struct TValueMaybeRls
	{
	char iString[1024*8];
	// RlsValue if it did come from an rls item, 0 if it did not.
	const RlsValue* iRlsSource;
	};

#endif
