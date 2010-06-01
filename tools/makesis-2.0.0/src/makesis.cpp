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
#include <cstdlib>
#include <iostream>
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
		
		

		if(ParseSource() && WriteTarget())
			{
			LPCWSTR fileNameU=m_CmdOptions.TargetFile();
#ifdef _UNICODE
			OUT << _T("Created ") << fileNameU << endl;
#else
			DWORD len=wcslen(fileNameU);
			LPSTR fileName=MakeMBCSString(fileNameU,CP_ACP,len);
			OUT << _T("Created ") << fileName << endl;
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
		OUT << _T("Processing ") << m_CmdOptions.SourceFile() << _T("...") << endl;
#else
		DWORD length=wcslen(m_CmdOptions.SourceFile());
		LPSTR fName=MakeMBCSString(m_CmdOptions.SourceFile(), CP_ACP, length);
		OUT << _T("Processing ") << fName << _T("...") << endl;
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
			DoVerbage(_T("Generating SIS stub file...\n"));
		else
			DoVerbage(_T("Generating SIS installation file..."));
		m_SISWriter.WriteSIS(m_CmdOptions.TargetFile(),stubFile);
		}
	catch(TGeneratorException err)
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
	OUT << endl << _T("MakeSIS, version ") << major << _T(".") << minor << endl;
	OUT << _T("A utility for creating Software Installation (SIS) files.") << endl;
#ifdef _DEBUG
	OUT << _T("Development Version") << endl;
#endif
	OUT << _T("Copyright (c) 2000 Symbian Ltd. All rights reserved.") << endl;
	OUT << endl;
	}

