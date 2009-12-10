// BMCONV.H
//
// Copyright (c) 1997-2001 Symbian Ltd.  All rights reserved.
//

#ifndef __BMCONV_H__
#define __BMCONV_H__

#include <string.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>

// As suggested by Gerardo Lamastra
#pragma pack(1)


const long int KWriteOnceFileStoreUid=268435511;
const long int KCBitwiseBitmapUid=268435520;
const long int KMultiBitmapRomImageUid=268435521;
const long int KMultiBitmapFileImageUid=268435522;
const long int KMultiBitmapFileImageChecksum=1194943545;

enum TBitmapColor
{
	EMonochromeBitmap,
	EColorBitmap
};

enum TBitmapfileCompression
{
	ENoBitmapCompression=0,
	EByteRLECompression,
	ETwelveBitRLECompression,
	ESixteenBitRLECompression,
	ETwentyFourBitRLECompression
};

enum TStoreType
{
	ENoStore,
	EFileStore,
	ERomStore,
	ECompressedRomStore
};

enum Errors
{
	NoCompression=-1,
	NoError=0,
	NoMemory,
	Arg,
	Files,
	SourceFile,
	DestFile,
	CommandFile,
	OutOfRange,
	TooManyArgs,
	UnknownCompression,
	CompressionError,
	DecompressionError,
	Bpp,
	PaletteFile,
	PaletteSupportNotImplemented
};

struct SEpocBitmapHeader
{
	int iBitmapSize;
	int iStructSize;
	int iWidthInPixels;
	int iHeightInPixels;
	int iWidthInTwips;
	int iHeightInTwips;
	int iBitsPerPixel;
	TBitmapColor iColor;
	int iPaletteEntries;	// always 0 in current implementations
	TBitmapfileCompression iCompression;
};

#pragma pack(2)
struct TRgb24bit
{
	unsigned char iBlue;
	unsigned char iGreen;
	unsigned char iRed;
};
#pragma pack()

#pragma pack(2)
struct TBitmapFileHeader
{
	unsigned short bfType;
	unsigned int bfSize;
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	unsigned int bfOffBits;
};
#pragma pack()

struct TBitmapInfoHeader
{
	unsigned int biSize;
	int biWidth;
	int biHeight;
	unsigned short biPlanes;
	unsigned short biBitCount;
	unsigned int biCompression;
	unsigned int biSizeImage;
	int biXPelsPerMeter;
	int biYPelsPerMeter;
	unsigned int biClrUsed;
	unsigned int biClrImportant;
};

#pragma pack(2)
struct TRgbTriple
{
	unsigned char rgbtBlue;
	unsigned char rgbtGreen;
	unsigned char rgbtRed;
};
#pragma pack()

struct TRgbQuad
{
	unsigned char iBlue;
	unsigned char iGreen;
	unsigned char iRed;
	unsigned char iReserved;
};

class TRgb
{
public:
	TRgb();
	TRgb(long unsigned int val);
	TRgb(int aRed,int aGreen,int aBlue);
	TRgb &operator=(const TRgb& aColor);
	int operator==(const TRgb& aColor);
	int Difference(const TRgb& col) const;
	int Gray2() const;
	int Gray4() const;
	int Gray16() const;
	int Gray256() const;
	int Color16() const;
	int Color256() const;
	int Color4K() const;
	int Color64K() const;
	long int Color16M() const;
	static TRgb Gray2(int aGray2);
	static TRgb Gray4(int aGray4);
	static TRgb Gray16(int aGray16);
	static TRgb Gray256(int aGray256);
	static TRgb Color16(int aColor16);
	static TRgb Color256(int aColor256);
	static TRgb Color4K(int aColor64K);
	static TRgb Color64K(int aColor64K);
	static TRgb Color16M(long int aColor16M);
public:
	unsigned char iRed;
	unsigned char iGreen;
	unsigned char iBlue;
	unsigned char iSpare;
};

class Bitmap
{
public:
	int iUid;
	int iDispMode;
	void* iHeap;
	void* iPile;
	int iByteWidth;
	SEpocBitmapHeader iHeader;
	int iChunk;
	int iDataOffset; // offset from "this"
};

// Historical note - Pbm is an abbreviation for "Psion Bitmap"

