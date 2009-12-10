// BMTOPBM.CPP
//
// Copyright (c) 1997-2003 Symbian Ltd.  All rights reserved.
//

#include "bmconv.h"

using namespace std;


BitmapLoader::BitmapLoader():
iNumBmpColors(0),
iBmpColors(NULL),
iBmpBits(NULL)
{}

BitmapLoader::~BitmapLoader()
{
	delete [] iBmpColors;
	delete [] iBmpBits;
}

int BitmapLoader::LoadBitmap(char* aFileName,int aBpp,TBitmapColor aColor,SEpocBitmapHeader*& aPbm)
{
	char sig[2];
	fstream file(aFileName,ios::in|ios::binary);
	if (file.is_open()==0)
		return Files;
	file.read(sig,2);
	file.close();
	if (file.gcount()!=2)
		return SourceFile ;
	if (sig[0]!='B'||sig[1]!='M')
		return SourceFile;

	int ret = DoLoad(aFileName);
	if (!ret)
		ret = DoConvert(aBpp,aColor,aPbm);
	return ret;
}

int BitmapLoader::DoLoad(char* aFileName)
{
	fstream file(aFileName,ios::in|ios::binary);
	if (file.is_open()==0)
		return Files;
	TBitmapFileHeader bmpfile;
	unsigned char hdr[sizeof(TBitmapFileHeader)];
	unsigned long size=sizeof(TBitmapFileHeader);

	/* Convert from little-endian (file) to machine byteorder */
	file.read((char *)hdr,size);

	bmpfile.bfSize = get_u32(&hdr[2]);
	if ((unsigned long)file.gcount()!=size)
		return SourceFile;
	size=sizeof(TBitmapInfoHeader);
	unsigned char info[sizeof(TBitmapInfoHeader)];
	file.read((char *)&info,size);

	/* Convert from little-endian (file) to machine byteorder */
	iBmpHeader.biSize = get_u32(&info[0]); 
	iBmpHeader.biWidth = get_u32(&info[4]); 
	iBmpHeader.biHeight = get_u32(&info[8]); 
	iBmpHeader.biPlanes = get_u16(&info[12]); 
	iBmpHeader.biBitCount = get_u16(&info[14]); 
	iBmpHeader.biCompression = get_u32(&info[16]); 
	iBmpHeader.biSizeImage = get_u32(&info[20]); 
	iBmpHeader.biXPelsPerMeter = get_u32(&info[24]); 
	iBmpHeader.biYPelsPerMeter = get_u32(&info[28]); 
	iBmpHeader.biClrUsed = get_u32(&info[32]); 
	iBmpHeader.biClrImportant = get_u32(&info[36]); 

	if ((unsigned long)file.gcount()!=size)
		return SourceFile;
	if (iBmpHeader.biCompression != 0)
		return UnknownCompression;
	size=bmpfile.bfSize-sizeof(TBitmapInfoHeader)-sizeof(TBitmapFileHeader);
	long bitcount=iBmpHeader.biBitCount;
	long colors=iBmpHeader.biClrUsed;
	if (colors==0)
	{
		if (bitcount==24)
			iNumBmpColors=0;
		else
			iNumBmpColors=1<<bitcount;
	}
	else
		iNumBmpColors=colors;
	if (iNumBmpColors > 256)
		return SourceFile;
	if (iNumBmpColors>0)
	{
		iBmpColors = new TRgbQuad[iNumBmpColors];
		if (iBmpColors == NULL)
			return NoMemory;
		memset(iBmpColors,0,iNumBmpColors*sizeof(TRgbQuad));
	}
	size-=iNumBmpColors*sizeof(TRgbQuad);
	iBmpBits = new char[size];
	if (iBmpBits == NULL)
		return NoMemory;
	memset(iBmpBits,0xff,size);
	file.read((char *)iBmpColors,iNumBmpColors*sizeof(TRgbQuad));
	if ((unsigned long)file.gcount()!=iNumBmpColors*sizeof(TRgbQuad))
		return SourceFile;
	file.read(iBmpBits,size);
	file.close();
	if ((unsigned long)file.gcount()!=size)
		return SourceFile;
	return NoError;
}

