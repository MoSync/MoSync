// PBMCOMP.CPP
//
// Copyright (c) 1997-2003 Symbian Ltd.  All rights reserved.
//

#include "bmconv.h"
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>

using namespace std;

extern TRgb* color256Palette;
extern char* color256InversePalette;

BitmapCompiler::BitmapCompiler(char* aSourcefilenames[],int aNumSources):
iSourcefilenames(aSourcefilenames),
iPbmSources(NULL),
iNumSources(aNumSources)
{}

BitmapCompiler::~BitmapCompiler()
{
	iDestFile.close();
	if(iPbmSources)
		for(int count=0;count<iNumSources;count++)
			delete iPbmSources[count];
	delete [] iPbmSources;
	iPbmSources = NULL;
}

int BitmapCompiler::Compile(TStoreType aSt,bool aCompress,char* aDestfilename,char* aHeaderFilename,char* aPaletteFilename)
{
	int ret = LoadPalette(aPaletteFilename);
	if (ret)
		return ret;

	iDestFile.open(aDestfilename,ios::out|ios::binary);
	if(iDestFile.is_open()==0)
		return DestFile;
	if (iNumSources < 1)
		return SourceFile;

	if (iNumSources==1 && iSourcefilenames[0][0]=='/' && iSourcefilenames[0][1]=='m')
	{
		// Source is an existing MBM file, presumably because we want to convert
		// a filestore into a ROM image.
		if (aHeaderFilename)
		{
			cout << "Header file generation is not permitted with /m\n";
			aHeaderFilename = NULL;
		}
		iSourcefilenames[0]+=2;
		EpocLoader pl;
		int romFormat;
		ret = pl.EpocBitmapCount(iSourcefilenames[0], iNumSources, romFormat);
		if (!ret)
			ret = LoadPbmFile(iSourcefilenames[0]);
	}
	else
	{
		// Read the usual list of [OPT]bmp_n source bitmaps.
		ret = LoadSourcefiles();
	}
	if (ret)
		return ret;

	ret = CreateHeader(aHeaderFilename);
	if (ret)
		return ret;

	if (aSt == ERomStore || aSt == ECompressedRomStore)
		ret = RomImage(aSt == ECompressedRomStore);
	else
		ret = FileImage(aCompress);
	return ret;
};

int BitmapCompiler::AllocatePbmSourcesArray()
{
	iPbmSources = new SEpocBitmapHeader*[iNumSources];
	if(iPbmSources == NULL)
		return NoMemory;

	memset(iPbmSources,0xff,sizeof(SEpocBitmapHeader*)*iNumSources);
	int count;
	for(count=0;count<iNumSources;count++)
		iPbmSources[count]=NULL;
	return NoError;
}

int BitmapCompiler::LoadSourcefiles()
{
	int ret = AllocatePbmSourcesArray();
	if (ret)
		return ret;
	for(int count=0;count<iNumSources;count++)
	{
		int bpp = 2;
		TBitmapColor color = EMonochromeBitmap;
		BitmapLoader bmpload;
		if(iSourcefilenames[count][0]=='/')
		{
			iSourcefilenames[count]++;
			if(iSourcefilenames[count][0]=='c')
			{
				color = EColorBitmap;
				iSourcefilenames[count]++;
			}
			bpp=iSourcefilenames[count][0]-'0';
			iSourcefilenames[count]++;
			int next=iSourcefilenames[count][0]-'0';
			if(next==2 || next==6 || next==4)
			{
				bpp=bpp*10+next;
				iSourcefilenames[count]++;
			}
			if (color == EColorBitmap && bpp!=4 && bpp!=8 && bpp!=12 && bpp!=16 && bpp!=24 && bpp!=32)
				return Bpp;
			if (color == EMonochromeBitmap && bpp!=1 && bpp!=2 && bpp!=4 && bpp!=8)
				return Bpp;
		}
		ret = bmpload.LoadBitmap(iSourcefilenames[count],bpp,color,iPbmSources[count]);
		if (ret)
			return ret;
	}
	return NoError;
}

int BitmapCompiler::LoadPbmFile(char* aPbmFilename)
{
	int ret = AllocatePbmSourcesArray();
	if (ret)
		return ret;
	for(int count=0;count<iNumSources;count++)
	{
		EpocLoader pl;
		ret = pl.LoadEpocBitmap(aPbmFilename,count);
		if (!ret)
			ret = pl.DupBitmap(iPbmSources[count]);
		if (ret)
			return ret;
	}
	return NoError;
}

