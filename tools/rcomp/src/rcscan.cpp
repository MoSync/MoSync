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
#include <stdio.h>

int yylex();

#if defined(__MSVCDOTNET__) || defined(__TOOLS2__)
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#else //!__MSVCDOTNET__
#include <iostream.h>
#endif //__MSVCDOTNET__

void yyrestart( FILE *new_file );
extern int yylineno;

#include "rcscan.h"
#include "errorhan.h"
#include "mem.h"
#include <stdarg.h>  // for va_... used by yyerror
#include <stdio.h>   // for vsprintf... used by yerror


rcscan::rcscan(const FileLineManager& aFileLineHandlerToSet,FILE* aSourceFile):
	iErrorFound(0),
	iFileLineHandler(aFileLineHandlerToSet)
	{
	yyrestart(aSourceFile);
	}

rcscan& rcscan::operator=(const rcscan& /*scan*/)
	{
	assert(0);
	return *this;
	}
#if defined(__TOOLS2_WINDOWS__)
void rcscan::yyerror(char* aCharPtr, __VALIST aList)
#else
void rcscan::yyerror(char* aCharPtr, va_list aList)
#endif
	{
	if (yylineno)
		{
		MOFF;
		cerr << iFileLineHandler.GetErrorLine(yylineno) << ") : ";
		MON;
		}

	char buffer[128];
    vsprintf(buffer, aCharPtr, aList);
    
	cerr << buffer << endl;
    iErrorFound = 1;
	}

void rcscan::yyerror(char* aCharPtr,...)
	{
	if(yylineno)
		{
		MOFF;
		cerr << *iFileLineHandler.GetCurrentFile() << "(";
		cerr << iFileLineHandler.GetErrorLine(yylineno) << ") : ";
		MON;
		}

    // Format a string that can be sent via the iostream mechanism
    va_list va;
    char buffer[128];
    va_start(va,aCharPtr);
    vsprintf(buffer, aCharPtr, va );
    va_end(va);

	cerr << buffer << endl;
	iErrorFound = 1;
	}

int rcscan::ErrorWasFound() const
	{
	return iErrorFound;
	}
