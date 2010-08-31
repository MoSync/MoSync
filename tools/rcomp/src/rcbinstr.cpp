/*
* Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
*
*/


#include <assert.h>
#include <string.h>
#include "astring.h"
#include <stdlib.h>
#include <malloc.h>

#include "rcbinstr.h"
#include "tokens.h"  
#include "stack.h"   
#include "numval.h"  
#include "datatype.h"

 
using namespace std ;

extern long CurrentId;
extern RCTypeArray gTypes;

RCBinaryStream::RCBinaryStream()
	{}

RCBinaryStream::~RCBinaryStream()
	{
	if (iOs.is_open())
		{
		iOs.flush();
		iOs.close();
		}
	}

void RCBinaryStream::OpenForAppend(const String& FileName)
	{
	assert( !IsOpen());
	
	iOs.open( FileName.GetAssertedNonEmptyBuffer(), ios::in | ios::out | ios::binary | ios::ate); // ios::in to prevent overwriting
	}

int RCBinaryStream::IsOpen()
	{
	return(iOs.is_open());
	}
		
RCBinaryStream & RCBinaryStream::operator<< ( char o)
	{
	Write((const unsigned char*)&o, 1);
	return * this;
	}

RCBinaryStream & RCBinaryStream::operator<< ( char * o)
	{
	Write((const unsigned char*)o, strlen(o));
	return * this;
	}

int RCBinaryStream::SizeOfCompressedInteger(unsigned int aInteger)
	{ // static
	assert((aInteger&~0x7fff)==0);
	return (aInteger&~0x7f)? 2: 1;
	}

void RCBinaryStream::WriteCompressedInteger(unsigned int aInteger)
	{
	assert((aInteger&~0x7fff)==0);
	if (aInteger&~0x7f)
		{
		*this << char((aInteger>>8)|0x80);
		}
	*this << char(aInteger&0xff);
	}

void RCBinaryStream::Write( const unsigned char * p, unsigned long count)
{
	if(count == 0)
		return;
	iOs.write( (const char*)p, count);
#if defined(_DEBUG)
	iOs.flush();
#endif
}

unsigned long RCBinaryStream::GetPosition()
	{
	return iOs.tellp();
	}

void RCBinaryStream::SetPosition(unsigned long aNewPosition)
	{
	assert(aNewPosition<=GetPosition());
	iOs.seekp(aNewPosition, ios::beg);
	}

// ResourceDataStream - this code makes the apparently valid assumption that the decompressing of compressed Unicode (done by BAFL) will yield exactly the same length of string as what you started with before it was compressed (by RCOMP)

ResourceDataStream::ResourceDataStream()
	:iBuffer(NULL),
	 iNumberOfBytesAllocated(0),
	 iNumberOfBytesUsed(0),
	 iContainsCompressedUnicode(false)
	{
	}

ResourceDataStream::~ResourceDataStream()
	{
	delete [] iBuffer;
	}

void ResourceDataStream::StartOfBlockWithSizePrefix(DataType aDataTypeOfSizePrefix)
	{
	NumericValue* const sizeOfBlockWhenUncompressed=new NumericValue(aDataTypeOfSizePrefix);
	assert(sizeOfBlockWhenUncompressed!=NULL);
	iArrayOfMarks.AppendMark(iNumberOfBytesUsed, EMarkType_StartOfBlockWithSizePrefix, (unsigned int)sizeOfBlockWhenUncompressed);
	}

void ResourceDataStream::EndOfBlockWithSizePrefix()
	{
	iArrayOfMarks.AppendMark(iNumberOfBytesUsed, EMarkType_EndOfBlockWithSizePrefix);
	}

