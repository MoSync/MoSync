// PARSEPKG.H
// Copyright (c) 1997-1999 Symbian Ltd. All rights reserved.
//
// handles parsing of PKG file
//

#ifndef __PKG_PARSER_H_
#define __PKG_PARSER_H_

// ===========================================================================
// INCLUDES
// ===========================================================================

#include "../include/instform.h"

#ifdef _MSC_VER
#pragma warning( disable : 4201) 
#endif

// ===========================================================================
// CONSTANTS
// ===========================================================================

// comment the following three lines to disable digital signature support
//#ifndef DIGITAL_SIGNATURES
//#define DIGITAL_SIGNATURES
//#endif

#ifdef DIGITAL_SIGNATURES
#undef DIGITAL_SIGNATURES
#endif


// maximum length of strings used in condition expressions
#define MAX_STRING 255

#define MAX_FUNC_ARGS 2

// list of languages
// this list has been copied from e32std.h for the benefit of makesis
// which is PC based not EPOC based
enum TLanguage
	{
	//This list is lifted directly from E32std.h
	ELangTest = 0,
	/** UK English. */
	ELangEnglish = 1,
	/** French. */
	ELangFrench = 2,
	/** German. */
	ELangGerman = 3,
	/** Spanish. */
	ELangSpanish = 4,
	/** Italian. */
	ELangItalian = 5,
	/** Swedish. */
	ELangSwedish = 6,
	/** Danish. */
	ELangDanish = 7,
	/** Norwegian. */
	ELangNorwegian = 8,
	/** Finnish. */
	ELangFinnish = 9,
	/** American. */
	ELangAmerican = 10,
	/** Swiss French. */
	ELangSwissFrench = 11,
	/** Swiss German. */
	ELangSwissGerman = 12,
	/** Portuguese. */
	ELangPortuguese = 13,
	/** Turkish. */
	ELangTurkish = 14,
	/** Icelandic. */
	ELangIcelandic = 15,
	/** Russian. */
	ELangRussian = 16,
	/** Hungarian. */
	ELangHungarian = 17,
	/** Dutch. */
	ELangDutch = 18,
	/** Belgian Flemish. */
	ELangBelgianFlemish = 19,
	/** Australian English. */
	ELangAustralian = 20,
	/** Belgian French. */
	ELangBelgianFrench = 21,
	/** Austrian German. */
	ELangAustrian = 22,
	/** New Zealand English. */
	ELangNewZealand = 23,
	/** International French. */
	ELangInternationalFrench = 24,
	/** Czech. */
	ELangCzech = 25,
	/** Slovak. */
	ELangSlovak = 26,
	/** Polish. */
	ELangPolish = 27,
	/** Slovenian. */
	ELangSlovenian = 28,
	/** Taiwanese Chinese. */
	ELangTaiwanChinese = 29,
	/** Hong Kong Chinese. */
	ELangHongKongChinese = 30,
	/** Peoples Republic of Chinas Chinese. */
	ELangPrcChinese = 31,
	/** Japanese. */
	ELangJapanese = 32,
	/** Thai. */
	ELangThai = 33,
	/** Afrikaans. */
	ELangAfrikaans = 34,
	/** Albanian. */
	ELangAlbanian = 35,
	/** Amharic. */
	ELangAmharic = 36,
	/** Arabic.*/
	ELangArabic = 37,
	/** Armenian. */
	ELangArmenian = 38,
	/** Tagalog. */
	ELangTagalog = 39,
	/** Belarussian. */
	ELangBelarussian = 40,
	/** Bengali. */ 
	ELangBengali = 41,
	/** Bulgarian. */
	ELangBulgarian = 42,
	/** Burmese. */ 
	ELangBurmese = 43,
	/** Catalan. */
	ELangCatalan = 44,
	/** Croation. */
	ELangCroatian = 45,
	/** Canadian English. */
	ELangCanadianEnglish = 46,
	/** International English. */
	ELangInternationalEnglish = 47,
	/** South African English. */
	ELangSouthAfricanEnglish = 48,
	/** Estonian. */
	ELangEstonian = 49,
	/** Farsi. */
	ELangFarsi = 50,
	/** Canadian French. */
	ELangCanadianFrench = 51,
	/** Gaelic. */
	ELangScotsGaelic = 52,
	/** Georgian. */
	ELangGeorgian = 53,
	/** Greek. */ 
	ELangGreek = 54,
	/** Cyprus Greek. */
	ELangCyprusGreek = 55,
	/** Gujarati. */
	ELangGujarati = 56,
	/** Hebrew. */
	ELangHebrew = 57,
	/** Hindi. */ 
	ELangHindi = 58,
	/** Indonesian. */
	ELangIndonesian = 59,
	/** Irish. */
	ELangIrish = 60,
	/** Swiss Italian. */
	ELangSwissItalian = 61,
	/** Kannada. */
	ELangKannada = 62,
	/** Kazakh. */
	ELangKazakh = 63,
	/** Kmer. */
	ELangKhmer = 64,
	/** Korean. */
	ELangKorean = 65,
	/** Lao. */
	ELangLao = 66,
	/** Latvian. */
	ELangLatvian = 67,
	/** Lithuanian. */
	ELangLithuanian = 68,
	/** Macedonian. */
	ELangMacedonian = 69,
	/** Malay. */
	ELangMalay = 70,
	/** Malayalam. */
	ELangMalayalam = 71,
	/** Marathi. */ 
	ELangMarathi = 72,
	/** Moldovian. */
	ELangMoldavian = 73,
	/** Mongolian. */
	ELangMongolian = 74,
	/** Norwegian Nynorsk. */
	ELangNorwegianNynorsk = 75,
	/** Brazilian Portuguese. */
	ELangBrazilianPortuguese = 76,
	/** Punjabi. */
	ELangPunjabi = 77,
	/** Romanian. */
	ELangRomanian = 78,
	/** Serbian. */
	ELangSerbian = 79,
	/** Sinhalese. */
	ELangSinhalese = 80,
	/** Somali. */
	ELangSomali = 81,
	/** International Spanish. */
	ELangInternationalSpanish = 82,
	/** American Spanish. */
	ELangLatinAmericanSpanish = 83,
	/** Swahili. */
	ELangSwahili = 84,
	/** Finland Swedish. */
	ELangFinlandSwedish = 85,
	ELangReserved1 = 86,		// reserved for future use
	/** Tamil. */ 
	ELangTamil = 87,
	/** Telugu. */
	ELangTelugu = 88,
	/** Tibetan. */
	ELangTibetan = 89,
	/** Tigrinya. */
	ELangTigrinya = 90,
	/** Cyprus Turkish. */
	ELangCyprusTurkish = 91,
	/** Turkmen. */
	ELangTurkmen = 92,
	/** Ukrainian. */
	ELangUkrainian = 93,
	/** Urdu. */ 
	ELangUrdu = 94,
	ELangReserved2 = 95,	// reserved for future use
	/** Vietnamese. */
	ELangVietnamese = 96,
	/** Welsh. */
	ELangWelsh = 97,
	/** Zulu. */
	ELangZulu = 98,
	/** @deprecated 6.2 */
	ELangOther = 99,
	ELangNone = 0xFFFF
	};

