// FILEACC.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#if !defined(__FILEACC_H__)
#define __FILEACC_H__
#if !defined(__ASTRING_H__)	
#include "astring.h"
#endif

class FileAccess
{
public:
	static String FullPath(const String& aPartialPath);
	static String GetDriveAndDirectory(const String& aFullFileName);
};

#endif
