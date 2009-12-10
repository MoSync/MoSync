// ERRORHAN.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#include "errorhan.h"
#include "mem.h"

using namespace std;

int ErrorHandler::iLineNumber = 0;
String ErrorHandler::iFileName;

void ErrorHandler::Register(const String& aFileNameToSet,int aLineNumberToSet)
{
	MOFF;	// iFileName is a static so it will be destroyed at program end.
	iFileName=aFileNameToSet;
	MON;
	iLineNumber=aLineNumberToSet;
}

void ErrorHandler::OutputErrorLine(const String& aAdditionalText)
{
	cerr << iFileName << "(" << iLineNumber << ") : " << aAdditionalText << endl;
}

