// MAKESIS.CPP
// Copyright (c) 1997-1999 Symbian Ltd. All rights reserved.
//
// makesis main function
//

// ===========================================================================
// INCLUDES
// ===========================================================================

#include <wchar.h>
#include <cstdio>
#include <iostream>
#include <stdlib.h>
#include "utils.h"
#include <tchar.h>
#include "makesis.h"

// ===========================================================================
// CONSTANTS
// ===========================================================================

// Set up the cout stream so that we can use it with either narrow or wide
// chars at build time
using namespace std;
#undef OUT			   // coz it's an IDL thing...

#ifdef _UNICODE
#define OUT wcout
#else
#define OUT cout
#endif


// ===========================================================================
// CMakeSIS
// The main application object. Controls the SIS file generation process
// ===========================================================================

CMakeSIS::CMakeSIS()
:m_wLineNo(0),
m_fVerbose(FALSE)
	{ ; }

int CMakeSIS::Run(int argc, _TCHAR* argv[], _TCHAR** /*envp*/)
// Inputs   : argc, argv, envp - The command line passed to the process
	{	
	int err=1;
	try
		{
		m_SISWriter.SetObserver(this);
		m_CmdOptions.ParseCommandLine(argc, argv);
		if(m_CmdOptions.ShowSyntax())
			{
			ShowSyntax();
			return 0;
			}
		// We now have valid parameters
		m_fVerbose = m_CmdOptions.Flags() & CParseCmd::EOptVerbose;

		if (m_CmdOptions.Flags() & CParseCmd::EOptPassword)
			{
			m_SISWriter.SetCmdPassword(m_CmdOptions.GetPassword());
			}
		if (m_CmdOptions.Flags() & CParseCmd::EOptSelfsign)
			{
			m_SISWriter.SetSelfsign(TRUE);
			}
		if (m_CmdOptions.SetTimestamp())
			{
			DWORD year, month, day, hours, minutes, seconds;
			m_CmdOptions.GetTimestamp(&year, &month, &day, &hours, &minutes, &seconds);
			m_SISWriter.SetTimestamp(year, month, day, hours, minutes, seconds);
			}
		

		if(ParseSource() && WriteTarget())
			{
			LPCWSTR fileNameU=m_CmdOptions.TargetFile();
#ifdef _UNICODE
			OUT << __T("Created ") << fileNameU << endl;
#else
			DWORD len=wcslen(fileNameU);
			LPSTR fileName=MakeMBCSString(fileNameU,CP_ACP,len);
			OUT << __T("Created ") << fileName << endl;
			delete [] fileName;
#endif
			err=0;
			}
		}
	catch(TCommandLineException err)
		{
		// Show the title
		ShowBanner();
		ShowCommandLineError(err);
		ShowUsage();
		}
	catch(TUtilsException err)
		{
		ShowUtilsError(err);
		}
	m_SISWriter.Release();
	return err;
	
	}

BOOL CMakeSIS::ParseSource()
// Processes the input PKG file
	{
	BOOL fResult = TRUE;
	WCHAR pszTempSource[MAX_PATH];
	BOOL converted=FALSE;
	HANDLE hFile=0;

#ifdef _UNICODE
		OUT << __T("Processing ") << m_CmdOptions.SourceFile() << __T("...") << endl;
#else
		DWORD length=wcslen(m_CmdOptions.SourceFile());
		LPSTR fName=MakeMBCSString(m_CmdOptions.SourceFile(), CP_ACP, length);
		OUT << __T("Processing ") << fName << __T("...") << endl;
		delete [] fName;
#endif

	BOOL stubFile=(m_CmdOptions.Flags() & CParseCmd::EOptMakeStub);

	try
		{
		CParsePkg psr;
		BOOL littleEndian;
		if (!FileIsUnicode(m_CmdOptions.SourceFile(),littleEndian))
			{
			converted=TRUE;
			wcscpy(pszTempSource,ConvertFileToUnicode(m_CmdOptions.SourceFile()));
			hFile = ::MakeSISOpenFile(pszTempSource,GENERIC_READ,OPEN_EXISTING);
			}
		else if (!littleEndian)
			{
			converted=TRUE;
			wcscpy(pszTempSource,ConvertFileToLittleEndianUnicode(m_CmdOptions.SourceFile()));
			hFile = ::MakeSISOpenFile(pszTempSource,GENERIC_READ,OPEN_EXISTING);
			}
		else
			{
			hFile = ::MakeSISOpenFile(m_CmdOptions.SourceFile(),GENERIC_READ,OPEN_EXISTING);
			}
		if (hFile==INVALID_HANDLE_VALUE) throw ErrCannotOpenFile;
		// Set the search path
		if(m_CmdOptions.Flags() & CParseCmd::EOptDirectory)
			psr.SetSearchDirectory(m_CmdOptions.SearchPath());
		psr.ParseL(hFile, &m_SISWriter, stubFile, this);
		}
	catch(TParseException x)
		{
		ShowParseError(x);
		fResult = FALSE;
		}
	
	if (hFile) ::CloseHandle(hFile);
	
	if (converted)
		{
		_wunlink(pszTempSource);
		}
	
	return fResult;
	}

