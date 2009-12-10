// UTILS.CPP
//
// Copyright (c) 1997-2003 Symbian Ltd.  All rights reserved.
//

#include "bmconv.h"

int BitmapUtils::ByteWidth(int aPixelWidth,int aBitsPerPixel)
{
	int wordWidth = 0;

	switch (aBitsPerPixel)
	{
	case 1:
		wordWidth = (aPixelWidth + 31) / 32;
		break;
	case 2:
		wordWidth = (aPixelWidth + 15) / 16;
		break;
	case 4:
		wordWidth = (aPixelWidth + 7) / 8;
		break;
	case 8:
		wordWidth = (aPixelWidth + 3) / 4;
		break;
	case 12:
	case 16:
		wordWidth = (aPixelWidth + 1) / 2;
		break;
	case 24:
		wordWidth = (((aPixelWidth * 3) + 11) / 12) * 3;
		break;
	case 32:
		wordWidth = aPixelWidth;
		break;
	default:
		break;
	};

	return wordWidth * 4;
}


/**
* Get a 16-bit value in little endian format and convert to machine byteorder.
*
* @param p Pointer to 16-bit little endian data.
*
* @return The corresponding value in machine byteorder.
*/
unsigned short get_u16(const void *p)
{
	const unsigned char *b = (const unsigned char *)p;
	const unsigned short ret = (b[0] | ((unsigned short)b[1] << 8));
	return ret;
}


/**
* Get a 32-bit value in little endian format and convert to machine byteorder.
*
* @param p Pointer to 32-bit little endian data.
*
* @return The corresponding value in machine byteorder.
*/
unsigned int get_u32(const void *p)
{
	const unsigned char *b = (const unsigned char *)p;
	const unsigned int ret = (b[0] | ((unsigned int)b[1] << 8) | ((unsigned int)b[2] << 16) | ((unsigned int)b[3] << 24));
	return ret;
}
