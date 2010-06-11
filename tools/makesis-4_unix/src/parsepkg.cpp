// PARSEPKG.cpp
// Copyright (c) 1997-1999 Symbian Ltd. All rights reserved.
//
// handles parsing of PKG file
//

// ===========================================================================
// INCLUDES
// ===========================================================================

#include <wctype.h>
#include <wchar.h>
#include "utils.h"
#include <assert.h>
#include <wchar.h>
#include <tchar.h>
#include <errno.h>
#include <instform.h>
#ifdef __SYMBIAN32__
#include <hal_data.h>
#endif
#include <iostream>
#include <stdio.h>
#include "parsepkg.h"
#include "parsecmd.h"
#include "crtsis.h"
#include "wcstub.h"


// ===========================================================================
// CONSTANTS
// ===========================================================================

// Set up the cout stream so that we can use it with either narrow or wide
// chars at build time
#undef OUT			   // coz it's an IDL thing...

#ifdef _UNICODE
#define OUT wcout
#else
#define OUT cout
#endif

// Parse options lookups
const SParseOpt KHeaderOptions[] =
	{
		{L"IU", EInstFlagIsUnicode},
		{L"SH", EInstFlagShutdownApps},
		{L"NC", EInstFlagNoCompress},
		{L"IUNICODE",		EInstFlagIsUnicode},
		{L"SHUTDOWNAPPS",	EInstFlagShutdownApps},
		{L"NOCOMPRESS",		EInstFlagNoCompress},
	};
#define NUMHEADEROPTIONS (sizeof(KHeaderOptions)/sizeof(SParseOpt))

const SParseOpt KSISTypes[] =
	{
		{L"SA", EInstApplication},
		{L"SP", EInstAugmentation},
		{L"PU", EInstPartialUpgrade},
		{L"SISAPP",	EInstApplication},
		{L"SISPATCH",	EInstAugmentation},
		{L"PARTIALUPGRADE", EInstPartialUpgrade},
	};
#define NUMSISTYPEOPTIONS (sizeof(KSISTypes)/sizeof(SParseOpt))

const SParseOpt KTypeOptions[] =
	{
		{L"FF", EInstFileTypeSimple},
		{L"FT", EInstFileTypeText},
		{L"FR", EInstFileTypeRun},
		{L"FN", EInstFileTypeNull},
		{L"FM", EInstFileTypeMime},
		{L"FILE",		EInstFileTypeSimple},
		{L"FILETEXT",	EInstFileTypeText},
		{L"FILERUN",	EInstFileTypeRun},
		{L"FILENULL",	EInstFileTypeNull},
		{L"FILEMIME",	EInstFileTypeMime},
	};
#define NUMTYPEOPTIONS (sizeof(KTypeOptions)/sizeof(SParseOpt))

const SParseOpt KTextOptions[] =
	{
		{L"TC", EInstFileTextOptionContinue},
		{L"TS", EInstFileTextOptionSkipOneIfNo},
		{L"TA", EInstFileTextOptionAbortIfNo},
		{L"TE", EInstFileTextOptionExitIfNo},
		{L"TEXTCONTINUE",	EInstFileTextOptionContinue},
		{L"TEXTSKIP",		EInstFileTextOptionSkipOneIfNo},
		{L"TEXTABORT",		EInstFileTextOptionAbortIfNo},
		{L"TEXTEXIT",		EInstFileTextOptionExitIfNo},
	};
#define NUMTEXTOPTIONS (sizeof(KTextOptions)/sizeof(SParseOpt))

const SParseOpt KRunOptions[] =
	{
		{L"RI",  EInstFileRunOptionInstall},
		{L"RR", (EInstFileRunOptionUninstall | EInstFileRunOptionWaitEnd)},
		{L"RB", (EInstFileRunOptionInstall | EInstFileRunOptionUninstall | EInstFileRunOptionWaitEnd)},
		{L"RS", EInstFileRunOptionSendEnd},
		{L"RW", EInstFileRunOptionWaitEnd},
		{L"RUNINSTALL",	EInstFileRunOptionInstall},
		{L"RUNREMOVE", (EInstFileRunOptionUninstall | EInstFileRunOptionWaitEnd)},
		{L"RUNBOTH",   (EInstFileRunOptionInstall | EInstFileRunOptionUninstall | EInstFileRunOptionWaitEnd)},
		{L"RUNSENDEND", EInstFileRunOptionSendEnd},
		{L"RUNWAITEND", EInstFileRunOptionWaitEnd},
	};
#define NUMRUNOPTIONS (sizeof(KRunOptions)/sizeof(SParseOpt))

// Parse options lookups
#define MAXTOKENLEN	30
struct SParseToken
	{
	WCHAR pszOpt[MAXTOKENLEN];
	DWORD dwOpt;
	};

const SParseToken KTokens[] =
	{
		{L"if",		IF_TOKEN},
		{L"elseif",	ELSEIF_TOKEN},
		{L"else",	ELSE_TOKEN},
		{L"endif",	ENDIF_TOKEN},
		{L"exists",	EXISTS_TOKEN},
		{L"package",	DEVPROP_TOKEN},
		{L"appprop",	APPPROP_TOKEN},
		{L"not",	NOT_TOKEN},
		{L"and",	AND_TOKEN},
		{L"or",		OR_TOKEN},
		{L"type",	TYPE_TOKEN},
		{L"key",	KEY_TOKEN},
	};
#define NUMPARSETOKENS (sizeof(KTokens)/sizeof(SParseToken))

// Language options
const SParseOpt KLangOptions[] = 
	{
		{L"EN", ELangEnglish},
		{L"FR", ELangFrench},
		{L"GE", ELangGerman},
		{L"SP", ELangSpanish},
		{L"IT", ELangItalian},
		{L"SW", ELangSwedish},
		{L"DA", ELangDanish},
		{L"NO", ELangNorwegian},
		{L"FI", ELangFinnish},
		{L"AM", ELangAmerican},
		{L"SF", ELangSwissFrench},
		{L"SG", ELangSwissGerman},
		{L"PO", ELangPortuguese},
		{L"TU", ELangTurkish},
		{L"IC", ELangIcelandic},
		{L"RU", ELangRussian},
		{L"HU", ELangHungarian},
		{L"DU", ELangDutch},
		{L"BL", ELangBelgianFlemish},
		{L"AU", ELangAustralian},
		{L"BF", ELangBelgianFrench},
		{L"AS", ELangAustrian},
		{L"NZ", ELangNewZealand},
		{L"IF", ELangInternationalFrench},
		{L"CS", ELangCzech},
		{L"SK", ELangSlovak},
		{L"PL", ELangPolish},
		{L"SL", ELangSlovenian},
		{L"TC", ELangTaiwanChinese},
		{L"HK", ELangHongKongChinese},
		{L"ZH", ELangPrcChinese},
		{L"JA", ELangJapanese},
		{L"TH", ELangThai},

		{L"AF", ELangAfrikaans },
		{L"SQ", ELangAlbanian },
		{L"AH", ELangAmharic },
		{L"AR", ELangArabic },
		{L"HY", ELangArmenian },
		{L"TL", ELangTagalog },
		{L"BE", ELangBelarussian },
		{L"BN", ELangBengali },
		{L"BG", ELangBulgarian },
		{L"MY", ELangBurmese },
		{L"CA", ELangCatalan },
		{L"HR", ELangCroatian },
		{L"CE", ELangCanadianEnglish },
		{L"IE", ELangInternationalEnglish },
		{L"SF", ELangSouthAfricanEnglish },
		{L"ET", ELangEstonian },
		{L"FA", ELangFarsi },
		{L"CF", ELangCanadianFrench },
		{L"GD", ELangScotsGaelic },
		{L"KA", ELangGeorgian },
		{L"EL", ELangGreek },
		{L"CG", ELangCyprusGreek },
		{L"GU", ELangGujarati },
		{L"HE", ELangHebrew },
		{L"HI", ELangHindi },
		{L"IN", ELangIndonesian },
		{L"GA", ELangIrish },
		{L"SZ", ELangSwissItalian },
		{L"KN", ELangKannada },
		{L"KK", ELangKazakh },
		{L"KM", ELangKhmer },
		{L"KO", ELangKorean },
		{L"LO", ELangLao },
		{L"LV", ELangLatvian },
		{L"LT", ELangLithuanian },
		{L"MK", ELangMacedonian },
		{L"MS", ELangMalay },
		{L"ML", ELangMalayalam },
		{L"MR", ELangMarathi },
		{L"MO", ELangMoldavian },
		{L"MN", ELangMongolian },
		{L"NN", ELangNorwegianNynorsk },
		{L"BP", ELangBrazilianPortuguese },
		{L"PA", ELangPunjabi },
		{L"RO", ELangRomanian },
		{L"SR", ELangSerbian },
		{L"SI", ELangSinhalese },
		{L"SO", ELangSomali },
		{L"OS", ELangInternationalSpanish },
		{L"LS", ELangLatinAmericanSpanish },
		{L"SH", ELangSwahili },
		{L"FS", ELangFinlandSwedish },
		//{L"??", ELangReserved1 },
		{L"TA", ELangTamil },
		{L"TE", ELangTelugu },
		{L"BO", ELangTibetan },
		{L"TI", ELangTigrinya },
		{L"CT", ELangCyprusTurkish },
		{L"TK", ELangTurkmen },
		{L"UK", ELangUkrainian },
		{L"UR", ELangUrdu },
		//{L"??", ELangReserved2 },
		{L"VI", ELangVietnamese },
		{L"CY", ELangWelsh },
		{L"ZU", ELangZulu },
		//{L"??", ELangOther },
		//{L"??", ELangNone  }

	};