int BitmapCompiler::RomImage(bool aCompress)
{
	int count=0;

	if (aCompress)
	{
		for(; count<iNumSources; count++)
		{
			int ret = CompressBitmap(iPbmSources[count]);
			if (ret)
				return ret;
		}
	}

	int ret = WriteRomheader();
	if (ret)
		return ret;

	for(count=0;count<iNumSources;count++)
	{
		ret = WriteRombitmap(iPbmSources[count]);
		if (ret)
			return ret;
	}
	return NoError;
}

int BitmapCompiler::FileImage(bool aCompress)
{
	int count = 0;
	if(aCompress)
	{
		for(;count<iNumSources;count++)
		{
			int ret = CompressBitmap(iPbmSources[count]);
			if(ret)
				return ret;
		}
	}
	int ret = WriteFileheader();
	if (ret)
		return ret;
	for (count=0;count<iNumSources;count++)
	{
		ret = WriteFilebitmap(iPbmSources[count], aCompress);
		if (ret)
			return ret;
	}
	ret = WriteHeadStream();
	return ret;
}

int BitmapCompiler::WriteRomheader()
{
	Write32(KMultiBitmapRomImageUid);
	Write32(iNumSources);
	int byteswritten=8+(iNumSources*4);
	for (int count=0;count<iNumSources;count++)
	{
		Write32(byteswritten);
		byteswritten+=iPbmSources[count]->iBitmapSize-sizeof(SEpocBitmapHeader)+sizeof(Bitmap);
	}
	return NoError;
}

int BitmapCompiler::WriteRombitmap(SEpocBitmapHeader* aPbm)
{
	if (aPbm->iPaletteEntries != 0)
		return PaletteSupportNotImplemented;

	int bitmapsize = aPbm->iBitmapSize-sizeof(SEpocBitmapHeader)+sizeof(Bitmap);

	char* buffer = new char[bitmapsize];
	if (buffer == NULL)
		return NoMemory;
	memset(buffer,0xff,bitmapsize);

	Bitmap* bmp = (Bitmap*)buffer;
	bmp->iUid=KCBitwiseBitmapUid;
	TBitmapColor color = aPbm->iColor;

	switch(aPbm->iBitsPerPixel)
	{
	case 1:
		bmp->iDispMode=1;
		break;
	case 2:
		bmp->iDispMode=2;
		break;
	case 4:
		if (color == EMonochromeBitmap)
			bmp->iDispMode=3;
		else
			bmp->iDispMode=5;
		break;
	case 8:
		if (color == EMonochromeBitmap)
			bmp->iDispMode=4;
		else
			bmp->iDispMode=6;
		break;
	case 12:
		bmp->iDispMode=10;
		break;
	case 16:
		bmp->iDispMode=7;
		break;
	case 24:
		bmp->iDispMode=8;
		break;
	case 32:
		bmp->iDispMode=11;
		break;
	default:
		delete [] buffer;
		return SourceFile;
	}

	bmp->iHeap = NULL;
	bmp->iPile = NULL;
	bmp->iHeader = *aPbm;
	bmp->iByteWidth = BitmapUtils::ByteWidth(bmp->iHeader.iWidthInPixels,bmp->iHeader.iBitsPerPixel);
	bmp->iDataOffset = sizeof(Bitmap);

	CopyTail(buffer + sizeof(Bitmap), aPbm, aPbm->iBitmapSize, sizeof(SEpocBitmapHeader));
	iDestFile.write(buffer,bitmapsize);

	delete [] buffer;

	return NoError;
}

int BitmapCompiler::WriteFileheader()
{
	Write32(KWriteOnceFileStoreUid);
	Write32(KMultiBitmapFileImageUid);
	Write32(0);
	Write32(KMultiBitmapFileImageChecksum);
	int byteswritten=16;
	for(int count=0;count<iNumSources;count++)
	{
		byteswritten+=iPbmSources[count]->iBitmapSize;
	}
	byteswritten+=4;
	Write32(byteswritten);
	return NoError;
}

