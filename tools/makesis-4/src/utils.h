// UTILS.H
// Copyright (c) 1997-1999 Symbian Ltd. All rights reserved.
//
// various utility functions
//

#ifndef __UTILS_H_
#define __UTILS_H_

// ===========================================================================
// INCLUDES
// ===========================================================================

#include <windows.h>

#include <tchar.h>

#ifndef __T
#define __T _T
#endif

enum TUtilsException
	{ErrNotEnoughMemory	=-1,
	ErrCannotOpenFile	=-2,
	ErrCannotReadFile	=-3,
	ErrCannotWriteFile	=-4,
	ErrCannotConvertFile=-5,
	ErrCertFileKeyFileMismatch = -6};

// ===========================================================================
// GLOBAL FUNCTIONS
// ===========================================================================

// convert a MBCS string to UNICODE
LPWSTR MakeUnicodeString(LPCSTR mbStr, UINT codePage, DWORD& length);
// convert a UNICODE string to MBCS
LPSTR MakeMBCSString(LPCWSTR uniStr, UINT codePage, DWORD& length);
// is a file unicode
BOOL FileIsUnicode(LPCWSTR fileName, BOOL& littleEndian);
// create a temporary file which is a little endian UNICODE version of a
// big endian UNICODE file
LPWSTR ConvertFileToLittleEndianUnicode(LPCWSTR fileName);
// create a temporary file which is a UNICODE version of an MBCS file
LPWSTR ConvertFileToUnicode(LPCWSTR fileName);
// get a temporary filename (in c:\temp\mks???\ with given file name)
LPWSTR TempFileName(LPCWSTR fileName);
// remove any temporary files created (in c:\temp\mks???\)
void CleanupTemp();
// Open file with Unicode filename correctly under Win95 and WinNT
HANDLE MakeSISOpenFile(LPCWSTR pszFilename, DWORD dwAccessMode, DWORD dwCreateFlags);
// Converts a relative path to an absolute path under Win95 and WinNT
int FullPath(LPWSTR pszAbsolutePath, const LPCWSTR pszRelativePath, size_t maxLength);


#endif