BOOL CMakeSIS::WriteTarget()
// Endeavours to contruct the output SIS file, or stubs
	{
	// Turn off line number output...
	m_wLineNo = 0;
	
	BOOL fResult = TRUE;
	try
		{
		BOOL stubFile=(m_CmdOptions.Flags() & CParseCmd::EOptMakeStub);
		if (stubFile)
			DoVerbage(__T("Generating SIS stub file...\n"));
		else
			DoVerbage(__T("Generating SIS installation file..."));
		m_SISWriter.WriteSIS(m_CmdOptions.TargetFile(),stubFile);
		}
	catch(TSISFileGeneratorException err)
		{
		ShowGeneratorError(err);
		fResult = FALSE;
		}
	return fResult;
	}

void CMakeSIS::ShowBanner()
// Displays the copyright gumph...
	{
	short major = KInstallerVersion / 100,
		minor = KInstallerVersion % 100;
	
	// Show the title
	OUT << endl << __T("MakeSIS, version ") << major << __T(".") << minor << endl;
	OUT << __T("A utility for creating Software Installation (SIS) files.") << endl;
#ifdef _DEBUG
	OUT << __T("Development Version") << endl;
#endif
	OUT << __T("Copyright (c) 2000 Symbian Ltd. All rights reserved.") << endl;
	OUT << endl;
	}

void CMakeSIS::ShowUtilsError(TUtilsException err)
// Purpose  : Write message for any error which occured whilst parsing the input pkg file 
// Inputs   : err - the error ID
	{
	switch(err)
		{
		case ErrNotEnoughMemory:
			DoErrMsg(__T("not enough memory"));
			break;
		case ErrCannotOpenFile:
			DoErrMsg(__T("cannot open file, check filename and access rights"));
			break;
		case ErrCannotReadFile:
			DoErrMsg(__T("cannot read file, check access rights"));
			break;
		case ErrCannotWriteFile:
			DoErrMsg(__T("cannot write file, check disk space"));
			break;
		case ErrCannotConvertFile:
			DoErrMsg(__T("cannot convert file to unicode"));
			DoErrMsg(__T("make sure .PKG and .TXT files are either UTF8 or UNICODE"));
			break;
		case ErrCertFileKeyFileMismatch:
			DoErrMsg(__T("key file does not match certificate(s) given"));
			DoErrMsg(__T("make sure key and certificate(s) specified correspond"));
			break;
		}
	}