void ResourceDataStream::StartOfCompressedUnicodeRun(int aUncompressedUnicodeSizeInBytes, const unsigned char* aUncompressedUnicodeBuffer)
	{
	if (!iContainsCompressedUnicode)
		{
		// the first run *must* be a compressed Unicode run, so if it isn't, insert a zero-length one at the start
		bool insertZeroLengthCompressedUnicodeRunAtStart=(iNumberOfBytesUsed>0);
		if (!insertZeroLengthCompressedUnicodeRunAtStart)
			{
			const int numberOfMarks=iArrayOfMarks.Size();
			for (int i=0; i<numberOfMarks; ++i)
				{
				const Mark& mark=iArrayOfMarks.MarkAt(i);
				assert(mark.iBufferPosition==0);
				assert(mark.iMarkType!=EMarkType_TwoByteAlignmentPoint); // it is only possible to have a EMarkType_TwoByteAlignmentPoint mark if iNumberOfBytesUsed>0, and we only execute this code if !insertZeroLengthCompressedUnicodeRunAtStart, i.e. if iNumberOfBytesUsed==0 (well, strictly speaking, if iNumberOfBytesUsed<=0)
				if (mark.iMarkType==EMarkType_StartOfBlockWithSizePrefix)
					{
					insertZeroLengthCompressedUnicodeRunAtStart=true;
					break;
					}
				}
			}
		if (insertZeroLengthCompressedUnicodeRunAtStart)
			{
			iArrayOfMarks.InsertMark(0, 0, EMarkType_StartOfCompressedUnicodeRun, (unsigned int)BinaryBuffer::New(0, NULL)); // mark the insertion point for the initial zero-length compressed-Unicode run
			iArrayOfMarks.InsertMark(1, 0, EMarkType_EndOfCompressedUnicodeRun); // mark the insertion point for the subsquent run of "other stuff"
			}
		}
	iArrayOfMarks.AppendMark(iNumberOfBytesUsed, EMarkType_StartOfCompressedUnicodeRun, (unsigned int)BinaryBuffer::New(aUncompressedUnicodeSizeInBytes, aUncompressedUnicodeBuffer));
	iContainsCompressedUnicode=true;
	}

void ResourceDataStream::EndOfCompressedUnicodeRun()
	{
#if !defined(NDEBUG)
	const Mark& markAtStartOfCompressedUnicodeRun=iArrayOfMarks.MarkAt(iArrayOfMarks.Size()-1);
	assert(markAtStartOfCompressedUnicodeRun.iMarkType==EMarkType_StartOfCompressedUnicodeRun);
	const int numberOfBytesWhenCompressed=iNumberOfBytesUsed-markAtStartOfCompressedUnicodeRun.iBufferPosition;
	const BinaryBuffer& runWhenUncompressed=*(const BinaryBuffer*)markAtStartOfCompressedUnicodeRun.iOtherData;
	assert(numberOfBytesWhenCompressed<runWhenUncompressed.NumberOfBytes());
#endif
	iArrayOfMarks.AppendMark(iNumberOfBytesUsed, EMarkType_EndOfCompressedUnicodeRun);
	}

void ResourceDataStream::TwoByteAlignmentPoint()
	{
	iArrayOfMarks.AppendMark(iNumberOfBytesUsed, EMarkType_TwoByteAlignmentPoint);
	}

void ResourceDataStream::EnquireStreamPositionWhenKnown(unsigned long& aStreamPosition)
	{
	iArrayOfMarks.AppendMark(iNumberOfBytesUsed, EMarkType_EnquireStreamPositionWhenKnown, (unsigned int)&aStreamPosition);
	}

void ResourceDataStream::StreamIn(const unsigned char* aBuffer, int aNumberOfBytes)
	{
	EnsureEnoughSpareBytes(aNumberOfBytes);
	memcpy(iBuffer+iNumberOfBytesUsed, aBuffer, aNumberOfBytes);
	iNumberOfBytesUsed+=aNumberOfBytes;
	}

void ResourceDataStream::MakePlaceHolder(int aNumberOfBytes)
	{
	EnsureEnoughSpareBytes(aNumberOfBytes);
	memset(iBuffer+iNumberOfBytesUsed, 0xeb, aNumberOfBytes);
	iNumberOfBytesUsed+=aNumberOfBytes;
	}