// Error codes for possible parser failures
enum TParseException
	{
	ErrUnknownLine,
	ErrHeaderAlreadyDefined,
	ErrLanguagesAlreadyDefined,
	ErrLanguagesNotDefined,
	ErrUnknownLanguagesId,
	ErrHeaderNotDefined,
	ErrNoMainDependency,
	ErrFileNotFound,
	ErrPackageNotASISFile,			
	ErrBadCondFormat,
	ErrReadFailed,
	ErrBadOption,
	ErrBadString,
	ErrUnexpectedToken,
	ErrInvalidEscape,
	ErrNumberOutOfRange,
	ErrUnknownVariable,
	ErrUninstallNeedsWaitEnd,
	ErrUIDMismatch,
	ErrCannotGetFullPath,
	ErrBadDestinationPath
	};

typedef int TOKEN;

#define EOF_TOKEN			0
#define NUMERIC_TOKEN		1
#define ALPHA_TOKEN			2
#define QUOTED_STRING_TOKEN	3
#define AND_TOKEN			4	
#define OR_TOKEN			5
#define NOT_TOKEN			6
#define EXISTS_TOKEN		7
#define DEVCAP_TOKEN		8
#define APPCAP_TOKEN		9
#define GE_TOKEN			10
#define LE_TOKEN			11
#define NE_TOKEN			12
#define IF_TOKEN			13
#define ELSEIF_TOKEN		14	
#define ELSE_TOKEN			15
#define ENDIF_TOKEN			16
#define TYPE_TOKEN			17
#define KEY_TOKEN			18
#define LAST_TOKEN			18

