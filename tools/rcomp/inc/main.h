/*
* Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* VT fix here
*
*/


#ifndef __MAIN_H__
#define __MAIN_H__

#ifdef __VC32__ 
#pragma warning( disable : 4786 )	// identifier truncated in debugging information.
#pragma warning( push, 1 )	// MS STL libraries do not compile cleanly, temporarily set warning level to 1
#pragma warning( disable : 4530 )	// and disable this one as well.
#pragma warning(disable : 4710 )
#endif
#include <vector>
#include <map>
#ifdef __VC32__
#pragma warning( pop )
#endif

// VT fix for Linux
#if defined(__LINUX__) || defined(DARWIN)
   #include <stdio.h>
#endif // LINUX
// end VT

#include "structst.h"
#include "rcstack.h"
#include "indextab.h"
#include "fileline.h"
#include "parser.h"
#include "qualifar.h"
#include "messages.h"

struct RlsValue
{
	RlsValue(const String* aFileName, int aLineNo,
		const char*a, TRlsType aType, TRlsCardinality aCard)
		: iValue(a), iType(aType), iCardinality(aCard), iCitationCount(0),
		iMaximumLength(0xFFFFFFF),
		iLineNumber(aLineNo), iFileName(aFileName) { }
	RlsValue(const String* aFileName, int aLineNo,
		const char*a, TRlsType aType, TRlsCardinality aCard,
		unsigned long aMaximumLength)
		: iValue(a), iType(aType), iCardinality(aCard), iCitationCount(0),
		iMaximumLength(aMaximumLength),
		iLineNumber(aLineNo), iFileName(aFileName) { }

	String iValue;
	TRlsType iType;
	TRlsCardinality iCardinality;
	int iCitationCount;
	unsigned long iMaximumLength;
	int iLineNumber;
	const String* iFileName;
};

typedef std::map<String, int, StringLess> TNameIndex;
typedef TNameIndex::iterator TNameIndexIterator;


// Global data is held in a structure allocated in main(). This is in order for the memory leakage
// mechanism to be able see the allocation of this data.
struct GlobalData
{
    StructHeaderArray          SHA;
    ResourceItemArrayStack     RIAStack;
    StructResourceItemStack    SRIStack;
    IndexTable                 Index;
    NameIdMap                  ResourceNameIds;
    FileLineManager            FileLineHandler;
    NameIdMap                  EnumValues;
    std::vector<RlsValue>      RlsValues;
    TNameIndex                 RlsNameIndex; // index from Label -> RlsValues[]
    bool                       WarningMultiExplained;
    StringArray                AllIdentifiers;
    QualifiedStringArray       UsedIdentifiers;
    MessageArray               Messages;
	char*					   StdInBuffer ;
	unsigned long			   StdInBufLength ;
	unsigned long			   StdInfBufPos ;
};

extern GlobalData *pG;
extern int verbose;
extern unsigned short logmemorysetting;		// remove this



extern int ParseSourceFile(FILE* aFile, unsigned short aYYDebug);

#endif // end VT __MAIN_H__
