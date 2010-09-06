// E32IMAGE.H
//
// Copyright (c) 1996-2001 Symbian Ltd.  All rights reserved.
//

#ifndef __E32IMAGE_H__
#define __E32IMAGE_H__

#include <fstream.h>
#include <e32std.h>
#include "pe_defs.h"
#include "pe_file.h"
// 4K alignment and 4 byte alignment macros
//#define ALIGN4K(a) ((a+0xfff)&0xfffff000)
//#define ALIGN4(a) ((a+0x3)&0xfffffffc)
//
const TInt KOrdinalBase=1;
const TUint KImageDll=0x01;
const TUint KImageNoCallEntryPoint=0x02;
const TUint KImageFixedAddressExe=0x04;
//
enum TCpu {ECpuUnknown=0, ECpuX86=0x1000, ECpuArm=0x2000, ECpuMCore=0x4000};
//
class E32ImageHeader
	{
public:
	TUint32	iUid1;
	TUint32	iUid2;
	TUint32	iUid3;
	TUint32	iCheck;
	TUint iSignature;			// 'EPOC'
	TCpu iCpu;					// 0x1000 = X86, 0x2000 = ARM, 0x4000 = M*Core
	TUint iCheckSumCode;		// sum of all 32 bit words in .text
	TUint iCheckSumData;		// sum of all 32 bit works in .data
	TVersion iVersion;
	TInt64 iTime;
	TUint iFlags;				// 0 = exe, 1 = dll, +2 = no call entry points
	TInt iCodeSize;				// size of code, import address table, constant data and export dir
	TInt iDataSize;				// size of initialised data
	TInt iHeapSizeMin;
	TInt iHeapSizeMax;
	TInt iStackSize;
	TInt iBssSize;
	TUint iEntryPoint;			// offset into code of entry point
	TUint iCodeBase;			// where the code is linked for	
	TUint iDataBase;			// where the data is linked for
	TInt iDllRefTableCount;		// filling this in enables E32ROM to leave space for it
	TUint iExportDirOffset;		// offset into the file of the export address table
	TInt iExportDirCount;
	TInt iTextSize;				// size of just the text section, also doubles as the offset for the iat w.r.t. the code section
	TUint iCodeOffset;			// file offset to code section
	TUint iDataOffset;			// file offset to data section
	TUint iImportOffset;		// file offset to import section
	TUint iCodeRelocOffset;		// relocations for code and const
	TUint iDataRelocOffset;		// relocations for data
	TProcessPriority iPriority;	// priority of this process
	};
//
class E32ImportBlock
	{
public:
	TInt Size();
public:
	TUint32 iOffsetOfDllName;	// offset of name of dll importing from
	TInt iNumberOfImports;		// number of imports from this dll
								// TUint32 iImport[iNumberOfImports];
	};
//
class E32ImportSection
	{
public:
	TInt iSize;					// size of this section
								// E32ImportBlock[iDllRefTableCount];
	};
//
class E32RelocSection
	{
public:
	TInt iSize;				// size of this relocation section
	TInt iNumberOfRelocs;			// number of relocations in this section
	};
//
class E32ImageFile
	{
public:
	E32ImageFile();
	~E32ImageFile();
	TInt Open(const TText *const aFileName);
	TBool Translate(PEFile &aPEFile);
	TBool Translate(const TText * const aFileName, TUint aDataBase, TBool aAllowDllData);
	void Adjust(TInt aSize);
	TUint VaOfOrdinal(TUint aOrdinal);
	void RelocateSection(char* aPtr, char *aRelocs, TUint aCodeDelta, TUint aDataDelta, char* aImagePtr, TLinAddr** aIATRefs);
	//
	TUint TextOffset();
	TUint ImportAddressTableOffset();
	TUint ConstOffset();
	TUint DataOffset();
	TUint BssOffset();
	TUint CheckSumCode();
	TUint CheckSumData();
	TInt NumberOfImports();

	TInt IsValid();
	TInt IsDll();
	void Dump(TText *aFileName);
	void DumpHeader();
	void DumpData();
	void CreateExportName(char *aName);
	void SetStackSize(TInt aSize);
	void SetHeapSizeMin(TInt aSize);
	void SetHeapSizeMax(TInt aSize);
	void SetUids(TUid aUid1, TUid aUid2, TUid aUid3);
	void SetCallEntryPoints(TInt aBool);
	void SetFixedAddress(TInt aBool);
	void SetPriority(TProcessPriority aPri);
	static TInt IsE32ImageFile(char *aFileName);
private:
	TInt DoCodeHeader(PEFile &aPeFile);
	TInt DoDataHeader(PEFile &aPeFile, TUint aDataBase);
	TInt CopyCode(char *aPtr, PEFile &aPeFile);
	TInt CopyData(char *aPtr, PEFile &aPeFile);
	TInt CopyImportAddrTable(char *aPtr, PEFile &aPeFile);
	char *CreateImportSection(const PEFile &aPeFile, TInt &aSize);
	void CreateExportSection(char *aPtr, PEFile &aPeFile);

	void CreateExportDirectory(char *aPtr, PEFile &aPeFile);
	void FixExportDirectory(TUint *aExportDir, PEFile &aPeFile);
	void FixRelocs(PEFile &aPeFile, TUint *relocation, TUint *relocsection, TInt aNumberOfRelocs);
	char *CreateCodeRelocs(TUint *reloc, TUint *relocsection, TInt nrelocs, TInt &aSize);
	char *CreateDataRelocs(TUint *reloc, TUint *relocsection, TInt nrelocs, TInt &aSize);

	TUint FixAddress(PEFile &aPeFile, TUint va);
	TUint FixImportThunk(PEFile &aPeFile, TUint va);
public:
	char *iData;
	TInt iSize;
	E32ImageHeader *iHeader;
	char *iFileName;
	char *iExportName;
private:
	PIMAGE_SECTION_HEADER iPeHeader[KNumberOfSections];
	char *iPeData[KNumberOfSections];
	TUint iConstOffset;
	TUint iCrtOffset;
	};
ofstream &operator<<(ofstream &os, const E32ImageFile &aImage);
ifstream &operator>>(ifstream &is, E32ImageFile &aImage);
#endif