void CMakeSIS::ShowParseError(TParseException err)
// Purpose  : Write message for any error which occured whilst parsing the input pkg file 
// Inputs   : err - the error ID
	{
	switch(err)
		{
		case ErrUnknownLine:
			DoErrMsg(__T("unknown line"));
			break;
		case ErrHeaderAlreadyDefined:
			DoErrMsg(__T("installation header already found"));
			break;
		case ErrUnknownLanguagesId:
			DoErrMsg(__T("unknown language specified"));
			OUT << __T("  Usage : &aa,bb,...zz") << endl;
	OUT << __T("          AF - Afrikaans, SQ - Albanian, AH - Amharic") << endl;
	OUT << __T("          AR - Arabic, HY - Armenian, AU - Australian") << endl;
	OUT << __T("          AS - Austrian, BE - Belarussian, BN - Bengali")<< endl;
	OUT << __T("          BG - Bulgarian, MY - Burmese, CA - Catalan") << endl;
	OUT << __T("          TC - Taiwan Chinese, HK - Hong Kong Chinese")<< endl;
	OUT << __T("          ZH - PRC Chinese")<< endl;
	OUT << __T("          HR - Croatian, CS - Czech, DA - Danish, DU - Dutch") << endl;
	OUT << __T("          EN - English, AM - American English, CE - Canadian English")<< endl;
	OUT << __T("          IE - International English, SF - South African English") << endl;
	OUT << __T("          ET - Estonian, FA - Farsi, FI - Finnish, BL - Belgian Flemish") << endl;
	OUT << __T("          FR - French, BF - Belgian French,  CF - Canadian French")<< endl;
	OUT << __T("          IF - International French, SF - Swiss French")<< endl;
	OUT << __T("          GD - Scots Gaelic, KA - Georgian, GE - German") << endl;
	OUT << __T("          SG - Swiss German, EL - Greek, GU - Gujarati")<< endl;
	OUT << __T("          HE - Hebrew, HI - Hindi, HU - Hungarian") << endl;
	OUT << __T("          IC - Icelandic, IN - Indonesian, GA - Irish")<< endl;
	OUT << __T("          IT - Italian, SZ - Swiss Italian, JA - Japanese, ") << endl;
	OUT << __T("          KN - Kannada, KK - Kazakh, KM - Khmer")<< endl;
	OUT << __T("          KO - Korean, LO - Laothian, LV - Latvian") << endl;
	OUT << __T("          LT - Lithuanian, MK - Macedonian, MS - Malay") << endl;
	OUT << __T("          ML - Malayalam, MR - Marathi, MO - Moldovian") << endl;
	OUT << __T("          MN - Mongolian, NZ - New Zealand, NO - Norwegian") << endl;
	OUT << __T("          NN - Norwegian Nynorsk, PL - Polish, PO - Portuguese")<< endl;
	OUT << __T("          BP - Brazilian Portuguese, PA - Punjabi, RO - Romanian") << endl;
	OUT << __T("          RU - Russian, SR - Serbian, SI - Sinhalese") << endl;
	OUT << __T("          SK - Slovak, SL - Slovenian, SO - Somali")<< endl;
	OUT << __T("          SP - Spanish, OS - International Spanish") << endl;
	OUT << __T("          LS - Latin American Spanish")<< endl;
	OUT << __T("          SH - Swahili, SW - Swedish, FS - Finland Swedish") << endl;
	OUT << __T("          TL - Tagalog, TA - Tamil, TE - Telugu, TH - Thai") << endl;
	OUT << __T("          BO - Tibetan, TI - Tigrinya, TU - Turkish")<< endl;
	OUT << __T("          CT - Cyprus Turkish, TK - Turkmen, UK - Ukrainian") << endl;
	OUT << __T("          UR - Urdu") << endl;
			//Put reserved here
	OUT << __T("          VI - Vietnamese, CY - Welsh, ZU - Zulu") << endl;
			break;
		case ErrLanguagesAlreadyDefined:
			DoErrMsg(__T("the languages have already been defined"));
			break;
		case ErrLanguagesNotDefined:
			DoErrMsg(__T("languages have not been defined yet"));
			break;
		case ErrHeaderNotDefined:
			DoErrMsg(__T("the installation header has not been defined"));
			break;
		case ErrNoMainDependency:
			DoErrMsg(__T("missing main component dependency"));
			OUT << __T("a dependency is required upon the main component being ");
			switch (m_SISWriter.GetType())
				{
				case EInstAugmentation:
					OUT << __T("patched") << endl;
					break;
				case EInstPartialUpgrade:
					OUT << __T("upgraded") << endl;
					break;
				default:
					break;
				}
			OUT << __T("the header UID should equal the UID of the main component and dependency") << endl;
			break;
		case ErrFileNotFound:
			DoErrMsg(__T("file not found"));
			break;
		case ErrPackageNotASISFile:
			DoErrMsg(__T("component is not a SIS file"));
			break;
		case ErrBadCondFormat:
			DoErrMsg(__T("incorrect condition format"));
			OUT << __T("  Usage : IF condition") << endl;
			OUT << __T("          ELSEIF condition") << endl;
			OUT << __T("          ELSE") << endl;
			OUT << __T("          ENDIF") << endl << endl;
			OUT << __T("  condition : var op \"string\"") << endl;
			OUT << __T("              var op number") << endl;
			OUT << __T("              EXISTS \"filename\"") << endl;
			OUT << __T("              (condition)AND(condition)") << endl;
			OUT << __T("              (condition)OR(condition)") << endl;
			OUT << __T("              NOT(condition)") << endl << endl;
			OUT << __T("  op :  =  <>  >  <  >=  <=") << endl;
			break;
		case ErrReadFailed:
			DoErrMsg(__T("failed to read file"));
			break;
		case ErrBadOption:
			DoErrMsg(__T("invalid option"));
			break;
		case ErrBadString:
			DoErrMsg(__T("invalid string"));
			break;
		case ErrUnexpectedToken:
			DoErrMsg(__T("unexpected text"));
			break;
		case ErrInvalidEscape:
			DoErrMsg(__T("invalid UNICODE character value"));
			break;
		case ErrNumberOutOfRange:
			DoErrMsg(__T("number out of range"));
			break;
		case ErrUnknownVariable:
			DoErrMsg(__T("unknown variable name"));
			OUT << __T("  Known variables :") << endl;
			OUT << __T("    Manufacturer, ManufacturerHardwareRev, ManufacturerSoftwareRev,") << endl;
			OUT << __T("      ManufacturerSoftwareBuild") << endl;
    		OUT << __T("    Model") << endl;
			OUT << __T("    MachineUid") << endl;
			OUT << __T("    DeviceFamily, DeviceFamilyRev") << endl;
			OUT << __T("    CPU, CPUArch, CPUABI, CPUSpeed") << endl;
			OUT << __T("    SystemTickPeriod") << endl;
			OUT << __T("    MemoryRAM, MemoryROM, MemoryPageSize") << endl;
			OUT << __T("    PowerBackup") << endl;
			OUT << __T("    Keyboard, KeyboardDeviceKeys, KeyboardAppKeys, KeyboardClick,") << endl;
			OUT << __T("      KeyboardClickVolumeMax") << endl;
			OUT << __T("    DisplayXPixels, DisplayYPixels, DisplayXTwips, DisplayYTwips,") << endl;
			OUT << __T("      DisplayColors, DisplayContrastMax") << endl;
			OUT << __T("    Backlight") << endl;
			OUT << __T("    Pen, PenX, PenY, PenDisplayOn, PenClick, PenClickVolumeMax") << endl;
			OUT << __T("    Mouse, MouseX, MouseY, MouseButtons") << endl;
			OUT << __T("    CaseSwitch") << endl;
			OUT << __T("    LEDs") << endl;
			OUT << __T("    IntegratedPhone") << endl;
			OUT << __T("    NumHalAttributes") << endl;
			OUT << __T("    Language") << endl;
			OUT << __T("    RemoteInstall") << endl;
			break;
		case ErrUninstallNeedsWaitEnd:
			DoErrMsg(__T("RW/RUNWAITEND option required with RR/RUNREMOVE or RB/RUNBOTH options"));
			break;
		case ErrUIDMismatch:
			DoErrMsg(__T("UID in the embedded SIS file is not the same as the UID given in the package file."));
			break;
		case ErrCannotGetFullPath:
			DoErrMsg(__T("Cannot get full path of embedded SIS file."));
			break;
		case ErrBadDestinationPath:
			DoErrMsg(__T("Bad destination path for file."));
			break;
		default:
			DoErrMsg(__T("syntax error"));
			break;
		}
	}

