/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

#include <ma.h>

#include "Font.h"
#include <MAUtil/Graphics.h>
#include <MAUtil/PlaceholderPool.h>

#include <conprint.h>

namespace MAUI {
	static MAHandle sFile;
	static unsigned char *sData;
	static int sFilePos;
	static unsigned int sFileSize;
	static unsigned int sBufPos;
	static unsigned char readByte();
	static unsigned short readShort();
	static unsigned int readInt();
	static void skipBytes(int b);
	//static void setFilePtr(int fptr);
	static int readChars(char *output, int num);
	static void readMoreData();
	static void closeFontReader();
	static void initFontReader(MAHandle file);
	static int parseBitmapFontGeneratorFile(MAHandle file, int size, Charset *mCharset);

#define BUFFER_SIZE 1024

	void initFontReader(MAHandle file) {
		sData = new unsigned char[BUFFER_SIZE];
		if(!sData) maPanic(0, "Not enough memory for initialization of MAUI::Font");
		sFile = file;
		sFileSize = maGetDataSize(sFile);
		sBufPos = sFilePos = 0;
		readMoreData();
	}

	void closeFontReader() {
		delete []sData;
	}

	void readMoreData() {
		sBufPos = 0;
		unsigned int readSize = BUFFER_SIZE;
		if(sFilePos+readSize > sFileSize) {
			readSize-=(sFilePos+BUFFER_SIZE)-sFileSize;
		}	

		maReadData(sFile, sData, sFilePos, readSize);
	}

	unsigned char readByte() {
		unsigned char b;
		if(sBufPos>=BUFFER_SIZE) readMoreData();
		b = sData[sBufPos];
		sFilePos++;
		sBufPos++;
		return b;
	}

	unsigned short readShort() {
		unsigned short ret = 0;
		if(sBufPos+1>=BUFFER_SIZE) readMoreData();

		/*
		if(sBufPos==43) {
			int a = 2;
		}
		*/

		for(int i = 0; i < 2; i++) {
			unsigned char b;
			b = sData[sBufPos+i];
			ret |= ((unsigned short)b)<<(i<<3);
		}
		sFilePos+=2;
		sBufPos+=2;
		return ret;
	}

	unsigned int readInt() {
		unsigned int ret = 0;
		if(sBufPos+3>=BUFFER_SIZE) readMoreData();
		for(int i = 0; i < 4; i++) {
			unsigned char b;
			b = sData[sBufPos+i];
			ret |= ((unsigned int)b)<<(i<<3);
		}
		sFilePos+=4;
		sBufPos+=4;
		return ret;
	}

	int readChars(char *output, int num) {
		for(int i = 0; i < num; i++) {

			output[i] = readByte();
			if(output[i]==0) {
				return i+1;
			}
		}
		return num;
	}

	void skipBytes(int b) {
		sFilePos+=b;
		sBufPos+=b;
	}

