// PARSECMD.H
// Copyright (c) 1997-1999 Symbian Ltd. All rights reserved.
//
// handles parsing of makesis command line args
//

#ifndef __PARSE_CMD_H_
#define __PARSE_CMD_H_

// ===========================================================================
// CONSTANTS
// ===========================================================================

#define SOURCEFILE		L".pkg"
#define DESTFILE		L".sis"

enum TCommandLineException
	{ErrInsufficientArgs,
	ErrBadCommandFlag,
	ErrBadSourceFile,
	ErrNoSourceFile,
	ErrCannotOpenSourceFile,
	ErrBadTargetFile
	};

// ===========================================================================
// CLASS DEFINITION
// ===========================================================================

class CParseCmd
// Responsible for processing and maintaining the command line options
	{
	public:
		CParseCmd();
		
		BOOL ParseCommandLine(int argc, _TCHAR *argv[]);
		
		BOOL  ShowSyntax()			const { return m_fShowSyntax; }
		DWORD Flags()				const { return m_dwOptions;   }
		LPCWSTR GetPassword()		const { return m_pszPassword; }
		LPCWSTR SourceFile()		const { return m_pszSource;   }
		LPCWSTR SearchPath()		const { return m_pszDir;   }
		LPCWSTR TargetFile();
		
		enum TOptions
			{EOptVerbose  = 0x01,
			EOptMakeStub  = 0x02,
			EOptDirectory = 0x04,
			EOptPassword  = 0x08
			};
		
	private:
		void SetDirectory(LPCWSTR pszPath);
		void SetSource(LPCWSTR pszSource);
		void SetTarget(LPCWSTR pszTarget);
		
		WCHAR m_pszDir[MAX_PATH];
		WCHAR m_pszSource[MAX_PATH];
		WCHAR m_pszTarget[MAX_PATH];
		WCHAR m_pszPassword[MAX_PATH];
		
		DWORD m_dwOptions;
		BOOL  m_fShowSyntax;
	};

#endif // __PARSE_CMD_H_
