// PE_FILE.H
//
// Copyright (c) 1995-2004 Symbian Software Ltd.  All rights reserved.
//

#if !defined(__PE_FILE_H__)
#define __PE_FILE_H__
#include <e32rom.h>
#include <e32image.h>
#include "pe_defs.h"
//
const TInt KTextSection=0;
const TInt KConstSection=1;
const TInt KExportSection=2;
const TInt KDataSection=3;
const TInt KBssSection=4;
const TInt KImportSection=5;
const TInt KCrtSection=6;
const TInt KRelocSection=7;
const TInt KNumberOfSections=8;
//
enum TImportStat {EImpError, EImpSuccess, EImpDone};
//
class PEFile
{
public:
	PEFile();
	~PEFile();
	TBool Init(const TText * const aFileName);
	void Close();
	//
	TImportStat GetNextImport(TText * &aDllName, TUint16 &aOrdinal, TUint32 * &aThunkPtr);
	TUint32 GetFixUp(const TUint16 aOrdinal);
	// 	TText *GetImageName();
	//	TUint8 *GetExportName(const TUint16 aOrdinal);
	//	TUint32 WriteDllRefTable(PEFile aPeArray[], TUint aMaxFiles);
	void RelocateExportTable();
	TUint GetNumberOfExportedFunctions();
	TUint GetOrdinalBase();
	TBool ExportSectionExists();
	TBool ImportSectionExists();
	TUint RoundToSectionSize(TUint aSize);
	void DumpPeHeaders();
	//
	TInt Normalise();
	void GetRelocs(TUint *aReloc, TUint *aRelocSection, TInt aNumberOfRelocs);
	TInt ReadSectionHeaders();
	TInt ReadData();
	TInt ReadExportDirectory();
	char *ReadSectionData(PIMAGE_SECTION_HEADER aHeader);
	TInt NumberOfImports() const;
	TInt NumberOfImportDlls() const;
	TInt NumberOfExports() const;
	TInt NumberOfRelocs();
public:
	static TInt CmpSectionName(PIMAGE_SECTION_HEADER apSectionHeader, char *aName);
	static TInt VirtualAddressInSection(TUint aVA, PIMAGE_SECTION_HEADER aHeader);
	static TInt FindSectionByVa(TUint aVa, PIMAGE_SECTION_HEADER *peSectionHeader);
	static TInt HasInitialisedData(PIMAGE_SECTION_HEADER aHeader);

	//	static TBool DoFixUps(PEFile aPeArray[], TUint aMaxFiles);
	//	static TBool FindByName(PEFile aPeArray[], TUint aMaxFiles,const TText * const aDllName, PEFile * &aExportPEFile);
	//	static void WriteDllRefTables(PEFile aPeArray[], TUint aMaxFiles);
	//	static void RelocateExportTables(PEFile aPeArray[], TUint aMaxFiles);
private:
	TBool IsInCode(TUint32 anAddr);
	TBool IsInData(TUint32 anAddr);
	TBool IsInDataReloc(TUint32 anAddr);
	TBool IsInExport(TUint32 anAddr);
	TBool IsInImport(TUint32 anAddr);
	TBool IsValidDOSHeader(PIMAGE_DOS_HEADER pDOSHeader);
	TBool IsValidNTHeader(PIMAGE_NT_HEADERS pNTHeader);
	TBool IsValidFileHeader(PIMAGE_FILE_HEADER pFileHeader);
	void DumpNextSectionInFile(PIMAGE_SECTION_HEADER pSectionHeader);
	void CopySectionData(TAny *source, TAny *dest, TUint32 fileLength, TUint32 memLength);
	TBool ProcessRelocData(TAny *relocData,TInt dataSize);
	PEFile(const PEFile&);
	const PEFile & operator = (const PEFile&);
public:
	static TUint32 iRomMemBase; // where the ROM is being loaded to
	static TUint32 iRomLinearBase; // where the ROM will run
	TUint32 iMemBase; // where this file is being loaded to
	TUint32 iEntryPoint;
	TUint32 iImageSize;
	TUint32 iCodeSize;
	TUint32 iDataSize;
	TUint32 iHeapReservedSize;
	TUint32 iHeapCommittedSize;
	TUint32 iStackReservedSize;
	TUint32 iStackCommittedSize;
	TUint32 iBssSize;
	TUint32 iBssOffset;
	TUint32 iSectionAlign;
	TUint32 iExpDirectoryOffset;
	TUint32 iDataOffset;
	TBool	iImageIsDll;
private:
	IMAGE_NT_HEADERS *iHeader;
	PIMAGE_EXPORT_DIRECTORY	iExpDirectory;
	PIMAGE_IMPORT_DESCRIPTOR iImpDescriptor; 
	TText *iFileName;
	TInt32 iFileHandle;
	TUint32 iLinkedBase;
	TUint32 iStartOfHeaders; // whether DOS header or not
	TUint32 iSizeOfHeaders; // Up to and including section headers
	TUint32 iNumSections;
	TUint32 iRomRunAddr; // where the code will run & rdata be accessed
	TUint32 iRamRunAddr; // and where the data & bss will be when it does
	TUint32 iRomDelta;
	TUint32 iRamDelta;
	TBool iHadDataSection;

	// stuff for relocating image successfully in 2 parts
	TUint32 iBssSectionLinkedAddr;
	TUint32 iBssSectionAddr;
	TUint32 iBssSectionSize;
	TUint32 iDataSectionLinkedAddr;
	TUint32 iDataSectionAddr;
	TUint32 iDataSectionSize;
	TUint32 iCodeSectionAddr;
	TUint32 iCodeSectionSize;
	TUint32 iRDataSectionAddr;
	TUint32 iRDataSectionSize;
	TUint32 iCRTSectionAddr;
	TUint32 iCRTSectionSize;
	TUint32 iExportDataDir;

	// stuff for the pe->e32image translator
	PIMAGE_SECTION_HEADER iSectionHeader[KNumberOfSections];
	char *iSectionData[KNumberOfSections];
	TInt iExportDirSize;
	TInt iCpu;
	friend class E32ImageFile;
	friend class E32ImageFile_PE;
};

class E32ImageFile_PE : public E32ImageFile
{
public:
	E32ImageFile_PE();
	virtual ~E32ImageFile_PE();
	virtual TBool Translate(const char* aFileName, TUint aDataBase, TBool aAllowDllData);
	TBool Translate(PEFile &aPEFile);
	TUint ImportAddressTableOffset();
	TUint ConstOffset();
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
private:
	PIMAGE_SECTION_HEADER iPeHeader[KNumberOfSections];
	char *iPeData[KNumberOfSections];
	TUint iConstOffset;
	TUint iCrtOffset;
};

#endif