#define NUMLANGOPTIONS (sizeof(KLangOptions)/sizeof(SParseOpt))

/*
 * Optional dependency on Symbian SDK
 */

#ifndef __SYMBIAN32__
#undef X
#define X(a) E ## a,

enum {
#include "haltable.hm"
};
#undef X
#endif

const SParseToken KVariableLookupTable[] =
	{
#ifndef __SYMBIAN32__
#undef X
#define X(a) {L ## #a , E ## a},
#include "haltable.hm"
#undef X
		{L"NumHalAttributes",			0x66},
#else
		{L"Manufacturer",				HALData::EManufacturer},
		{L"ManufacturerHardwareRev",	HALData::EManufacturerHardwareRev},
		{L"ManufacturerSoftwareRev",	HALData::EManufacturerSoftwareRev},
		{L"ManufacturerSoftwareBuild",	HALData::EManufacturerSoftwareBuild},
		{L"Model",						HALData::EModel},
		{L"MachineUid",					HALData::EMachineUid},
		{L"DeviceFamily",				HALData::EDeviceFamily},
		{L"DeviceFamilyRev",			HALData::EDeviceFamilyRev},
		{L"CPU",						HALData::ECPU},
		{L"CPUArch",					HALData::ECPUArch},
		{L"CPUABI",						HALData::ECPUABI},
		{L"CPUSpeed",					HALData::ECPUSpeed},
		//{L"SystemStartupReason",		HALData::ESystemStartupReason},
		//{L"SystemException",			HALData::ESystemException},
		{L"SystemTickPeriod",			HALData::ESystemTickPeriod},
		{L"MemoryRAM",					HALData::EMemoryRAM},
		{L"MemoryRAMFree",				HALData::EMemoryRAMFree},
		{L"MemoryROM",					HALData::EMemoryROM},
		{L"MemoryPageSize",				HALData::EMemoryPageSize},
		//{L"PowerGood",				HALData::EPowerGood},
		//{L"PowerBatteryGood",			HALData::EPowerBatteryGood},
		{L"PowerBackup",				HALData::EPowerBackup},
		//{L"PowerBackupGood",			HALData::EPowerBackupGood},
		//{L"PowerExternal",				HALData::EPowerExternal},
		{L"Keyboard",					HALData::EKeyboard},
		{L"KeyboardDeviceKeys",			HALData::EKeyboardDeviceKeys},
		{L"KeyboardAppKeys",			HALData::EKeyboardAppKeys},
		{L"KeyboardClick",				HALData::EKeyboardClick},
		//{L"KeyboardClickState",		HALData::EKeyboardClickState},
		//{L"KeyboardClickVolume",		HALData::EKeyboardClickVolume},
		{L"KeyboardClickVolumeMax",		HALData::EKeyboardClickVolumeMax},
		{L"DisplayXPixels",				HALData::EDisplayXPixels},
		{L"DisplayYPixels",				HALData::EDisplayYPixels},
		{L"DisplayXTwips",				HALData::EDisplayXTwips},
		{L"DisplayYTwips",				HALData::EDisplayYTwips},
		{L"DisplayColors",				HALData::EDisplayColors},
		//{L"DisplayState",				HALData::EDisplayState},
		//{L"DisplayContrast",			HALData::EDisplayContrast},
		{L"DisplayContrastMax",			HALData::EDisplayContrastMax},
		{L"Backlight",					HALData::EBacklight},
		//{L"BacklightState",			HALData::EBacklightState},
		{L"Pen",						HALData::EPen},
		{L"PenX",						HALData::EPenX},
		{L"PenY",						HALData::EPenY},
		{L"PenDisplayOn",				HALData::EPenDisplayOn},
		{L"PenClick",					HALData::EPenClick},
		//{L"PenClickState",			HALData::EPenClickState},
		//{L"PenClickVolume",			HALData::EPenClickVolume},
		{L"PenClickVolumeMax",			HALData::EPenClickVolumeMax},
		{L"Mouse",						HALData::EMouse},
		{L"MouseX",						HALData::EMouseX},
		{L"MouseY",						HALData::EMouseY},
		//{L"MouseState",				HALData::EMouseState},
		//{L"MouseSpeed",				HALData::EMouseSpeed},
		//{L"MouseAcceleration",		HALData::EMouseAcceleration},
		{L"MouseButtons",				HALData::EMouseButtons},
		//{L"MouseButtonState",			HALData::EMouseButtonState},
		//{L"CaseState",					HALData::ECaseState},
		{L"CaseSwitch",					HALData::ECaseSwitch},
		//{L"CaseSwitchDisplayOn",		HALData::ECaseSwitchDisplayOn},
		//{L"CaseSwitchDisplayOff",		HALData::ECaseSwitchDisplayOff},
		{L"LEDs",						HALData::ELEDs},
		//{L"LEDmask",					HALData::ELEDmask},
		{L"IntegratedPhone",			HALData::EIntegratedPhone},
		//{L"DisplayBrightness",			HALData::EDisplayBrightness},
		{L"DisplayBrightnessMax",		HALData::EDisplayBrightnessMax},
		{L"KeyboardBacklightState",		HALData::EKeyboardBacklightState},
		{L"AccessoryPower",				HALData::EAccessoryPower},

		{L"NumHalAttributes",			HALData::ENumHalAttributes},
#endif
		{L"Language",					EVarLanguage},
		{L"RemoteInstall",				EVarRemoteInstall},
	};


#define NUMVAROPTIONS (sizeof(KVariableLookupTable)/sizeof(SParseOpt))

// ===========================================================================
// CParsePkg
// ===========================================================================

CParsePkg::CParsePkg()
:m_nLineNo(0),	   		// The line we are currently on
m_fInLangBlock(0),		// Are we processing a language block
m_fInLangFileBlock(0),	// Are we processing a language dependent file block
m_wCurrLang(0),   		// If we are in a lang/lang file block - which lang are we processing
m_pSISWriter(NULL)		// Stuff all our intermediate data in here
	{
	memset((void *)m_pszSearchDir, 0x0000, MAX_PATH * sizeof(WCHAR));
	}

void CParsePkg::SetSearchDirectory(LPCWSTR pszPath)
// Purpose  : Sets the search directory
// Inputs   : pszPath  - The directory path (including trailing backslash!)
	{
	wcscpy(m_pszSearchDir, pszPath);
	DWORD len=wcslen(m_pszSearchDir);
	if(len>0 && m_pszSearchDir[len-1] != '/') 	// Ensure a trailing slash
		{
		m_pszSearchDir[len] = '/';
		m_pszSearchDir[len+1] = '\0';
		}
	}

void CParsePkg::ParseL(HANDLE hFile, CSISWriter *pWriter, BOOL fIsStub, MParserObserver *pObserver /*= NULL*/)
// Purpose  : Check the contents of an input line and defers parsing to the appropriate ParseXXXL()
//			  method
// Inputs   : m_pkgPtr - The line to process
// Returns  : Success or failure (actually, always TRUE - failure causes an exception to be thrown)
	{
	assert(hFile != INVALID_HANDLE_VALUE);
	assert(pWriter);
	
	m_file = hFile;
	m_pSISWriter = pWriter;
	m_pObserver  = pObserver;
	m_stubFile = fIsStub;
	m_enoughForStub = FALSE;
	
	if (m_pObserver) m_pObserver->SetLineNumber(++m_nLineNo);
	
	// Make sure we're at the beginning of the file	
	::SetFilePointer(m_file, 0L, NULL, FILE_BEGIN);	
	
	GetNextChar();
	// skip unicode marker if present
	if (m_pkgChar==0xFEFF) GetNextChar();
	GetNextToken();
	while(m_token!=EOF_TOKEN && !(m_stubFile && m_enoughForStub))
		{
		ParseEmbeddedBlockL();
		switch (m_token)
			{
			case '&':
				GetNextToken();
				ParseLanguagesL();
				break;
			case '#':
				GetNextToken();
				ParseHeaderL();
				break;
			case '(':
				GetNextToken();
				ParseDependencyL();
				break;
			case '[':
				GetNextToken();
				ParsePlatformDependencyL();
				break;
			case '*':
				GetNextToken();
				ParseSignatureL();
				break;
			case '%':
				GetNextToken();
				ParseLocalizedVendorNamesL();
				break;
			case ':':
				GetNextToken();
				ParseVendorNameL();
				break;
			case '=':
				GetNextToken();
				ParseLogoL();
				break;
			case EOF_TOKEN:
				break;
			default :
				{
				  printf("unknown token '%c' (%0x)\n", m_token, m_token);
				throw ErrUnknownLine;
				}
			}
		}

	// for patch/upgrade/config SIS types check is valid
	// (must declare a dependency on component with same UID)
/*
	TSISType wType=m_pSISWriter->GetType();
	if ((wType==EInstPartialUpgrade) || (wType==EInstAugmentation))
		{
		const DEPENDNODE* depend=m_pSISWriter->GetDependencyBase();
		DWORD dwUid=m_pSISWriter->GetUID();
		while (depend && dwUid!=depend->dwUID)
			depend=depend->pNext;
		if (!depend) throw ErrNoMainDependency;
		}
*/
	}

