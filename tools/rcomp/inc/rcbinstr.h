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


#ifndef __RCOFSTRM_H__
#define __RCOFSTRM_H__

#if defined(__VC32__) && !defined(__MSVCDOTNET__)
#pragma warning( disable : 4710 )	// function not inlined
#endif

#if defined(__MSVCDOTNET__) || defined(__TOOLS2__)
#include <fstream>
using std::ofstream;
#else //!__MSVCDOTNET__
#include <fstream.h>
#endif //__MSVCDOTNET__

#include "astring.h"
#include "numval.h"
#include "array.h" 

class RCBinaryStream
	{
public:
	RCBinaryStream();
	~RCBinaryStream();
	void OpenForAppend(const String& aFileName);
	int IsOpen();
	RCBinaryStream& operator<<(char aChar);
	RCBinaryStream& operator<<(char* aCharPtr);
	static int SizeOfCompressedInteger(unsigned int aInteger);
	void WriteCompressedInteger(unsigned int aInteger);
	void Write(const unsigned char* aCharPtr, unsigned long aCount);
	unsigned long GetPosition();
	void SetPosition(unsigned long NewPosition);
private:
	ofstream iOs;
	};

class ResourceDataStream // this class knows about resources that contain compressed Unicode - i.e. that each run of alternating compressed-Unicode and other-stuff needs to be preceded by a 1-2 byte run-length
	{
public:
	ResourceDataStream();
	~ResourceDataStream();
	void StartOfBlockWithSizePrefix(DataType aDataTypeOfSizePrefix); // StartOfBlockWithSizePrefix/EndOfBlockWithSizePrefix blocks can be nested
	void EndOfBlockWithSizePrefix();
	void StartOfCompressedUnicodeRun(int aUncompressedUnicodeSizeInBytes, const unsigned char* aUncompressedUnicodeBuffer); // StartOfCompressedUnicodeRun/EndOfCompressedUnicodeRun runs *cannot* be nested
	void EndOfCompressedUnicodeRun();
	void TwoByteAlignmentPoint();
	void EnquireStreamPositionWhenKnown(unsigned long& aStreamPosition);
	void StreamIn(const unsigned char* aBuffer, int aNumberOfBytes);
	void MakePlaceHolder(int aNumberOfBytes);
	bool StreamOutReturningWhetherContainsCompressedUnicode(RCBinaryStream& aStream, int& aSizeWhenUncompressed);
	void Dump(const char* aDumpFile) const;
private:
	enum MarkType
		{
		EMarkType_StartOfBlockWithSizePrefix, // uses iOtherData to store the address of the NumericValue of the size-prefix
		EMarkType_EndOfBlockWithSizePrefix,
		EMarkType_StartOfCompressedUnicodeRun, // uses iOtherData to store a BinaryBuffer which contains the uncompressed Unicode - a EMarkType_StartOfCompressedUnicodeRun mark is always followed in ResourceDataStream's iArrayOfMarks by a EMarkType_EndOfCompressedUnicodeRun
		EMarkType_EndOfCompressedUnicodeRun, // uses iOtherData to store the combined size of the run taking into account any extra bytes caused by other marks, e.g. a padding byte caused by a EMarkType_TwoByteAlignmentPoint
		EMarkType_TwoByteAlignmentPoint, // uses iOtherData to store whether a padding byte is required or not (iOtherData is set to non-zero if it is and zero if it isn't)
		EMarkType_EnquireStreamPositionWhenKnown // uses iOtherData to store the address of the unsigned long to write the stream-position to
		};
	class BinaryBuffer
		{
	public:
		static BinaryBuffer* New(int aNumberOfBytes, const unsigned char* aBuffer);
		void Destroy();
		inline int NumberOfBytes() const {return iNumberOfBytes;}
		inline const unsigned char* Buffer() const {return iBuffer;}
	private:
		BinaryBuffer(); // defined (as private) but not declared to prevent anyone instantiating this class apart from the static New function
	private:
		int iNumberOfBytes;
		unsigned char iBuffer[1];
		};
	class Mark : public ArrayItem
		{
	public:
		inline Mark(int aBufferPosition, MarkType aMarkType, unsigned int aOtherData) :iBufferPosition(aBufferPosition), iMarkType(aMarkType), iOtherData(aOtherData) {}
	public:
		int iBufferPosition;
		MarkType iMarkType;
		unsigned int iOtherData;
		};
	class ArrayOfMarks : public Array
		{
	public:
		inline ArrayOfMarks() {}
		inline void AppendMark(int aBufferPosition, MarkType aMarkType, unsigned int aOtherData=0xbebebebe) {Add(new Mark(aBufferPosition, aMarkType, aOtherData));}
		inline void InsertMark(int aIndex, int aBufferPosition, MarkType aMarkType, unsigned int aOtherData=0xbebebebe) {Add(aIndex, new Mark(aBufferPosition, aMarkType, aOtherData));}
		inline void RemoveMark(int aIndex) {Discard(aIndex);}
		inline Mark& MarkAt(int aIndex) const {return *(Mark*)(*this)[aIndex];}
		};
private:
	void EnsureEnoughSpareBytes(int aNumberOfBytes);
	int NumberOfBytesToNextMark(int aMarkIndex) const;
	void ConvertCompressedRunToUncompressed(int aIndexOfStartOfCompressedUnicodeRun); // aIndexOfStartOfCompressedUnicodeRun is an index into iArrayOfMarks
private:
	unsigned char* iBuffer;
	int iNumberOfBytesAllocated;
	int iNumberOfBytesUsed;
	ArrayOfMarks iArrayOfMarks;
	bool iContainsCompressedUnicode;
	};

#endif