int BitmapCompiler::WriteHeadStream()
{
	Write32(iNumSources);
	int byteswritten=20;
	for(int count=0;count<iNumSources;count++)
	{
		Write32(byteswritten);
		byteswritten+=iPbmSources[count]->iBitmapSize;
	}
	return NoError;
}

/**
* Write a 16-bit value to the open file in little endian byteorder.
*
* @param v 16-bit value to be written.
*/
void BitmapCompiler::Write16(unsigned short v)
{
	unsigned char b[2];

	b[0] = (unsigned char)((v >>  0) & 0xff);
	b[1] = (unsigned char)((v >>  8) & 0xff);

	iDestFile.write((char*)b, 2);
}


/**
* Write a 16-bit buffer to the open file in little endian byteorder.
*
* @param buf  16-bit buffer to be written.
* @param size Number of bytes in the buffer.
*/
void BitmapCompiler::WriteBuf16(const char *buf, int size)
{
	size /= 2;
	const unsigned short *ptr = (const unsigned short *)buf;
	for(int i=0;i<size;i++) {
		Write16(*ptr++);
	}
}


/**
* Write a 32-bit value to the open file in little endian byteorder.
*
* @param v 32-bit value to be written.
*/
void BitmapCompiler::Write32(unsigned int v)
{
	unsigned char b[4];

	b[0] = (unsigned char)((v >>  0) & 0xff);
	b[1] = (unsigned char)((v >>  8) & 0xff);
	b[2] = (unsigned char)((v >> 16) & 0xff);
	b[3] = (unsigned char)((v >> 24) & 0xff);

	iDestFile.write((char*)b, 4);
}


/**
* Write a 32-bit buffer to the open file in little endian byteorder.
*
* @param buf  32-bit buffer to be written.
* @param size Number of bytes in the buffer.
*/
void BitmapCompiler::WriteBuf32(const char *buf, int size)
{
	size /= 4;
	const unsigned int *ptr = (const unsigned int *)buf;
	for(int i=0;i<size;i++) {
		Write32(*ptr++);
	}
}


int BitmapCompiler::WriteFilebitmap(SEpocBitmapHeader* aPbm, bool aCompress)
{
	if (aPbm->iPaletteEntries != 0)
		return PaletteSupportNotImplemented;

	int dataSize = aPbm->iBitmapSize - sizeof(SEpocBitmapHeader);

	WriteBuf32((char*)(aPbm),sizeof(SEpocBitmapHeader));
	if (aPbm->iBitsPerPixel==12 || (aPbm->iBitsPerPixel==16 && !aCompress))
		WriteBuf16(((char*)(aPbm)) + sizeof(SEpocBitmapHeader),dataSize);
	else
		iDestFile.write(((char*)(aPbm)) + sizeof(SEpocBitmapHeader),dataSize);

	return NoError;
}

int BitmapCompiler::LoadPalette(char* aPaletteFilename)
{
	if (!aPaletteFilename)
	{
		iDefaultPalette = 1;
		return NoError;
	}

	iDefaultPalette = 0;
	color256Palette = iPalette;
	color256InversePalette = iInversePalette;

	struct stat fileinfo;
	if (stat(aPaletteFilename,&fileinfo)==-1)
		return CommandFile;

	int filesize = fileinfo.st_size;
	if (filesize == 0)
		return PaletteFile;

	fstream paletteFile(aPaletteFilename,ios::in|ios::binary);
	if(!paletteFile.is_open())
		return PaletteFile;

	char* paletteData = new char[filesize];
	if (!paletteData)
		return NoMemory;

	memset(paletteData,0,filesize);
	paletteFile.read(paletteData,filesize);
	paletteFile.close();

	char* dataPtr = (char*)paletteData;
	char* dataPtrLimit = dataPtr + filesize;

	for (int index = 0; index < 256; index++)
	{
		char hexBuf[10];
		int ret = ReadHexString(hexBuf,dataPtr,dataPtrLimit);
		if (ret)
		{
			delete paletteData;
			return ret;
		}

		int red = HexToInt(hexBuf[8],hexBuf[9]);
		int green = HexToInt(hexBuf[6],hexBuf[7]);
		int blue = HexToInt(hexBuf[4],hexBuf[5]);

		iPalette[index] = TRgb(red,green,blue);
	}

	delete paletteData;
	CalculateInversePalette();
	return NoError;
}