void CParsePkg::ParseEmbeddedBlockL()
{
	while(m_token!=EOF_TOKEN)
	{
		switch (m_token & 0x0000ffff)
		{
			case QUOTED_STRING_TOKEN:
				ParseFileL();
				break;
			case '@':
				GetNextToken();
				ParsePackageL();
				break;
			case '!':
				GetNextToken();
				ParseOptionsBlockL();
				break;
			case '{':
				GetNextToken();
				ParseLanguageBlockL();
				break;
			case '+':
				GetNextToken();
				ParseCapabilityL();
				break;
			case IF_TOKEN:
				GetNextToken();
				ParseIfBlockL();
				break;
			case ';':
				ParseCommentL();
				break;
			default:
				return;
		}
	}
}

void CParsePkg::ParseLanguagesL()
	// Parses the language definition line
{
	Verbage(__T("processing languages"));

	if(m_pSISWriter->GetNoLanguages())
		throw ErrLanguagesAlreadyDefined;

	for(;;)
	{
		LANGNODE *pNode = new LANGNODE;
		if (m_token==ALPHA_TOKEN)
		{
			// Look for the option
			WORD wLoop;
			for(wLoop = 0; wLoop < NUMLANGOPTIONS; wLoop++)
			{
				if(!wcsicmp(m_tokenValue.pszString, KLangOptions[wLoop].pszOpt))
				{ // found match
					if (!pNode) throw ErrNotEnoughMemory;
					pNode->wLang = (WORD)KLangOptions[wLoop].dwOpt;
					break;
				}
			}
			if(wLoop == NUMLANGOPTIONS)
				throw ErrUnknownLanguagesId;
		}
		else if (m_token==NUMERIC_TOKEN && /*m_tokenValue.dwNumber>=0 &&*/ m_tokenValue.dwNumber<=1000)
			// language codes may be given as a numeric value
		{
			if (!pNode) throw ErrNotEnoughMemory;
			pNode->wLang = (WORD)m_tokenValue.dwNumber;
		}
		else
			throw ErrUnknownLanguagesId;
		GetNextToken();
		if (m_token == '(') {
			GetNextToken();
			ExpectToken(NUMERIC_TOKEN);
			pNode->wLang += m_tokenValue.dwNumber;
			GetNextToken();
			ExpectToken(')');
			GetNextToken();
		}
		m_pSISWriter->AddLanguageNode(pNode);
		if (m_token!=',') return;
		GetNextToken();
	}
}

void CParsePkg::ParseHeaderL()
	// Parses the pkg header line
{
	Verbage(__T("processing header"));

	if(m_pSISWriter->AreLangStringInit())
		throw ErrHeaderAlreadyDefined;

	// Test if there are languages defined, if there are none then add english
	if(!m_pSISWriter->GetNoLanguages())
	{
		Verbage(__T("no languages defined, assuming English only"));
		LANGNODE *pNode = new LANGNODE;
		if (!pNode) throw ErrNotEnoughMemory;
		pNode->wLang = ELangEnglish;
		m_pSISWriter->AddLanguageNode(pNode);
	}

	// process application names
	ExpectToken('{');
	for (WORD wNumLangs=0; wNumLangs<m_pSISWriter->GetNoLanguages();wNumLangs++)
	{
		GetNextToken();
		ExpectToken(QUOTED_STRING_TOKEN);
		LANGSTRINGNODE *pLSNode = new LANGSTRINGNODE;
		if (!pLSNode) throw ErrNotEnoughMemory;
		wcscpy(pLSNode->pszString,m_tokenValue.pszString);
		m_pSISWriter->AddLangStringNode(pLSNode);
		GetNextToken();
		if (wNumLangs < m_pSISWriter->GetNoLanguages()-1) ExpectToken(',');
	}
	ExpectToken('}');
	GetNextToken();

	// Now look for the uid & version numbers
	DWORD dwUID	  = 0L,
	      dwBuild   = 0L,
	      dwFlags   = 0L,
	      dwType	  = 0L;
	WORD wMajor   = 0, 
	     wMinor    = 0;

	ExpectToken(',');
	GetNextToken();
	ExpectToken('(');
	GetNextToken();

	ExpectToken(NUMERIC_TOKEN);
	dwUID=m_tokenValue.dwNumber;
	GetNextToken();

	ExpectToken(')');
	GetNextToken();
	ExpectToken(',');
	GetNextToken();

	ExpectToken(NUMERIC_TOKEN);
	wMajor=(WORD)m_tokenValue.dwNumber;
	GetNextToken();
	ExpectToken(',');
	GetNextToken();

	ExpectToken(NUMERIC_TOKEN);
	wMinor=(WORD)m_tokenValue.dwNumber;
	GetNextToken();
	ExpectToken(',');
	GetNextToken();

	ExpectToken(NUMERIC_TOKEN);
	dwBuild=m_tokenValue.dwNumber;
	GetNextToken();

	// Parse any options
	BOOL narrow=FALSE;
	while (m_token==',')
	{
		GetNextToken();
		if (m_token==TYPE_TOKEN)
		{
			GetNextToken();
			ExpectToken('=');
			GetNextToken();
			ParseOption(KSISTypes,NUMSISTYPEOPTIONS,&dwType);
		}
		else
		{
			DWORD option=ParseOption(KHeaderOptions,NUMHEADEROPTIONS,&dwFlags);
			narrow=(option==0);
		}
	}

	// if narrow not explicitly set default to unicode
	if (!narrow) dwFlags|=EInstFlagIsUnicode;

	m_pSISWriter->SetVersionInfo(dwUID, wMajor, wMinor, dwBuild, (TSISType)dwType, (WORD)dwFlags);
}

void CParsePkg::ParseLocalizedVendorNamesL()
	// Parses the localized vendor names
{
	Verbage(__T("processing localized vendor names"));

	if(m_pSISWriter->AreLocalizedVendorNamesInit())
		throw ErrLocalizedVendorNamesAlreadyDefined;

	// Test if there are languages defined, if there are none then add english
	if(!m_pSISWriter->GetNoLanguages())
	{
		Verbage(__T("no languages defined, assuming English only"));
		LANGNODE *pNode = new LANGNODE;
		if (!pNode) throw ErrNotEnoughMemory;
		pNode->wLang = ELangEnglish;
		m_pSISWriter->AddLanguageNode(pNode);
	}

	// process localized vendor names
	ExpectToken('{');
	for (WORD wNumLangs=0; wNumLangs<m_pSISWriter->GetNoLanguages();wNumLangs++)
	{
		GetNextToken();
		ExpectToken(QUOTED_STRING_TOKEN);
		LANGSTRINGNODE *pLSNode = new LANGSTRINGNODE;
		if (!pLSNode) throw ErrNotEnoughMemory;
		wcscpy(pLSNode->pszString,m_tokenValue.pszString);
		m_pSISWriter->AddLocalizedVendorNode(pLSNode);
		GetNextToken();
		if (wNumLangs < m_pSISWriter->GetNoLanguages()-1) ExpectToken(',');
	}
	ExpectToken('}');
	GetNextToken();
}

void CParsePkg::ParseVendorNameL()
	// Parses the vendor name
{
	Verbage(__T("processing vendor name"));

	if(m_pSISWriter->IsVendorNameInit())
		throw ErrVendorNameAlreadyDefined;

	// process vendor name
	ExpectToken(QUOTED_STRING_TOKEN);
	LANGSTRINGNODE *pLSNode = new LANGSTRINGNODE;
	if (!pLSNode) throw ErrNotEnoughMemory;
	wcscpy(pLSNode->pszString,m_tokenValue.pszString);
	m_pSISWriter->AddVendorNode(pLSNode);
	GetNextToken();
}

