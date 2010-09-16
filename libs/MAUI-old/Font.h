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

/** 
* \file Font.h 
* \brief Bitmap font handling, line spacing, and line breaks
* \author Patrick Broman and Niklas Nummelin
*/

#ifndef _SE_MSAB_MAUI_FONT_H_
#define _SE_MSAB_MAUI_FONT_H_

#include <ma.h>

#include <MAUtil/Geometry.h>

namespace MAUI {

	using namespace MAUtil;

	/** 
	* \brief A character descriptor for a bitmap font.
	**/
	
	struct CharDescriptor
	{
		unsigned short x, y;
		unsigned short width, height;
		short xOffset, yOffset;
		short xAdvance;
		unsigned short page;
	};

	#define CHARSET_MAX_NAME_LEN 16

	/** 
	* \brief A character set for a bitmap font.
	**/

	struct Charset
	{
		unsigned short lineHeight;
		unsigned short base;
		unsigned short width, height;
		unsigned short pages;	
		CharDescriptor chars[256];

		char name[CHARSET_MAX_NAME_LEN];
	};

	/** \brief Bitmap font.
	*
	* This class represents and embodies bitmap fonts used by MAUI applications.
	**/
	
	class Font {
	public:
		
		/** Constructor, takes a handle to a binary resource with font data **/
		Font(MAHandle font);
		
		~Font();
		
		/** Sets the binary resource with font data to be used by this font **/
		void setResource(MAHandle font);

		/** Returns a handle to the binary resource used by the font **/
		MAHandle getHandle() const;
		
		/** Returns the plain-text name of the font **/
		const char *getName() const;

		/**
		* Sets the number of pixels between each line of text.
		* The default is 0.
		*/
		void setLineSpacing(int size = 0);

		/**
		* Returns the line spacing.
		* \see setLineSpacing()
		*/
		int getLineSpacing() const;
		
		/** Returns the width and height of str when rendered with this font **/
		MAExtent getStringDimensions(const char* str, int length=-1) const;
		
		/** Returns the width and height of str when rendered with this font and
		* bounded by the rectangle specified by bound. **/
		MAExtent getBoundedStringDimensions(const char *str, const Rect& bound, int length=-1) const;
		
		/** Returns the index of the first character of str on the line given by "line",
		  * assuming that the string has been linebroken according to bounds.
		  **/
		int getLineBreak(int line, const char *str, const Rect& bound) const;
		
		/** Returns the line on which the character at index in str would end up, if the string were
		  * linebroken according to bounds.
		  **/
		int calculateLine(int index, const char *str, const Rect& bound) const;
		
		/**
		  **/
		Rect calculateRectOfIndex(int index, const char *str, const Rect& bound) const;

		/** Renders str at x, y
		  **/
		void drawString(const char* str, int x, int y);

		/**
		* Renders str, linebroken according to bounds, at x,y.
		* Adds \a lineSpacing pixels of space between each line.
		*/
		void drawBoundedString(const char* str, int x, int y, const Rect& bound);

		const Charset& getCharset() const;

	protected:
		
		void calcCharPos(char c, int *x, int *y);
		void calcLineBreaks(const char* str, int x, int y, const Rect& bound) const;

		MAHandle fontImage;		
		Charset *charset;
		int mLineSpacing;
	};
}

#endif
