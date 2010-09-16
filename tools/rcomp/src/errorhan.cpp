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


#include "errorhan.h"
#include "mem.h"

#if defined(__MSVCDOTNET__) || defined(__TOOLS2__)
using std::cerr;
using std::endl;
#endif //__MSVCDOTNET__

int ErrorHandler::iLineNumber = 0;
const String* ErrorHandler::iFileName;

void ErrorHandler::Register(const String* aFileNameToSet,int aLineNumberToSet)
	{
	iFileName=aFileNameToSet;
	iLineNumber=aLineNumberToSet;
	}

void ErrorHandler::OutputErrorLine(const String& aAdditionalText)
	{
	cerr << *iFileName << "(" << iLineNumber << ") : " << aAdditionalText << endl;
	}

void ErrorHandler::OutputWholeLine(String aFileName, int aLineNumber, String aComment)
	{
	Register(&aFileName, aLineNumber);
	OutputErrorLine(aComment);
	}