void CParsePkg::ParseLogoL()
	// Parses the logo
{
	Verbage(__T("processing logo name"));

	// process vendor name

	LOGO *pLogo = new LOGO;
	ExpectToken(QUOTED_STRING_TOKEN);
	wcscpy(pLogo->pszSource, m_tokenValue.pszString);
	if (!DoesExist(pLogo->pszSource, &pLogo->dwSize))
		throw ErrFileNotFound;
	GetNextToken();

	ExpectToken(',');
	GetNextToken();

	ExpectToken(QUOTED_STRING_TOKEN);
	wcscpy(pLogo->pszMimeType, m_tokenValue.pszString);
	GetNextToken();

	ExpectToken(',');
	GetNextToken();

	ExpectToken(QUOTED_STRING_TOKEN);
	wcscpy(pLogo->pszDest, m_tokenValue.pszString);
	GetNextToken();

	m_pSISWriter->AddLogo(pLogo);
}

void CParsePkg::ParseFileL()
	// Parses a file definition line
{
	Verbage(__T("processing file"));

	if(!m_pSISWriter->AreLangStringInit())
		throw ErrHeaderNotDefined;

	PKGLINENODE *pNode = new PKGLINENODE;
	if (!pNode) throw ErrNotEnoughMemory;
	memset((void *)pNode, '\0', sizeof(PKGLINENODE));
	pNode->iPackageLineType=EInstPkgLineFile;
	pNode->file = new PKGLINEFILE;
	if (!pNode->file) throw ErrNotEnoughMemory;
	memset((void *)pNode->file, '\0', sizeof(PKGLINEFILE));

	pNode->file->type=EInstFileTypeSimple;

	wcscpy(pNode->file->pszSource, m_tokenValue.pszString);
	if (pNode->file->pszSource[0] && !DoesExist(pNode->file->pszSource, &pNode->file->dwSize))
		throw ErrFileNotFound;
	GetNextToken();

	ExpectToken('-');
	GetNextToken();

	ExpectToken(QUOTED_STRING_TOKEN);

	// BAW-5CQEA5 Check for invalid destination (i.e starts with ':')
	if ( wcslen(m_tokenValue.pszString) > 0 && m_tokenValue.pszString[0] == ':' )
		throw ErrBadDestinationPath;

	// ANN-5ESFPV If there is no drive specified, assume it's C:
	if ((wcslen(m_tokenValue.pszString) > 0) &&
			((wcslen(m_tokenValue.pszString) < 3) ||
			 (m_tokenValue.pszString[1] != ':') ||
			 (m_tokenValue.pszString[2] != '\\')))
	{// The drive is not specified, we will assume the user meant C:
		wcscpy(pNode->file->pszDest, L"c:\\");
		wcscat(pNode->file->pszDest, m_tokenValue.pszString);
	}
	else
	{
		wcscpy(pNode->file->pszDest, m_tokenValue.pszString);
	}

	GetNextToken();

	// Test for options
	if (m_token==',')
	{
		DWORD type=0;
		GetNextToken();
		ParseOption(KTypeOptions,NUMTYPEOPTIONS, &type);
		pNode->file->type=(TInstFileType)type;
		if (pNode->file->type==EInstFileTypeMime)
		{
			ExpectToken(',');
			GetNextToken();
			ExpectToken(QUOTED_STRING_TOKEN);
			wcscpy(pNode->file->pszMimeType, m_tokenValue.pszString);
			GetNextToken();
			pNode->file->options.iMimeOption = EInstFileRunOptionByMimeType | EInstFileRunOptionInstall;
		}
		while (m_token==',')
		{
			DWORD options=0;
			switch (pNode->file->type)
			{
				case EInstFileTypeText:
					GetNextToken();
					ParseOption(KTextOptions,NUMTEXTOPTIONS, &options);
					pNode->file->options.iTextOption=(TInstFileTextOption)options;
					pNode->file->pszDest[0]='\0';
					break;
				case EInstFileTypeRun:
					GetNextToken();
					ParseOption(KRunOptions,NUMRUNOPTIONS, &options);
					if (options & (EInstFileRunOptionWaitEnd | EInstFileRunOptionSendEnd))
						pNode->file->options.iRunOption &= ~(EInstFileRunOptionWaitEnd | EInstFileRunOptionSendEnd);
					pNode->file->options.iRunOption |= options;
					break;
				case EInstFileTypeMime:
					GetNextToken();
					ParseOption(KRunOptions,NUMRUNOPTIONS, &options);
					if (options & EInstFileRunOptionUninstall)
						pNode->file->options.iMimeOption &= ~EInstFileRunOptionInstall;
					if (options & (EInstFileRunOptionWaitEnd | EInstFileRunOptionSendEnd))
						pNode->file->options.iMimeOption &= ~(EInstFileRunOptionWaitEnd | EInstFileRunOptionSendEnd);
					pNode->file->options.iMimeOption|=options;
					break;
				default:
					throw ErrBadOption;
					break;
			}
		}
	}	

	// Test that the file exists
	if(pNode->file->type==EInstFileTypeNull)
	{
		pNode->file->dwSize = 0;
		pNode->file->pszSource[0]='\0';
	}

	m_pSISWriter->AddPkgLineNode(pNode);
}

void CParsePkg::ParsePackageL()
	// Parses a package (component SIS file) line
{
	Verbage(__T("processing embedded package file"));

	if(!m_pSISWriter->AreLangStringInit())
		throw ErrHeaderNotDefined;

	PKGLINENODE *pNode = new PKGLINENODE;
	if (!pNode) throw ErrNotEnoughMemory;
	memset((void *)pNode, '\0', sizeof(PKGLINENODE));
	pNode->iPackageLineType=EInstPkgLineFile;
	pNode->file = new PKGLINEFILE;
	if (!pNode->file) throw ErrNotEnoughMemory;
	memset((void *)pNode->file, '\0', sizeof(PKGLINEFILE));

	pNode->file->type=EInstFileTypeComponent;

	ExpectToken(QUOTED_STRING_TOKEN);
	wcscpy(pNode->file->pszSource,m_tokenValue.pszString);			
/*
	if (!FullPath(pNode->file->pszSource, m_tokenValue.pszString, MAX_PATH))
	{
		throw ErrCannotGetFullPath;
	}
*/
	// Test that the file exists
	if(!DoesExist(pNode->file->pszSource, &pNode->file->dwSize))
		throw ErrFileNotFound;
	GetNextToken();

	ExpectToken(',');
	GetNextToken();	
	ExpectToken('(');
	GetNextToken();	
	ExpectToken(NUMERIC_TOKEN);
	pNode->file->options.iComponentUid=m_tokenValue.dwNumber;
	GetNextToken();	
	ExpectToken(')');
	GetNextToken();

	// Check that the UID given in the PKG file is the same as the UID of the SIS
	// file we are embedding
	HANDLE hEmbeddedFile = ::MakeSISOpenFile(pNode->file->pszSource, GENERIC_READ, OPEN_EXISTING);
	if (hEmbeddedFile == INVALID_HANDLE_VALUE)
		{
		throw ErrReadFailed;
		}
	else
		{
		uint8_t uid[4];
		unsigned long numberOfBytesRead;
		int ok;
		ok = ReadFile(hEmbeddedFile, uid, 4, &numberOfBytesRead,NULL);
		if (!ok) throw ErrReadFailed;
		ok = ReadFile(hEmbeddedFile, uid, 4, &numberOfBytesRead,NULL);
		if (!ok) throw ErrReadFailed;
		ok = ReadFile(hEmbeddedFile, uid, 4, &numberOfBytesRead,NULL);
		if (!ok) throw ErrReadFailed;
		int32_t embeddedUID = uid[0] | (uid[1] << 8) | (uid[2] << 16) | (uid[3] << 24);
		if (embeddedUID != pNode->file->options.iComponentUid)
			{
			throw ErrUIDMismatch;
			}
		CloseHandle(hEmbeddedFile);
		}
	
	// Test that the filename is *.SIS
	if(wcsicmp(&pNode->file->pszSource[wcslen(pNode->file->pszSource) - wcslen(DESTFILE)], DESTFILE))
		throw ErrPackageNotASISFile;
	
	pNode->file->pszDest[0] = '\0';
	m_pSISWriter->AddPkgLineNode(pNode);
	}

DWORD CParsePkg::GetVersionNumToken() {
	if (m_token == NUMERIC_TOKEN)
		return m_tokenValue.dwNumber;
	if (m_token == '*')
		return -1;
	ExpectToken(NUMERIC_TOKEN);
	return 0;
}

