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


#ifndef __RCSCAN_H__
#define __RCSCAN_H__

#include "fileline.h"
#include <stdio.h>
#include <stdarg.h>

class rcscan
	{
public:
	rcscan(const FileLineManager& aFileLineHandler,FILE* aSourceFile);
	virtual void yyerror(char *,...);
	#if defined(__TOOLS2_WINDOWS__) 
	void yyerror(char* aCharPtr, __VALIST aList);
	#else
	void yyerror(char* aCharPtr, va_list aList);
	#endif
	int ErrorWasFound() const;
	rcscan& operator=(const rcscan&);
private:
	int iErrorFound;
	const FileLineManager& iFileLineHandler;
	};

#endif //__RCSCAN_H__