	int parseBitmapFontGeneratorFile(MAHandle file, int end, Charset *charset) {
		char ident[4];
		// read magic
		//maReadData(file, ident, filePtr, 4); 
		readChars(ident, 4);
		if(ident[0] != 'B' || ident[1] != 'M' || ident[2] != 'F' || ident[3] < 1 || ident[3] > 2) {
			//PANIC_MESSAGE("Wrong .fnt format!");
			//printf("Wrong .fnt format!\n");
			return 0;
		}

		for(int i = 0; i < 255; i++) {
			charset->chars[i].x	= 0xffff;
		}

		while(sFilePos != end) {
			// read block type and block size
			unsigned char id = readByte();
			unsigned int blockSize = readInt();
			switch(id) {
				case 1:
					{
						int initialFilePos = sFilePos;
						skipBytes(14);
						int i = readChars(charset->name, CHARSET_MAX_NAME_LEN);
						charset->name[i-1] = 0;
						skipBytes(((blockSize-4)+initialFilePos)-sFilePos);
					}
					break;
				case 2:
					// type common
					//#pragma pack(1)
					//struct commonBlock
					//{
					//    int  blockSize;
					//    WORD lineHeight;
					//    WORD base;
					//    WORD scaleW;
					//    WORD scaleH;
					//    WORD pages;
					//    BYTE packed  :1;
					//    BYTE encoded :1;      // Added with version 2
					//    BYTE reserved:6;
					//}; 
					//int blockSize = readInt(file, &filePtr);
					{
						//int filePtrBefore = filePtr;
						//printf("Reading common block!\n");
						charset->lineHeight		= readShort();
						charset->base			= readShort();
						charset->width			= readShort();
						charset->height			= readShort();
						charset->pages			= readShort();
						skipBytes(1);

						//int currentBlockSize = (filePtr-filePtrBefore)+4;	
						//printf("blockSize: %d, currentBlockSize: %d\n", blockSize, currentBlockSize);
					}
					break;
				case 4:
					// type char
					//struct charsBlock
					//{
					//    int blockSize;
					//    struct charInfo
					//    {
					//        WORD  id;
					//        WORD  x;
					//        WORD  y;
					//        WORD  width;
					//        WORD  height;
					//        short xoffset;
					//        short yoffset;
					//        short xadvance;
					//        BYTE  page;
					//        BYTE  chnl;
					//    } chars[1];
					//};
					//int blockSize = readInt(file, &filePtr);
					{
						//int filePtrBefore = filePtr;
						int numTimes = (blockSize-4)/18;
						while(numTimes--) {
							//printf("Reading char block! filePtr: %d\n", filePtr);
							unsigned short c			= readShort();
							if(c>256) {
								maPanic(1, "Font contains unicode characters! Only ascii is supported.");
							}
							//printf("Reading char %c!\n", (char)c);
							charset->chars[c].x			= readShort();
							charset->chars[c].y			= readShort();
							charset->chars[c].width		= readShort();
							charset->chars[c].height		= readShort();
							charset->chars[c].xOffset	= readShort();
							charset->chars[c].yOffset	= readShort();
							charset->chars[c].xAdvance	= readShort();
							charset->chars[c].page		= readByte();
							skipBytes(1);
						}

						//int currentBlockSize = (filePtr-filePtrBefore)+4;
						//printf("blockSize: %d, currentBlockSize: %d\n", blockSize, currentBlockSize);		
					}
					break;
				default:
					//printf("not reading this id\n");;
					skipBytes(blockSize-4);
					break;

			}
		}

		for(int i = 0; i < 255; i++) {
			if(charset->chars[i].x == 0xffff) {
				charset->chars[i] = charset->chars[(byte)' '];
			}
		}

		return 1;
	}

	Font::Font(MAHandle font) : mFontImage(0), mCharset(NULL), mLineSpacing(0) {
		setResource(font);
	}

	Font::~Font() {
		if(mFontImage) {
			maDestroyObject(mFontImage);
			PlaceholderPool::put(mFontImage);
		}
	}

/*	void Font::setColor(int color) {
		if(mFontImage == 0) return;
		rebuildFont(color);
	} */

	const char* Font::getName() const {
		return mCharset->name;
	}
	
	void Font::setLineSpacing(int size) {
		mLineSpacing = size;
	}
	
	int Font::getLineSpacing() const {
		return mLineSpacing;
	}

	void Font::setResource(MAHandle font) {
		//printf("Font is using resource: %d\n", font);
		if(font == 0) {
			mFontImage = 0;
			return;
		}

		initFontReader(font);

		int fontInfoSize = readInt();
		int imageSize = readInt();

		if(this->mCharset)
			delete this->mCharset;

		this->mCharset = new Charset;
		if(!parseBitmapFontGeneratorFile(font, 8+fontInfoSize, this->mCharset)) {
			maPanic(0, "Font::setResource(MAHandle font), could not parse font");
			return;
		}
		closeFontReader();

		this->mFontImage = PlaceholderPool::alloc();//maCreatePlaceholder();
		int err;
		if((err=maCreateImageFromData(this->mFontImage, font, 8+fontInfoSize, imageSize))!=RES_OK) {
			if(err==RES_OUT_OF_MEMORY)
				maPanic(0, "Font::setResource(MAHandle font), out of memory, image could not be loaded");
			if(err==RES_BAD_INPUT)
				maPanic(0, "Font::setResource(MAHandle font), image could not be loaded due to bad input");
		}
	}