class SizePrefix : public StackItem
	{
public:
	inline SizePrefix(int aUncompressedSizeOfResourceUpToStartOfBlock, NumericValue& aSizeOfBlockWhenUncompressed) :iUncompressedSizeOfResourceUpToStartOfBlock(aUncompressedSizeOfResourceUpToStartOfBlock), iSizeOfBlockWhenUncompressed(aSizeOfBlockWhenUncompressed) {}
	inline int UncompressedSizeOfResourceUpToStartOfBlock() const {return iUncompressedSizeOfResourceUpToStartOfBlock;}
	inline NumericValue& SizeOfBlockWhenUncompressed() const {return iSizeOfBlockWhenUncompressed;}
private:
	inline void operator=(const SizePrefix&);
private:
	int iUncompressedSizeOfResourceUpToStartOfBlock;
	NumericValue& iSizeOfBlockWhenUncompressed;
	};

class SizePrefixStack : public Stack
	{
public:
	inline SizePrefixStack() {}
	inline void Push(int aUncompressedSizeOfResourceUpToStartOfBlock, NumericValue& aSizeOfBlockWhenUncompressed) {Stack::Push(new SizePrefix(aUncompressedSizeOfResourceUpToStartOfBlock, aSizeOfBlockWhenUncompressed));}
	inline SizePrefix* Pop() {return (SizePrefix*)Stack::Pop();}
	};