void CMakeSIS::ShowUtilsError(TUtilsException err)
// Purpose  : Write message for any error which occured whilst parsing the input pkg file 
// Inputs   : err - the error ID
	{
	switch(err)
		{
		case ErrNotEnoughMemory:
			DoErrMsg(_T("not enough memory"));
			break;
		case ErrCannotOpenFile:
			DoErrMsg(_T("cannot open file, check filename and access rights"));
			break;
		case ErrCannotReadFile:
			DoErrMsg(_T("cannot read file, check access rights"));
			break;
		case ErrCannotWriteFile:
			DoErrMsg(_T("cannot write file, check disk space"));
			break;
		case ErrCannotConvertFile:
			DoErrMsg(_T("cannot convert file to unicode"));
			DoErrMsg(_T("make sure .PKG and .TXT files are either UTF8 or UNICODE"));
			break;
		case ErrCertFileKeyFileMismatch:
			DoErrMsg(_T("key file does not match certificate(s) given"));
			DoErrMsg(_T("make sure key and certificate(s) specified correspond"));
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
			DoErrMsg(_T("unknown line"));
			break;
		case ErrHeaderAlreadyDefined:
			DoErrMsg(_T("installation header already found"));
			break;
		case ErrUnknownLanguagesId:
			DoErrMsg(_T("unknown language specified"));
			OUT << _T("  Usage : &aa,bb,...zz") << endl;
	OUT << _T("          AF - Afrikaans, SQ - Albanian, AH - Amharic") << endl;
	OUT << _T("          AR - Arabic, HY - Armenian, AU - Australian") << endl;
	OUT << _T("          AS - Austrian, BE - Belarussian, BN - Bengali")<< endl;
	OUT << _T("          BG - Bulgarian, MY - Burmese, CA - Catalan") << endl;
	OUT << _T("          TC - Taiwan Chinese, HK - Hong Kong Chinese")<< endl;
	OUT << _T("          ZH - PRC Chinese")<< endl;
	OUT << _T("          HR - Croatian, CS - Czech, DA - Danish, DU - Dutch") << endl;
	OUT << _T("          EN - English, AM - American English, CE - Canadian English")<< endl;
	OUT << _T("          IE - International English, SF - South African English") << endl;
	OUT << _T("          ET - Estonian, FA - Farsi, FI - Finnish, BL - Belgian Flemish") << endl;
	OUT << _T("          FR - French, BF - Belgian French,  CF - Canadian French")<< endl;
	OUT << _T("          IF - International French, SF - Swiss French")<< endl;
	OUT << _T("          GD - Scots Gaelic, KA - Georgian, GE - German") << endl;
	OUT << _T("          SG - Swiss German, EL - Greek, GU - Gujarati")<< endl;
	OUT << _T("          HE - Hebrew, HI - Hindi, HU - Hungarian") << endl;
	OUT << _T("          IC - Icelandic, IN - Indonesian, GA - Irish")<< endl;
	OUT << _T("          IT - Italian, SZ - Swiss Italian, JA - Japanese, ") << endl;
	OUT << _T("          KN - Kannada, KK - Kazakh, KM - Khmer")<< endl;
	OUT << _T("          KO - Korean, LO - Laothian, LV - Latvian") << endl;
	OUT << _T("          LT - Lithuanian, MK - Macedonian, MS - Malay") << endl;
	OUT << _T("          ML - Malayalam, MR - Marathi, MO - Moldovian") << endl;
	OUT << _T("          MN - Mongolian, NZ - New Zealand, NO - Norwegian") << endl;
	OUT << _T("          NN - Norwegian Nynorsk, PL - Polish, PO - Portuguese")<< endl;
	OUT << _T("          BP - Brazilian Portuguese, PA - Punjabi, RO - Romanian") << endl;
	OUT << _T("          RU - Russian, SR - Serbian, SI - Sinhalese") << endl;
	OUT << _T("          SK - Slovak, SL - Slovenian, SO - Somali")<< endl;
	OUT << _T("          SP - Spanish, OS - International Spanish") << endl;
	OUT << _T("          LS - Latin American Spanish")<< endl;
	OUT << _T("          SH - Swahili, SW - Swedish, FS - Finland Swedish") << endl;
	OUT << _T("          TL - Tagalog, TA - Tamil, TE - Telugu, TH - Thai") << endl;
	OUT << _T("          BO - Tibetan, TI - Tigrinya, TU - Turkish")<< endl;
	OUT << _T("          CT - Cyprus Turkish, TK - Turkmen, UK - Ukrainian") << endl;
	OUT << _T("          UR - Urdu") << endl;
			//Put reserved here
	OUT << _T("          VI - Vietnamese, CY - Welsh, ZU - Zulu") << endl;
			break;
		case ErrLanguagesAlreadyDefined:
			DoErrMsg(_T("the languages have already been defined"));
			break;
		case ErrLanguagesNotDefined:
			DoErrMsg(_T("languages have not been defined yet"));
			break;
		case ErrHeaderNotDefined:
			DoErrMsg(_T("the installation header has not been defined"));
			break;
		case ErrNoMainDependency:
			DoErrMsg(_T("missing main component dependency"));
			OUT << _T("a dependency is required upon the main component being ");
			switch (m_SISWriter.GetType())
				{
				case EInstSISConfig:
					OUT << _T("configured") << endl;
					break;
				case EInstSISPatch:
					OUT << _T("patched") << endl;
					break;
				case EInstSISUpgrade:
					OUT << _T("upgraded") << endl;
					break;
				default:
					break;
				}
			OUT << _T("the header UID should equal the UID of the main component and dependency") << endl;
			break;
		case ErrFileNotFound:
			DoErrMsg(_T("file not found"));
			break;
		case ErrPackageNotASISFile:
			DoErrMsg(_T("component is not a SIS file"));
			break;
		case ErrBadCondFormat:
			DoErrMsg(_T("incorrect condition format"));
			OUT << _T("  Usage : IF condition") << endl;
			OUT << _T("          ELSEIF condition") << endl;
			OUT << _T("          ELSE") << endl;
			OUT << _T("          ENDIF") << endl << endl;
			OUT << _T("  condition : var op \"string\"") << endl;
			OUT << _T("              var op number") << endl;
			OUT << _T("              EXISTS \"filename\"") << endl;
			OUT << _T("              (condition)AND(condition)") << endl;
			OUT << _T("              (condition)OR(condition)") << endl;
			OUT << _T("              NOT(condition)") << endl << endl;
			OUT << _T("  op :  =  <>  >  <  >=  <=") << endl;
			break;
		case ErrReadFailed:
			DoErrMsg(_T("failed to read file"));
			break;
		case ErrBadOption:
			DoErrMsg(_T("invalid option"));
			break;
		case ErrBadString:
			DoErrMsg(_T("invalid string"));
			break;
		case ErrUnexpectedToken:
			DoErrMsg(_T("unexpected text"));
			break;
		case ErrInvalidEscape:
			DoErrMsg(_T("invalid UNICODE character value"));
			break;
		case ErrNumberOutOfRange:
			DoErrMsg(_T("number out of range"));
			break;
		case ErrUnknownVariable:
			DoErrMsg(_T("unknown variable name"));
			OUT << _T("  Known variables :") << endl;
			OUT << _T("    Manufacturer, ManufacturerHardwareRev, ManufacturerSoftwareRev,") << endl;
			OUT << _T("      ManufacturerSoftwareBuild") << endl;
    		OUT << _T("    Model") << endl;
			OUT << _T("    MachineUid") << endl;
			OUT << _T("    DeviceFamily, DeviceFamilyRev") << endl;
			OUT << _T("    CPU, CPUArch, CPUABI, CPUSpeed") << endl;
			OUT << _T("    SystemTickPeriod") << endl;
			OUT << _T("    MemoryRAM, MemoryROM, MemoryPageSize") << endl;
			OUT << _T("    PowerBackup") << endl;
			OUT << _T("    Keyboard, KeyboardDeviceKeys, KeyboardAppKeys, KeyboardClick,") << endl;
			OUT << _T("      KeyboardClickVolumeMax") << endl;
			OUT << _T("    DisplayXPixels, DisplayYPixels, DisplayXTwips, DisplayYTwips,") << endl;
			OUT << _T("      DisplayColors, DisplayContrastMax") << endl;
			OUT << _T("    Backlight") << endl;
			OUT << _T("    Pen, PenX, PenY, PenDisplayOn, PenClick, PenClickVolumeMax") << endl;
			OUT << _T("    Mouse, MouseX, MouseY, MouseButtons") << endl;
			OUT << _T("    CaseSwitch") << endl;
			OUT << _T("    LEDs") << endl;
			OUT << _T("    IntegratedPhone") << endl;
			OUT << _T("    NumHalAttributes") << endl;
			OUT << _T("    Language") << endl;
			OUT << _T("    RemoteInstall") << endl;
			break;
		case ErrUninstallNeedsWaitEnd:
			DoErrMsg(_T("RW/RUNWAITEND option required with RR/RUNREMOVE or RB/RUNBOTH options"));
			break;
		case ErrUIDMismatch:
			DoErrMsg(_T("UID in the embedded SIS file is not the same as the UID given in the package file."));
			break;
		case ErrCannotGetFullPath:
			DoErrMsg(_T("Cannot get full path of embedded SIS file."));
			break;
		case ErrBadDestinationPath:
			DoErrMsg(_T("Bad destination path for file."));
			break;
		default:
			DoErrMsg(_T("syntax error"));
			break;
		}
	}

