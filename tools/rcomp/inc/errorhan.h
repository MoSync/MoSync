// ERRORHAN.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#ifndef __ERRORHAN_H__
#define __ERRORHAN_H__

#if defined(__VC32__)
#pragma warning( disable : 4710 )	// function not inlined
#endif

#include "astring.h"

class ErrorHandler
{
public:
	static void Register(const String& FileName,int LineNumber);
	static void OutputErrorLine(const String& AdditionalText);
	static const String& GetFileName() {return(iFileName);}
	static int GetLineNumber() {return(iLineNumber);}
private:
	static String iFileName;
	static int iLineNumber;
};

#endif
