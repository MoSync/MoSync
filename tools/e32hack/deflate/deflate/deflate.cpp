// e32tools\petran\Szip\deflate.cpp
//
// Copyright (c) 1998-2004 Symbian Software Ltd. All rights reserved.
//
#include "deflate.h"
#include "h_utl.h"
#include "panic.h"

class HDeflateHash
{
public:
	inline static HDeflateHash* NewLC(TInt aLinks);
	inline ~HDeflateHash();
	//
	inline TInt First(const TUint8* aPtr,TInt aPos);
	inline TInt Next(TInt aPos,TInt aOffset) const;
private:
	inline HDeflateHash(TInt aLinks);
	inline static TInt Hash(const TUint8* aPtr);
private:
	typedef TUint16 TOffset;
private:
	TInt iHash[256];
	TOffset* iOffset;
};

class MDeflater
{
public:
	void DeflateL(const TUint8* aBase,TInt aLength);
private:
	const TUint8* DoDeflateL(const TUint8* aBase,const TUint8* aEnd,HDeflateHash& aHash);
	static TInt Match(const TUint8* aPtr,const TUint8* aEnd,TInt aPos,HDeflateHash& aHas);
	void SegmentL(TInt aLength,TInt aDistance);
	virtual void LitLenL(TInt aCode) =0;
	virtual void OffsetL(TInt aCode) =0;
	virtual void ExtraL(TInt aLen,TUint aBits) =0;
};

class TDeflateStats : public MDeflater
{
public:
	inline TDeflateStats(TEncoding& aEncoding);
private:
	// from MDeflater
	void LitLenL(TInt aCode);
	void OffsetL(TInt aCode);
	void ExtraL(TInt aLen,TUint aBits);
private:
	TEncoding& iEncoding;
};

class TDeflater : public MDeflater
{
public:
	inline TDeflater(TBitOutput& aOutput,const TEncoding& aEncoding);
private:
	// from MDeflater
	void LitLenL(TInt aCode);
	void OffsetL(TInt aCode);
	void ExtraL(TInt aLen,TUint aBits);
private:
	TBitOutput& iOutput;
	const TEncoding& iEncoding;
};


// Class HDeflateHash

inline HDeflateHash::HDeflateHash(TInt aLinks)
{TInt* p=iHash+256;do *--p=-KDeflateMaxDistance-1; while (p>iHash); iOffset = new TOffset[aLinks]; }

inline HDeflateHash::~HDeflateHash()
{ delete [] iOffset; }

inline HDeflateHash* HDeflateHash::NewLC(TInt aLinks)
{
	return new HDeflateHash(Min(aLinks,KDeflateMaxDistance));
}

inline TInt HDeflateHash::Hash(const TUint8* aPtr)
{
	TUint x=aPtr[0]|(aPtr[1]<<8)|(aPtr[2]<<16);
	return (x*KDeflateHashMultiplier)>>KDeflateHashShift;
}

inline TInt HDeflateHash::First(const TUint8* aPtr,TInt aPos)
{
	TInt h=Hash(aPtr);
	TInt offset=Min(aPos-iHash[h],KDeflateMaxDistance<<1);
	iHash[h]=aPos;
	iOffset[aPos&(KDeflateMaxDistance-1)]=TOffset(offset);
	return offset;
}

inline TInt HDeflateHash::Next(TInt aPos,TInt aOffset) const
{return aOffset+iOffset[(aPos-aOffset)&(KDeflateMaxDistance-1)];}


// Class TDeflater
//
// generic deflation algorithm, can do either statistics and the encoder

TInt MDeflater::Match(const TUint8* aPtr,const TUint8* aEnd,TInt aPos,HDeflateHash& aHash)
{
	TInt offset=aHash.First(aPtr,aPos);
	if (offset>KDeflateMaxDistance)
		return 0;
	TInt match=0;
	aEnd=Min(aEnd,aPtr+KDeflateMaxLength);
	TUint8 c=*aPtr;
	do
	{
		const TUint8* p=aPtr-offset;
		if (p[match>>16]==c)
		{	// might be a better match
			const TUint8* m=aPtr;
			for (;;)
			{
				if (*p++!=*m++)
					break;
				if (m<aEnd)
					continue;
				return ((int)(m-aPtr)<<16)|offset;
			}
			TInt l=(int)(m-aPtr)-1;
			if (l>match>>16)
			{
				match=(l<<16)|offset;
				c=m[-1];
			}
		}
		offset=aHash.Next(aPos,offset);
	} while (offset<=KDeflateMaxDistance);
	return match;
}

