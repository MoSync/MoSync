// INSTFORM.H
// Copyright (c) 1997-1999 Symbian Ltd. All rights reserved.
//
// Defines software install SIS file format
// NOTE: this include is used for building both EPOC & Windows programs
//

#ifndef __INST_FORMAT_H__
#define __INST_FORMAT_H__

// define a limited number of simple types for cross EPOC/Windows source
#ifdef __SYMBIAN32__
// EPOC build - include standard types
#include <e32def.h>
#else
#include <stdint.h>

typedef int16_t TInt16;
typedef uint16_t TUint16;
typedef int32_t TInt32;
typedef uint32_t TUint32;
typedef uint8_t TUint8;
#endif

// file offset type
typedef TInt32 FOFF;

// Current SIS file version
// (was 100 up to and including ER5)
const TInt32 KInstallerVersion=400;

// UID values
#define KUidInstallNumUid 3
#define KUidInstallAppUid1  0x1000006D	// narrow
#define KUidInstallAppUid1U 0x10003A12	// unicode
#define KUidInstallAppValue 0x10000419

// number of spare words at end of header block
#define KInstallSISSpareLen 8
// maximum number of nested IF...ENDIF blocks
#define KInstallMaxIfDepth 10
// maximum length of a condition value/variable string, e.g. "dfrd"
#define KInstallMaxCondString 255

// maximum number of options in an options statement
// options are stored as bit flags in four 32bit ints
#define KInstallMaxOptionsInts	4
#define KInstallMaxOptions		(KInstallMaxOptionsInts*32)	// 4 => 4*32=128 options

// ===========================================================================
// SIS FILE FORMAT STRUCTURES
// ===========================================================================

struct TInstString
// string format within SIS file
	{
	TInt32 iCount;		// length
	FOFF iStringPtr;	// file offset to string within string block
	};

struct TInstVersion
// version format
	{
	TInt16 iMajor;
	TInt16 iMinor;
	TInt32 iBuild;
	};

struct TInstUID
// standard UID
	{
	TInt32 iUid[3];
	};

// Language value
typedef TInt16 TInstLang;

// struct InstLangString
// InstLangString - imaginary struct
// {
//		TInt32 dwLength[wNumLangs];
//		FOFF ffString[wNumLangs];
// };

enum TInstFileType
	{
	EInstFileTypeSimple = 0,		// Simple file to be copied to destination
	EInstFileTypeText,				// Text file to be displayed (and discarded)
	EInstFileTypeComponent,			// Component file to be installed
	EInstFileTypeRun,				// File to be run (and discarded)
	EInstFileTypeNull,				// Null File, will not be installed but will be uninstalled
	EInstFileTypeMime,				// MIME file
	EInstFileTypeSubSIS,			// Component file to be installed but that can be
									// removed whithout affecting the container
	EInstFileTypeContainerSIS,		// This gives the name of the SIS file which 
									// contains this SIS in a EInstFileTypeSubSIS
									// package line
	EInstFileTypeTextUninstall,		// Text file to be stored on the device and displayed
									// when uninstalling the package
	EInstFileTypeNotInstalled=99	// Flag to indicate file was not installed
	};

enum TInstFileSimpleOption
	{
	EInstFileFileOptionNone = 0
	};

enum TInstFileTextOption
	{
//	EInstFileTextOptionContinue		= 1<<9,		// Just a continue button - don't cancel whole install
	EInstFileTextOptionContinue		= 0,		// Just a continue button - don't cancel whole install
	EInstFileTextOptionSkipOneIfNo		= 1<<10,	// Yes/No - skip next file if user selects No
	EInstFileTextOptionAbortIfNo		= 1<<11,	// Yes/No - abort whole install if user selects No
	EInstFileTextOptionExitIfNo		= 1<<12		// Yes/No - uninstall if user selects No
	};

enum TInstFileRunOption
	{
	EInstFileRunOptionInstall		= 1<<1,	// Only run on installation
	EInstFileRunOptionUninstall		= 1<<2,	// Only run on de-installation
	EInstFileRunOptionBoth			= (EInstFileRunOptionInstall | EInstFileRunOptionUninstall),
	EInstFileRunOptionByMimeType		= 1<<3,
	EInstFileRunOptionWaitEnd		= 1<<4,
	EInstFileRunOptionSendEnd		= 1<<5
	};

enum TInstFileMimeOption
	{
	EInstFileMimeOptionNone = 0,
	};

enum TInstCtrlOption
	{
	EInstRunSendEnd = 1<<8,
	EInstRunWaitEnd = 1<<9,
	};