void BitmapCompiler::CalculateInversePalette()
{
	for (int index = 0; index < 4096; index++)
	{
		TRgb color = TRgb((index & 0x00f) * 17,((index & 0x0f0) >> 4) * 17,((index & 0xf00) >> 8) * 17);

		int nearest = 0;
		int distance = iPalette[0].Difference(color);

		for (int paletteIndex = 1; paletteIndex < 256; paletteIndex++)
		{
			TRgb paletteColor = iPalette[paletteIndex];

			if (paletteColor == color)
			{
				nearest = paletteIndex;
				break;
			}

			int paletteDistance = paletteColor.Difference(color);
			if (paletteDistance < distance)
			{
				nearest = paletteIndex;
				distance = paletteDistance;
			}
		}

		iInversePalette[index] = (char)nearest;
		TRgb palColor = iPalette[nearest];
		color = palColor;
	}
}

int BitmapCompiler::CreateHeader(char* aHeaderFilename)
{
	if (!aHeaderFilename)
		return NoError;

	fstream hf(aHeaderFilename,ios::out);
	if(!hf.is_open())
		return DestFile;

	char* basename = strrchr(aHeaderFilename,'\\');
	if (basename==0)
		basename = aHeaderFilename;
	else
		basename++;	// skip directory separator

	char unadornedFile[256];
	strcpy(unadornedFile, UnadornedName(aHeaderFilename));

	hf << "// " << basename << "\n";
	hf << "// Generated by BitmapCompiler\n";
	hf << "// Copyright (c) 1998-2001 Symbian Ltd.  All rights reserved.\n";
	hf << "//\n\n";
	hf << "enum TMbm" << unadornedFile << "\n";
	hf << "\t{\n";

	for (int count=0;count<iNumSources;count++)
	{
		hf << "\tEMbm" << unadornedFile << UnadornedName(iSourcefilenames[count]);
		if(count<iNumSources-1)
			hf << ',';
		hf << '\n';
	}
	hf << "\t};\n";
	hf.close();
	return NoError;
}

char* BitmapCompiler::UnadornedName(char* aName)
{
	size_t len = strlen(aName);

	char* foundDir = strrchr(aName,'\\');
	char* foundUrl = strrchr(aName,'/');
	char* foundExt = strrchr(aName,'.');

	char* foundPath = (foundDir > foundUrl) ? foundDir : foundUrl;
	char* firstchar = (foundPath > 0) ? foundPath + 1 : aName;
	char* lastchar = (foundExt > firstchar) ? foundExt : aName+len;

	static char result[256];
	if (lastchar-firstchar > 255)
	{
		strcpy(result,"NameTooLong");
		return result;
	}
	int i=0;
	result[0] = (char)toupper(*firstchar);
	firstchar+=1;
	for (i=1; firstchar<lastchar; i++,firstchar++)
	{
		result[i] = (char)tolower(*firstchar);
	}
	result[i] = '\0';
	return result;
}

int BitmapCompiler::CompressBitmap(SEpocBitmapHeader*& aPbm)
{
	int originalsize = aPbm->iBitmapSize-sizeof(SEpocBitmapHeader);
	char* newbits=new char[originalsize+sizeof(SEpocBitmapHeader)];
	if(!newbits)
		return NoMemory;

	memset(newbits,0xff,sizeof(SEpocBitmapHeader)+originalsize);
	memcpy(newbits,aPbm,sizeof(SEpocBitmapHeader));
	char* newbitsptr=newbits+sizeof(SEpocBitmapHeader);
	char* oldbits=((char*)(aPbm))+sizeof(SEpocBitmapHeader);

	TBitmapfileCompression compression = ENoBitmapCompression;
	int ret = NoCompression;

	if (aPbm->iBitsPerPixel <= 8)
	{
		compression = EByteRLECompression;
		ret = CompressByteData(newbitsptr,oldbits,originalsize);
	}
	else if (aPbm->iBitsPerPixel == 12)
	{
		compression = ETwelveBitRLECompression;
		ret = CompressTwelveBitData(newbitsptr,oldbits,originalsize);
	}
	else if (aPbm->iBitsPerPixel == 16)
	{
		compression = ESixteenBitRLECompression;
		ret = CompressSixteenBitData(newbitsptr,oldbits,originalsize);
	}
	else if (aPbm->iBitsPerPixel == 24)
	{
		compression = ETwentyFourBitRLECompression;
		ret = CompressTwentyFourBitData(newbitsptr,oldbits,originalsize);
	}
	else if (aPbm->iBitsPerPixel == 32)
	{
		//TODO compression!?
	}

	if(ret)
	{
		delete [] newbits;
		if(ret>0)
			return ret;
		return NoError;
	}

	delete aPbm;
	aPbm = (SEpocBitmapHeader*)newbits;
	aPbm->iBitmapSize = (int)(newbitsptr-(newbits+sizeof(SEpocBitmapHeader))+sizeof(SEpocBitmapHeader));
	aPbm->iCompression = compression;
	return NoError;
}

