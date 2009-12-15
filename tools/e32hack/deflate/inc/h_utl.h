// H_UTL.H
//
// Copyright (c) 1995-2004 Symbian Software Ltd.  All rights reserved.
//

#if !defined(__H_UTL_H__)
#define __H_UTL_H__
//
#include <stdio.h>
#include <string.h>

#ifdef __VC32__
#ifdef __MSVCDOTNET__
#include <iostream>
//  #include <strstream>
#include <fstream>
using namespace std;
#else //!__MSVCDOTNET__
#include <iostream.h>
//  #include <strstrea.h>
#include <fstream.h>
#endif //__MSVCDOTNET__
#else //!__VC32__
#include <iostream>
//  #include <strstream>
#include <fstream>
using namespace std;
#endif // __VC32__

#include <e32cmn.h>
#include <e32def.h>
#include <e32base.h>

#define ALIGN4K(a) ((a+0xfff)&0xfffff000)
#define ALIGN4(a) ((a+0x3)&0xfffffffc)


#ifdef HEAPCHK
#define NOIMAGE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
void HeapCheck();
#endif
#define Print H.PrintString
//
const TInt KMaxStringLength=0x400;
//
class HFile
{
public:
	static TInt Open(const TText * const aFileName, TInt32 * const aFileHandle);
	static TBool Read(const TInt32 aFileHandle, TAny * const aBuffer, const TUint32 aCount);
	static TBool Seek(const TInt32 aFileHandle, const TUint32 aOffset);
	static TUint32 GetPos(const TInt32 aFileHandle);
	static TAny Close(const TInt32 aFileHandle);
	static TUint32 GetLength(const TInt32 aFileHandle);
	static TUint32 GetLength(TText *aName);
	static TUint32 Read(TText *aName, TAny *someMem);
};
//
//inline TAny* operator new(TUint /*aSize*/, TAny* aBase)
//	{return aBase;}

class HMem
{
public:
	static TAny *Alloc(TAny * const aBaseAddress,const TUint32 aImageSize);
	static void Free(TAny * const aMem);
	static void Copy(TAny * const aDestAddr,const TAny * const aSourceAddr,const TUint32 aLength);
	static void Move(TAny * const aDestAddr,const TAny * const aSourceAddr,const TUint32 aLength);
	static void Set(TAny * const aDestAddr, const TUint8 aFillChar, const TUint32 aLength);
	static void FillZ(TAny * const aDestAddr, const TUint32 aLength);

	static TUint CheckSum(TUint *aPtr, TInt aSize);
	static TUint CheckSum8(TUint8 *aPtr, TInt aSize);
	static TUint CheckSumOdd8(TUint8 *aPtr, TInt aSize);
	static TUint CheckSumEven8(TUint8 *aPtr, TInt aSize);

	static void Crc32(TUint32& aCrc, const TAny* aPtr, TInt aLength);
};
//
enum TPrintType {EAlways, EScreen, ELog, EWarning, EError, EPeError, ESevereError, EDiagnostic};
//
class HPrint
{
public:
	~HPrint();
	void SetLogFile(TText *aFileName);
	TInt PrintString(TPrintType aType,const char *aFmt,...);
public:
	TText iText[KMaxStringLength];
	TBool iVerbose;
private:
	ofstream iLogFile;
};
//
extern HPrint H;
extern TBool PVerbose;
//
TAny *operator new(size_t aSize);
void operator delete(TAny *aPtr);
//
//istrstream &operator>>(istrstream &is, TVersion &aVersion);
//
TInt StringToTime(TInt64 &aTime, char *aString);

void ByteSwap(TUint &aVal);
void ByteSwap(TUint16 &aVal);
void ByteSwap(TUint *aPtr, TInt aSize);

extern TBool gLittleEndian;


/**
Convert string to number.
*/
/*
template <class T>
TInt Val(T& aVal, char* aStr)
{

T x;
istrstream val(aStr,strlen(aStr));
val >> x;
if (!val.eof() || val.fail())
return KErrGeneral;
aVal=x;
return KErrNone;
}
*/

// Filename decompose routines
enum TDecomposeFlag
{
	EUidPresent=1,
	EVerPresent=2
};

class TFileNameInfo
{
public:
	TFileNameInfo(const char* aFileName, TBool aLookForUid);
public:
	const char* iFileName;
	TInt iTotalLength;
	TInt iBaseLength;
	TInt iExtPos;
	TUint32 iUid3;
	TUint32 iModuleVersion;
	TUint32 iFlags;
};

extern char* NormaliseFileName(const char* aName);
extern char* SplitFileName(const char* aName, TUint32& aUid, TUint32& aModuleVersion, TUint32& aFlags);
extern char* SplitFileName(const char* aName, TUint32& aModuleVersion, TUint32& aFlags);
extern TInt ParseCapabilitiesArg(SCapabilitySet& aCapabilities, const char *aText);
extern TInt ParseBoolArg(TBool& aValue, const char *aText);

#endif
