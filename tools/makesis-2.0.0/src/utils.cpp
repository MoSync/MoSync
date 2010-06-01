// UTILS.CPP
// Copyright (c) 1997-1999 Symbian Ltd. All rights reserved.
//
// Various utility functions
//

// ===========================================================================
// INCLUDES
// ===========================================================================

#define _GNU_SOURCE 1

#include <stdio.h>
#include <wchar.h>
#include "utils.h"
#include "utf8.h"

// ===========================================================================
// GLOBAL UTILS FUNCTIONS
// ===========================================================================

LPWSTR MakeUnicodeString(LPCSTR mbStr, UINT codePage, DWORD& length)
// Convert a multi-byte string to a UNICODE string
	{
	LPWSTR unicodeStr;
	// get num unicode chars required
	DWORD len = MultiByteToWideChar(codePage, 0, mbStr, length, NULL, 0);
	unicodeStr = new WCHAR[len+1];
	if (!unicodeStr) throw ErrNotEnoughMemory;
	// convert
	MultiByteToWideChar(codePage, 0, mbStr, length, unicodeStr, len);
	unicodeStr[len]='\0';
	length=len;
	
	return unicodeStr;
	}

LPSTR MakeMBCSString(LPCWSTR uniStr, UINT codePage, DWORD& length)
// Convert a UNICODE string to a multi-byte string
	{
	LPSTR mbStr;
	// get num unicode chars required
	DWORD len = WideCharToMultiByte(codePage, 0, uniStr, length, NULL, 0, NULL, NULL);
	mbStr = new CHAR[len+1];
	if (!mbStr) throw ErrNotEnoughMemory;
	// convert
	WideCharToMultiByte(codePage, 0, uniStr, length, mbStr, len, NULL, NULL);
	mbStr[len]='\0';
	length=len;
	
	return mbStr;
	}

BOOL FileIsUnicode(LPCWSTR fileName, BOOL& littleEndian)
// check whether a text file is in UNICODE format & whether little/big-endian
	{
	WCHAR pBuf;
	DWORD dwNumBytes;
	DWORD ok;

	HANDLE hFile = ::MakeSISOpenFile(fileName, GENERIC_READ, OPEN_EXISTING);
	if(hFile == INVALID_HANDLE_VALUE)
		throw ErrCannotOpenFile;
				
	// Make sure we're at the beginning of the file
	::SetFilePointer(hFile, 0L, NULL, FILE_BEGIN);

	ok=::ReadFile(hFile, (LPVOID)&pBuf, sizeof(WCHAR), &dwNumBytes, NULL);

	::CloseHandle(hFile);
	
	if (!ok) throw ErrCannotReadFile;

	if (dwNumBytes==sizeof(WCHAR) && pBuf==0xFEFF)
		{
		littleEndian=TRUE;
		return TRUE;
		}
	else if (dwNumBytes==sizeof(WCHAR) && pBuf==0xFFFE)
		{
		littleEndian=FALSE;
		return TRUE;
		}
	else
		{
		littleEndian=FALSE;
		return FALSE;
		}
	}

LPWSTR ConvertFileToLittleEndianUnicode(LPCWSTR fileName)
// convert a big-endian UNICODE file to a little-endian UNICODE file
	{
	LPWSTR pszTempSource;
	DWORD dwNumBytes;
	HANDLE hFile;
	DWORD fileSize;
	LPWSTR pBuf;
	BOOL ok;

	// open file & get file size
	hFile = ::MakeSISOpenFile(fileName, GENERIC_READ, OPEN_EXISTING);
	if(hFile == INVALID_HANDLE_VALUE) throw ErrCannotOpenFile;
	fileSize = ::GetFileSize(hFile, NULL);

	// read in whole file
	pBuf = new WCHAR [fileSize/2];
	if (!pBuf) throw ErrNotEnoughMemory;
	ok=::ReadFile(hFile, (LPVOID)pBuf, fileSize, &dwNumBytes, NULL);
	::CloseHandle(hFile);
	if (!ok) throw ErrCannotReadFile;

	// convert text to little endian unicode
	for (DWORD i=0; i<fileSize/2; i++)
		pBuf[i]=(WCHAR)(((pBuf[i]&0xFF00)>>8) | ((pBuf[i]&0xFF)<<8));

	// write to new temporary file
	pszTempSource=TempFileName(fileName);
	hFile = ::MakeSISOpenFile(pszTempSource, GENERIC_WRITE|GENERIC_READ, CREATE_NEW);
	if (hFile==INVALID_HANDLE_VALUE) throw ErrCannotOpenFile;
	ok=::WriteFile(hFile, (LPVOID)pBuf, fileSize, &dwNumBytes, NULL);
	::CloseHandle(hFile);
	delete [] pBuf;
	if (!ok) throw ErrCannotWriteFile;

	return pszTempSource;
	}

