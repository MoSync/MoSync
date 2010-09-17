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


#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "fileacc.h"

#if defined(__LINUX__) || defined(DARWIN)
#ifdef __LINUX__
#include <linux/limits.h>
#else
#include <limits.h>
#endif
#include <stdlib.h>
#include <libgen.h>
#endif //__LINUX__

// The functions in this class use non-standard run-time routines e.g. _fullpath.

String FileAccess::FullPath(const String& aPartialPath)
	{
#if !defined(__LINUX__) && !defined(DARWIN)
	char path[_MAX_PATH];
	if(aPartialPath.Length()==0)
		assert(_fullpath(path,"",_MAX_PATH)!=NULL);
	else
		assert(_fullpath(path,aPartialPath.GetAssertedNonEmptyBuffer(),_MAX_PATH)!=NULL);
	return path;
#else
	char path[PATH_MAX];
	if (aPartialPath.Length() == 0)
		assert(realpath("", path) != NULL);
	else
		assert(realpath(aPartialPath.GetAssertedNonEmptyBuffer(), path) != NULL);
	return path;
#endif
	}

String FileAccess::GetDriveAndDirectory(const String& aFullFileName)
	{
#if !defined(__LINUX__) && !defined(DARWIN)
	char drive[_MAX_DRIVE];
	char directory[_MAX_DIR];
	char filename[_MAX_FNAME];
	char extension[_MAX_EXT];
	_splitpath(aFullFileName.GetAssertedNonEmptyBuffer(),drive,directory,filename,extension);
	char path[_MAX_PATH];
	strcpy(path,drive);
	strcpy(path+strlen(path),directory);
	return path;
#else
	return dirname(const_cast<char*>(aFullFileName.GetAssertedNonEmptyBuffer()));
#endif //__LINUX__
	}