class BitmapCompiler
{
public:
	BitmapCompiler(char* aSourcefilenames[],int aNumSources);
	~BitmapCompiler();
	int Compile(TStoreType aSt,bool aCompress,char* aDestfilename,char* aHeaderFilename,char* aPaletteFilename);
private:
	int AllocatePbmSourcesArray();
	int LoadSourcefiles();
	int LoadPbmFile(char* aPbmFilename);
	int RomImage(bool aCompress);
	int FileImage(bool aCompress);
	int WriteRomheader();
	int WriteRombitmap(SEpocBitmapHeader* aPbm);
	int WriteFileheader();
	int WriteFilebitmap(SEpocBitmapHeader* aPbm, bool aCompress);
	int WriteHeadStream();
	void Write16(unsigned short v);
	void WriteBuf16(const char *buf, int size);
	void Write32(unsigned int v);
	void WriteBuf32(const char *buf, int size);
	int CreateHeader(char* aHeaderFilename);
	char* UnadornedName(char* aName);	// returns pointer to static buffer
	int LoadPalette(char* aPaletteFilename);
	void CalculateInversePalette();
	int CompressBitmap(SEpocBitmapHeader*& aPbm);
	int CompressByteData(char*& aDest,char* aSrce,int aSize);
	int WriteCompressedByteData(char*& aDest,char* aData,int aLength);
	int WriteCompressedByteValues(char*& aDest,char aValue,int aLength);
	int CompressTwelveBitData(char*& aDest,char* aSrce,int aSizeInBytes);
	int WriteCompressedTwelveBitData(unsigned short*& aDest,unsigned short aData,int aLength);
	int CompressSixteenBitData(char*& aDest,char* aSrce,int aSize);
	int WriteCompressedSixteenBitData(char*& aDest,unsigned short* aData,int aLength);
	int WriteCompressedSixteenBitValues(char*& aDest,unsigned short aValue,int aLength);
	int CompressTwentyFourBitData(char*& aDest,char* aSrce,int aSizeInBytes);
	int WriteCompressedTwentyFourBitData(char*& aDest,char* aData,int aLength);
	int WriteCompressedTwentyFourBitValues(char*& aDest,char aComponent1,char aComponent2,char aComponent3,int aLength);
	int TrueColorPointerCompare(char* aColorPointer,char aComponent1,char aComponent2,char aComponent3);
	int ReadHexString(char aHexBuf[10],char*& aDataPtr,char* aDataPtrLimit);
	int HexToInt(char aHighNibble,char aLowNibble);
	int HexToInt(char aNibble);
	static void CopyTail(void* aDst, void* aSrc, int aFullSize, int aSkipped);
private:
	std::fstream iDestFile;
	char** iSourcefilenames;
	SEpocBitmapHeader** iPbmSources;
	int iNumSources;
	int iDefaultPalette;
	TRgb iPalette[256];
	char iInversePalette[4096];
};

class BitmapLoader
{
public:
	BitmapLoader();
	~BitmapLoader();
	int LoadBitmap(char* aFileName,int aBpp,TBitmapColor aColor,SEpocBitmapHeader*& aPbm);
private:
	int DoLoad(char* aFileName);
	int DoConvert(int aBpp,TBitmapColor aColor,SEpocBitmapHeader*& aPbm);
	TRgb GetBmpPixel(long aXCoord,long aYCoord);
private:
	TBitmapInfoHeader iBmpHeader;
	int iNumBmpColors;
	TRgbQuad* iBmpColors;
	char* iBmpBits;
};

class EpocLoader
{
public:
	EpocLoader();
	~EpocLoader();
	int EpocBitmapCount(char* aFilename,int& aCount,int& isRomFormat);
	int LoadEpocBitmap(char* aFilename,int aIndex);
	int SaveBitmap(char* aFilename);
	int DupBitmap(SEpocBitmapHeader*& aPbm);
	SEpocBitmapHeader Header() const { return iOriginalPbmHeader;}
private:
	int LoadFile(char* aFilename,int aIndex);
	int LoadRom(char* aFilename,int aIndex);
	TRgb GetPixel(int aXCoord,int aYCoord);
	int DoLoadFile(std::fstream& aFile);
	int DoLoadRom(std::fstream& aFile);
	int ExpandByteRLEData(char* aDest,int aDestSize,char* aSrce,int aSrceSize);
	int ExpandTwelveBitRLEData(char* aDest,int aDestSizeInBytes,char* aSrce,int aSrceSizeInBytes);
	int ExpandSixteenBitRLEData(char* aDest,int aDestSizeInBytes,char* aSrce,int aSrceSizeInBytes);
	int ExpandTwentyFourBitRLEData(char* aDest,int aDestSizeInBytes,char* aSrce,int aSrceSizeInBytes);
private:
	SEpocBitmapHeader iOriginalPbmHeader;
	SEpocBitmapHeader iPbmHeader;
	char* iPbmBits;
};

class BitmapUtils
{
public:
	static int ByteWidth(int aPixelWidth,int aBitsPerPixel);
};


unsigned short get_u16(const void *p);
unsigned int get_u32(const void *p);


#endif
