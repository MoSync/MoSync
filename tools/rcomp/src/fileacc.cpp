// FILEACC.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "fileacc.h"

// The functions in this class use non-standard run-time routines e.g. _fullpath.

String FileAccess::FullPath(const String& aPartialPath)
{
	char path[_MAX_PATH];
	if(aPartialPath.Length()==0)
		assert(_fullpath(path,"",_MAX_PATH)!=NULL);
	else
		assert(_fullpath(path,aPartialPath.GetBuffer(),_MAX_PATH)!=NULL);
	return path;
}

String FileAccess::GetDriveAndDirectory(const String& aFullFileName)
{
	char drive[_MAX_DRIVE];
	char directory[_MAX_DIR];
	char filename[_MAX_FNAME];
	char extension[_MAX_EXT];
	_splitpath(aFullFileName.GetBuffer(),drive,directory,filename,extension);
	char path[_MAX_PATH];
	strcpy(path,drive);
	strcpy(path+strlen(path),directory);
	return path;
}