LPWSTR ConvertFileToUnicode(LPCWSTR fileName)
// convert a UTF8 text file to UNICODE
	{
	LPWSTR pszTempSource;
	DWORD dwNumBytes;
	HANDLE hFile;
	DWORD fileSize;
	LPSTR pNarrowBuf;
	LPWSTR pBufU;
	BOOL ok;

	// open file & get file size
	hFile = ::MakeSISOpenFile(fileName, GENERIC_READ, OPEN_EXISTING);
	if(hFile == INVALID_HANDLE_VALUE) throw ErrCannotOpenFile;
	fileSize = ::GetFileSize(hFile, NULL);

	// read in whole file
	pNarrowBuf = new CHAR [fileSize];
	if (!pNarrowBuf) throw ErrNotEnoughMemory;
	ok=::ReadFile(hFile, (LPVOID)pNarrowBuf, fileSize, &dwNumBytes, NULL);
	::CloseHandle(hFile);
	if (!ok) throw ErrCannotReadFile;

	// write to new temporary file
	pszTempSource=TempFileName(fileName);
	hFile = ::MakeSISOpenFile(pszTempSource, GENERIC_WRITE|GENERIC_READ, CREATE_NEW);
	if (hFile==INVALID_HANDLE_VALUE) throw ErrCannotOpenFile;

	pBufU=new WCHAR [fileSize];
	if (!pBufU) throw ErrNotEnoughMemory;

	// convert text to unicode
	const UTF8* sourceStart=(UTF8*)pNarrowBuf;
	const UTF8* sourceEnd=sourceStart+fileSize; 
	UTF16* targetStart;
	UTF16* targetEnd;
	while (ok && sourceStart<sourceEnd)
		{
		targetStart=(UTF16 *)pBufU;
		targetEnd=(UTF16 *)(pBufU+fileSize);
		ok=(ConvertUTF8toUTF16(&sourceStart, sourceEnd, &targetStart, targetEnd,lenientConversion)!=sourceExhausted);
		if (ok) ok=::WriteFile(hFile, (LPVOID)pBufU, ((char*)targetEnd-(char*)pBufU), &dwNumBytes, NULL);
		}

	::CloseHandle(hFile);
	delete [] pNarrowBuf;
	delete [] pBufU;
	if (!ok) throw ErrCannotConvertFile;

	return pszTempSource;
	}

