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
		BOOL  SetTimestamp()			const { return m_fSetTimestamp; }
		void  GetTimestamp(DWORD* aYear, DWORD* aMonth, DWORD* aDay, DWORD* aHours, DWORD* aMinutes, DWORD* aSeconds) const {
			*aYear = m_dwYear;
			*aMonth = m_dwMonth;
			*aDay = m_dwDay;
			*aHours = m_dwHours;
			*aMinutes = m_dwMinutes;
			*aSeconds = m_dwSeconds;
		}
		LPCWSTR TargetFile();
		
		enum TOptions
			{EOptVerbose  = 0x01,
			EOptMakeStub  = 0x02,
			EOptDirectory = 0x04,
			EOptPassword  = 0x08,
			EOptSelfsign  = 0x10
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
		BOOL  m_fSetTimestamp;
		DWORD m_dwYear, m_dwMonth, m_dwDay;
		DWORD m_dwHours, m_dwMinutes, m_dwSeconds;
	};

#endif // __PARSE_CMD_H_
