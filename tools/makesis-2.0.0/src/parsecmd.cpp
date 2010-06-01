// PARSECMD.CPP
// Copyright (c) 1997-1999 Symbian Ltd. All rights reserved.
//
// handles parsing of makesis command line args
//

// ===========================================================================
// INCLUDES
// ===========================================================================

#include "utils.h"
#include <wchar.h>
#include <tchar.h>
#include "parsecmd.h"
#include "crtsis.h"
#include "wcstub.h"

#ifdef _MSC_VER
#pragma warning( disable : 4996 )
#endif


// ===========================================================================
// CParseCmd
// Responsable for processing and maintaining the command line options
// ===========================================================================

CParseCmd::CParseCmd()
:m_dwOptions(0L),
m_fShowSyntax(FALSE)
	{
	m_pszDir   [0] = '\0';
	m_pszSource[0] = '\0';
	m_pszTarget[0] = '\0';
	}

BOOL CParseCmd::ParseCommandLine(int argc, _TCHAR *argv[])
// Purpose  : Proceses the command line, and options
// Inputs   : argc, argv - command line as passed to the process
	{
	// Test the number of arguments
	if(argc == 1)
		throw ErrInsufficientArgs;
	
	int wCount = 1;
	// Parse the command line
	// Check for options
	while(wCount < argc)
		{
		if(argv[wCount][0] != '-' )
			break;
		else
			{
			if(argv[wCount][1] == '\0')
				throw ErrBadCommandFlag;
			switch(argv[wCount][1])
				{
				case 'h' : // Help. i.e Show Syntax	
				case 'H' : m_fShowSyntax = TRUE;
					return TRUE;
				case 'p' : // Use Password for Private Key Decryption	
				case 'P' : m_dwOptions |= EOptPassword;
					{
					wCount++;
					DWORD pwlen=_tcslen(argv[wCount]);
					LPWSTR pwStr=MakeUnicodeString((LPCSTR)argv[wCount], CP_OEMCP, pwlen);
					wcscpy(m_pszPassword, pwStr);
					
					}
					break;
				case 'v' : // Verbose
				case 'V' : m_dwOptions |= EOptVerbose;
					break;
				case 's' : // Generate stub
				case 'S' : m_dwOptions |= EOptMakeStub;
					break;
				case 'd' : // Search directory
				case 'D' : m_dwOptions |= EOptDirectory;
#ifdef _UNICODE
					SetDirectory(&argv[wCount][2]);
#else
						{
						DWORD len=_tcslen(&argv[wCount][2]);
						LPWSTR uniStr=MakeUnicodeString(&argv[wCount][2], CP_OEMCP, len);
						SetDirectory(uniStr);
						delete [] uniStr;
						}
#endif
					break;
				default  : // Uh-Oh...
					throw ErrBadCommandFlag;
				}
			wCount++;
			}
		}
	// Get the source and (optionally) target filenames
	if(wCount < argc)
		{
#ifdef _UNICODE
		SetSource(argv[wCount++]);
#else
		DWORD len=_tcslen(argv[wCount]);
		LPWSTR uniStr=MakeUnicodeString(argv[wCount], CP_OEMCP, len);
		SetSource(uniStr);
		delete [] uniStr;
		wCount++;
#endif
		}
	else
		throw ErrNoSourceFile;
	
	if(wCount < argc)
		{
#ifdef _UNICODE
		SetTarget(argv[wCount]);
#else
		DWORD len=_tcslen(argv[wCount]);
		LPWSTR uniStr=MakeUnicodeString(argv[wCount], CP_OEMCP, len);
		SetTarget(uniStr);
		delete [] uniStr;
		wCount++;
#endif
		}
	
	return TRUE;
	}

void CParseCmd::SetDirectory(LPCWSTR pszPath)
// Purpose  : Sets the search directory
// Inputs   : pszCmdLine - the path
	{
	WCHAR cEndChar = (*pszPath == '\"') ? '\"' : ' ';
	
	WORD wCount = 0;
	while((*pszPath != cEndChar) && 
		(*pszPath != '\0')     &&
		(wCount < MAX_PATH - 1))
		{
		m_pszDir[wCount++] = *pszPath++;
		}
	// Is the dir path legal?
	if(wCount < 1)
		throw ErrBadCommandFlag;
	
	// Check for trailing backslash
	if(m_pszDir[wCount - 1] != '/')
		m_pszDir[wCount++] = '/';
	m_pszDir[wCount] = '\0';
	}

void CParseCmd::SetTarget(LPCWSTR pszTarget)
// Purpose  : Sets the target filename
// Inputs   : pszTarget - The target filename
	{
	// Check that the filename is valid
	if (wcslen(pszTarget) < (wcslen(DESTFILE) + sizeof(WCHAR)))
		throw ErrBadTargetFile;
	else
		{
		if (wcsicmp(&pszTarget[wcslen(pszTarget) - (wcslen(DESTFILE))], DESTFILE) != 0)
			throw ErrBadTargetFile;
		else
			wcscpy(m_pszTarget, pszTarget);
		}
	}

void CParseCmd::SetSource(LPCWSTR pszSource)
// Purpose  : Sets the source filename
// Inputs   : pszSource - The source filename
	{
	// Check that the filename is valid
	if (wcslen(pszSource) <= wcslen(SOURCEFILE))
		throw ErrBadSourceFile;
	else
		{
		if (wcsicmp(&pszSource[wcslen(pszSource) - (wcslen(SOURCEFILE))], SOURCEFILE) != 0)
			throw ErrBadSourceFile;
		else
			wcscpy(m_pszSource, pszSource);
		}
	}

LPCWSTR CParseCmd::TargetFile()
// Returns the destination file. If none was set then it mugnes the source filename into
// something acceptable
	{
	// Has a destination file been set ?
	if(wcslen(m_pszTarget) < 1)
		{
		wcscpy(m_pszTarget, m_pszSource);
		m_pszTarget[wcslen(m_pszTarget) - (wcslen(SOURCEFILE))] = '\0';
		wcscat(m_pszTarget, DESTFILE);
		}
	return m_pszTarget;
	}