int BitmapCompiler::CompressByteData(char*& aDest,char* aSrce,int aSize)
{
	char* bytepointer=aSrce;
	char* limitpointer=bytepointer+aSize;
	int margin = (aSize>>6);
	char* limitmargin = limitpointer - ((margin > 4) ? margin : 4);
	char* cutoffpointer=aDest+(aSize>>1)+(aSize>>2);
	int ret=NoError;
	char* oldbytepointer=NULL;
	while(bytepointer<limitmargin)
	{
		char value=*bytepointer;
		if(*(bytepointer+1)==value && *(bytepointer+2)==value)
		{
			oldbytepointer=bytepointer;
			bytepointer+=3;
			while(*bytepointer==value && bytepointer<limitmargin)
				bytepointer++;
			ret = WriteCompressedByteValues(aDest,value,(int)(bytepointer-oldbytepointer));
			if(ret) return ret;
		}
		else
		{
			oldbytepointer=bytepointer;
			while((bytepointer<limitmargin) && (*(bytepointer+1)!=value || *(bytepointer+2)!=value))
			{
				bytepointer++;
				value=*bytepointer;
			}
			ret = WriteCompressedByteData(aDest,oldbytepointer,(int)(bytepointer-oldbytepointer));
			if(ret) return ret;
		}
		if(aDest>cutoffpointer)
			return NoCompression;
	}

	int remaining = (int)(limitpointer-bytepointer);
	if(remaining > 0)
	{
		if (aDest + remaining > cutoffpointer)
			return NoCompression;
		ret=WriteCompressedByteData(aDest,bytepointer,remaining);
		if(ret) return ret;
	}
	return NoError;
}

int BitmapCompiler::WriteCompressedByteData(char*& aDest,char* aData,int aLength)
{
	while(aLength>128)
	{
		*aDest++=-128;
		for(int count=0;count<128;count++)
			*aDest++=*aData++;
		aLength-=128;
	}
	if(aLength>128 || aLength<1) return CompressionError;
	*aDest++=char(-aLength);
	for(int count=0;count<aLength;count++)
		*aDest++=*aData++;
	return NoError;
}

int BitmapCompiler::WriteCompressedByteValues(char*& aDest,char aValue,int aLength)
{
	if (aLength < 1)
		return CompressionError;

	while (aLength > 128)
	{
		*aDest++ = 127;
		*aDest++ = aValue;
		aLength -= 128;
	}

	*aDest++ = char(aLength-1);
	*aDest++ = aValue;

	return NoError;
}

int BitmapCompiler::CompressTwelveBitData(char*& aDest,char* aSrce,int aSizeInBytes)
{
	unsigned short* srcePtr = (unsigned short*)aSrce;
	unsigned short* srceLimitPtr = srcePtr + (aSizeInBytes / 2);
	unsigned short* dest = (unsigned short*)aDest;

	while (srcePtr < srceLimitPtr)
	{
		unsigned short* runStartPtr = srcePtr;
		unsigned short value = *srcePtr;
		do
		srcePtr++;
		while (srcePtr < srceLimitPtr && *srcePtr == value);
		WriteCompressedTwelveBitData(dest,*runStartPtr,(int)(srcePtr - runStartPtr)); // Ignore error return as 12bpp compression never fails
	}

	aDest = (char*)dest;
	return NoError;
}

