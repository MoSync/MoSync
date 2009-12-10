// RGB.CPP
//
// Copyright (c) 1997-2000 Symbian Ltd.  All rights reserved.
//

#include "bmconv.h"
#include "rgb.h"

extern TRgb* color256Palette;
extern char* color256InversePalette;

TRgb* color256Palette = NULL;
char* color256InversePalette = NULL;

TRgb::TRgb()
: iRed(255),iGreen(255),iBlue(255),iSpare(0)
{}

TRgb::TRgb(long unsigned int val)
: iRed((unsigned char)(val&0xff)),iGreen((unsigned char)((val>>8)&0xff)),iBlue((unsigned char)((val>>16)&0xff)),iSpare(0)
{}

TRgb::TRgb(int r,int g,int b)
: iRed((unsigned char)r),iGreen((unsigned char)g),iBlue((unsigned char)b),iSpare(0)
{}

TRgb &TRgb::operator=(const TRgb &col)
{
	iRed=col.iRed;
	iGreen=col.iGreen;
	iBlue=col.iBlue;
	return(*this);
}

int TRgb::operator==(const TRgb &col)
{
	return(iRed==col.iRed && iGreen==col.iGreen && iBlue==col.iBlue);
}

int TRgb::Difference(const TRgb& col) const
{
	return abs(iRed-col.iRed) + abs(iGreen-col.iGreen) + abs(iBlue-col.iBlue);
}

int TRgb::Gray2() const
{
	return Gray256() / 128;
}

int TRgb::Gray4() const
{
	return Gray256() / 64;
}

int TRgb::Gray16() const
{
	return Gray256() / 16;
}

int TRgb::Gray256() const
{
	return((2*iRed+5*iGreen+iBlue)/8);
}

int TRgb::Color16() const
{
	int index = (iRed >> 5) & 0x007;
	index |= (iGreen  >> 2) & 0x038;
	index |= (iBlue << 1) & 0x1c0;
	return color16inverse[index];
}

int TRgb::Color256() const
{
	int index = (iRed >> 4) & 0x00f;
	index |= iGreen & 0x0f0;
	index |= (iBlue << 4) & 0xf00;

	if (color256InversePalette)
		return color256InversePalette[index];
	else
		return color256inverse[index];
}

int TRgb::Color4K() const
{
	return(((iRed&0xf0)<<4)|(iGreen&0xf0)|((iBlue&0xf0)>>4));
}

int TRgb::Color64K() const
{
	return(((iRed&0xf8)<<8)|((iGreen&0xfc)<<3)|((iBlue&0xf8)>>3));
}

long int TRgb::Color16M() const
{
	return((iRed<<16)|(iGreen<<8)|iBlue);
}

TRgb TRgb::Gray2(int aGray2)
{
	aGray2 *= 255;
	return TRgb(aGray2,aGray2,aGray2);
}

TRgb TRgb::Gray4(int aGray4)
{
	aGray4 *= 85;
	return TRgb(aGray4,aGray4,aGray4);
}

TRgb TRgb::Gray16(int aGray16)
{
	aGray16 *= 17;
	return TRgb(aGray16,aGray16,aGray16);
}

TRgb TRgb::Gray256(int aGray256)
{
	return TRgb(aGray256,aGray256,aGray256);
}

TRgb TRgb::Color16(int aColor16)
{
	return TRgb(color16array[aColor16&0xf]);
}

TRgb TRgb::Color256(int aColor256)
{
	if (color256Palette)
		return color256Palette[aColor256&0xff];
	else
		return TRgb(color256array[aColor256&0xff]);
}

TRgb TRgb::Color4K(int aColor4K)
{
	return TRgb(((aColor4K>>8)&0xf)*17,((aColor4K>>4)&0xf)*17,(aColor4K&0xf)*17);
}

TRgb TRgb::Color64K(int aColor64K)
{
	return TRgb(((aColor64K>>11)&0x1f)*255/31,((aColor64K>>5)&0x3f)*255/63,(aColor64K&0x1f)*255/31);
}

TRgb TRgb::Color16M(long int aColor16M)
{
	return TRgb(((aColor16M>>16)&0xff),(aColor16M>>8)&0xff,aColor16M&0xff);
}

