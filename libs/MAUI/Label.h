/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/** 
* \file Label.h 
* \brief Text label widget with alignment control
* \author Patrick Broman and Niklas Nummelin
*/

#ifndef _SE_MSAB_MAUI_LABEL_H_
#define _SE_MSAB_MAUI_LABEL_H_

#include "Widget.h"
#include "Font.h"

namespace MAUI {


	/** \brief Text label widget with alignment control
	* 
	* A label widget is a simple widget capable of displaying
	* text. The text can be aligned left, right or centered
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
		/** Controls the vertical alignment of the labels text. **/
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