int BitmapCompiler::WriteCompressedTwelveBitData(unsigned short*& aDest,unsigned short aData,int aLength)
{
	// The run length l is stored as l-1 in the top 4 bits of each 16-bit word (between 1 and 16)
	aData &= 0x0fff;
	unsigned short maxLengthData = (unsigned short)(aData | 0xf000);

	while(aLength>16)
	{
		*aDest++ = maxLengthData;
		aLength -= 16;
	}

	if (aLength > 0)
		*aDest++ = (unsigned short)(aData | ((aLength - 1) << 12));

	return NoError;
}

int BitmapCompiler::CompressSixteenBitData(char*& aDest,char* aSrce,int aSizeInBytes)
{
	unsigned short* srcePtr = (unsigned short*)aSrce;
	unsigned short* srceLimitPtr = srcePtr + (aSizeInBytes / 2);
	unsigned short* srceLimitPtrMinusOne = srceLimitPtr - 1;
	char* destCompressedLimitPtr = aDest + (aSizeInBytes * 7 / 8);
	int ret = NoError;

	while (srcePtr < srceLimitPtrMinusOne)
	{
		unsigned short value = *srcePtr;
		unsigned short* runStartPtr = srcePtr++;

		if(*srcePtr == value)
		{
			do
			srcePtr++;
			while(*srcePtr == value && srcePtr < srceLimitPtr);

			ret = WriteCompressedSixteenBitValues(aDest,value,(int)(srcePtr-runStartPtr));
			if (ret)
				return ret;
		}
		else
		{
			value = *srcePtr;
			while (srcePtr < srceLimitPtrMinusOne && *(srcePtr + 1) != value)
			{
				srcePtr++;
				value = *srcePtr;
			}

			ret = WriteCompressedSixteenBitData(aDest,runStartPtr,(int)(srcePtr - runStartPtr));
			if (ret)
				return ret;
		}
		if (aDest > destCompressedLimitPtr)
			return NoCompression;
	}

	if (srcePtr < srceLimitPtr)
	{
		ret = WriteCompressedSixteenBitData(aDest,srcePtr,(int)(srceLimitPtr - srcePtr));
		if (ret)
			return ret;
	}

	if (aDest > destCompressedLimitPtr)
		return NoCompression;
	return NoError;
}

int BitmapCompiler::WriteCompressedSixteenBitData(char*& aDest,unsigned short* aData,int aLength)
{
	if (aLength < 1)
		return CompressionError;

	char* srcePtr = (char*)aData;

	while (aLength > 128)
	{
		*aDest++ = -128;
		//		memcpy(aDest,srcePtr,256);
		//		aDest += 256;
		unsigned short *sptr = (unsigned short*) srcePtr;
		for (int i = 0; i < 128; i++)
		{
			char lowByte = char(*sptr);
			char highByte = char(*sptr >> 8);
			sptr++;
			*aDest++ = lowByte;
			*aDest++ = highByte;
		}
		srcePtr += 256;
		aLength -= 128;
	}

	*aDest++ = char(-aLength);

	//	int remainingBytes = aLength * 2;
	//	memcpy(aDest,srcePtr,remainingBytes);
	//	aDest += remainingBytes;
	unsigned short *sptr = (unsigned short*) srcePtr;
	for (int i = 0; i < aLength; i++)
	{
		char lowByte = char(*sptr);
		char highByte = char(*sptr >> 8);
		sptr++;
		*aDest++ = lowByte;
		*aDest++ = highByte;
	}

	return NoError;
}

int BitmapCompiler::WriteCompressedSixteenBitValues(char*& aDest,unsigned short aValue,int aLength)
{
	if (aLength < 1)
		return CompressionError;

	char lowByte = char(aValue);
	char highByte = char(aValue >> 8);

	while (aLength > 128)
	{
		*aDest++ = 127;
		*aDest++ = lowByte;
		*aDest++ = highByte;
		aLength -= 128;
	}

	*aDest++ = char(aLength-1);
	*aDest++ = lowByte;
	*aDest++ = highByte;

	return NoError;
}