void CMakeSIS::ShowGeneratorError(TSISFileGeneratorException err)
// Purpose  : Write message for any error which occured whilst generating the output SIS file (or 
//			  stub)(to STDOUT (cout/wcout) - naturally)
// Inputs   : err - the error ID
	{
	switch(err)
		{
		case ErrSISFileNotFound:
			DoErrMsg(__T("SIS file to embed not found"));
			break;
		case ErrEmbeddedSIS:
			DoErrMsg(__T("Bad SIS file to embed"));
			break;
		case ErrCantWrite:
			DoErrMsg(__T("Can't write to output SIS file"));
			break;
		case ErrCantRead:
			DoErrMsg(__T("Can't read source file"));
			break;
		case ErrNoVendorName:
			DoErrMsg(__T("No vendor name specified"));
			break;
		case ErrNoLocalizedVendorName:
			DoErrMsg(__T("No localized vendor name specified"));
			break;
		case ErrCantSign:
			DoErrMsg(__T("Can't sign the package"));
			break;
		default:
			DoErrMsg(__T("internal error"));
			break;
		}
	};

void CMakeSIS::ShowCommandLineError(TCommandLineException err)
// Purpose  : Write message for any error which occured whilst processing the command line (to 
//            STDOUT (cout/wcout) - naturally)
// Inputs   : err - the error ID
	{
	switch(err)
		{
		case ErrInsufficientArgs:
			DoErrMsg(__T("wrong number of arguments"));
			break;
		case ErrBadCommandFlag:
			DoErrMsg(__T("unknown flag"));
			break;
		case ErrBadSourceFile:
			DoErrMsg(__T("invalid source file"));
			break;
		case ErrNoSourceFile:
			DoErrMsg(__T("no source file specified"));
			break;
		case ErrCannotOpenSourceFile:
			DoErrMsg(__T("cannot open source file"));
			break;
		case  ErrBadTargetFile:
			DoErrMsg(__T("invalid destination file"));
			break;
		default:
			DoErrMsg(__T("bad command line argumants"));
			break;
		}
	}

