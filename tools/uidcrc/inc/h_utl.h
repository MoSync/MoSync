// H_UTL.H
//
// Copyright (c) 1995-1999 Symbian Ltd.  All rights reserved.
//

#if !defined(__H_UTL_H__)
#define __H_UTL_H__
//
#include <stdio.h>
#include <iostream.h>
#if __GNUC__ >= 3
#include <sstream>
#define istrstream std::istringstream
#else
#include <strstream.h>
#endif
#include <fstream.h>
#include <e32std.h>

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
const TInt KMaxStringLength=0x100;
//
class HFile
	{
public:
	static TBool Open(const TText * const aFileName, TInt32 * const aFileHandle);
	static TBool Read(const TInt32 aFileHandle, TAny * const aBuffer, const TUint32 aCount);
	static TBool Seek(const TInt32 aFileHandle, const TUint32 aOffset);
	static TUint32 GetPos(const TInt32 aFileHandle);
	static TAny Close(const TInt32 aFileHandle);
	static TUint32 GetLength(TText *aName);
	static TUint32 Read(TText *aName, TAny *someMem);
	};
//
class HMem
	{
public:
	static TAny *Alloc(TAny * const aBaseAddress,const TUint32 aImageSize);
	static void Free(TAny * const aMem);
	static void Copy(TAny * const aDestAddr,const TAny * const aSourceAddr,const TUint32 aLength);
	static void Set(TAny * const aDestAddr, const TUint8 aFillChar, const TUint32 aLength);
	static TUint CheckSum(TUint *aPtr, TInt aSize);
	static TUint CheckSum8(TUint8 *aPtr, TInt aSize);
	static TUint CheckSumOdd8(TUint8 *aPtr, TInt aSize);
	static TUint CheckSumEven8(TUint8 *aPtr, TInt aSize);
	};
//
enum TPrintType {EAlways, EScreen, ELog, EWarning, EError, EPeError, ESevereError};
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
istrstream &operator>>(istrstream &is, TVersion &aVersion);
//

void ByteSwap(TUint &aVal);
void ByteSwap(TUint16 &aVal);
void ByteSwap(TUint *aPtr, TInt aSize);

extern TBool gLittleEndian;


/**
 Convert string to number.
*/
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


unsigned short get_u16(const void *p);
unsigned int get_u32(const void *p);


#endif