TRgb BitmapLoader::GetBmpPixel(long aXCoord,long aYCoord)
{
	TRgb darkgray(128,128,128);
	TRgb darkgrayex(127,127,127);
	TRgb lightgray(192,192,192);
	TRgb lightgrayex(187,187,187);
	unsigned char col;
	TRgb color;

	switch(iBmpHeader.biBitCount)
	{
	case 1:
		col=iBmpBits[(iBmpHeader.biHeight-aYCoord-1)*(((iBmpHeader.biWidth+31)>>5)<<2)+(aXCoord>>3)];
		col&=(0x80>>(aXCoord%8));
		if (iBmpColors)
		{
			TRgbQuad rgbq;
			if (col)
				rgbq = iBmpColors[1];
			else
				rgbq = iBmpColors[0];
			color = TRgb(rgbq.iRed,rgbq.iGreen,rgbq.iBlue);
		}
		else
		{
			if (col)
				color = TRgb(0x00ffffff);
			else
				color = TRgb(0);
		}
		break;
	case 4:
		col=iBmpBits[(iBmpHeader.biHeight-aYCoord-1)*(((iBmpHeader.biWidth+7)>>3)<<2)+(aXCoord>>1)];
		if (aXCoord%2==0)
			col=(unsigned char)(col>>4);
		col&=0x0f;
		if (iBmpColors)
		{
			TRgbQuad rgbq = iBmpColors[col];
			color = TRgb(rgbq.iRed,rgbq.iGreen,rgbq.iBlue);
		}
		else
		{
			col *= 17;
			color = TRgb(col,col,col);
		}
		break;
	case 8:
		col=iBmpBits[(iBmpHeader.biHeight-aYCoord-1)*((iBmpHeader.biWidth+3)&~3)+aXCoord];
		if (iBmpColors)
		{
			TRgbQuad rgbq = iBmpColors[col];
			color = TRgb(rgbq.iRed,rgbq.iGreen,rgbq.iBlue);
		}
		else
			color = TRgb(col,col,col);
		break;
	case 16:
		{
			unsigned short int* wordptr = (unsigned short int*)&iBmpBits[(iBmpHeader.biHeight-aYCoord-1)*(((iBmpHeader.biWidth+1)&~1)<<1)+(aXCoord<<1)];
			color = TRgb((*wordptr&0x7c)>>10,(*wordptr&0x3e)>>5,(*wordptr&0x1f));
		}
		break;
	case 24:
		{
			TRgbTriple rgbcol = *((TRgbTriple *)&(iBmpBits[(iBmpHeader.biHeight-aYCoord-1)*((3*iBmpHeader.biWidth+3)&~3)+aXCoord+(aXCoord<<1)]));
			color = TRgb(rgbcol.rgbtRed,rgbcol.rgbtGreen,rgbcol.rgbtBlue);
		}
		break;
	case 32:
		{
			unsigned long int* dwordptr = (unsigned long int*)&iBmpBits[(iBmpHeader.biHeight-aYCoord-1)*((iBmpHeader.biWidth)<<2)+(aXCoord<<2)];
			color = TRgb((*dwordptr&0xff0000)>>16,(*dwordptr&0xff00)>>8,*dwordptr&0xff);
		}
		break;
	default:
		break;
	}
	if (color == darkgray)
		color = darkgrayex;
	else if (color == lightgray)
		color = lightgrayex;
	return color;
}