const TUint8* MDeflater::DoDeflateL(const TUint8* aBase,const TUint8* aEnd,HDeflateHash& aHash)
//
// Apply the deflation algorithm to the data [aBase,aEnd)
// Return a pointer after the last byte that was deflated (which may not be aEnd)
//
{
	const TUint8* ptr=aBase;
	TInt prev=0;		// the previous deflation match
	do
	{
		TInt match=Match(ptr,aEnd,(int)(ptr-aBase),aHash);
		// Extra deflation applies two optimisations which double the time taken
		// 1. If we have a match at p, then test for a better match at p+1 before using it
		// 2. When we have a match, add the hash links for all the data which will be skipped 
		if (match>>16 < prev>>16)
		{	// use the previous match--it was better
			TInt len=prev>>16;
			SegmentL(len,prev-(len<<16));
			// fill in missing hash entries for better compression
			const TUint8* e=ptr+len-2;
			do
			{
				++ptr;
				aHash.First(ptr,(int)(ptr-aBase));
			} while (ptr<e);
			prev=0;
		}
		else if (match<=(KDeflateMinLength<<16))
			LitLenL(*ptr);			// no deflation match here
		else
		{	// save this match and test the next position
			if (prev)	// we had a match at ptr-1, but this is better
				LitLenL(ptr[-1]);
			prev=match;
		}
		++ptr;
	} while (ptr+KDeflateMinLength-1<aEnd);
	if (prev)
	{		// emit the stored match
		TInt len=prev>>16;
		SegmentL(len,prev-(len<<16));
		ptr+=len-1;
	}
	return ptr;
}

void MDeflater::DeflateL(const TUint8* aBase,TInt aLength)
//
// The generic deflation algorithm
//
{
	const TUint8* end=aBase+aLength;
	if (aLength>KDeflateMinLength)
	{	// deflation kicks in if there is enough data
		HDeflateHash* hash=HDeflateHash::NewLC(aLength);
		if(hash==NULL)
			Panic(EHuffmanOutOfMemory);

		aBase=DoDeflateL(aBase,end,*hash);
		delete hash;
	}
	while (aBase<end)					// emit remaining bytes
		LitLenL(*aBase++);
	LitLenL(TEncoding::EEos);	// eos marker
}

void MDeflater::SegmentL(TInt aLength,TInt aDistance)
//
// Turn a (length,offset) pair into the deflation codes+extra bits before calling
// the specific LitLen(), Offset() and Extra() functions.
//
{
	aLength-=KDeflateMinLength;
	TInt extralen=0;
	TUint len=aLength;
	while (len>=8)
	{
		++extralen;
		len>>=1;
	}
	LitLenL((extralen<<2)+len+TEncoding::ELiterals);
	if (extralen)
		ExtraL(extralen,aLength);
	//
	aDistance--;
	extralen=0;
	TUint dist=aDistance;
	while (dist>=8)
	{
		++extralen;
		dist>>=1;
	}
	OffsetL((extralen<<2)+dist);
	if (extralen)
		ExtraL(extralen,aDistance);
}

// Class TDeflateStats
//
// This class analyses the data stream to generate the frequency tables 
// for the deflation algorithm

inline TDeflateStats::TDeflateStats(TEncoding& aEncoding)
:iEncoding(aEncoding)
{}

void TDeflateStats::LitLenL(TInt aCode)
{
	++iEncoding.iLitLen[aCode];
}

void TDeflateStats::OffsetL(TInt aCode)
{
	++iEncoding.iDistance[aCode];
}

void TDeflateStats::ExtraL(TInt,TUint)
{}

// Class TDeflater
//
// Extends MDeflater to provide huffman encoding of the output

inline TDeflater::TDeflater(TBitOutput& aOutput,const TEncoding& aEncoding)
//
// construct for encoding
//
:iOutput(aOutput),iEncoding(aEncoding)
{}

void TDeflater::LitLenL(TInt aCode)
{
	iOutput.HuffmanL(iEncoding.iLitLen[aCode]);
}

void TDeflater::OffsetL(TInt aCode)
{
	iOutput.HuffmanL(iEncoding.iDistance[aCode]);
}

void TDeflater::ExtraL(TInt aLen,TUint aBits)
{
	iOutput.WriteL(aBits,aLen);
}

static void DoDeflateL(const TUint8* aBuf,TInt aLength,TBitOutput& aOutput,TEncoding& aEncoding)
{
	// analyse the data for symbol frequency 
	TDeflateStats analyser(aEncoding);
	analyser.DeflateL(aBuf,aLength);

	// generate the required huffman encodings
	Huffman::HuffmanL(aEncoding.iLitLen,TEncoding::ELitLens,aEncoding.iLitLen);
	Huffman::HuffmanL(aEncoding.iDistance,TEncoding::EDistances,aEncoding.iDistance);

	// Store the encoding table
	Huffman::ExternalizeL(aOutput,aEncoding.iLitLen,KDeflationCodes);

	// generate the tables
	Huffman::Encoding(aEncoding.iLitLen,TEncoding::ELitLens,aEncoding.iLitLen);
	Huffman::Encoding(aEncoding.iDistance,TEncoding::EDistances,aEncoding.iDistance);

	// now finally deflate the data with the generated encoding
	TDeflater deflater(aOutput,aEncoding);
	deflater.DeflateL(aBuf,aLength);
	aOutput.PadL(1);
}

void DeflateL(const TUint8* aBuf, TInt aLength, TBitOutput& aOutput)
{
	TEncoding* encoding=new TEncoding;
	HMem::FillZ(encoding,sizeof(TEncoding));
	DoDeflateL(aBuf,aLength,aOutput,*encoding);
	delete encoding;
}