enum TInstFileNullOption
	{
	EInstFileNullOptionNone = 0
	};

union TInstFileOption
	{
	TInstFileSimpleOption iSimpleOption;
	TInstFileTextOption iTextOption;
	TInt32 iComponentUid;
	TInt32 iRunOption;
	TInt32 iMimeOption;
	TInstFileNullOption iNullOption;
	};

struct TInstFile
// Files block
	{
	TInstFileType iFileType;		// What type of file is this 
	TInstFileOption iOption;		// Extra info for this file type
	TInstString iSource;			// Source string (i.e. the filename)
	TInstString iDestination;		// Destination string (i.e. the target filepath and filename) 
	// The destination string is something like;
	//   c:\system\loopy\loops.txt - for a file for internal only
	//   !:\system\loopy\loop.aif - for  a file that can be on any drive
	// The "!" character could be replaced by actual installation drive
	// The following are present, but are not defined in the structure
	//	TInt32 iLength[iNumLanguages];	// Length of this file in bytes (compressed length)
	//	FOFF ffFiles[iNumLanguages];	// Pointer to block containing this file
	//	TInt32 iULength[iNumLanguages];	// v2.00: Uncompressed file lengths
	//	TInstString iMimeType;			// v2.00: MIME type e.g. "text/html"
	};

struct TInstDependency
// Dependencies block 
	{
	TInt32 iUid;
	TInstVersion iVersion;	
	// InstLangString ilsNames;		// Dependent names
	};

struct TInstCapability
// Capability block 
	{
	TInt32 iKey;
	TInt32 iValue;
	};
/*
enum TInstCondExprType
	{
	// binary ops
	EInstCondBinOpEq = 0,			// equal to
	EInstCondBinOpNe,				// not equal to
	EInstCondBinOpGt,				// greater than
	EInstCondBinOpLt,				// less than
	EInstCondBinOpGe,				// greater than or equal to
	EInstCondBinOpLe,				// less than or equal to
	
	EInstCondLogOpAnd,				// logical AND
	EInstCondLogOpOr,				// logical OR
	
	// functions
	EInstCondFuncExists,			// EXISTS() - does file exists
	EInstCondFuncDevCap,			// DEVCAP() - query device capabilities
	EInstCondFuncAppCap,			// APPCAP() - query application capabilities

	// unary ops
	EInstCondUnaryOpNot,			// NOT() - logical NOT
	
	// primitives
	EInstCondPrimTypeString,		// string value follows
	EInstCondPrimTypeVariable,		// variable follows
	EInstCondPrimTypeNumber			// numeric integer value follows
	};
*/

enum TInstCondExprType {
	EInstCondBinOpEq = 1,
	EInstCondBinOpNe,
	EInstCondBinOpGt,
	EInstCondBinOpLt,
	EInstCondBinOpGe,
	EInstCondBinOpLe,
	EInstCondLogOpAnd,
	EInstCondLogOpOr,
	EInstCondUnaryOpNot,
	EInstCondFuncExists, 
	EInstCondFuncAppProperties,
	EInstCondFuncDevProperties,
	EInstCondPrimTypeString,
	EInstCondPrimTypeOption,
	EInstCondPrimTypeVariable,
	EInstCondPrimTypeNumber
};

enum TVariableToken
	{
	// values from 0 up to 0x1000 are reserved for use by HALData::TAttribute
	
	// a few extra variables which aren't part of the EPOC Hal API
	EVarLanguage=0x1000,
	EVarRemoteInstall,
		
	};

union TInstCondPrim
	{
	TInt32 iNumber;			// condition numeric value, e.g. 100 or variable Id
	TInstString iString;	// condition string value, e.g. "crystal"
	};

struct TInstCondExpr
// Condition block 
	{
	TInstCondExprType iExprType;			// type of expression
	// if exprType = binary op then followed by:
	//		LHS InstCondExpr, RHS InstCondExpr
	// if exprType = unary op then followed by:
	//		InstCondExpr
	// if exprType = function then followed by:
	//		InstCondExpr,... (number depends upon actual function)
	// if exprType = primitive then followed by:
	//		InstCondPrim
	};

// following macros for accessing option bits
#define GETBIT(i,bit)	(i&(1<<bit))
#define SETBIT(i,bit)	i|=(1<<bit)
#define UNSETBIT(i,bit)	i&=~(1<<bit)

struct TInstOptions
// options block
	{
	TInt32 iNumOptions;					// number of options
	// InstLangString ilsNames[iCount];		// options text
	// TUint32 iSelected[KInstallMaxOptionsInts];	// options selected (1 bit flag for each option)
	};
	
