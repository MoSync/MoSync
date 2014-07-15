// H_FILE.CPP
//
// Copyright (c) 1995-2004 Symbian Software Ltd.  All rights reserved.
//

#ifdef WIN32
#include <io.h>
#else
#define O_BINARY 0
#endif
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <h_utl.h>

TInt HFile::Open(const TText * const aFileName, TInt32 * const aFileHandle)
{
	TInt32 hFile = open((const char *)aFileName, O_RDONLY | O_BINARY);
	if (hFile == -1)
	{
		switch (errno)
		{
		case EACCES:
			Print(EError,"Can't open file %s - access violation.\n",aFileName);
			break;
		case EEXIST:
			Print(EAlways,"Tried to create existing file %s\n",aFileName);
			break;
		case EINVAL:
			Print(EError,"Can't open file %s - invalid open flags.\n",aFileName);
			break;
		case EMFILE:
			Print(EError,"Can't open file %s - too many open files.\n",aFileName);
			break;
		case ENOENT:
			Print(EError,"Can't open file %s - file or path not found.\n",aFileName);
			break;
		default:
			Print(EError,"Can't open file %s, error %d (decimal).\n",aFileName,errno);
			break;
		}
		return errno;
	}
	*aFileHandle = hFile;
	return 0;
}	

/******************************************************************************/
TBool HFile::Read(const TInt32 aFileHandle, TAny * const aBuffer, const TUint32 aCount)
{
	TInt32 bytesRead =  read(aFileHandle, aBuffer, aCount);
	if (bytesRead != (TInt32)aCount)
		return EFalse;
	else
		return ETrue;
}  

/******************************************************************************/
TBool HFile::Seek(const TInt32 aFileHandle, const TUint32 aOffset)
{
	TInt32 newPos = lseek(aFileHandle, aOffset, SEEK_SET);
	if (newPos == -1)
		return(EFalse);
	else
		return ETrue;
}

/******************************************************************************/
TUint32 HFile::GetPos(const TInt32 aFileHandle)
{
	TUint32 pos = lseek(aFileHandle, 0, SEEK_CUR);
	return pos;
}

/******************************************************************************/
TUint32 HFile::GetLength(TText *aName)
{
	TInt32 handle;
	if (HFile::Open(aName, &handle)==0)
	{
		TUint32 size = GetLength(handle);
		HFile::Close(handle);
		return size;
	}
	else
		return 0;
}

/******************************************************************************/
TUint32 HFile::GetLength(const TInt32 aFileHandle)
{
	off_t curPos = lseek(aFileHandle, 0, SEEK_CUR);
	TUint32 size = lseek(aFileHandle, 0, SEEK_END);
	lseek(aFileHandle, curPos, SEEK_SET);
	return size;
}


/******************************************************************************/
void HFile::Close(const TInt32 aFileHandle)
{
	close(aFileHandle);
}


/******************************************************************************/
TUint32 HFile::Read(TText *aName, TAny *someMem)
{
	TInt32 handle;
	if (HFile::Open(aName, &handle)==0)
	{
		TUint32 size = HFile::GetLength(handle);
		if (HFile::Read(handle, someMem, size))
		{
			HFile::Close(handle);
			return size;
		}
		else
		{
			HFile::Close(handle);
			return 0;
		}
	}
	else
		return 0;
}


/******************************************************************************/