int BitmapCompiler::CompressTwentyFourBitData(char*& aDest,char* aSrce,int aSizeInBytes)
{
	char* srceLimitPtr = aSrce + aSizeInBytes;
	char* srceLimitPtrMinusThree = srceLimitPtr - 3; // three bytes == one pixel
	char* destCompressedLimitPtr = aDest + (aSizeInBytes * 7 / 8);
	int ret = NoError;

	while (aSrce < srceLimitPtrMinusThree)
	{
		char* runStartPtr = aSrce;
		char component1 = *aSrce++;
		char component2 = *aSrce++;
		char component3 = *aSrce++;

		if (TrueColorPointerCompare(aSrce,component1,component2,component3))
		{
			do
			aSrce += 3;
			while (aSrce < srceLimitPtr && TrueColorPointerCompare(aSrce,component1,component2,component3));

			ret = WriteCompressedTwentyFourBitValues(aDest,component1,component2,component3,(int)((aSrce - runStartPtr)) / 3);
			if (ret)
				return ret;
		}
		else
		{
			component1 = *aSrce++;
			component2 = *aSrce++;
			component3 = *aSrce++;
			while (aSrce < srceLimitPtrMinusThree && !TrueColorPointerCompare(aSrce + 3,component1,component2,component3))
			{
				aSrce += 3;
				component1 = *aSrce++;
				component2 = *aSrce++;
				component3 = *aSrce;
				aSrce -= 2;
			}

			ret = WriteCompressedTwentyFourBitData(aDest,runStartPtr,(int)((aSrce - runStartPtr)) / 3);
			if (ret)
				return ret;
		}
		if (aDest > destCompressedLimitPtr)
			return NoCompression;
	}

	if (aSrce < srceLimitPtr)
	{
		ret = WriteCompressedTwentyFourBitData(aDest,aSrce,(int)(srceLimitPtr - aSrce) / 3);
		if (ret)
			return ret;
	}

	if (aDest > destCompressedLimitPtr)
		return NoCompression;
	return NoError;
}

int BitmapCompiler::WriteCompressedTwentyFourBitData(char*& aDest,char* aData,int aLength)
{
	if (aLength < 1)
		return CompressionError;

	while (aLength > 128)
	{
		*aDest++ = -128;
		memcpy(aDest,aData,384);
		aDest += 384;
		aData += 384;
		aLength -= 128;
	}

	*aDest++ = char(-aLength);

	int remainingBytes = aLength * 3;
	memcpy(aDest,aData,remainingBytes);
	aDest += remainingBytes;

	return NoError;
}

int BitmapCompiler::WriteCompressedTwentyFourBitValues(char*& aDest,char aComponent1,char aComponent2,char aComponent3,int aLength)
{
	if (aLength < 1)
		return CompressionError;

	while (aLength > 128)
	{
		*aDest++ = 127;
		*aDest++ = aComponent1;
		*aDest++ = aComponent2;
		*aDest++ = aComponent3;
		aLength -= 128;
	}

	*aDest++ = char(aLength-1);
	*aDest++ = aComponent1;
	*aDest++ = aComponent2;
	*aDest++ = aComponent3;

	return NoError;
}

int BitmapCompiler::TrueColorPointerCompare(char* aColorPointer,char aComponent1,char aComponent2,char aComponent3)
{
	return (*aColorPointer == aComponent1 && *(aColorPointer + 1) == aComponent2 && *(aColorPointer + 2) == aComponent3);
}

int BitmapCompiler::ReadHexString(char aHexBuf[10],char*& aDataPtr,char* aDataPtrLimit)
{
	while (aDataPtr < aDataPtrLimit)
	{
		if (aDataPtr[0] == '0')
		{
			if (aDataPtr[1] == 'x' || aDataPtr[1] == 'X')
			{
				memcpy(aHexBuf,aDataPtr,10);
				aDataPtr += 10;
				return NoError;
			}
		}
		aDataPtr++;
	}

	return PaletteFile;
}

int BitmapCompiler::HexToInt(char aHighNibble,char aLowNibble)
{
	return (HexToInt(aHighNibble) << 4) + HexToInt(aLowNibble);
}

int BitmapCompiler::HexToInt(char aNibble)
{
	int value = 0;

	if (aNibble >= '0' && aNibble <= '9')
		value = aNibble - '0';
	else if (aNibble >= 'a' && aNibble <= 'f')
		value = aNibble - 'a' + 10;
	else if (aNibble >= 'A' && aNibble <= 'F')
		value = aNibble - 'A' + 10;

	return value;
}

void BitmapCompiler::CopyTail(void* aDst, void* aSrc, int aFullSize, int aSkipped)
{
	memcpy(aDst, ((char*)aSrc)+aSkipped, aFullSize-aSkipped);
}
