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
* \brief Bitmap font handling, line spacing, and line breaks.
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
	 */
	struct CharDescriptor
	{
		/**
		 * TODO: Document variable.
		 */
		unsigned short x;
		
		/**
		 * TODO: Document variable.
		 */
		unsigned short y;
		
		/**
		 * TODO: Document variable.
		 */
		unsigned short width;
		
		/**
		 * TODO: Document variable.
		 */
		unsigned short height;
		
		/**
		 * TODO: Document variable.
		 */
		short xOffset;
		
		/**
		 * TODO: Document variable.
		 */
		short yOffset;
		
		/**
		 * TODO: Document variable.
		 */
		short xAdvance;
		
		/**
		 * TODO: Document variable.
		 */
		unsigned short page;
	};

	#define CHARSET_MAX_NAME_LEN 16

	/** 
	* \brief A character set for a bitmap font.
	**/
	struct Charset
	{
		/**
		 * TODO: Document variable.
		 */
		unsigned short lineHeight;
		
		/**
		 * TODO: Document variable.
		 */
		unsigned short base;
		
		/**
		 * TODO: Document variable.
		 */
		unsigned short width;
		
		/**
		 * TODO: Document variable.
		 */
		unsigned short height;
		
		/**
		 * TODO: Document variable.
		 */
		unsigned short pages;	
		
		/**
		 * TODO: Document variable.
		 */
		CharDescriptor chars[256];

		/**
		 * TODO: Document variable.
		 */
		char name[CHARSET_MAX_NAME_LEN];
	};

	/** 
	 * \brief Bitmap font.
	 *
	 * This class represents and embodies bitmap fonts used 
	 * by MAUI applications.
	 */
	class Font {
	public:
		/**
		 * Constructor, takes a handle to a binary resource with font data.
		 * \param font The font data.
		 */
		Font(MAHandle font);
		
		/**
		 * Destructor
		 * TODO: Should be virtual?
		 */
		~Font();
		
		/** 
		 * Sets the binary resource with font data to be used by this font.
		 * \param font The font data.
		 */
		void setResource(MAHandle font);

		/** 
		 * Returns a handle to the binary resource used by the font. 
		 * \return The font data.
		 */
		MAHandle getHandle() const;
		
		/**
		 * Returns the plain-text name of the font. 
		 * \return The font name.
		 */
		const char* getName() const;

		/**
		 * Sets the number of pixels between each line of text.
		 * The default is 0.
		 * \param size The line spacing.
		 */
		void setLineSpacing(int size = 0);

		/**
		* Returns the line spacing.
		* \return The line spacing.
		* \see setLineSpacing()
		*/
		int getLineSpacing() const;
		
		/** 
		 * Returns the width and height of a string rendered with this font. 
		 * \param str The string to be measured.
		 * \param length TODO: Document param.
		 * \return The extent of the string when rendered.
		 */
		MAExtent getStringDimensions(const char* str, int length=-1) const;
		
		/** 
		 * Returns the width and height of a string rendered with this font 
		 * when bounded by a rectangle.
		 * \param str The string to be measured.
		 * \param bound Rectangle that defines the text rendering area.
		 * \param length TODO: Document param.
		 * \return The extent of the string when rendered.
		 */
		MAExtent getBoundedStringDimensions(
			const char *str, 
			const Rect& bound, 
			int length=-1) const;
		
		/** 
		 * Returns the index of the first character of str on the line 
		 * given by "line", assuming that the string has been linebroken 
		 * according to bounds.
		 * \param line Index of line for which to get character start index.
		 * \param str The string to be rendered.
		 * \param bound Rectangle that defines the text rendering area.
		 * \return Index of first character on the requested line.
		 */
		int getLineBreak(int line, const char *str, const Rect& bound) const;
		
		/** 
		 * Returns the line on which the character at index in str would end up, 
		 * if the string were linebroken according to bounds.
		 * \param index Character index for which to get line.
		 * \param str The string to be rendered.
		 * \param bound Rectangle that defines the text rendering area.
		 * \return Index of the line for the requested character index.
		 */
		int calculateLine(int index, const char *str, const Rect& bound) const;
		
		/**
		 * TODO: Document.
		 * \param index TODO: Document.
		 * \param str TODO: Document.
		 * \param bound TODO: Document.
		 * \return TODO: Document.
		 */
		Rect calculateRectOfIndex(
			int index, 
			const char *str, 
			const Rect& bound) const;

		/** 
		 * Render a text string.
		 * \param str String to render.
		 * \param x Left coordinate of string.
		 * \param y Top coordinate of string.
		 */
		void drawString(const char* str, int x, int y);

		/**
		 * Renders str, linebroken according to bounds, at x,y.
		 * Adds \a lineSpacing pixels of space between each line.
		 * \param str String to render.
		 * \param x Left coordinate of string.
		 * \param y Top coordinate of string.
		 * \param bound Rectangle that defines the rendering area.
		 * TODO: Document how x and y relates to the x and y or the bounding
		 * rectangle. Are they redundant? Is only width and height of 
		 * the rectangle used?
		 */
		void drawBoundedString(
			const char* str, 
			int x, 
			int y, 
			const Rect& bound);

		/**
		 * TODO: Document.
		 */
		const Charset& getCharset() const;

	protected:
		/**
		 * TODO: Document.
		 * \param c TODO: Document.
		 * \param x TODO: Document.
		 * \param y TODO: Document.
		 */
		void calcCharPos(char c, int *x, int *y);
		
		/**
		 * TODO: Document.
		 * \param str TODO: Document.
		 * \param x TODO: Document.
		 * \param y TODO: Document.
		 * \param bound TODO: Document.
		 */
		void calcLineBreaks(
			const char* str, 
			int x, 
			int y, 
			const Rect& bound) const;

		/**
		 * TODO: Document.
		 */
		MAHandle mFontImage;
		
		/**
		 * TODO: Document.
		 */
		Charset *mCharset;
		
		/**
		 * Spacing between lines. In pixels?
		 */
		int mLineSpacing;
	};
}

#endif
