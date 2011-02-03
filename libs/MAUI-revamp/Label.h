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
* \file Label.h 
* \brief Text label widget with alignment control.
* \author Patrick Broman and Niklas Nummelin
*/

#ifndef _SE_MSAB_MAUI_LABEL_H_
#define _SE_MSAB_MAUI_LABEL_H_

#include "Widget.h"
#include "Font.h"

namespace MAUI {


	/** 
	 * \brief Text label widget with alignment control
	 * 
	 * A label widget is a simple widget capable of displaying
	 * text. The text can be aligned left, right or centered
	 * both vertically and horizontally.
	 * The widget draws its background by default, 
	 * use setDrawBackground(false) to disable it.	
	 *
	 * Note: Doesn't support unicode strings yet, because the 
	 * font-system doesn't (will change in the future).
	 */
	class Label : public Widget {
	public:
		
		/**
		 * Constants for horizontal alingment.
		 */
		enum HorizontalAlignment {
			HA_LEFT,
			HA_CENTER,
			HA_RIGHT
		};

		/**
		 * Constants for vertical alingment.
		 */
		enum VerticalAlignment {
			VA_TOP,
			VA_CENTER,
			VA_BOTTOM
		};

		/** 
		 * Constructor
		 * \param x Left coordinate of the Label.
		 * \param y Top coordinate of the Label.
		 * \param width Width of the Label.
		 * \param height Height of the Label.
		 * \param caption The text displayed by the Label.
		 */
		Label(
			int x=0, 
			int y=0, 
			int width=0, 
			int height=0, 
			const String &caption="");

		/** 
		 * Turns multiline mode on or off.
		 * \param multiline true for multiline on, false for off.
		 */
		void setMultiLine(bool multiline=true);
		
		/** 
		 * Returns whether multiline is enabled or not.
		 * \return true for multiline on, false for off.
		 */
		bool isMultiLine();

		/** 
		 * Set the horizontal alignment of the label text.
		 * \param alignment The alingment constant.
		 */
		void setHorizontalAlignment(HorizontalAlignment alignment);
		
		/** 
		 * Set the vertical alignment of the label text.
		 * \param alignment The alingment constant.
		 */
		void setVerticalAlignment(VerticalAlignment alignment);

		/** 
		 * Sets the text that the label displays.
		 * \param caption The text of the label.
		 */
		virtual void setCaption(const String& caption);

		/** 
		 * When horizontal autosize is set to true the label will automatically
		 * be resized in the horizontal axis to perfectly fit the text. 
		 * Only new line characters in the label string will line break 
		 * the text.
		 * \param autosize The horizontal autosize behaviour, true for
		 *  autosize on, false for off.
		 */
		void setAutoSizeX(bool autosize=true);

		/** 
		 * When vertical autosize is set to true the label will automatically 
		 * be resized in the vertical axis to perfectly fit the text.
		 * \param autosize The vertical autosize behaviour, true for
		 *  autosize on, false for off.
		 */
		void setAutoSizeY(bool autosize=true);
		
		/** 
		 * Returns the text displayed by the label.
		 * \return The label text.
		 */
		const String& getCaption() const;
		
		/** 
		 * Returns the horizontal alignment of the label.
		 * \return The horizontal alignment.
		 */
		HorizontalAlignment getHorizontalAlignment() const;
		
		/** 
		 * Returns the vertical alignment of the label.
		 * \return The vertical alignment.
		 */
		VerticalAlignment getVerticalAlignment() const;
		
		/** 
		 * Returns wether the label automatically adjusts 
		 * its width to accomodate its caption.
		 * \return true if horizontal autosize is on, false if off.
		 */
		bool getAutoSizeX() const;
		
		/** 
		 * Returns wether the label automatically adjusts 
		 * its height to accomodate its caption.
		 * \return true if vertical autosize is on, false if off.
		 */
		bool getAutoSizeY() const;

		// TODO: Delete or document why commented out.
		/*
		void setPaddingLeft(int l);
		void setPaddingTop(int t);
		void setPaddingRight(int r);
		void setPaddingBottom(int b);
		void setWidth(int width);
		void setHeight(int height);
		*/

		/** 
		 * Returns the transparency of the widget.
		 * \return true if transparent, false if opaque.
		 */
		virtual bool isTransparent() const;
		
	protected:
		/** 
		 * TODO: Document.
		 */
		void updateInternal();

		/** 
		 * TODO: Document.
		 */
		void getTextStart(int *x, int *y);
		
		/** 
		 * Render the label.
		 */
		void drawWidget();
		
		/** 
		 * Set the size of the label.
		 * \param width The width of the label.
		 * \param height The height of the label.
		 */
		void resize(int width, int height);
		
		/** 
		 * TODO: Document.
		 */
		void calcStrSize();

		/** 
		 * TODO: Document.
		 */
		virtual void restyle();

		// TODO: Delete or document why commented out.
		//bool mMustCalcStrSize;

		/** 
		 * The text displayed by the label.
		 */
		String mCaption;
		
		/** 
		 * TODO: Document.
		 */
		String mCuttedCaption;

		/** 
		 * The font used by the label.
		 */
		Font* mFont;

		/** 
		 * Horizontal autosize flag.
		 */
		bool mAutoSizeX;
		
		/** 
		 * Vertical autosize flag.
		 */
		bool mAutoSizeY;

		/** 
		 * Multiline flag.
		 */
		bool mMultiLine;

		/** 
		 * Horizontal alignment.
		 */
		HorizontalAlignment mHorizontalAlignment;
		
		/** 
		 * Vertical alignment.
		 */
		VerticalAlignment mVerticalAlignment;

		/** 
		 * TODO: Document.
		 */
		MAExtent mStrSize;
		
		/** 
		 * TODO: Document.
		 */
		int mStrWidth;
		
		/** 
		 * TODO: Document.
		 */
		int mStrHeight;
	};

	/**
	 * Class that defined the style of a label.
	 * Available properties:
	 * font = FontProperty
	 * paddingLeft = IntegerProperty
	 * paddingRight = IntegerProperty
	 * paddingTop = IntegerProperty
	 * paddingBottom = IntegerProperty
	 * backgroundSkinFocused = SkinProperty
	 * backgroundSkinUnfocused = SkinProperty
	 */
	class LabelStyle : public Style {
	public:
		/**
		 * Constructor.
		 * \param font The font of the style.
		 * \param paddingLeft The left padding of the style.
		 * \param paddingRight The right padding of the style.
		 * \param paddingTop The top padding of the style.
		 * \param paddingBottom The bottom padding of the style.
		 * \param backgroundSkinFocused The skin for a focused label.
		 * \param backgroundSkinUnfocused = The skin for an unfocused label.
		 */
		LabelStyle(
			FontProperty* font,
			int paddingLeft = 0,
			int paddingRight = 0,
			int paddingTop = 0,
			int paddingBottom = 0,
			DrawableProperty* backgroundSkinFocused = NULL,
			DrawableProperty* backgroundSkinUnfocused = NULL
		);
	};
}

#endif	//_SE_MSAB_MAUI_LABEL_H_