LPWSTR TempFileName(LPCWSTR fileName)
// generate a unique temporary filename
// creates a sub-directory in TEMP and uses fileName as a guide for the
// filename
// returns name of temporary directory if fileName is NULL
	{
	static WCHAR tmpPath[MAX_PATH]={'\0'};
	static WCHAR tmpFileName[MAX_PATH]={'\0'};
	HANDLE hFile;
	if (*tmpPath=='\0')
		{
		// first time called so generate temporary directory
		// first get TEMP directory
#ifdef _UNICODE
		WCHAR tmpDir[MAX_PATH];
		GetTempPathW(MAX_PATH,tmpDir);
		// now create a unique sub-directory
		for (WORD i=0; i<10000;i++)
			{
			(void)swprintf(tmpPath, sizeof(tmpPath), L"%S/MKS%d",tmpDir,i);
			if (::CreateDirectoryW(tmpPath,NULL)) break;
			}
#else
		char tempPath[MAX_PATH];
		char tmpDir[MAX_PATH];
		GetTempPath(MAX_PATH,tempPath);
		// now create a unique sub-directory
		for (WORD i=0; i<10000;i++)
			{
			sprintf(tmpDir,"%sMKS%d",tempPath,i);
			if (::CreateDirectory(tmpDir,NULL)) break;
			}
		::MultiByteToWideChar(CP_OEMCP, 0, tmpDir, -1, tmpPath, MAX_PATH);
#endif
		}
	if (fileName)
		{
		/* Find the last backslash */
		int index = 0;
		for(size_t i=0;i<wcslen(fileName);i++) {
			if(fileName[i] == '/'
			   ||
			   fileName[i] == '\\')
				index = i;
		}
		if(index > 0)
			fileName = &fileName[index+1];

		swprintf(tmpFileName,
#ifndef __GNUC__
			sizeof(tmpFileName),
#endif
			L"%s\\%s",tmpPath,fileName);
		for (WORD i=0; i<10000;i++)
			{
			hFile = ::MakeSISOpenFile(tmpFileName, GENERIC_READ, OPEN_EXISTING);
			if (hFile==INVALID_HANDLE_VALUE) break;
			CloseHandle(hFile);
			swprintf(tmpFileName,
#ifndef __GNUC__
			sizeof(tmpFileName),
#endif
				L"%s\\%s%d",tmpPath,fileName,i);
			}
		/** Convert backslash to underscore for the generated filename */
		WCHAR *tmp = &tmpFileName[0];
		while(*tmp) {
			if(*tmp == '/') {
				*tmp = '\\';
			}
			tmp++;
		}
		}
	else
		wcscpy(tmpFileName,tmpPath);
	return tmpFileName;
	}

void CleanupTemp()
	{
	LPWSTR tmpDir=TempFileName(NULL);
#ifdef _UNICODE
	WCHAR *tmpDirT=tmpDir;
#else
	char tmpDirT[MAX_PATH] = "\0";
	::WideCharToMultiByte(CP_OEMCP, 0, tmpDir, -1, tmpDirT, MAX_PATH, NULL, NULL);
#endif
	::RemoveDirectory(tmpDirT);
	}


HANDLE MakeSISOpenFile(LPCWSTR pszFilename, DWORD dwAccessMode, DWORD dwCreateFlags)
// Open file with Unicode filename correctly under Win95 and WinNT
	{
	HANDLE hFile;
	char pszMultiByte[MAX_PATH] = "\0";
	LPWSTR p=(LPWSTR)pszFilename;
		
	if (!wcsncmp(pszFilename,L"./",2))
	  p+=2;
	::WideCharToMultiByte(CP_OEMCP,				// code page
			      0,					// performance and mapping flags
			      p,			 		// address of wide-character string
			      -1,					// number of characters in string
			      pszMultiByte, 		// address of buffer for new string
			      MAX_PATH,		// size of buffer
			      NULL,				// address of default for unmappable characters
			      NULL);				// address of flag set when default char. used
		
	hFile = ::CreateFileA(pszMultiByte, dwAccessMode, 0, NULL,
			      dwCreateFlags, FILE_ATTRIBUTE_NORMAL, NULL);

#if 0	//Fredrik
	if(hFile == INVALID_HANDLE_VALUE) {
		printf("Cannot open file \"%S\"\n", pszFilename);
	}
#endif

	return hFile;
	}

int FullPath(LPWSTR pszAbsolutePath, LPCWSTR pszRelativePath, size_t maxLength)
// Converts a relative path to an absolute path under Win95 and WinNT
	{
	char pszMultiByteRelative[MAX_PATH] = "\0";
	char pszMultiByteAbsolute[MAX_PATH] = "\0";
	LPWSTR p=(LPWSTR)pszRelativePath;
	
	if (!wcsncmp(pszRelativePath,L"./",2)) p+=2;
	::WideCharToMultiByte(CP_OEMCP,				// code page
		0,					// performance and mapping flags
		p,			 		// address of wide-character string
		-1,					// number of characters in string
		pszMultiByteRelative,	// address of buffer for new string
		MAX_PATH,		// size of buffer
		NULL,				// address of default for unmappable characters
		NULL);				// address of flag set when default char. used
	
	int returnValue = _fullpath(pszMultiByteAbsolute, pszMultiByteRelative, maxLength)
			? 1 : 0;

	::MultiByteToWideChar(CP_OEMCP,
		0,
		pszMultiByteAbsolute,
		-1,
		pszAbsolutePath,
		MAX_PATH);
	
	return returnValue;
	}
