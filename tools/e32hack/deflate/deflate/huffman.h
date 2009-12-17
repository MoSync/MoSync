// e32tools\petran\Szip\huffman.h
//
// Copyright (c) 1998-2004 Symbian Software Ltd. All rights reserved.
//
#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__

#include <e32std.h>

/** Bit output stream.
Good for writing bit streams for packed, compressed or huffman data algorithms.

This class must be derived from and OverflowL() reimplemented if the bitstream data
cannot be generated into a single memory buffer.

@since 8.0
@library euser.lib
*/
class TBitOutput
{
public:
	TBitOutput();
	TBitOutput(TUint8* aBuf,TInt aSize);
	virtual ~TBitOutput() {}
	inline void Set(TUint8* aBuf,TInt aSize);
	inline const TUint8* Ptr() const;
	inline TInt BufferedBits() const;
	//
	void WriteL(TUint aValue, TInt aLength);
	void HuffmanL(TUint aHuffCode);
	void PadL(TUint aPadding);
private:
	void DoWriteL(TUint aBits, TInt aSize);
	virtual void ATTRIBUTE(noreturn, OverflowL());
private:
	TUint iCode;		// code in production
	TInt iBits;
	TUint8* iPtr;
	TUint8* iEnd;
};

/** Set the memory buffer to use for output

Data will be written to this buffer until it is full, at which point OverflowL() will
be called. This should handle the data and then can Set() again to reset the buffer
for further output.

@param "TUint8* aBuf" The buffer for output
@param "TInt aSize" The size of the buffer in bytes
*/
inline void TBitOutput::Set(TUint8* aBuf,TInt aSize)
{iPtr=aBuf;iEnd=aBuf+aSize;}
/** Get the current write position in the output buffer

In conjunction with the address of the buffer, which should be known to the
caller, this describes the data in the bitstream.
*/
inline const TUint8* TBitOutput::Ptr() const
{return iPtr;}
/** Get the number of bits that are buffered

This reports the number of bits that have not yet been written into the
output buffer. It will always lie in the range 0..7. Use PadL() to
pad the data out to the next byte and write it to the buffer.
*/
inline TInt TBitOutput::BufferedBits() const
{return iBits+8;}


/** Bit input stream.
Good for reading bit streams for packed, compressed or huffman data algorithms.
@since 8.0
@library euser.lib
*/
class TBitInput
{
public:
	TBitInput();
	TBitInput(const TUint8* aPtr, TInt aLength, TInt aOffset=0);
	virtual ~TBitInput() {}
	void Set(const TUint8* aPtr, TInt aLength, TInt aOffset=0);
	//
	TUint ReadL();
	TUint ReadL(TInt aSize);
	TUint HuffmanL(const TUint32* aTree);
private:
	virtual ATTRIBUTE(noreturn, void UnderflowL());
private:
	TInt iCount;
	TUint iBits;
	TInt iRemain;
	const TUint32* iPtr;
};

/** Huffman code toolkit.

This class builds a huffman encoding from a frequency table and builds
a decoding tree from a code-lengths table

The encoding generated is based on the rule that given two symbols s1 and s2, with 
code length l1 and l2, and huffman codes h1 and h2:

if l1<l2 then h1<h2 when compared lexicographically
if l1==l2 and s1<s2 then h1<h2 ditto

This allows the encoding to be stored compactly as a table of code lengths

@since 8.0
@library euser.lib
*/
class Huffman
{
public:
	enum {KMaxCodeLength=27};
	enum {KMetaCodes=KMaxCodeLength+1};
	enum {KMaxCodes=0x8000};
public:
	static void HuffmanL(const TUint32 aFrequency[],TInt aNumCodes,TUint32 aHuffman[]);
	static void Encoding(const TUint32 aHuffman[],TInt aNumCodes,TUint32 aEncodeTable[]);
	static void Decoding(const TUint32 aHuffman[],TInt aNumCodes,TUint32 aDecodeTree[],TInt aSymbolBase=0);
	static TBool IsValid(const TUint32 aHuffman[],TInt aNumCodes);
	//
	static void ExternalizeL(TBitOutput& aOutput,const TUint32 aHuffman[],TInt aNumCodes);
	static void InternalizeL(TBitInput& aInput,TUint32 aHuffman[],TInt aNumCodes);
};

#endif
