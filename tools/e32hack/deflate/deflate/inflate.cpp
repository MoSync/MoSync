// e32tools\petran\Szip\inflate.cpp
//
// Copyright (c) 1998-2004 Symbian Ltd. All rights reserved.
//
#include "deflate.h"
#include "panic.h"
#include "h_utl.h"

// Class RInflater
//
// The inflation algorithm, complete with huffman decoding

inline CInflater::CInflater(TBitInput& aInput)
:iBits(&aInput),iEncoding(0),iOut(0)
{}

void CInflater::ConstructL()
{
	iEncoding=new TEncoding;
	if(iEncoding==NULL)
		Panic(EHuffmanOutOfMemory);
	InitL();
	iLen=0;
	iOut=new TUint8[KDeflateMaxDistance];
	if(iOut==NULL)
		Panic(EHuffmanOutOfMemory);

	iAvail=iLimit=iOut;
}

CInflater* CInflater::NewLC(TBitInput& aInput)
{
	CInflater* self=new CInflater(aInput);
	if(self==NULL)
		Panic(EHuffmanOutOfMemory);
	self->ConstructL();
	return self;
}

CInflater::~CInflater()
{
	delete iEncoding;
	delete [] iOut;
}

TInt CInflater::ReadL(TUint8* aBuffer,TInt aLength)
{
	TInt tfr=0;
	for (;;)
	{
		TInt len=Min((int)aLength, (int)(iLimit-iAvail));
		if (len && aBuffer)
		{
			HMem::Copy(aBuffer,iAvail,len);
			aBuffer+=len;
		}
		aLength-=len;
		iAvail+=len;
		tfr+=len;
		if (aLength==0)
			return tfr;
		len=InflateL();
		if (len==0)
			return tfr;
		iAvail=iOut;
		iLimit=iAvail+len;
	}
}

TInt CInflater::SkipL(TInt aLength)
{
	return ReadL(0,aLength);
}

void CInflater::InitL()
{
	// read the encoding
	Huffman::InternalizeL(*iBits,iEncoding->iLitLen,KDeflationCodes);
	// validate the encoding
	if (!Huffman::IsValid(iEncoding->iLitLen,TEncoding::ELitLens) ||
		!Huffman::IsValid(iEncoding->iDistance,TEncoding::EDistances))
		Panic(EHuffmanCorruptFile);
	// convert the length tables into huffman decoding trees
	Huffman::Decoding(iEncoding->iLitLen,TEncoding::ELitLens,iEncoding->iLitLen);
	Huffman::Decoding(iEncoding->iDistance,TEncoding::EDistances,iEncoding->iDistance,KDeflateDistCodeBase);
}

TInt CInflater::InflateL()
//
// consume all data lag in the history buffer, then decode to fill up the output buffer
// return the number of available bytes in the output buffer. This is only ever less than
// the buffer size if the end of stream marker has been read
//
{
	// empty the history buffer into the output
	TUint8* out=iOut;
	TUint8* const end=out+KDeflateMaxDistance;
	const TUint32* tree=iEncoding->iLitLen;
	if (iLen<0)	// EOF
		return 0;
	if (iLen>0)
		goto useHistory;
	//
	while (out<end)
	{
		// get a huffman code
		{
			TInt val=iBits->HuffmanL(tree)-TEncoding::ELiterals;
			if (val<0)
			{
				*out++=TUint8(val);
				continue;			// another literal/length combo
			}
			if (val==TEncoding::EEos-TEncoding::ELiterals)
			{	// eos marker. we're done
				iLen=-1;
				break;
			}
			// get the extra bits for the code
			TInt code=val&0xff;
			if (code>=8)
			{	// xtra bits
				TInt xtra=(code>>2)-1;
				code-=xtra<<2;
				code<<=xtra;
				code|=iBits->ReadL(xtra);
			}
			if (val<KDeflateDistCodeBase-TEncoding::ELiterals)
			{	// length code... get the code
				iLen=code+KDeflateMinLength;
				tree=iEncoding->iDistance;
				continue;			// read the huffman code
			}
			// distance code
			iRptr=out-(code+1);
			if (iRptr+KDeflateMaxDistance<end)
				iRptr+=KDeflateMaxDistance;
		}
useHistory:
		TInt tfr=Min((int)(end-out), (int)iLen);
		iLen-=tfr;
		const TUint8* from=iRptr;
		do
		{
			*out++=*from++;
			if (from==end)
				from-=KDeflateMaxDistance;
		} while (--tfr!=0);
		iRptr=from;
		tree=iEncoding->iLitLen;
	};
	return (int)(out-iOut);
}