bool ResourceDataStream::StreamOutReturningWhetherContainsCompressedUnicode(RCBinaryStream& aStream, int& aSizeWhenUncompressed)
	{
startOfFirstPass:
	bool encounteredCompressedUnicode[3];
	encounteredCompressedUnicode[0]=false;
	encounteredCompressedUnicode[1]=false;
	encounteredCompressedUnicode[2]=false;
	{
	// do a first pass (i) to calculate run-lengths of the "other-stuff" runs (i.e. the second, fourth, sixth, etc), (ii) to calculate which EMarkType_TwoByteAlignmentPoint marks require a padding byte, and (iii) to calculate the size-prefixes
	// do a second pass to see if there are any compressed-Unicode runs that don't actually make the resource smaller (taking into consideration the preceding run-length byte(s) and any trailing run-length byte(s)) - if so, replace the EMarkType_StartOfCompressedUnicodeRun/EMarkType_EndOfCompressedUnicodeRun mark-pair with a single EMarkType_TwoByteAlignmentPoint mark and go right back to the start of the first pass
	// do a third and final pass to actually write out the resource data to aStream
	const int numberOfMarks=iArrayOfMarks.Size(); // caching this is done *after* the "startOfFirstPass" as ConvertCompressedRunToUncompressed (which is called just before "goto startOfFirstPass") changes the number of items in iArrayOfMarks
	const unsigned int* lengthOfLastRun=NULL;
	for (int pass=0; pass<3; ++pass)
		{
		const int numberOfBytesBeforeFirstMark=NumberOfBytesToNextMark(-1);
		int runLength=numberOfBytesBeforeFirstMark;
		int uncompressedSize=numberOfBytesBeforeFirstMark;
		unsigned int* addressToWriteSizeOfCompressedIntegerTo=NULL; // used on the first pass
		SizePrefixStack sizePrefixStack; // used on the first pass
		int bufferIndex=0; // used on the third pass
		if (pass==2)
			{
			const int numberOfBytesToFirstMark=NumberOfBytesToNextMark(-1);
			assert(bufferIndex==0);
			aStream.Write(reinterpret_cast<unsigned char*>(iBuffer), numberOfBytesToFirstMark);
			bufferIndex+=numberOfBytesToFirstMark;
			}
		for (int i=0; i<numberOfMarks; ++i) // must iterate forwards so that lengthOfLastRun is correctly assigned (after the end of this loop) to point to the iOtherData (i.e. run-length) of the last EMarkType_EndOfCompressedUnicodeRun mark
			{
			Mark& mark=iArrayOfMarks.MarkAt(i);
			switch (mark.iMarkType)
				{
			case EMarkType_StartOfBlockWithSizePrefix:
				{
				NumericValue* const sizeOfBlockWhenUncompressed=(NumericValue*)mark.iOtherData;
				assert(sizeOfBlockWhenUncompressed!=NULL);
				const int sizeOfSizePrefix=gTypes.GetSize(sizeOfBlockWhenUncompressed->NumericValueType());
				runLength+=sizeOfSizePrefix;
				uncompressedSize+=sizeOfSizePrefix;
				if (pass==0)
					{
					sizePrefixStack.Push(uncompressedSize, *(NumericValue*)mark.iOtherData);
					}
				else if (pass==2)
					{
					aStream << *sizeOfBlockWhenUncompressed;
					delete sizeOfBlockWhenUncompressed;
					mark.iOtherData=(unsigned int)(NumericValue*)NULL;
					}
				}
				break;
			case EMarkType_EndOfBlockWithSizePrefix:
				if (pass==0)
					{
					SizePrefix* const sizePrefix=sizePrefixStack.Pop();
					assert(sizePrefix!=NULL);
					sizePrefix->SizeOfBlockWhenUncompressed()=uncompressedSize-sizePrefix->UncompressedSizeOfResourceUpToStartOfBlock();
					delete sizePrefix;
					}
				break;
			case EMarkType_StartOfCompressedUnicodeRun:
				{
				assert(runLength>=0);
				if (addressToWriteSizeOfCompressedIntegerTo!=NULL)
					{
					*addressToWriteSizeOfCompressedIntegerTo=runLength;
					addressToWriteSizeOfCompressedIntegerTo=NULL;
					}
				runLength=0;
				BinaryBuffer& runWhenUncompressed=*(BinaryBuffer*)mark.iOtherData;
				const int numberOfBytesWhenUncompressed=runWhenUncompressed.NumberOfBytes();
				const int numberOfBytesWhenUncompressedIncludingAnyPrecedingPaddingByte=numberOfBytesWhenUncompressed+((uncompressedSize%2!=0)? 1: 0);
				assert((numberOfBytesWhenUncompressed>0) || !encounteredCompressedUnicode[pass]); // compressed-Unicode runs are of non-zero size apart from the compulsory initial compressed-Unicode run if the resource does not actually start with compressed Unicode
				uncompressedSize+=numberOfBytesWhenUncompressedIncludingAnyPrecedingPaddingByte;
				assert(i+1<numberOfMarks);
				++i; // skip a loop iteration - we know that EMarkType_StartOfCompressedUnicodeRun marks are always followed by a EMarkType_EndOfCompressedUnicodeRun mark
				Mark& nextMark=iArrayOfMarks.MarkAt(i);
				assert(nextMark.iMarkType==EMarkType_EndOfCompressedUnicodeRun);
				const int numberOfBytesWhenCompressed=nextMark.iBufferPosition-mark.iBufferPosition;
				assert((numberOfBytesWhenCompressed>0) || !encounteredCompressedUnicode[pass]); // compressed-Unicode runs are of non-zero size apart from the compulsory initial compressed-Unicode run if the resource does not actually start with compressed Unicode
				assert((numberOfBytesWhenCompressed<numberOfBytesWhenUncompressed) || ((!encounteredCompressedUnicode[pass]) && (numberOfBytesWhenCompressed==0) && (numberOfBytesWhenUncompressed==0)));
				if (pass==0)
					{
					addressToWriteSizeOfCompressedIntegerTo=&nextMark.iOtherData; // iOtherData in "other-stuff" runs (i.e. the second run, the fourth run, the sixth run, etc) stores the combined size of the run taking into account any extra bytes caused by other marks, e.g. a padding byte caused by a ETwoByteAlignmentPoint
					}
				else if (pass==1)
					{
					const unsigned int* const lengthOfOtherStuffRun=&nextMark.iOtherData;
					const bool isTheFirstCompressedUnicodeRun=!encounteredCompressedUnicode[pass];
					const bool isTheLastCompressedUnicodeRun=(lengthOfLastRun==lengthOfOtherStuffRun);
					if (isTheLastCompressedUnicodeRun || !isTheFirstCompressedUnicodeRun) // if this is the first compressed-Unicode run and there are others, then we can't get rid of it, hence this check
						{
						int numberOfBytesWhenCompressedIncludingRunLengthsEitherSide=RCBinaryStream::SizeOfCompressedInteger(numberOfBytesWhenCompressed)+numberOfBytesWhenCompressed;
						if ((lengthOfLastRun!=lengthOfOtherStuffRun) || (*lengthOfOtherStuffRun>0))
							{
							numberOfBytesWhenCompressedIncludingRunLengthsEitherSide+=RCBinaryStream::SizeOfCompressedInteger(*lengthOfOtherStuffRun);
							}
						if (numberOfBytesWhenCompressedIncludingRunLengthsEitherSide>=numberOfBytesWhenUncompressedIncludingAnyPrecedingPaddingByte) // use ">=" rather than just ">" as we want to get rid of any compressed-Unicode runs that don't actually give any benefit, so that if possible we can remove the initial compressed-Unicode run (if it's of zero length and it's the only compressed-Unicode run, in which case it's unnecessary overhead)
							{
							if (isTheFirstCompressedUnicodeRun && isTheLastCompressedUnicodeRun) // if this is the only compressed-Unicode run...
								{
								iContainsCompressedUnicode=false;
								}
							ConvertCompressedRunToUncompressed(i-1);
							goto startOfFirstPass; // go back to the start of the first pass again as we need to calculate all the fiddly stuff again, e.g. whether two-byte alignment points need padding bytes, etc.
							}
						}
					}
				else
					{
					assert(pass==2);
					runWhenUncompressed.Destroy();
					mark.iOtherData=(unsigned int)(BinaryBuffer*)NULL;
					aStream.WriteCompressedInteger(numberOfBytesWhenCompressed);
					aStream.Write(reinterpret_cast<unsigned char*>(iBuffer+bufferIndex), numberOfBytesWhenCompressed);
					bufferIndex+=numberOfBytesWhenCompressed;
					const unsigned int* const lengthOfOtherStuffRun=&nextMark.iOtherData;
					if ((lengthOfLastRun!=lengthOfOtherStuffRun) || (*lengthOfOtherStuffRun>0))
						{
						aStream.WriteCompressedInteger(*lengthOfOtherStuffRun);
						}
					}
				encounteredCompressedUnicode[pass]=true;
				}
				break;
			case EMarkType_TwoByteAlignmentPoint:
				{
				const bool needPaddingByte=(uncompressedSize%2!=0);
				if (needPaddingByte)
					{
					++runLength;
					++uncompressedSize;
					if (pass==2)
						{
						aStream << (unsigned char)(0xab);
						}
					}
				if (pass==0)
					{
					mark.iOtherData=needPaddingByte;
					}
				else
					{
					assert((mark.iOtherData!=0)==needPaddingByte);
					}
				}
				break;
			case EMarkType_EnquireStreamPositionWhenKnown:
				if (pass==2)
					{
					unsigned long& streamPosition=*(unsigned long*)mark.iOtherData;
					streamPosition=aStream.GetPosition();
					}
				break;
			default:
				assert(0);
				break;
				}
			const int numberOfBytesToNextMark=NumberOfBytesToNextMark(i);
			if (pass==2)
				{
				aStream.Write(reinterpret_cast<unsigned char*>(iBuffer+bufferIndex), numberOfBytesToNextMark);
				}
			runLength+=numberOfBytesToNextMark;
			uncompressedSize+=numberOfBytesToNextMark;
			bufferIndex+=numberOfBytesToNextMark;
			}
		if (pass>0)
			{
			assert(aSizeWhenUncompressed==uncompressedSize);
			}
		else
			{
			aSizeWhenUncompressed=uncompressedSize;
			lengthOfLastRun=addressToWriteSizeOfCompressedIntegerTo;
			}
		assert(runLength>=0);
		if (addressToWriteSizeOfCompressedIntegerTo!=NULL)
			{
			*addressToWriteSizeOfCompressedIntegerTo=runLength;
			addressToWriteSizeOfCompressedIntegerTo=NULL;
			}
		runLength=0;
		}
	}

	assert(encounteredCompressedUnicode[0]==iContainsCompressedUnicode);
	assert(encounteredCompressedUnicode[1]==iContainsCompressedUnicode);
	assert(encounteredCompressedUnicode[2]==iContainsCompressedUnicode);
	return encounteredCompressedUnicode[0];
	}

