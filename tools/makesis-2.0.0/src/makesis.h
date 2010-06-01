// MAKESIS.H
// Copyright (c) 1997-1999 Symbian Ltd. All rights reserved.
//
// makesis main function
//

#ifndef __MAKESIS_H__
#define __MAKESIS_H__

// ===========================================================================
// INCLUDES
// ===========================================================================

#include "parsecmd.h"
#include "parsepkg.h"
#include "crtsis.h"
#include "utils.h"

// ===========================================================================
// CLASS DEFINITION
// ===========================================================================

class CMakeSIS : public MParserObserver
// The main application object. Controls the SIS file generation process
	{
	public:
		CMakeSIS();
		
		int  Run(int argc, _TCHAR* argv[], _TCHAR** /*envp*/);
		
		BOOL ParseSource();
		BOOL WriteTarget();
		
	public: // From MParserObserver
		void DoVerbage(const _TCHAR* pszText) const;
		void DoErrMsg(const _TCHAR* pszText) const;
		BOOL Verbose() const				  { return m_fVerbose; };
		BOOL SetVerbose(BOOL fVerbose)		  { BOOL v = m_fVerbose; m_fVerbose = fVerbose; return v; };
		void SetLineNumber(WORD wLineNumber)  { m_wLineNo = wLineNumber; };
		
	private:
		
		void DoMsg(const _TCHAR* pszText1, const _TCHAR* pszText2) const;
		void ShowBanner();
		void ShowUsage ();
		void ShowSyntax();
		
		void ShowUtilsError(TUtilsException err);
		void ShowParseError(TParseException err);
		void ShowCommandLineError(TCommandLineException err);
		void ShowGeneratorError(TGeneratorException err);
		
		WORD m_wLineNo;
		BOOL m_fVerbose;
		
		CSISWriter m_SISWriter;
		CParseCmd m_CmdOptions;
	};

#endif // __MAKESIS_H__