void CMakeSIS::ShowGeneratorError(TGeneratorException err)
// Purpose  : Write message for any error which occured whilst generating the output SIS file (or 
//			  stub)(to STDOUT (cout/wcout) - naturally)
// Inputs   : err - the error ID
	{
	switch(err)
		{
		case ErrFailedToWriteHeader:		
			DoErrMsg(_T("failed whilst writing HEADER block"));
			break;
		case ErrFailedToWriteLanguages:
			DoErrMsg(_T("failed whilst writing LANGUAGE block"));
			break;
		case ErrFailedToWriteFilesBlock:
			DoErrMsg(_T("failed whilst writing FILES block"));
			break;
		case ErrFailedToWriteConditionBlock:
			DoErrMsg(_T("failed whilst writing CONDITIONS block"));
			break;
		case ErrFailedToWriteOptionsBlock:
			DoErrMsg(_T("failed whilst writing OPTIONS block"));
			break;
		case ErrFailedToWriteDependencyBlock:
			DoErrMsg(_T("failed whilst writing DEPENDENCY block"));
			break;
		case ErrFailedToWriteStringsBlock:
			DoErrMsg(_T("failed whilst writing STRINGS block"));
			break;
		case ErrFailedToWriteCRC:
			DoErrMsg(_T("failed whilst to write CRC"));
			break;
		case ErrFailedToOpenStubTarget:
			DoErrMsg(_T("cannot Open Destination Stub File"));
			break;
		case ErrFailedCompression:
			DoErrMsg(_T("Failed to compress Files"));
			break;
		case ErrCannotOpenFile:
			DoErrMsg(_T("unable to open file"));
			break;
		case ErrCannotReadFile:
			DoErrMsg(_T("unable to read file"));
			break;
		case ErrCannotWriteFile:
			DoErrMsg(_T("failed whilst writing file"));
			break;
		default:
			DoErrMsg(_T("internal error"));
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
			DoErrMsg(_T("wrong number of arguments"));
			break;
		case ErrBadCommandFlag:
			DoErrMsg(_T("unknown flag"));
			break;
		case ErrBadSourceFile:
			DoErrMsg(_T("invalid source file"));
			break;
		case ErrNoSourceFile:
			DoErrMsg(_T("no source file specified"));
			break;
		case ErrCannotOpenSourceFile:
			DoErrMsg(_T("cannot open source file"));
			break;
		case  ErrBadTargetFile:
			DoErrMsg(_T("invalid destination file"));
			break;
		default:
			DoErrMsg(_T("bad command line argumants"));
			break;
		}
	}

