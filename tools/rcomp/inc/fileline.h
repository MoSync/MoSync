// FILELINE.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#if !defined(__FILELINE_H__)
#define __FILELINE_H__
#if !defined(__ASTRING_H__)	
#include "astring.h"
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
	String GetCurrentFile() const;
private:
	String ExtractFileName(const String& aText);
private:
	String iCurrentFileName;
	String iBaseFileName;
	int iOffsetLineNumber;	// Line number of line in preprocessed file corresponding to # line for CurrentFileName.
	String iBasePath;
};

#endif