void ResourceDataStream::Dump(const char* aDumpFile) const
	{
	ofstream fileStream;
	fileStream.open(aDumpFile, ios::out | ios::binary | ios::trunc);
	fileStream.write((const char*)iBuffer, iNumberOfBytesUsed);
	fileStream.flush();
	fileStream.close();
	}

void ResourceDataStream::EnsureEnoughSpareBytes(int aNumberOfBytes)
	{
	const int numberOfBytesSpare=iNumberOfBytesAllocated-iNumberOfBytesUsed;
	assert(numberOfBytesSpare>=0);
	if (aNumberOfBytes>numberOfBytesSpare)
		{
		const int newNumberOfBytesAllocated=iNumberOfBytesAllocated+aNumberOfBytes+16; // 16 is just some extra bytes to stop the heap being thrashed too much
		unsigned char* const newBuffer = new unsigned char[newNumberOfBytesAllocated];
		if (iNumberOfBytesUsed>0)
			{
			memcpy(newBuffer, iBuffer, iNumberOfBytesUsed);
			}
		delete [] iBuffer;
		iNumberOfBytesAllocated=newNumberOfBytesAllocated;
		iBuffer=newBuffer;
		}
	}

int ResourceDataStream::NumberOfBytesToNextMark(int aMarkIndex) const
	{
	assert(aMarkIndex>=-1);
	assert(aMarkIndex<iArrayOfMarks.Size());
	int numberOfBytesToNextMark=(aMarkIndex+1<iArrayOfMarks.Size())? iArrayOfMarks.MarkAt(aMarkIndex+1).iBufferPosition: iNumberOfBytesUsed;
	if (aMarkIndex>=0)
		{
		numberOfBytesToNextMark-=iArrayOfMarks.MarkAt(aMarkIndex).iBufferPosition;
		}
	return numberOfBytesToNextMark;
	}