	MAHandle Font::getHandle() const {
		return mFontImage;
	}

/*	void Font::rebuildFont(int color) {
		return;
		if(!mFontImage) return;
		MAExtent imageSize = maGetImageSize(mFontImage);
		int imageWidth = EXTENT_X(imageSize);
		int imageHeight = EXTENT_Y(imageSize);
		int *rebuiltFont = new int[imageWidth*imageHeight];
		MAASSERT(rebuiltFont);
		MARect srcRect = {0, 0, imageWidth, imageHeight};
		maGetImageData(this->mFontImage, rebuiltFont, &srcRect, imageWidth);
		color&=0x00ffffff;
		int *ptr = rebuiltFont;
		for(int i = 0; i < imageWidth*imageHeight; i++) {
			*ptr&=0xff000000;
			*ptr|=color;
			ptr++;
		}
		maDestroyObject(this->mFontImage);
		this->mFontImage = maCreatePlaceholder();
		maCreateImageRaw(this->mFontImage, rebuiltFont, imageSize, 1);
		delete []rebuiltFont;
	} */

	void Font::drawString(const char* strS, int x, int y) {
		if(!mFontImage) return;
		const unsigned char* str = (const unsigned char*)strS;
		MARect srcRect = {0, 0, 0, 0};
		MAPoint2d cursor = {x,y};

		CharDescriptor *chars = mCharset->chars;
		while(*str) {
			if((*str)=='\n')
			{
				cursor.x = x;
				cursor.y += mCharset->lineHeight + mLineSpacing;
				str++;
				continue;
			}

			srcRect.left = chars[*str].x;
			srcRect.top = chars[*str].y;
			srcRect.width = chars[*str].width;
			srcRect.height = chars[*str].height;

			MAPoint2d destPoint = {cursor.x + chars[*str].xOffset,
				cursor.y + chars[*str].yOffset};

			//maDrawImageRegion(this->mFontImage, &srcRect, &destPoint, 0);
			Gfx_drawImageRegion(this->mFontImage, &srcRect, &destPoint, 0);

			cursor.x += chars[*str].xAdvance;
			str++;
		}
	}

	short lineBreaks[2048]; // TODO: should probably change this to a vector (no good with limitations)
	int numLineBreaks;

	void Font::calcLineBreaks(const char* strS, int x, int y, const Rect& bound) const {
		int i = 0;
		int j = 0;
		int lastSpace = -1;
		MAPoint2d cursor = {x, y};
		CharDescriptor *chars = mCharset->chars;
		const unsigned char* str = (const unsigned char*)strS;

		while(str[i]) {
			if(str[i] == ' ') {
				lastSpace = i;
			} else if(str[i]=='\n') {

				cursor.x = x;
				cursor.y += mCharset->lineHeight + mLineSpacing;
				lineBreaks[j++] = i;
				i++;
				continue;
			}


			 cursor.x += chars[str[i]].xAdvance;
			 if(cursor.x > bound.x + bound.width) {
				if(lastSpace == -1) {
					lineBreaks[j++] = i;
				} else {
					lineBreaks[j++] = lastSpace+1;
					i = lastSpace+1;
					lastSpace = -1;
				} 
				cursor.x = x + chars[str[i]].xAdvance;
			}

			i++;
		}
		numLineBreaks = j;
		lineBreaks[j] = -1;
	}

	void Font::drawBoundedString(const char* strS, int x, int y, const Rect& bound) {
		int i = 0;
		int j = 0;
		if(!mFontImage) return;
		const unsigned char* str = (const unsigned char*)strS;
		calcLineBreaks(strS, x, y, bound);
		MARect srcRect = {0, 0, 0, 0};
		MAPoint2d cursor = {x, y};
		CharDescriptor *chars = mCharset->chars;
		while(str[i]) {
			if(lineBreaks[j] == i) {
				j++;
				cursor.x = x;
				cursor.y += mCharset->lineHeight + mLineSpacing;
				if(str[i]=='\n') {
					i++;
					continue;
				}
			}

			srcRect.left = chars[str[i]].x;
			srcRect.top = chars[str[i]].y;
			srcRect.width = chars[str[i]].width;
			srcRect.height = chars[str[i]].height;

			MAPoint2d destPoint = {cursor.x + chars[str[i]].xOffset,
				cursor.y + chars[str[i]].yOffset};

			//maDrawImageRegion(this->mFontImage, &srcRect, &destPoint, 0);
			Gfx_drawImageRegion(this->mFontImage, &srcRect, &destPoint, 0);

			cursor.x += chars[str[i]].xAdvance;
			i++;
		}
	}