// ===========================================================================
// INTERNAL DATA STRUCTURES CONSTRUCTED WHEN PARSING PKG FILE
// ===========================================================================

struct LANGSTRINGNODE
// llist of language dependent names (app names & dependency names)
	{
	WCHAR pszString[MAX_PATH];
	struct LANGSTRINGNODE* pNext;
	};

struct LANGNODE
// llist of languages supported by this SIS file
	{
	WORD wLang;
	struct LANGNODE* pNext;
	};

struct DEPENDNODE
// llist of dependencies
	{
	DWORD dwUID;
	WORD wMajor;
	WORD wMinor;
	DWORD dwBuild;
	LANGSTRINGNODE* pLangStringBase;
	struct DEPENDNODE* pNext;
	};

struct CAPABILITYNODE
// llist of capabilities
	{
	INT iKey;
	INT iValue;
	struct CAPABILITYNODE* pNext;
	};

#define MAX_MIME_TYPE 200

struct PKGLINEFILE
// file to include in SIS file
	{
	TInstFileType type;
	TInstFileOption options;
	WCHAR pszSource[MAX_PATH];
	WCHAR pszDest[MAX_PATH];
	WCHAR pszMimeType[MAX_MIME_TYPE];
	DWORD dwSize;
	DWORD dwUncompressedSize;
	};

union VARIANTVAL
// a variant, i.e. either a number or string value
	{
	LONG dwNumber;					// numeric value, e.g. 100
	WCHAR pszString[MAX_STRING];	// string value, e.g. "crystal"
	};

struct PKGLINECONDITION
// a conditional expression used within an IF/ELSEIF
	{
	TInstCondExprType exprType;		// type of expression/operator
	union
		{
		// if exprType = binary op then followed by:
		//		LHS InstCondExpr, RHS InstCondExpr
		struct {
			struct PKGLINECONDITION* pLhs;
			struct PKGLINECONDITION* pRhs;
			}b;
		// if exprType = unary op then followed by:
		//		InstCondExpr
		struct PKGLINECONDITION* pExpr;
		// if exprType = function then followed by:
		//		array of InstCondExpr
		struct PKGLINECONDITION* pArg[MAX_FUNC_ARGS];
		// if exprType = primitive then followed by:
		//		InstCondPrim
		union VARIANTVAL* pPrim;
		};
	};

struct PKGLINEOPTIONS
// an options node, i.e. ! in PKG format
	{
	DWORD dwNumOptions;				// number of options
	LANGSTRINGNODE* pLangStringBase;// options text (one for each language)
	};

struct PKGLINENODE
// generic PKG line (file, condition or option)
	{
	TInstPackageLineType iPackageLineType;
	union
		{
		struct PKGLINEFILE* file;
		struct PKGLINEOPTIONS* option;
		struct PKGLINECONDITION* cond;
		};
	struct PKGLINENODE* pNext;		// linked list
	};

#define MAX_PASSWORD 100