void CMakeSIS::ShowUsage()
// Write command line help to STDOUT (cout/wcout)
	{
#ifdef TEST
	OUT << __T("Usage : makesis [-h] [-v] [-s] [-c] [-d directory] [-p password] pkgfile [sisfile]") << endl << endl;
#else
	OUT << __T("Usage : makesis [-h] [-v] [-s] [-c] [-d directory] [-p password] [-t timestamp] pkgfile [sisfile]") << endl << endl;
#endif
	OUT << __T("  Options : -h Show help page") << endl;
	OUT << __T("  Options : -v Verbose output to screen") << endl;
	OUT << __T("  Options : -s Create stub SIS file") << endl;
	OUT << __T("  Options : -d Specify a directory path to search") << endl;
	OUT << __T("  Options : -p Specify a password to decrypt private key") << endl << endl;
	OUT << __T("  Options : -c Sign the SIS with a selfsigned certificate") << endl;
#ifdef TEST
	OUT << __T("  Options : -t Specify a timestamp") << endl << endl;
#endif
	OUT << __T("  use -h option to see pkgfile format") << endl;
	}

void CMakeSIS::ShowSyntax()
// Writes a PKG syntax summary to STDOUT (cout/wcout)
	{
	// Show the title
	ShowBanner();
	OUT << __T("MakeSIS PKG File format help") << endl;
	OUT << __T("============================") << endl << endl;
	OUT << __T("&aa,bb,...,zz    Define languages used in installation. Choose from:-") << endl;
	OUT << __T("          AF - Afrikaans, SQ - Albanian, AH - Amharic") << endl;
	OUT << __T("          AR - Arabic, HY - Armenian, AU - Australian") << endl;
	OUT << __T("          AS - Austrian, BE - Belarussian, BN - Bengali")<< endl;
	OUT << __T("          BG - Bulgarian, MY - Burmese, CA - Catalan") << endl;
	OUT << __T("          TC - Taiwan Chinese, HK - Hong Kong Chinese")<< endl;
	OUT << __T("          ZH - PRC Chinese")<< endl;
	OUT << __T("          HR - Croatian, CS - Czech, DA - Danish, DU - Dutch") << endl;
	OUT << __T("          EN - English, AM - American English, CE - Canadian English")<< endl;
	OUT << __T("          IE - International English, SF - South African English") << endl;
	OUT << __T("          ET - Estonian, FA - Farsi, FI - Finnish, BL - Belgian Flemish") << endl;
	OUT << __T("          FR - French, BF - Belgian French,  CF - Canadian French")<< endl;
	OUT << __T("          IF - International French, SF - Swiss French")<< endl;
	OUT << __T("          GD - Scots Gaelic, KA - Georgian, GE - German") << endl;
	OUT << __T("          SG - Swiss German, EL - Greek, GU - Gujarati")<< endl;
	OUT << __T("          HE - Hebrew, HI - Hindi, HU - Hungarian") << endl;
	OUT << __T("          IC - Icelandic, IN - Indonesian, GA - Irish")<< endl;
	OUT << __T("          IT - Italian, SZ - Swiss Italian, JA - Japanese, ") << endl;
	OUT << __T("          KN - Kannada, KK - Kazakh, KM - Khmer")<< endl;
	OUT << __T("          KO - Korean, LO - Laothian, LV - Latvian") << endl;
	OUT << __T("          LT - Lithuanian, MK - Macedonian, MS - Malay") << endl;
	OUT << __T("          ML - Malayalam, MR - Marathi, MO - Moldovian") << endl;
	OUT << __T("          MN - Mongolian, NZ - New Zealand, NO - Norwegian") << endl;
	OUT << __T("          NN - Norwegian Nynorsk, PL - Polish, PO - Portuguese")<< endl;
	OUT << __T("          BP - Brazilian Portuguese, PA - Punjabi, RO - Romanian") << endl;
	OUT << __T("          RU - Russian, SR - Serbian, SI - Sinhalese") << endl;
	OUT << __T("          SK - Slovak, SL - Slovenian, SO - Somali")<< endl;
	OUT << __T("          SP - Spanish, OS - International Spanish") << endl;
	OUT << __T("          LS - Latin American Spanish")<< endl;
	OUT << __T("          SH - Swahili, SW - Swedish, FS - Finland Swedish") << endl;
	OUT << __T("          TL - Tagalog, TA - Tamil, TE - Telugu, TH - Thai") << endl;
	OUT << __T("          BO - Tibetan, TI - Tigrinya, TU - Turkish")<< endl;
	OUT << __T("          CT - Cyprus Turkish, TK - Turkmen, UK - Ukrainian") << endl;
	OUT << __T("          UR - Urdu") << endl;
			//Put reserved here
	OUT << __T("          VI - Vietnamese, CY - Welsh, ZU - Zulu") << endl;

	OUT << __T("#{\"NAMEaa\", ... \"NAMEzz\"},(UID), Major, Minor, Build, Options") << endl;
	OUT << __T("                 Define file header, including name, uid and version") << endl;
	OUT << __T("*PrivateKeyFile,CertificateChainFile,[KEY=password]") << endl;
	OUT << __T("                 Digitally sign the SIS file") << endl;
	OUT << __T("\"Source\"-\"Destination\",Options") << endl;
	OUT << __T("                 File to be installed/viewed") << endl;
	OUT << __T("@\"Component\",(UID)") << endl;
	OUT << __T("                 Sub-component with UID to be installed") << endl;
	OUT << __T("(UID),Major,Minor,Build,{\"DEPENDaa\", ... \"DEPENDzz\"}") << endl;
	OUT << __T("                 Define a dependency on another component version") << endl;
	OUT << __T("{                Start of a language block") << endl;
	OUT << __T("\"Source\"         A file to install within a language block") << endl;
	OUT << __T("@\"Component\"     A component to install within a language block") << endl;
	OUT << __T("}-\"Destination\", Options") << endl;
	OUT << __T("                 End a language block containing files") << endl;
	OUT << __T("},(UID)          End a language block containing components") << endl;
	OUT << __T("IF condition     Start of block which is installed if condition evaluates") << endl;
	OUT << __T("                 to TRUE at install time. Condition format is of the form:-") << endl;
	OUT << __T("                 condition : variable op number") << endl;
	OUT << __T("                             EXISTS \"filename\"") << endl;
	OUT << __T("                             (condition)AND(condition)") << endl;
	OUT << __T("                             (condition)OR(condition)") << endl;
	OUT << __T("                             NOT(condition)") << endl;
	OUT << __T("                 op :  =  <>  >  <  >=  <=") << endl;
	OUT << __T("ELSEIF condition Block which is installed if a previous condition block") << endl;
	OUT << __T("                 was not evaluated and condition is TRUE") << endl;
	OUT << __T("ELSE             Block which is installed if no previous condition blocks") << endl;
	OUT << __T("                 were evaluated") << endl;
	OUT << __T("ENDIF            Marks end of condition block(s)") << endl;
	}