void CParsePkg::ParseDependencyL()
// Parses a dependency line                         
	{
	Verbage(__T("processing dependency"));
	
	if(!m_pSISWriter->AreLangStringInit())
		throw ErrHeaderNotDefined;
	
	DEPENDNODE *pNode = new DEPENDNODE;
	if (!pNode) throw ErrNotEnoughMemory;
	memset((void *)pNode, '\0', sizeof(DEPENDNODE));
	pNode->hasTo = FALSE;
	
	ExpectToken(NUMERIC_TOKEN);
	pNode->dwUID=m_tokenValue.dwNumber;
	GetNextToken();	
	
	ExpectToken(')');
	GetNextToken();	
	ExpectToken(',');
	GetNextToken();	
	
	pNode->fromMajor = GetVersionNumToken();
	GetNextToken();	
	
	ExpectToken(',');
	GetNextToken();	
	
	pNode->fromMinor = GetVersionNumToken();
	GetNextToken();	
	
	ExpectToken(',');
	GetNextToken();	
	
	pNode->fromBuild = GetVersionNumToken();
	GetNextToken();	

	if (m_token == '~') {
		GetNextToken();	
		pNode->hasTo = TRUE;

		pNode->toMajor = GetVersionNumToken();
		GetNextToken();	

		ExpectToken(',');
		GetNextToken();	
	
		pNode->toMinor = GetVersionNumToken();
		GetNextToken();	
	
		ExpectToken(',');
		GetNextToken();	
	
		pNode->toBuild = GetVersionNumToken();
		GetNextToken();	
	}

	ExpectToken(',');
	GetNextToken();	
	ExpectToken('{');
	
	WORD wNumLangs = 0;
	while(wNumLangs < m_pSISWriter->GetNoLanguages())
		{
		GetNextToken();
		ExpectToken(QUOTED_STRING_TOKEN);
		LANGSTRINGNODE *pLSNode = new LANGSTRINGNODE;
		if (!pLSNode) throw ErrNotEnoughMemory;
		memset((void *)pLSNode, '\0', sizeof(LANGSTRINGNODE));
		wcscpy(pLSNode->pszString,m_tokenValue.pszString);
		GetNextToken();		
		if(wNumLangs < (m_pSISWriter->GetNoLanguages() - 1))
			ExpectToken(',');
		pLSNode->pNext = pNode->pLangStringBase;
		pNode->pLangStringBase = pLSNode;
		wNumLangs++;
		}
	ExpectToken('}');
	GetNextToken();	
	
	m_pSISWriter->AddDependencyNode(pNode);
	}

void CParsePkg::ParsePlatformDependencyL()
// Parses a dependency line                         
	{
	Verbage(__T("processing platform dependency"));
	
	if(!m_pSISWriter->AreLangStringInit())
		throw ErrHeaderNotDefined;
	
	DEPENDNODE *pNode = new DEPENDNODE;
	if (!pNode) throw ErrNotEnoughMemory;
	memset((void *)pNode, '\0', sizeof(DEPENDNODE));
	pNode->hasTo = FALSE;
	
	ExpectToken(NUMERIC_TOKEN);
	pNode->dwUID=m_tokenValue.dwNumber;
	GetNextToken();	
	
	ExpectToken(']');
	GetNextToken();	
	ExpectToken(',');
	GetNextToken();	
	
	pNode->fromMajor = GetVersionNumToken();
	GetNextToken();	
	
	ExpectToken(',');
	GetNextToken();	
	
	pNode->fromMinor = GetVersionNumToken();
	GetNextToken();	
	
	ExpectToken(',');
	GetNextToken();	
	
	pNode->fromBuild = GetVersionNumToken();
	GetNextToken();	

	if (m_token == '~') {
		GetNextToken();	
		pNode->hasTo = TRUE;

		pNode->toMajor = GetVersionNumToken();
		GetNextToken();	

		ExpectToken(',');
		GetNextToken();	
	
		pNode->toMinor = GetVersionNumToken();
		GetNextToken();	
	
		ExpectToken(',');
		GetNextToken();	
	
		pNode->toBuild = GetVersionNumToken();
		GetNextToken();	
	}

	ExpectToken(',');
	GetNextToken();	
	ExpectToken('{');
	
	WORD wNumLangs = 0;
	while(wNumLangs < m_pSISWriter->GetNoLanguages())
		{
		GetNextToken();
		ExpectToken(QUOTED_STRING_TOKEN);
		LANGSTRINGNODE *pLSNode = new LANGSTRINGNODE;
		if (!pLSNode) throw ErrNotEnoughMemory;
		memset((void *)pLSNode, '\0', sizeof(LANGSTRINGNODE));
		wcscpy(pLSNode->pszString,m_tokenValue.pszString);
		GetNextToken();		
		if(wNumLangs < (m_pSISWriter->GetNoLanguages() - 1))
			ExpectToken(',');
		pLSNode->pNext = pNode->pLangStringBase;
		pNode->pLangStringBase = pLSNode;
		wNumLangs++;
		}
	ExpectToken('}');
	GetNextToken();	
	
	m_pSISWriter->AddPlatformDependencyNode(pNode);
	}

void CParsePkg::ParseSignatureL()
{
	// Parses the package-signature
	Verbage(__T("processing signature"));

	if(!m_pSISWriter->AreLangStringInit())
		throw ErrHeaderNotDefined;
	
	// If generating a stub file then once have signature line don't need to
	// process rest of PKG file
	m_enoughForStub=TRUE;

	ExpectToken(QUOTED_STRING_TOKEN);
	
	SIGNATURENODE* pSig = new SIGNATURENODE;
	if (!pSig) throw ErrNotEnoughMemory;
	memset((void *)pSig, '\0', sizeof(SIGNATURENODE));

	wcscpy(pSig->pszPrivateKey,m_tokenValue.pszString);

	
	pSig->iNumCerts = 1;

	//get a temporary filename for creating certificate chain
	LPWSTR pszTempChain;
	pszTempChain = TempFileName(L"chain.cer");
	wcscpy(pSig->pszChainFile, pszTempChain);


	//get a temporary filename for Base64 decoding
	LPWSTR pszTempB64;
	pszTempB64 = TempFileName(L"temp.bin");
	wcscpy(pSig->pszB64File, pszTempB64);

	GetNextToken();
	ExpectToken(',');
	GetNextToken();

	ExpectToken(QUOTED_STRING_TOKEN);
	wcscpy(pSig->pszPublicKey,m_tokenValue.pszString);
/*	
	DWORD dwSize;

	// Test that the files exist
	if (!DoesExist(pSig->pszPrivateKey, &dwSize) ||
		!DoesExist(pSig->pszPublicKey, &dwSize))
		throw ErrFileNotFound;
*/
	GetNextToken();
	
	//check for optional items (password)
	if (m_token==',')
		{
		GetNextToken();
		if (m_token==KEY_TOKEN)
			{
			GetNextToken();
			ExpectToken('=');
			GetNextToken();
			ExpectToken(QUOTED_STRING_TOKEN);
			wcscpy(pSig->pszPassword,m_tokenValue.pszString);
			GetNextToken();		
			}
		}

	delete pSig;
//	m_pSISWriter->AddSignatureNode(pSig);
	printf("Signatures directly from the pkg files are deprecated, ignoring\n");
}

void CParsePkg::ParseCapabilityL()
// Parses a capability line
	{
	Verbage(__T("processing capability"));
	
	if(!m_pSISWriter->AreLangStringInit())
		throw ErrHeaderNotDefined;
	
	ExpectToken('(');
	do
		{
		GetNextToken();

		CAPABILITYNODE *pNode = new CAPABILITYNODE;
		if (!pNode) throw ErrNotEnoughMemory;
		memset((void *)pNode, '\0', sizeof(CAPABILITYNODE));
	
		ExpectToken(NUMERIC_TOKEN);
		pNode->iKey=m_tokenValue.dwNumber;
		GetNextToken();
		ExpectToken('=');
		GetNextToken();
		ExpectToken(NUMERIC_TOKEN);
		pNode->iValue=m_tokenValue.dwNumber;
		m_pSISWriter->AddCapabilityNode(pNode);

		GetNextToken();
		} while (m_token==',');
	ExpectToken(')');
	GetNextToken();
	}