void CMakeSIS::ShowUsage()
// Write command line help to STDOUT (cout/wcout)
	{
	OUT << _T("Usage : makesis [-h] [-v] [-s] [-d directory] [-p password] pkgfile [sisfile]") << endl << endl;
	OUT << _T("  Options : -h Show help page") << endl;
	OUT << _T("  Options : -v Verbose output to screen") << endl;
	OUT << _T("  Options : -s Create stub SIS file") << endl;
	OUT << _T("  Options : -d Specify a directory path to search") << endl;
	OUT << _T("  Options : -p Specify a password to decrypt private key") << endl << endl;
	OUT << _T("  use -h option to see pkgfile format") << endl;
	}

void CMakeSIS::ShowSyntax()
// Writes a PKG syntax summary to STDOUT (cout/wcout)
	{
	// Show the title
	ShowBanner();
	OUT << _T("MakeSIS PKG File format help") << endl;
	OUT << _T("============================") << endl << endl;
	OUT << _T("&aa,bb,...,zz    Define languages used in installation. Choose from:-") << endl;
	OUT << _T("          AF - Afrikaans, SQ - Albanian, AH - Amharic") << endl;
	OUT << _T("          AR - Arabic, HY - Armenian, AU - Australian") << endl;
	OUT << _T("          AS - Austrian, BE - Belarussian, BN - Bengali")<< endl;
	OUT << _T("          BG - Bulgarian, MY - Burmese, CA - Catalan") << endl;
	OUT << _T("          TC - Taiwan Chinese, HK - Hong Kong Chinese")<< endl;
	OUT << _T("          ZH - PRC Chinese")<< endl;
	OUT << _T("          HR - Croatian, CS - Czech, DA - Danish, DU - Dutch") << endl;
	OUT << _T("          EN - English, AM - American English, CE - Canadian English")<< endl;
	OUT << _T("          IE - International English, SF - South African English") << endl;
	OUT << _T("          ET - Estonian, FA - Farsi, FI - Finnish, BL - Belgian Flemish") << endl;
	OUT << _T("          FR - French, BF - Belgian French,  CF - Canadian French")<< endl;
	OUT << _T("          IF - International French, SF - Swiss French")<< endl;
	OUT << _T("          GD - Scots Gaelic, KA - Georgian, GE - German") << endl;
	OUT << _T("          SG - Swiss German, EL - Greek, GU - Gujarati")<< endl;
	OUT << _T("          HE - Hebrew, HI - Hindi, HU - Hungarian") << endl;
	OUT << _T("          IC - Icelandic, IN - Indonesian, GA - Irish")<< endl;
	OUT << _T("          IT - Italian, SZ - Swiss Italian, JA - Japanese, ") << endl;
	OUT << _T("          KN - Kannada, KK - Kazakh, KM - Khmer")<< endl;
	OUT << _T("          KO - Korean, LO - Laothian, LV - Latvian") << endl;
	OUT << _T("          LT - Lithuanian, MK - Macedonian, MS - Malay") << endl;
	OUT << _T("          ML - Malayalam, MR - Marathi, MO - Moldovian") << endl;
	OUT << _T("          MN - Mongolian, NZ - New Zealand, NO - Norwegian") << endl;
	OUT << _T("          NN - Norwegian Nynorsk, PL - Polish, PO - Portuguese")<< endl;
	OUT << _T("          BP - Brazilian Portuguese, PA - Punjabi, RO - Romanian") << endl;
	OUT << _T("          RU - Russian, SR - Serbian, SI - Sinhalese") << endl;
	OUT << _T("          SK - Slovak, SL - Slovenian, SO - Somali")<< endl;
	OUT << _T("          SP - Spanish, OS - International Spanish") << endl;
	OUT << _T("          LS - Latin American Spanish")<< endl;
	OUT << _T("          SH - Swahili, SW - Swedish, FS - Finland Swedish") << endl;
	OUT << _T("          TL - Tagalog, TA - Tamil, TE - Telugu, TH - Thai") << endl;
	OUT << _T("          BO - Tibetan, TI - Tigrinya, TU - Turkish")<< endl;
	OUT << _T("          CT - Cyprus Turkish, TK - Turkmen, UK - Ukrainian") << endl;
	OUT << _T("          UR - Urdu") << endl;
			//Put reserved here
	OUT << _T("          VI - Vietnamese, CY - Welsh, ZU - Zulu") << endl;

	OUT << _T("#{\"NAMEaa\", ... \"NAMEzz\"},(UID), Major, Minor, Build, Options") << endl;
	OUT << _T("                 Define file header, including name, uid and version") << endl;
	OUT << _T("*PrivateKeyFile,CertificateChainFile,[KEY=password]") << endl;
	OUT << _T("                 Digitally sign the SIS file") << endl;
	OUT << _T("\"Source\"-\"Destination\",Options") << endl;
	OUT << _T("                 File to be installed/viewed") << endl;
	OUT << _T("@\"Component\",(UID)") << endl;
	OUT << _T("                 Sub-component with UID to be installed") << endl;
	OUT << _T("(UID),Major,Minor,Build,{\"DEPENDaa\", ... \"DEPENDzz\"}") << endl;
	OUT << _T("                 Define a dependency on another component version") << endl;
	OUT << _T("{                Start of a language block") << endl;
	OUT << _T("\"Source\"         A file to install within a language block") << endl;
	OUT << _T("@\"Component\"     A component to install within a language block") << endl;
	OUT << _T("}-\"Destination\", Options") << endl;
	OUT << _T("                 End a language block containing files") << endl;
	OUT << _T("},(UID)          End a language block containing components") << endl;
	OUT << _T("IF condition     Start of block which is installed if condition evaluates") << endl;
	OUT << _T("                 to TRUE at install time. Condition format is of the form:-") << endl;
	OUT << _T("                 condition : variable op number") << endl;
	OUT << _T("                             EXISTS \"filename\"") << endl;
	OUT << _T("                             (condition)AND(condition)") << endl;
	OUT << _T("                             (condition)OR(condition)") << endl;
	OUT << _T("                             NOT(condition)") << endl;
	OUT << _T("                 op :  =  <>  >  <  >=  <=") << endl;
	OUT << _T("ELSEIF condition Block which is installed if a previous condition block") << endl;
	OUT << _T("                 was not evaluated and condition is TRUE") << endl;
	OUT << _T("ELSE             Block which is installed if no previous condition blocks") << endl;
	OUT << _T("                 were evaluated") << endl;
	OUT << _T("ENDIF            Marks end of condition block(s)") << endl;
	}

void CMakeSIS::DoVerbage(const _TCHAR* pszText) const
// Purpose  : Displays 'verbose' output text - if appropriate
// Inputs   : pszText - The text to display
	{
	if(Verbose())
		DoMsg(_T(""),pszText);
	}

void CMakeSIS::DoErrMsg(const _TCHAR* pszText) const
	{
	DoMsg(_T("error: "),pszText);
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
		OUT << _T("(")  << m_wLineNo << _T(")") << _T(" : ");
		}
	
	OUT << pszText1 << pszText2 << endl;
	}

// main function - constructs a CMakeSIS object and calls it's run method
extern "C" {
	int main(int argc, _TCHAR *argv[], _TCHAR *envp[])
	{
		if(sizeof(WCHAR) < 2) {
			OUT << "WCHAR error. Check your compiler\n";
			exit(1);
		}

		CMakeSIS app;
		return app.Run(argc, argv, envp);
	}
}