void CMakeSIS::DoVerbage(const _TCHAR* pszText) const
// Purpose  : Displays 'verbose' output text - if appropriate
// Inputs   : pszText - The text to display
	{
	if(Verbose())
		DoMsg(__T(""),pszText);
	}

void CMakeSIS::DoErrMsg(const _TCHAR* pszText) const
	{
	DoMsg(__T("error: "),pszText);
	}

void CMakeSIS::DoMsg(const _TCHAR* pszText1, const _TCHAR* pszText2) const
// Purpose  : Displays 'verbose' output text - if appropriate
// Inputs   : pszText - The text to display
	{
	if(m_wLineNo > 0)
		{
		LPCWSTR fileNameU=m_CmdOptions.SourceFile();
#ifdef _UNICODE
		OUT << fileNameU;
#else
		DWORD len=wcslen(fileNameU);
		LPSTR fileName=MakeMBCSString(fileNameU,CP_ACP,len);
		OUT << fileName;
		delete [] fileName;
#endif
		OUT << __T("(")  << m_wLineNo << __T(")") << __T(" : ");
		}
	
	OUT << pszText1 << pszText2 << endl;
	}

// main function - constructs a CMakeSIS object and calls it's run method
extern "C" {
	int main(int argc, _TCHAR *argv[], _TCHAR *envp[])
	{
		if(sizeof(WCHAR) != 4) {
			OUT << "WCHAR error. Check your compiler\n";
			exit(1);
		}

		CMakeSIS app;
		return app.Run(argc, argv, envp);
	}
}