void CParsePkg::ParseOptionsBlockL()
// To parse an options block
	{
	WORD wNumLangs;
	LANGSTRINGNODE *pLSNode;

	Verbage(__T("processing options block"));
	
	ExpectToken('(');
	GetNextToken();

	PKGLINENODE *pNode = new PKGLINENODE;
	if (!pNode) throw ErrNotEnoughMemory;
	memset((void *)pNode, '\0', sizeof(PKGLINENODE));
	pNode->iPackageLineType=EInstPkgLineOption;
	pNode->option = new PKGLINEOPTIONS;
	if (!pNode->option) throw ErrNotEnoughMemory;
	memset((void *)pNode->option, '\0', sizeof(PKGLINEOPTIONS));
	for (;;)
		{
		ExpectToken('{');
		GetNextToken();

		wNumLangs = 0;
		while (wNumLangs < m_pSISWriter->GetNoLanguages())
			{
			ExpectToken(QUOTED_STRING_TOKEN);
			pLSNode = new LANGSTRINGNODE;
			if (!pLSNode) throw ErrNotEnoughMemory;
			memset((void *)pLSNode, '\0', sizeof(LANGSTRINGNODE));
			wcscpy(pLSNode->pszString,m_tokenValue.pszString);
			pLSNode->pNext=pNode->option->pLangStringBase;
			pNode->option->pLangStringBase = pLSNode;
			GetNextToken();
			if (wNumLangs < m_pSISWriter->GetNoLanguages()-1)
				{
				ExpectToken(',');
				GetNextToken();
				}
			wNumLangs++;
			}

		ExpectToken('}');
		pNode->option->dwNumOptions++;
		GetNextToken();
		if (m_token!=',') break;
		GetNextToken();
		}

	ExpectToken(')');
	GetNextToken();

	m_pSISWriter->AddPkgLineNode(pNode);
	}

void CParsePkg::ParseLanguageBlockL()
// To parse the start of a language block											 
	{
	Verbage(__T("processing language block"));
	
	if(!m_pSISWriter->AreLangStringInit())
		throw ErrHeaderNotDefined;
	
	BOOL packages=(m_token=='@');
	
	for (DWORD m_wCurrLang = 0; m_wCurrLang<m_pSISWriter->GetNoLanguages(); m_wCurrLang++)
		{
		PKGLINENODE *pNode = new PKGLINENODE;
		if (!pNode) throw ErrNotEnoughMemory;
		memset((void *)pNode, '\0', sizeof(PKGLINENODE));
		pNode->iPackageLineType=EInstPkgLineLanguageFile;
		pNode->file = new PKGLINEFILE;
		if (!pNode->file) throw ErrNotEnoughMemory;
		memset((void *)pNode->file, '\0', sizeof(PKGLINEFILE));
		if (packages)
			{
			ExpectToken('@');
			GetNextToken();
			pNode->file->type = EInstFileTypeComponent;
			}
		else
			{
			pNode->file->type = EInstFileTypeSimple;
			}
		ExpectToken(QUOTED_STRING_TOKEN);
		wcscpy(pNode->file->pszSource,m_tokenValue.pszString);			
		// Test that the file exists
		if(!DoesExist(pNode->file->pszSource, &pNode->file->dwSize))
			throw ErrFileNotFound;
		m_pSISWriter->AddPkgLineNode(pNode);
		GetNextToken();	
		}
	
	ExpectToken('}');
	GetNextToken();	
	
	// Test if it was a file or package block
	if(!packages)
		{
		ExpectToken('-');
		GetNextToken();	
		
		// Get the destination & options
		ExpectToken(QUOTED_STRING_TOKEN);
		WCHAR pszDest[MAX_PATH] = L"";
		wcscpy(pszDest,m_tokenValue.pszString);
		GetNextToken();
		
		DWORD dwType=0;
		DWORD dwOptions=0;
		WCHAR pszMimeType[MAX_MIME_TYPE] = L"";
		// Test for options
		if (m_token==',')
			{
			GetNextToken();
			ParseOption(KTypeOptions,NUMTYPEOPTIONS, &dwType);
			if ((TInstFileType)dwType==EInstFileTypeNull) throw ErrBadOption;
			if ((TInstFileType)dwType==EInstFileTypeMime)
				{
				ExpectToken(',');
				GetNextToken();
				ExpectToken(QUOTED_STRING_TOKEN);
				wcscpy(pszMimeType, m_tokenValue.pszString);
				GetNextToken();
				dwOptions = EInstFileRunOptionByMimeType | EInstFileRunOptionInstall;
				}
			while (m_token==',')
				{
				DWORD options = 0;
				switch ((TInstFileType)dwType)
					{
					case EInstFileTypeText:
						GetNextToken();
						ParseOption(KTextOptions,NUMTEXTOPTIONS, &dwOptions);
						break;
					case EInstFileTypeRun:
						GetNextToken();
						ParseOption(KRunOptions,NUMRUNOPTIONS, &options);
						if (options & (EInstFileRunOptionWaitEnd | EInstFileRunOptionSendEnd))
							dwOptions &= ~(EInstFileRunOptionWaitEnd | EInstFileRunOptionSendEnd);
						dwOptions |= options;
						break;
					case EInstFileTypeMime:
						GetNextToken();
						ParseOption(KRunOptions,NUMRUNOPTIONS, &options);
						if (options & EInstFileRunOptionUninstall)
							dwOptions &= ~EInstFileRunOptionInstall;
						if (options & (EInstFileRunOptionWaitEnd | EInstFileRunOptionSendEnd))
							dwOptions &= ~(EInstFileRunOptionWaitEnd | EInstFileRunOptionSendEnd);
						dwOptions |= options;
						break;
					default:
						throw ErrBadOption;
						break;
					}
				}
			}
		
		m_pSISWriter->SetLangDependFileDestinations(pszDest, pszMimeType, dwType, dwOptions);
		}
	else
		{
		ExpectToken(',');
		GetNextToken();	
		ExpectToken('(');
		GetNextToken();	
		
		ExpectToken(NUMERIC_TOKEN);
		m_pSISWriter->SetLangDependCompDestinations(m_tokenValue.dwNumber);
		GetNextToken();	
		
		ExpectToken(')');
		GetNextToken();	
		
		}
	}

void CParsePkg::ParseIfBlockL()
	{
	Verbage(__T("processing IF block"));
	
	ParseConditionL(EInstPkgLineCondIf);
	ParseEmbeddedBlockL();
	
	while (m_token==ELSEIF_TOKEN)
		{
		GetNextToken();
		ParseConditionL(EInstPkgLineCondElseIf);
		ParseEmbeddedBlockL();
		}
	
	if (m_token==ELSE_TOKEN)
		{
		GetNextToken();
		ParseConditionL(EInstPkgLineCondElse);
		ParseEmbeddedBlockL();
		}
	
	ExpectToken(ENDIF_TOKEN);
	ParseConditionL(EInstPkgLineCondEndIf);
	GetNextToken();
	}

void CParsePkg::ParseCommentL()
// Purpose  : Parses a comment line (Does nothing, just throws the line away)
// Inputs   : m_pkgPtr - The string to parse
// Returns  : Success or failure
	{
	Verbage(__T("processing comment"));
	
	// parse to end of line
	while (m_pkgChar && m_pkgChar!='\n')
		GetNextChar();
	GetNextToken();
	}

void CParsePkg::ParseConditionL(TInstPackageLineType type)
// Purpose  : Parses a condition line                         
// Inputs   : m_pkgPtr - The string to parse
// returns  : Success or failure (actually, always TRUE - failure causes an exception to be thrown)
	{
	PKGLINENODE *pNode = new PKGLINENODE;
	if (!pNode) throw ErrNotEnoughMemory;
	memset((void *)pNode, '\0', sizeof(PKGLINENODE));
	pNode->iPackageLineType=type;

	if (type==EInstPkgLineCondElse || type==EInstPkgLineCondEndIf)
		{
		pNode->cond=NULL;
		}
	else
		{
		pNode->cond=ParseExpr();
		}
	
	m_pSISWriter->AddPkgLineNode(pNode);
	}

PKGLINECONDITION* CParsePkg::ParseExpr()
	{
    return ParseLogicalOp();
	}

PKGLINECONDITION* CParsePkg::ParseLogicalOp()
	{
    PKGLINECONDITION* left=ParseRelation();
    for (;;)
		{
		switch (m_token)
			{
			case AND_TOKEN:
			case OR_TOKEN:
				{
				PKGLINECONDITION* expr=new PKGLINECONDITION;
				if (!expr) throw ErrNotEnoughMemory;
				memset((void *)expr, '\0', sizeof(PKGLINECONDITION));
				if (m_token==AND_TOKEN)
					expr->exprType=EInstCondLogOpAnd;
				else
					expr->exprType=EInstCondLogOpOr;
				expr->b.pLhs=left;
				GetNextToken();
				expr->b.pRhs=ParseLogicalOp();
				left=expr;
				}
				continue;
			default:
				return left;
			}
		}
	}

PKGLINECONDITION* CParsePkg::ParseRelation()
	{
    PKGLINECONDITION* left=ParseUnary();
	switch (m_token)
		{
		case '=':
		case '>':
		case '<':
		case GE_TOKEN:
		case LE_TOKEN:
		case NE_TOKEN:
			{
			PKGLINECONDITION* expr=new PKGLINECONDITION;
			if (!expr) throw ErrNotEnoughMemory;
			memset((void *)expr, '\0', sizeof(PKGLINECONDITION));
			switch (m_token)
				{
				case '=':
					expr->exprType=EInstCondBinOpEq;
					break;
				case '>':
					expr->exprType=EInstCondBinOpGt;
					break;
				case '<':
					expr->exprType=EInstCondBinOpLt;
					break;
				case GE_TOKEN:
					expr->exprType=EInstCondBinOpGe;
					break;
				case LE_TOKEN:
					expr->exprType=EInstCondBinOpLe;
					break;
				case NE_TOKEN:
					expr->exprType=EInstCondBinOpNe;
					break;
				}
			expr->b.pLhs=left;
			GetNextToken();
			expr->b.pRhs=ParseUnary();
			return expr;
			}
		}
	return left;
	}

