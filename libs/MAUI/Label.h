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

/** \file Label.h Copyright (c) Mobile Sorcery AB 2005-2006 **/

#ifndef _SE_MSAB_MAUI_LABEL_H_
#define _SE_MSAB_MAUI_LABEL_H_

#include "Widget.h"
#include "Font.h"

namespace MAUI {


	/** A label widget is a simple widget capable of displaying
	    *text. The text can be aligned left, right or centered
	    * both vertically and horizontally.
	   * The widget draws its background by default, use setDrawBackground(false) to disable it.	
	**/

	class Label : public Widget {
	public:
		
		enum HorizontalAlignment {
			HA_LEFT,
			HA_CENTER,
			HA_RIGHT
		};

		enum VerticalAlignment {
			VA_TOP,
			VA_CENTER,
			VA_BOTTOM
		};

		/** Constructor.
		  * \param x the horizontal position of the Label relative to its parent's top left padded corner.
		  * \param y the vertical position of the Label relative to its parent's top left padded corner
		  * \param width the width of the Label.
		  * \param height the height of the Label.
		  * \param parent pointer to the parent widget. Passing anything else than NULL causes the Label to be added to the parent's children.
		  **/
		Label(int x, int y, int width, int height, Widget* parent);
		/** Constructor
		  * \param x the horizontal position of the Label relative to its parent's top left padded corner.
		  * \param y the vertical position of the Label relative to its parent's top left padded corner
		  * \param width the width of the Label.
		  * \param height the height of the Label.
		  * \param parent pointer to the parent widget. Passing anything else than NULL causes the widget to be added to the parent's children.
		  * \param caption the text displayed by the Label.
		  * \param backColor the background color of the widget.
		  * \param font the font to be used by to render the caption.
		  */
		Label(int x, int y, int width, int height, Widget* parent, const String &caption,
			int backColor, Font* font);

		/** Turns multiline mode on or off **/
		void setMultiLine(bool b=true);
		/** Returns whether multiline is enabled or not **/
		bool isMultiLine();

		/** Controls the horizontal alignment of the labels text. **/
		void setHorizontalAlignment(HorizontalAlignment alignment);
		/** Controls the horizontal alignment of the labels text. **/
		void setVerticalAlignment(VerticalAlignment alignment);

		/** Sets the text that appears on the label **/
		virtual void setCaption(const String& caption);
		/** Sets the font used to render text on the label **/
		void setFont(Font* font);

		/** 
		 * When this is set to true the label will automatically be resized in the 
		 * horizontal axis to perfectly fit the text. Then only new line characters in 
		 * the label string will line break the text.
		 **/
		void setAutoSizeX(bool f=true);

		/** 
		 * When this is set to true the label will automatically be resized in the 
		 * vertical axis to perfectly fit the text.
		 **/
		void setAutoSizeY(bool f=true);
		/** Returns the text displayed on the label **/
		const String& getCaption() const;
		/** Returns the font used by this label **/
		Font* getFont() const;
		/** Returns the horizontal alignment of the label **/
		HorizontalAlignment getHorizontalAlignment() const;
		/** Returns the vertical alignment of the label **/
		VerticalAlignment   getVerticalAlignment() const;
		/** Returns wether the label automatically adjusts its width to accomodate its caption **/
		bool getAutoSizeX() const;
		/** Returns wether the label automatically adjusts its height to acommodate its caption **/
		bool getAutoSizeY() const;

		void setPaddingLeft(int l);
		void setPaddingTop(int t);
		void setPaddingRight(int r);
		void setPaddingBottom(int b);

		virtual void setParameter(const String& name, const String& value);
	
		void update();

	protected:
		void getTextStart(int *x, int *y);

		void drawWidget();
		void resize(int width, int height);

		bool mustCalcStrSize;
		void calcStrSize();

		String caption;
		String cuttedCaption;

		Font* font;

		bool autoSizeX;
		bool autoSizeY;

		bool multiLine;

		HorizontalAlignment horizontalAlignment;
		VerticalAlignment   verticalAlignment; 

		MAExtent strSize;
		int strWidth, strHeight;
	};
}

#endif	//_SE_MSAB_MAUI_LABEL_H_
