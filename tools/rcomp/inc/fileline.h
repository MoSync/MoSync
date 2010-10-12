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


#if !defined(__FILELINE_H__)
#define __FILELINE_H__
#if !defined(__ASTRING_H__)	
#include "astring.h"
#endif

#ifdef __VC32__
#pragma warning( disable : 4786 )	// identifier truncated in debugging information.
#pragma warning( push, 1 )	// MS STL libraries do not compile cleanly, temporarily set warning level to 1
#pragma warning( disable : 4710 )	// function not inlined.
#endif
#include <set>
#ifdef __VC32__
#pragma warning( pop )
#endif

class FileLineManager
	{
public:
	FileLineManager();
	virtual ~FileLineManager();
	void SetBase(const String& aFileName,int aLineNumber);
	void SetPath(const String& aDriveAndDirectory);
	void SetInclude(const String& aNameText,int aLineNumber);
	void PostInclude(char* aNameText,char* aRealLineNumber,int aLineNumber);
	int	GetErrorLine(int aCurrentLineNumber) const;
	const String* GetCurrentFile() const;

private:
	void SetCurrentFile(const String&);
	String ExtractFileName(const String& aText);

private:
	const String* iCurrentFileName;
	const String* iBaseFileName;
	int iOffsetLineNumber;	// Line number of line in preprocessed file corresponding to # line for CurrentFileName.
	String iBasePath;
	// All filenames that have ever been seen.
	std::set<String, StringLess> iAllFileNames;
	};

#endif //__FILELINE_H__