PKGLINECONDITION* CParsePkg::ParseUnary()
	{
    switch (m_token)
		{
		case NOT_TOKEN:
			{
			PKGLINECONDITION* expr=new PKGLINECONDITION;
			if (!expr) throw ErrNotEnoughMemory;
			memset((void *)expr, '\0', sizeof(PKGLINECONDITION));
			if (m_token==EXISTS_TOKEN)
				expr->exprType=EInstCondFuncExists;
			else
				expr->exprType=EInstCondUnaryOpNot;
			GetNextToken();
			expr->pExpr=ParseUnary();
			return expr;
			}
		case APPPROP_TOKEN:
			{	// 2 arg function
			GetNextToken();
			ExpectToken('(');
			PKGLINECONDITION* expr=new PKGLINECONDITION;
			if (!expr) throw ErrNotEnoughMemory;
			memset((void *)expr, '\0', sizeof(PKGLINECONDITION));
			expr->exprType=EInstCondFuncAppProperties;
			GetNextToken();
			ExpectToken(NUMERIC_TOKEN);
			expr->pArg[0]=ParseUnary();
			ExpectToken(',');
			GetNextToken();
			ExpectToken(NUMERIC_TOKEN);
			expr->pArg[1]=ParseUnary();
			ExpectToken(')');
			GetNextToken();
			return expr;
			}
		case EXISTS_TOKEN:
		case DEVPROP_TOKEN:
			{	// 1 arg function
			TOKEN token=m_token;
			GetNextToken();
			ExpectToken('(');
			PKGLINECONDITION* expr=new PKGLINECONDITION;
			if (!expr) throw ErrNotEnoughMemory;
			memset((void *)expr, '\0', sizeof(PKGLINECONDITION));
			GetNextToken();
			if (token==EXISTS_TOKEN)
				{
				expr->exprType=EInstCondFuncExists;
				ExpectToken(QUOTED_STRING_TOKEN);
				}
			else
				{
				expr->exprType=EInstCondFuncDevProperties;
				ExpectToken(NUMERIC_TOKEN);
				}
			expr->pArg[0]=ParseUnary();
			ExpectToken(')');
			GetNextToken();
			return expr;
			}
		default:
			return ParseFactor();
		}
	}

PKGLINECONDITION* CParsePkg::ParseFactor()
	{
	PKGLINECONDITION* expr;
    switch (m_token) {
		case '(':
			{
			GetNextToken();
			expr=ParseExpr();
			ExpectToken(')');
			}
			break;
		case QUOTED_STRING_TOKEN:
		case ALPHA_TOKEN:
		case NUMERIC_TOKEN:
			{
			expr=new PKGLINECONDITION;
			if (!expr) throw ErrNotEnoughMemory;
			memset((void *)expr, '\0', sizeof(PKGLINECONDITION));
			expr->pPrim=new VARIANTVAL;
			if (!expr->pPrim) throw ErrNotEnoughMemory;
			switch (m_token)
				{
				case QUOTED_STRING_TOKEN:
					expr->exprType=EInstCondPrimTypeString;
					memcpy(expr->pPrim, &m_tokenValue, sizeof(VARIANTVAL));
					break;
				case ALPHA_TOKEN:
					{
						expr->exprType=EInstCondPrimTypeVariable;
						// check if it is "option1", "option2",...
						if(!wcsnicmp(m_tokenValue.pszString, L"option",6))
							{
							LPWSTR temp,end;
							temp=&m_tokenValue.pszString[6];
							DWORD optionNum = wcstol(temp, &end, 10);
							if (end==temp || errno==ERANGE)
								throw ErrUnknownVariable;
							expr->exprType=EInstCondPrimTypeOption;
							expr->pPrim->dwNumber = optionNum;
							}
						else
							{
							// convert string to variable token ID
							// Look for the option
							WORD wLoop;
							for(wLoop = 0; wLoop < NUMVAROPTIONS; wLoop++)
								{
								if(!wcsicmp(m_tokenValue.pszString, KVariableLookupTable[wLoop].pszOpt))
									{ // found match
									expr->pPrim->dwNumber = (WORD)KVariableLookupTable[wLoop].dwOpt;
									break;
									}
								}
							if(wLoop == NUMVAROPTIONS)
								throw ErrUnknownVariable;
							}
					}
					break;
				case NUMERIC_TOKEN:
					expr->exprType=EInstCondPrimTypeNumber;
					memcpy(expr->pPrim, &m_tokenValue, sizeof(VARIANTVAL));
					break;
				}
			}
			break;
		default:
			throw ErrBadCondFormat;
		}
	GetNextToken();
	return expr;
	}

DWORD CParsePkg::ParseOption(const SParseOpt* options, DWORD dwNumOptions, DWORD* pdwOptions)
// Parse the options part of an input line
	{
	DWORD option=0;
	ExpectToken(ALPHA_TOKEN);
	// Look for the option
	WORD wLoop;
	for(wLoop = 0; wLoop < dwNumOptions; wLoop++)
		{
		if(wcsicmp(m_tokenValue.pszString, options[wLoop].pszOpt) == 0)
			{
			option=options[wLoop].dwOpt;
			break;
			}
		}		
	if(wLoop == dwNumOptions)
		throw ErrBadOption;
	*pdwOptions |= option;
	GetNextToken();
	return option;
	}

void CParsePkg::ExpectToken(TOKEN aToken)
	{
	if (m_token!=aToken)
		{
		if (m_pObserver)
			{
			_TCHAR msg[255]=__T("Expected ");
			if (aToken<=LAST_TOKEN)
				_tcscat(msg,GetTokenText(aToken));
			else
				{
				_TCHAR tmp[2]={(_TCHAR)aToken,0};
				_tcscat(msg,tmp);
				}
			_tcscat(msg,__T(" read "));
			if (m_token<=LAST_TOKEN)
				_tcscat(msg,GetTokenText(m_token));
			else
				{
				_TCHAR tmp[2]={(_TCHAR)m_token,0};
				_tcscat(msg,tmp);
				}
			m_pObserver->DoErrMsg(msg);
			}
		throw ErrUnexpectedToken;
		}
	}

const _TCHAR* CParsePkg::GetTokenText(TOKEN aToken)
	{
	switch(aToken)
		{
		case NUMERIC_TOKEN:
			return __T("numeric value");
		case ALPHA_TOKEN:
			return __T("alphanumeric value");
		case QUOTED_STRING_TOKEN:
			return __T("quoted string");
		case AND_TOKEN:
			return __T("AND");
		case OR_TOKEN:
			return __T("OR");
		case NOT_TOKEN:
			return __T("NOT");
		case EXISTS_TOKEN:
			return __T("EXISTS");
		case GE_TOKEN:
			return __T(">=");
		case LE_TOKEN:
			return __T("<=");
		case NE_TOKEN:
			return __T("<>");
		case IF_TOKEN:
			return __T("IF");
		case ELSEIF_TOKEN:
			return __T("ELSEIF");
		case ELSE_TOKEN:
			return __T("ELSE");
		case ENDIF_TOKEN:
			return __T("ENDIF");
		default:
			return __T("?");
		}
	}

void CParsePkg::GetNextToken()
// lexical analyzer
	{
	// skip any white space & CR's
	while (m_pkgChar == '\n' || iswspace(m_pkgChar))
		{
		if (m_pkgChar == '\n' && m_pObserver)
			m_pObserver->SetLineNumber(++m_nLineNo);
		GetNextChar();
		}
	
	if (m_pkgChar == '\0')
		m_token=EOF_TOKEN;
	else if (IsNumericToken())
		{ 
		GetNumericToken();
		m_token=NUMERIC_TOKEN;
		}
	else if (iswalpha(m_pkgChar))
		{ // have some alphanumeric text
		GetAlphaNumericToken();
		m_token=ALPHA_TOKEN;
		// check if it is a keyword
		for(WORD wLoop = 0; wLoop < NUMPARSETOKENS; wLoop++)
			{
			if(wcsicmp(m_tokenValue.pszString, KTokens[wLoop].pszOpt) == 0)
				{
				m_token=KTokens[wLoop].dwOpt;
				break;
				}
			}		
		}
	else if (m_pkgChar == '\"')
		{ // have a quoted string
		GetStringToken();
		m_token=QUOTED_STRING_TOKEN;
		}
	else if (m_pkgChar == '>')
		{
		GetNextChar();
		if (m_pkgChar == '=') 
			{
			m_token=GE_TOKEN;
			GetNextChar();
			} 
		else 
			m_token='>';
		} 
	else if (m_pkgChar == '<')
		{
		// check if start of an escaped string, e.g. <123>"abc"
		if (GetStringToken())
			m_token=QUOTED_STRING_TOKEN;
		else
			{
			GetNextChar();
			if (m_pkgChar == '=') 
				{
				m_token=LE_TOKEN;
				GetNextChar();
				} 
			else if (m_pkgChar == '>') 
				{
				m_token=NE_TOKEN;
				GetNextChar();
				} 
			else 
				m_token='<';
			}
		} 
	else
		{
		m_token=m_pkgChar;
		GetNextChar();
		}
	}