void ResourceDataStream::ConvertCompressedRunToUncompressed(int aMarkIndexOfStartOfCompressedUnicodeRun)
	{
	// remove the EMarkType_StartOfCompressedUnicodeRun/EMarkType_EndOfCompressedUnicodeRun mark-pair
	Mark* markForStartOfCompressedUnicodeRun=&iArrayOfMarks.MarkAt(aMarkIndexOfStartOfCompressedUnicodeRun);
	assert(markForStartOfCompressedUnicodeRun->iMarkType==EMarkType_StartOfCompressedUnicodeRun);
	BinaryBuffer& runWhenUncompressed=*(BinaryBuffer*)markForStartOfCompressedUnicodeRun->iOtherData;
	markForStartOfCompressedUnicodeRun->iOtherData=(unsigned int)(BinaryBuffer*)NULL;
	Mark* markForEndOfCompressedUnicodeRun=&iArrayOfMarks.MarkAt(aMarkIndexOfStartOfCompressedUnicodeRun+1);
	assert(markForEndOfCompressedUnicodeRun->iMarkType==EMarkType_EndOfCompressedUnicodeRun);
	const int bufferPositionOfStartOfCompressedUnicodeRun=markForStartOfCompressedUnicodeRun->iBufferPosition;
	const int bufferPositionOfEndOfCompressedUnicodeRun=markForEndOfCompressedUnicodeRun->iBufferPosition;
	const int numberOfBytesWhenCompressed=bufferPositionOfEndOfCompressedUnicodeRun-bufferPositionOfStartOfCompressedUnicodeRun;
	iArrayOfMarks.RemoveMark(aMarkIndexOfStartOfCompressedUnicodeRun); // remove the EMarkType_StartOfCompressedUnicodeRun mark
	iArrayOfMarks.RemoveMark(aMarkIndexOfStartOfCompressedUnicodeRun); // remove the EMarkType_EndOfCompressedUnicodeRun mark
	markForStartOfCompressedUnicodeRun=NULL; // just in case we're tempted to use it again now that it's been removed
	markForEndOfCompressedUnicodeRun=NULL; // just in case we're tempted to use it again now that it's been removed
	const int numberOfBytesWhenUncompressed=runWhenUncompressed.NumberOfBytes();
	const int numberOfExtraBytes=numberOfBytesWhenUncompressed-numberOfBytesWhenCompressed;

	// insert a EMarkType_TwoByteAlignmentPoint mark if necessary
	int startOfMarksToUpdate=aMarkIndexOfStartOfCompressedUnicodeRun;
	if (numberOfBytesWhenCompressed==0)
		{
		assert(numberOfExtraBytes==0);
		assert(numberOfBytesWhenUncompressed==0);
		assert(aMarkIndexOfStartOfCompressedUnicodeRun==0);
		}
	else
		{
		assert(numberOfBytesWhenCompressed>0);
		iArrayOfMarks.InsertMark(aMarkIndexOfStartOfCompressedUnicodeRun, bufferPositionOfStartOfCompressedUnicodeRun, EMarkType_TwoByteAlignmentPoint);
		++startOfMarksToUpdate;
		assert(startOfMarksToUpdate==aMarkIndexOfStartOfCompressedUnicodeRun+1);
		}

	// replace the compressed-Unicode bytes in the buffer with the uncompressed equivalent
	if (numberOfExtraBytes==0)
		{
		assert(numberOfBytesWhenCompressed==0);
		assert(numberOfBytesWhenUncompressed==0);
		assert(aMarkIndexOfStartOfCompressedUnicodeRun==0);
		}
	else
		{
		assert(numberOfExtraBytes>0);

		// make room in the buffer to insert the uncompressed Unicode (replacing the compressed-Unicode run)
		const int numberOfBytesToMove=iNumberOfBytesUsed-bufferPositionOfEndOfCompressedUnicodeRun; // must be done before MakePlaceHolder is called as MakePlaceHolder will increment iNumberOfBytesUsed by numberOfExtraBytes
		MakePlaceHolder(numberOfExtraBytes);
		unsigned char* basePointer=iBuffer+bufferPositionOfEndOfCompressedUnicodeRun;
		memmove(basePointer+numberOfExtraBytes, basePointer, numberOfBytesToMove); // memmove copes with overlapping source and target areas

		// adjust all the subsequent mark's iBufferPositions
		for (int i=iArrayOfMarks.Size()-1; i>=startOfMarksToUpdate; --i)
			{
			iArrayOfMarks.MarkAt(i).iBufferPosition+=numberOfExtraBytes;
			}
		}

	// copy in the uncompressed Unicode and destroying the old copy
	memcpy(iBuffer+bufferPositionOfStartOfCompressedUnicodeRun, runWhenUncompressed.Buffer(), numberOfBytesWhenUncompressed);
	runWhenUncompressed.Destroy();
	}

ResourceDataStream::BinaryBuffer* ResourceDataStream::BinaryBuffer::New(int aNumberOfBytes, const unsigned char* aBuffer)
	{ // static
		BinaryBuffer *dummy = reinterpret_cast<BinaryBuffer *>(0) ;
		size_t offset = reinterpret_cast<size_t>(&(dummy->iBuffer[aNumberOfBytes])) ;
	BinaryBuffer* const binaryBuffer=(BinaryBuffer*)malloc(offset);
	assert(binaryBuffer!=NULL);
	binaryBuffer->iNumberOfBytes=aNumberOfBytes;
	assert(aNumberOfBytes>=0);
	if (aNumberOfBytes>0)
		{
		assert(aBuffer!=NULL);
		memcpy(binaryBuffer->iBuffer, aBuffer, aNumberOfBytes);
		}
	return binaryBuffer;
	}

void ResourceDataStream::BinaryBuffer::Destroy()
	{
	free(this);
	}