struct SIGNATURENODE
// NOTE: only the public key certificate is included in the SIS file
	{
	WCHAR pszPrivateKey[MAX_PATH];
	WCHAR pszPublicKey[MAX_PATH];
	WCHAR pszPassword[MAX_PASSWORD];
	WCHAR pszChainFile[MAX_PATH]; //temporary filename used for chain / certificate re-writing
	WCHAR pszB64File[MAX_PATH]; //temporary filename used for base64 decoding
	TUint16 iNumCerts;
	};

#define MAXOPTLEN 20

struct SParseOpt
	{
	WCHAR pszOpt[MAXOPTLEN];
	DWORD dwOpt;
	};

// ===========================================================================
// CLASS DEFINITIONS
// ===========================================================================

class MParserObserver
// Callback mixin - allows disemination of progress data
	{
	public:
		virtual void DoVerbage(const _TCHAR* pszText) const = 0;		// Output 'verbose' messages
		virtual void DoErrMsg(const _TCHAR* pszText) const = 0;		// Output error messages
		virtual BOOL Verbose() const = 0;							// Is verbose output required?
		virtual BOOL SetVerbose(BOOL fVerbose) = 0;		  			// Reset the verbosity status
		virtual void SetLineNumber(WORD wLineNumber) = 0; 			
	};

class CSISWriter;

class CParsePkg
// The base input (pkg) parser class. Pass details to a CSISWriter for later use.
	{
	public:
		CParsePkg();
		void ParseL(HANDLE hFile, CSISWriter *pWriter, BOOL fIsStub, MParserObserver *pObserver = NULL);
		void SetSearchDirectory(LPCWSTR pszPath);	    
		
	private:
		void ParseEmbeddedBlockL();
		void ParseLanguagesL();
		void ParseHeaderL();
		void ParseFileL();
		void ParsePackageL();
		void ParseOptionsBlockL();
		void ParseLanguageBlockL();
		void ParseIfBlockL();
		void ParseDependencyL();
		void ParseSignatureL();
		void ParseCapabilityL();
		void ParseCommentL();
		void ParseConditionL(TInstPackageLineType type);
		PKGLINECONDITION* ParseExpr();
		PKGLINECONDITION* ParseInnerExpr();
		PKGLINECONDITION* ParseLogicalOp();
		PKGLINECONDITION* ParseRelation();
		PKGLINECONDITION* ParseUnary();
		PKGLINECONDITION* ParseFactor();
		
		DWORD ParseOption(const SParseOpt* options, DWORD dwNumOptions, DWORD* pdwOptions);
		
		void ExpectToken(TOKEN aToken);
		void GetNextToken();
		BOOL GetStringToken();
		WORD ParseEscapeChars();
		void GetAlphaNumericToken();
		void GetNumericToken();
		BOOL IsNumericToken();
		void GetNextChar();
		
		const _TCHAR* GetTokenText(TOKEN aToken);
	private:
		BOOL m_stubFile;
		WCHAR m_pszSearchDir[MAX_PATH];				 // Directory to search for component files
		MParserObserver *m_pObserver;					 // The obseverer object
		inline void Verbage(const _TCHAR* pszText);		 // Wrapper for MParserObserver::Verbage()
		BOOL DoesExist(LPWSTR pszFile, DWORD *pdwSize);	 // Does the component file exist, how big is it
		WORD m_nLineNo;					// The line we are currently on
		BOOL m_fInLangBlock;			// Are we processing a language block
		BOOL m_fInLangFileBlock;		// Are we processing a language dependent file block
		WORD m_wCurrLang;				// If we are in a lang/lang file block - which lang are we processing
		CSISWriter *m_pSISWriter;		// Stuff all our intermediate data in here
		
		WCHAR m_pkgChar;
		HANDLE m_file;
		TOKEN m_token;
		VARIANTVAL m_tokenValue;

		BOOL m_enoughForStub;
	};

inline void CParsePkg::Verbage(const _TCHAR* pszText) 
	{ 
	if(m_pObserver) 
		m_pObserver->DoVerbage(pszText); 
	}

#endif // __PKG_PARSER_H_