struct TInstTimeStamp
// timestamp used with digital signature
	{
	TInt16 iYear;
	TInt16 iMonth;
	TInt16 iDay;
	TInt16 iHour;
	TInt16 iMinute;
	TInt16 iSecond;
	};

struct TInstCertBlock
// certificate block header
	{
	TInstTimeStamp iTimeStamp;
	TInt32		   iNumCerts;
	};

enum TInstPackageLineType
	{
	EInstPkgLineFile			=0,	// a file
	EInstPkgLineLanguageFile	=1,	// a language dependent file
	EInstPkgLineOption			=2,	// an options line
	EInstPkgLineCondIf			=3,	// start of IF block
	EInstPkgLineCondElseIf		=4,	// start of ELSEIF block
	EInstPkgLineCondElse		=5,	// start of ELSE block
	EInstPkgLineCondEndIf		=6,	// end of IF/ELSEIF/ELSE block
	};

struct TInstPackageLine
// package line block (file, option or condition)
	{
	TInstPackageLineType iPackageLineType;
	// if iPackageLineType = EInstPkgLineFile or EInstPkgLineLanguageFile then followed by:
	//		TInstFile
	// if iPackageLineType = EInstPkgLineOption then followed by:
	//		TInstOptions
	// if iPackageLineType = EInstPkgLineCondIf or EInstPkgLineCondElseIf then followed by:
	//		TInt32 (size of condition data)
	//		TInstCondExpr
	};

enum THeaderFlags
	{
	EInstFlagShutdownApps		= 1<<0,
	EInstFlagIsUnicode		= 1<<1,
	EInstFlagNoCompress		= 1<<2
	};

enum TInstallationFileType
	{
	EInstApplication = 0,
	EInstAugmentation,
	EInstPartialUpgrade,
	EInstPreInstalledApp,
	EInstPreInstalledPatch
	};

// The name of the enum above changed because it no longer applies to SIS files only
// The typedef ensures source files can still use TSISType
typedef TInstallationFileType TSISType;

struct TInstInstallation
// Header block
	{
	TInstUID iThisUID;			// SIS file UID
	TUint32 iUidChecksum;		// UID checksum
	TUint16 iChecksum;			// Checksum value
	TInt16 iNumLanguages;		// Number of language variants in this component
	TInt16 iNumPkgLines;		// Number of PKG lines to process
	TInt16 iNumDependencies;	// Number of dependencies
	TInt16 iLangChosen;			// The language that was chosen
	TInt16 iNumPkgLinesDone;	// Number of PKG lines to processed
	TInt16 iInstallDrive;		// The installation drive (it is actually a character)
	TInt16 iNumCapabilities;	// Application capabilities added by this package
	TInt32 iInstVersion;		// Compatible with which Installer version
	TUint16 iFlags;				// Defined above
	TInt16 iType;				// Defined above
	TInstVersion iCompVersion;	// The component version information

	// Language block
	FOFF iLanguageBlock;		// Offset to the languages
	// Equivalent to InstLang*
	
	// Package lines block
	FOFF iPackageLinesBlock;	// Offset to the package lines block
	// Equivalent to TInstPackageLine*
	
	// Dependencies block
	FOFF iDependenciesBlock;	// Offset to dependency block
	// Equivalant to InstDependency*
	
	// Certificate(s) block
	FOFF iCertificatesBlock;	// Offset to certificate(s) block,
	// or 0 if no digital signature/certificate(s)

	// Language-dependent names
	FOFF iNames;				// Offset to an InstLangString block
	
	// Signature/certificates block
	FOFF iSignatureBlock;		// Offset to a signature block
	// or 0 if no digital signature/certificate(s)
	
	// Capabilities block
	FOFF iCapabilitiesBlock;	// Offset to a capabilities block
								// (sequence of iNumCapabilities TInstCapability values)
	
	/// following for V2 of installer only
	TInt32 iSpaceUsed;			// Space used by installed files (=0 for V1)
	TInt32 iMaximumSpaceNeeded;	// maximum space required to install
	TInt16 iSpare2[KInstallSISSpareLen];		// for future expansion
	};

// Note about blocks:
// Installation files are arranged into blocks as follows.
//
// HEADER block			|
// LANGUAGE block		|
// PACKAGE LINES block	| These are in a SIS stub file
// DEPENDENCY block		|
// CERTIFICATES block	|
// CAPABILITIES block	|
// STRING block 		|
// CODE block
// SIGNATURE block

#endif //  __INST_FORMAT_H__