BOOL CParsePkg::GetStringToken()
// Purpose  : Parse a quoted string from the input line
// Inputs   : m_pkgPtr    - The string to parse
//			  pszString   - The output string
//            wMaxLength  - The max length of pszString
	{
	DWORD wCount = 0;
	BOOL done=FALSE;	
	BOOL finished=FALSE;
	DWORD escapeChars = 0;

	while (!finished)
		{
		if (m_pkgChar == '\"')
			{
			GetNextChar();
			while(m_pkgChar && m_pkgChar != '\"')
				{
				if(wCount < (MAX_STRING - 1))
					m_tokenValue.pszString[wCount++] = m_pkgChar;
				GetNextChar();
				}
			if(m_pkgChar == '\0')
				throw ErrBadString;
			GetNextChar();
			done=TRUE;
			}
		if (m_pkgChar == '<')
			{
			m_tokenValue.pszString[wCount] = L'\0';
			escapeChars=ParseEscapeChars();
			if (escapeChars>0)
				{
				done=TRUE;
				wCount+=escapeChars;
				if (wCount>=MAX_STRING) wCount=MAX_STRING-1;
				}
			}
		if (escapeChars==0 || m_pkgChar != '\"')
			finished=TRUE;
		}

	m_tokenValue.pszString[wCount] = L'\0';
	return done;
	}

WORD CParsePkg::ParseEscapeChars()
	{
	WORD found=0;
	WCHAR temp[MAX_STRING];
	while (m_pkgChar == '<')
		{
		wcscpy(temp,m_tokenValue.pszString);
		DWORD fileOffset=::SetFilePointer(m_file, 0L, NULL, FILE_CURRENT);
		try
			{
			GetNextChar();
			GetNumericToken();
			if (m_pkgChar=='>') 
				found++;
			else
				{
				::SetFilePointer(m_file, fileOffset, NULL, FILE_BEGIN);
				break;
				}
			}
		catch (TParseException)
			{
			wcscpy(m_tokenValue.pszString,temp);
			::SetFilePointer(m_file, fileOffset, NULL, FILE_BEGIN);
			break;
			}
		DWORD num=m_tokenValue.dwNumber;
		// watch for CP1252 escapes which aren't appropriate for UNICODE
		if (num>=0x80 && num<=0x9F) throw ErrInvalidEscape;
		DWORD len=wcslen(temp);
		wcscpy(m_tokenValue.pszString,temp);
		if (len+2<=MAX_STRING)
			{
			m_tokenValue.pszString[len]=(WCHAR)num;
			len++;
			m_tokenValue.pszString[len]='\0';
			}
		GetNextChar();
		}
	return found;
	}

void CParsePkg::GetAlphaNumericToken()
// Purpose  : Parse an alphanumeric string from the input line
// Inputs   : m_pkgPtr    - The string to parse
//			  pszString   - The output string
//            wMaxLength  - The max length of pszString
	{
	WORD wCount = 0;
	while(m_pkgChar && (m_pkgChar == '_' || iswalnum(m_pkgChar)))
		{
		if(wCount < (MAX_STRING - 1))
			m_tokenValue.pszString[wCount++] = m_pkgChar;
		GetNextChar();
		}
	m_tokenValue.pszString[wCount] = L'\0';
	}

BOOL CParsePkg::IsNumericToken()
// Purpose : Determines if the next lexeme is a numeric token
	{
	BOOL lexemeIsNumber = FALSE;
	if (iswdigit(m_pkgChar))
		lexemeIsNumber = TRUE;
	else if (m_pkgChar == '+' || m_pkgChar == '-')
		{ 
		// we may have a number but we must look ahead one char to be certain
		
		WCHAR oldChar = m_pkgChar;
		DWORD fileOffset=::SetFilePointer(m_file, 0L, NULL, FILE_CURRENT);
		GetNextChar();
		lexemeIsNumber = iswdigit(m_pkgChar);
		m_pkgChar = oldChar;
		::SetFilePointer(m_file,fileOffset,NULL,FILE_BEGIN);
		}

	return lexemeIsNumber;
	}


void CParsePkg::GetNumericToken()
// Purpose  : Parse a number from the input line
// Inputs   : m_pkgPtr    - The string to parse
//			  pdwNumber  - The output number
	{
	WCHAR temp[MAX_STRING];
	LPWSTR end;
	BOOL hexString = FALSE;
	DWORD dwBytesRead;	
	DWORD fileOffset=::SetFilePointer(m_file, 0L, NULL, FILE_CURRENT);
	
	temp[0]=m_pkgChar;

	/* Read one digit for each iteration. */
	(void)fileOffset;
	int i = 1;
	unsigned short tmp_char;
	int parse = TRUE;
	while(parse) {
		BOOL ok = FALSE;

		ok = ::ReadFile(m_file, &tmp_char, sizeof(tmp_char),
				&dwBytesRead, NULL);
		if(!ok || dwBytesRead != sizeof(tmp_char)) {
			throw ErrReadFailed;
		}

		if(iswxdigit(tmp_char) || tmp_char == L'x') {
			temp[i++] = (WCHAR)tmp_char;
		} else {
			parse = FALSE;
		}
	};

	temp[i] = '\0';

	/* Rewind */
	(void)::SetFilePointer(m_file, -sizeof(tmp_char), NULL, FILE_CURRENT);

	hexString = (!wcsnicmp(temp, L"0x", 2) || !wcsnicmp(&temp[1], L"0x", 2));
	m_tokenValue.dwNumber = wcstoul(temp, &end, (hexString) ? 16 : 10);

	if (end==temp) throw ErrReadFailed;
	if (errno==ERANGE)
		throw ErrNumberOutOfRange;
	GetNextChar();
	}


void CParsePkg::GetNextChar()
	{
	DWORD dwBytesRead;
	unsigned short tmp;
	/*
	 * NOTE! we are reading 4 bytes for each char, but only using 2 bytes
	 */
	if (!::ReadFile(m_file, (LPVOID)&tmp, 2, &dwBytesRead, NULL) ||
		dwBytesRead!=2)
		tmp='\0';
	m_pkgChar = tmp;
	}


BOOL CParsePkg::DoesExist(LPWSTR pszFile, DWORD *pdwSize)
// Purpose  : Attempt to determine whether the file exists (w. or W.out the search path), and gets 
//		      it's file size.
// Inputs   : pszFile	-	The file to find (as a UNICODE string)
//			  pdwSize   - store its size here (set to zero if not found)
// Returns  : Yes or No
	{
	BOOL fFound = FALSE;		
	*pdwSize = 0;
	
	/* Convert to Unix filename */
	LPWSTR tmp = pszFile;
	while(*tmp) {
		if(*tmp == '\\')
			*tmp = '/';
		tmp++;
	}

	try
		{
		HANDLE hFile = ::MakeSISOpenFile(pszFile, GENERIC_READ, OPEN_EXISTING);
		if (hFile != INVALID_HANDLE_VALUE)
			{
			*pdwSize = ::GetFileSize(hFile, NULL);
			::CloseHandle(hFile);
			fFound = TRUE;
			}
		else
			{
			// If we are using a search directory
			if(m_pszSearchDir[0] != '\0')		
				{
				WCHAR pszNewPath[MAX_PATH];
				wcscpy(pszNewPath, m_pszSearchDir);
				wcscat(pszNewPath, pszFile);
				HANDLE hFile = ::MakeSISOpenFile(pszNewPath, GENERIC_READ, OPEN_EXISTING);
				if (hFile != INVALID_HANDLE_VALUE)
					{
					*pdwSize = ::GetFileSize(hFile, NULL);
					::CloseHandle(hFile);
					wcscpy(pszFile, pszNewPath);
					fFound = TRUE;
					}
				}
			}
		}
	catch (TUtilsException excp)
		{
		// ignore error if writing a stub file
		if (!m_stubFile) throw excp;
		m_pObserver->DoVerbage(__T("warning :file does not exist"));
		fFound=TRUE;
		}
	return fFound;
	}

