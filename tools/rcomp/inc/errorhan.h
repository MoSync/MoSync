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


#ifndef __ERRORHAN_H__
#define __ERRORHAN_H__

#if defined(__VC32__)
#pragma warning( disable : 4710 )	// function not inlined
#endif

#include "astring.h"

class ErrorHandler
	{
public:
	static void Register(const String* FileName,int LineNumber);
	static void OutputErrorLine(const String& AdditionalText);
	static void OutputWholeLine(String aFileName, int aLineNumber, String aComment);
	static const String* GetFileName() {return iFileName;}
	static int GetLineNumber() {return(iLineNumber);}
private:
	static const String* iFileName;
	static int iLineNumber;
	};

#endif //__ERRORHAN_H__