	MAExtent Font::getStringDimensions(const char *strS, int length) const {
		if(length == 0) return EXTENT(0, 0);
		MAPoint2d cursor = {0, 0};
		int width = 0, height = mCharset->lineHeight;
		if(!mFontImage) return EXTENT(0, 0);
		CharDescriptor *chars = mCharset->chars;
		int i = 0;
		const unsigned char* str = (const unsigned char*)strS;
		while(*str && (i!=length)) {
			if((*str)=='\n')
			{
				cursor.x = 0;//chars[*str].xAdvance;
				cursor.y += mCharset->lineHeight + mLineSpacing;
				height += mCharset->lineHeight + mLineSpacing;
				str++;
				i++;
				continue;
			}

			cursor.x += chars[*str].xAdvance;

			if(cursor.x
				//mCharset.chars[*str].xOffset + mCharset.chars[*str].width
		> width) {
			//width = (cursor.x + mCharset.chars[*str].xOffset + mCharset.chars[*str].width);
				width = cursor.x;
			}

			str++;
			i++;
		}	

		return EXTENT(width, height);
	}

	MAExtent Font::getBoundedStringDimensions(const char *strS, const Rect &bound,
		int length) const
	{
		if(length == 0) return EXTENT(0, 0);
		int i = 0;
		int j = 0;
		if(!mFontImage) return EXTENT(0, 0);
		calcLineBreaks(strS, bound.x, bound.y, bound);
		//MARect srcRect = {0, 0, 0, 0};
		MAPoint2d cursor = {0, 0};
		int height = 0;
		int width = 0;
		CharDescriptor *chars = mCharset->chars;
		const unsigned char* str = (const unsigned char*)strS;
		while(*str && (i!=length)) {
			if((lineBreaks[j] == i) || ((*str) == '\n')) {
				j++;
				cursor.x = 0;
				cursor.y += mCharset->lineHeight + mLineSpacing;
				str++;
				i++;
				continue;
			} else {
				cursor.x += chars[*str].xAdvance;
			}

			if(cursor.y + mCharset->lineHeight//mCharset.chars[*str].yOffset + mCharset.chars[*str].height
			> height) {
				//height = (cursor.y + mCharset.chars[*str].yOffset + mCharset.chars[*str].height);
				height = cursor.y + mCharset->lineHeight;
			}

			if(cursor.x//mCharset.chars[*str].xOffset + mCharset.chars[*str].width
			> width) {
				//width = (cursor.x + mCharset.chars[*str].xOffset + mCharset.chars[*str].width);
				width = cursor.x; //cursor.x + mCharset.chars[*str].xAdvance;
			}


			str++;
			i++;
		}
		return EXTENT(width, height);
	}

	int Font::getLineBreak(int line, const char *str, const Rect& bound) const {
		calcLineBreaks(str, bound.x, bound.y, bound);
		if(line < 0 || line >= numLineBreaks) return -1;
		else return lineBreaks[line];
	}

	int Font::calculateLine(int index, const char *str, const Rect& bound) const {
		calcLineBreaks(str, bound.x, bound.y, bound);
		//short *ptrLineBreaks = lineBreaks;
		int i = 0;
		for(i = 0; i < numLineBreaks; i++) {
			if(index<=lineBreaks[i]) {
				return i;
			}
		}
		return i;
	}

	Rect Font::calculateRectOfIndex(int index, const char *str, const Rect& bound) const {
		int line = calculateLine(index, str, bound);
		int lineBreak = 0;
		if(line>0) 
			lineBreak = getLineBreak(line-1, str, bound);
	
		MAPoint2d cursor = {0, 0};
		cursor.y+=line*mCharset->lineHeight;
		MAExtent lineRect = getStringDimensions(&str[lineBreak], index-lineBreak);
		cursor.x+=EXTENT_X(lineRect);
		MAExtent charRect = getStringDimensions(&str[index], 1);
		return Rect(cursor.x, cursor.y, EXTENT_X(charRect), mCharset->lineHeight);

	}

	const Charset& Font::getCharset() const {
		return *mCharset;
	}

}
