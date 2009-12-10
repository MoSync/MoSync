// UNICODE_COMPRESSOR.H
//
// Copyright (c) 2001 Symbian Ltd. All rights reserved.
//

// the API to the Unicode compressor

extern void CompressUnicode(unsigned char* aOutputBuffer, int& aOutputLength, int aMaximumOutputLength, const unsigned short* aInputBuffer, int aInputLength);

// the Symbian things that the Symbian Unicode-compression classes use

#define IMPORT_C
#define EXPORT_C
#ifndef NULL
#define NULL 0
#endif
#define TRUE true
#define FALSE false
typedef unsigned char TUint8;
typedef unsigned short TUint16;
typedef unsigned long TUint32;
typedef unsigned int TUint;
typedef int TInt;
typedef bool TBool;
const TInt KMaxTInt = 0x7fffffff;
const TInt KErrNotFound = -1;
class RWriteStream;
extern void Panic(int aCode);

// the rest of the contents of this file is a selective copy of base\store\inc\S32UCMP.H

class TUnicodeCompressionState
{
public:
	TUnicodeCompressionState();
	void Reset();
	static TInt StaticWindowIndex(TUint16 aCode);
	static TInt DynamicWindowOffsetIndex(TUint16 aCode);
	static TUint32 DynamicWindowBase(TInt aOffsetIndex);
	static TBool EncodeAsIs(TUint16 aCode);

	enum TPanic
	{
		EUnhandledByte,			// expander code fails to handle all possible byte codes
		ENotUnicode,			// expander can't handle Unicode values outside range 0x0..0x10FFFF;
		// that is, 16-bit codes plus 32-bit codes that can be expressed using
		// 16-bit surrogates
		EOutputBufferOverflow	// output buffer is not big enough
		,ECannotUseStreams // not in the file from which this file is derived (i.e. not in base\store\inc\S32UCMP.H)
	};

	static void Panic(TPanic aPanic);

protected:

	enum
	{
		EStaticWindows = 8,
		EDynamicWindows = 8,
		ESpecialBases = 7
	};

	TBool iUnicodeMode;									// TRUE if in Unicode mode as opposed to single-byte mode
	TUint32 iActiveWindowBase;							// base of the active window - bases are 32-bit because they
	// can be set to the surrogate area, which represents codes
	// from 0x00010000 to 0x0010FFFF - planes 1-16 of ISO-10646.
	static const TUint32 iStaticWindow[EStaticWindows];	// bases of the static windows
	static const TUint32 iDynamicWindowDefault[EDynamicWindows];	// default bases of the dynamic windows
	static const TUint16 iSpecialBase[ESpecialBases];	// bases for window offsets F9..FF

	TUint32 iDynamicWindow[EDynamicWindows];			// bases of the dynamic windows
	TInt iUnicodeWords;									// Unicode words processed; read by compressor, written by expander
	TInt iMaxUnicodeWords;								// maximum number of Unicode words to read or write
	TInt iCompressedBytes;								// compressed bytes processed: read by expander, written by compressor
	TInt iMaxCompressedBytes;							// maximum number of compressed bytes to read or write
};

class MUnicodeSource
{
public:
	virtual TUint16 ReadUnicodeValueL() = 0;
};

// A class to read Unicode values directly from memory.
class TMemoryUnicodeSource: public MUnicodeSource
{
public:
	inline TMemoryUnicodeSource(const TUint16* aPtr);
	inline TUint16 ReadUnicodeValueL();

private:
	const TUint16* iPtr;
};

/**
A class to hold functions to compress text using the Standard Compression Scheme for Unicode.

A note on error handling and leaving.

Although all the public functions except the constructor can leave, it is possible to guarantee success: that is,
guarantee that a call will not leave, and that compression will be completed. To do this, (i) supply a MUnicodeSource
object with a non-leaving ReadUnicodeValueL function, such as a TMemoryUnicodeSource; (ii) write output to a
RWriteStream with a non-leaving WriteL function, or to a buffer that you already know to be big enough, which can be
found out using CompressedSizeL.

This guarantee of success is particularly useful when compressing from one memory buffer to another.
*/
class TUnicodeCompressor: public TUnicodeCompressionState
{
public:
	IMPORT_C TUnicodeCompressor();
	IMPORT_C void CompressL(TUint8* aOutput,MUnicodeSource& aInput,
		TInt aMaxOutputBytes = KMaxTInt,TInt aMaxInputWords = KMaxTInt,
		TInt* aOutputBytes = NULL,TInt* aInputWords = NULL);
	IMPORT_C TInt FlushL(TUint8* aOutput,TInt aMaxOutputBytes,TInt& aOutputBytes);
	IMPORT_C static TInt CompressedSizeL(MUnicodeSource& aInput,TInt aInputWords);

private:
	// A structure to store a character and its treatment code
	struct TAction
	{
		// Treatment codes: static and dynamic window numbers, plain ASCII or plain Unicode
		enum
		{
			EPlainUnicode = -2,	// character cannot be expressed as ASCII or using static or dynamic windows
			EPlainASCII = -1,	// character can be emitted as an ASCII code
			EFirstDynamic = 0,	// values 0..255 are for dynamic windows with offsets at these places in the offset table
			ELastDynamic = 255,
			EFirstStatic = 256,	// values 256..263 are for static windows 0..7
			ELastStatic = 263
		};

		inline TAction();
		TAction(TUint16 aCode);

		TUint16 iCode;		// Unicode value of the character
		TInt iTreatment;	// treatment code: see above
	};

	void DoCompressL(RWriteStream* aOutputStream,TUint8* aOutputPointer,MUnicodeSource* aInput,
		TInt aMaxCompressedBytes,TInt aMaxUnicodeWords,
		TInt* aCompressedBytes,TInt* aUnicodeWords);
	void FlushInputBufferL();
	void FlushOutputBufferL();
	void WriteRunL();
	void WriteCharacter(const TAction& aAction);
	void WriteSCharacter(const TAction& aAction);
	void WriteUCharacter(TUint16 aCode);
	void WriteByte(TUint aByte);
	void WriteCharacterFromBuffer();
	void SelectTreatment(TInt aTreatment);

	enum
	{
		EMaxInputBufferSize = 4,
		EMaxOutputBufferSize = EMaxInputBufferSize * 3	// no Unicode character can be encoded as more than three bytes
	};
	TAction iInputBuffer[EMaxInputBufferSize];			// circular buffer; queue of Unicode characters to be processed
	TInt iInputBufferStart;								// position of first Unicode character to be processed
	TInt iInputBufferSize;								// characters in the input buffer
	TUint8 iOutputBuffer[EMaxOutputBufferSize];			// circular buffer; queue of compressed bytes to be output
	TInt iOutputBufferStart;							// position of first compressed byte to be output
	TInt iOutputBufferSize;								// characters in the output buffer
	TInt iDynamicWindowIndex;							// index of the current dynamic window
	RWriteStream* iOutputStream;						// if non-null, output is to this stream
	TUint8* iOutputPointer;								// if non-null, output is to memory
	MUnicodeSource* iInput;								// input object
};

// inline functions start here

inline TMemoryUnicodeSource::TMemoryUnicodeSource(const TUint16* aPtr):
iPtr(aPtr)
{
}

inline TUint16 TMemoryUnicodeSource::ReadUnicodeValueL()
{
	return *iPtr++;
}

inline TUnicodeCompressor::TAction::TAction():
iCode(0),
iTreatment(EPlainUnicode)
{
}