int BitmapLoader::DoConvert(int aBpp,TBitmapColor aColor,SEpocBitmapHeader*& aPbm)
{
	long desttwipswidth = 0;
	long desttwipsheight = 0;

	long bytewidth = BitmapUtils::ByteWidth(iBmpHeader.biWidth,aBpp);
	long destlength = iBmpHeader.biHeight * bytewidth;

	if (iBmpHeader.biXPelsPerMeter>0)
		desttwipswidth = iBmpHeader.biWidth*1440000/254/iBmpHeader.biXPelsPerMeter;
	if (iBmpHeader.biYPelsPerMeter>0)
		desttwipsheight = iBmpHeader.biHeight*1440000/254/iBmpHeader.biYPelsPerMeter;

	aPbm = (SEpocBitmapHeader*)new char[sizeof(SEpocBitmapHeader) + destlength];
	if (aPbm == NULL)
		return NoMemory;
	memset(aPbm,0,sizeof(SEpocBitmapHeader));

	// aBitmap->iByteWidth = bytewidth;
	// aBitmap->iDataOffset = sizeof(Bitmap);

	aPbm->iBitmapSize = sizeof(SEpocBitmapHeader) + destlength;
	aPbm->iStructSize = sizeof(SEpocBitmapHeader);
	aPbm->iWidthInPixels = iBmpHeader.biWidth;
	aPbm->iHeightInPixels = iBmpHeader.biHeight;
	aPbm->iWidthInTwips = desttwipswidth;
	aPbm->iHeightInTwips = desttwipsheight;
	aPbm->iBitsPerPixel = aBpp;
	aPbm->iColor = aColor;
	aPbm->iPaletteEntries = 0;
	aPbm->iCompression = ENoBitmapCompression;

	char* pbmBits = ((char*)aPbm) + sizeof(SEpocBitmapHeader);
	memset(pbmBits,0xff,destlength);

	long col = 0;
	char* pixadd = 0;

	switch(aBpp)
	{
	case 1:
		{
			for(long ycrd=0;ycrd<iBmpHeader.biHeight;ycrd++)
				for(long xcrd=0;xcrd<iBmpHeader.biWidth;xcrd++)
				{
					TRgb color=GetBmpPixel(xcrd,ycrd);
					col=color.Gray2();
					pixadd=&(pbmBits[ycrd*bytewidth+(xcrd>>3)]);
					(*pixadd)&=~(1<<((xcrd&7)));
					(*pixadd)|=(unsigned char)(col<<(xcrd&7));
				}
		}
		break;
	case 2:
		{
			for(long ycrd=0;ycrd<iBmpHeader.biHeight;ycrd++)
				for(long xcrd=0;xcrd<iBmpHeader.biWidth;xcrd++)
				{
					TRgb color=GetBmpPixel(xcrd,ycrd);
					col=color.Gray4();
					pixadd=&(pbmBits[ycrd*bytewidth+(xcrd>>2)]);
					(*pixadd)&=~(0x3<<(2*(xcrd%4)));
					(*pixadd)|=(unsigned char)(col<<(2*(xcrd%4)));
				}
		}
		break;
	case 4:
		{
			for(long ycrd=0;ycrd<iBmpHeader.biHeight;ycrd++)
				for(long xcrd=0;xcrd<iBmpHeader.biWidth;xcrd++)
				{
					TRgb color=GetBmpPixel(xcrd,ycrd);
					if (aColor == EMonochromeBitmap)
						col = color.Gray16();
					else
						col = color.Color16();
					pixadd=&(pbmBits[ycrd*bytewidth+(xcrd>>1)]);
					if (xcrd%2!=0)
						*pixadd=(unsigned char)((unsigned char)((col<<4)|(*pixadd&0x0f)));
					else
						*pixadd=(unsigned char)((unsigned char)(col|(*pixadd&0xf0)));
				}
		}
		break;
	case 8:
		{
			for(long ycrd=0;ycrd<iBmpHeader.biHeight;ycrd++)
				for(long xcrd=0;xcrd<iBmpHeader.biWidth;xcrd++)
				{
					TRgb color=GetBmpPixel(xcrd,ycrd);
					if (aColor == EMonochromeBitmap)
						col = color.Gray256();
					else
						col = color.Color256();
					pixadd=&(pbmBits[ycrd*((iBmpHeader.biWidth+3)&~3)+xcrd]);
					*pixadd=(unsigned char)col;
				}
		}
		break;
	case 12:
		{
			for(long ycrd=0;ycrd<iBmpHeader.biHeight;ycrd++)
				for(long xcrd=0;xcrd<iBmpHeader.biWidth;xcrd++)
				{
					TRgb color=GetBmpPixel(xcrd,ycrd);
					col=color.Color4K();
					pixadd=&(pbmBits[ycrd*bytewidth+(xcrd<<1)]);
					unsigned short* wordadd=(unsigned short*)pixadd;
					*wordadd=(unsigned short)col;
				}
		}
		break;
	case 16:
		{
			for(long ycrd=0;ycrd<iBmpHeader.biHeight;ycrd++)
				for(long xcrd=0;xcrd<iBmpHeader.biWidth;xcrd++)
				{
					TRgb color=GetBmpPixel(xcrd,ycrd);
					col=color.Color64K();
					pixadd=&(pbmBits[ycrd*bytewidth+(xcrd<<1)]);
					unsigned short* wordadd=(unsigned short*)pixadd;
					*wordadd=(unsigned short)col;
				}
		}
		break;
	case 24:
		{
			for(long ycrd=0;ycrd<iBmpHeader.biHeight;ycrd++)
			{
				unsigned char* bytePtr = (unsigned char*)&pbmBits[ycrd*bytewidth];
				for(long xcrd=0;xcrd<iBmpHeader.biWidth;xcrd++)
				{
					TRgb col = GetBmpPixel(xcrd,ycrd);
					*bytePtr++ = (unsigned char)(col.iBlue);
					*bytePtr++ = (unsigned char)(col.iGreen);
					*bytePtr++ = (unsigned char)(col.iRed);
				}
			}
		}
		break;
	case 32:
		{
			for(long ycrd=0;ycrd<iBmpHeader.biHeight;ycrd++)
			{
				unsigned char* bytePtr = (unsigned char*)&pbmBits[ycrd*bytewidth];
				for(long xcrd=0;xcrd<iBmpHeader.biWidth;xcrd++)
				{
					TRgb col = GetBmpPixel(xcrd,ycrd);
					*bytePtr++ = (unsigned char)(col.iBlue);
					*bytePtr++ = (unsigned char)(col.iGreen);
					*bytePtr++ = (unsigned char)(col.iRed);
					*bytePtr++ = 0;	
				}
			}
		}
		break;
	};

	return NoError;
}

