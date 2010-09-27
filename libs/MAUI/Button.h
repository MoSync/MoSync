/* Copyright (C) 2010 MoSync AB

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
* \file Button.h
* \brief Button widget
* \author Patrick Broman and Niklas Nummelin
*/

#ifndef _SE_MSAB_MAUI_BUTTON_H_
#define _SE_MSAB_MAUI_BUTTON_H_

#include "Label.h"
#include "Style.h"
#include <MAUtil/String.h>
#include <MAUtil/ListenerSet.h>

namespace MAUI {

	class Button;
	class ButtonListener {
	public:
		virtual void onButtonEvent(Button* b, bool pressed) = 0;
	};

	class Button : public Label {
	public:
		Button(int x, int y, int width, int height, Widget* parent=NULL, const String& caption="");
		bool pointerPressed(MAPoint2d p, int id);
		bool pointerMoved(MAPoint2d p, int id);
		bool pointerReleased(MAPoint2d p, int id);
		void setCaption(const String& caption);

		virtual bool isTransparent() const;
		void setFocused(bool focused=true);

		void addButtonListener(ButtonListener* l);
		void removeButtonListener(ButtonListener* l);

	protected:
		void restyle();
		void drawWidget();
		//String mCaption;
		bool mPressed;
		int mStartX, mStartY;

		WidgetSkin *mSkinFocusedPressed;
		WidgetSkin *mSkinFocusedReleased;
		WidgetSkin *mSkinUnfocusedPressed;
		WidgetSkin *mSkinUnfocusedReleased;

		ListenerSet<ButtonListener> mListeners;
	};

	/**
	 * Available properties:
	 * paddingLeft = IntegerProperty
	 * paddingRight = IntegerProperty
	 * paddingTop = IntegerProperty
	 * paddingBottom = IntegerProperty
	 * marginLeft = IntegerProperty
	 * marginRight = IntegerProperty
	 * marginTop = IntegerProperty
	 * marginBottom = IntegerProperty
	 * backgroundSkinFocused = SkinProperty (may be null, in which it's not used)
	 * backgroundSkinUnfocused = SkinProperty (may be null, in which it's not used)
	 * skinFocusedPressed = SkinProperty
	 * skinFocusedReleased = SkinProperty
	 * skinUnfocusedReleased = SkinProperty
	 */
	class ButtonStyle : public LabelStyle {
	public:
		ButtonStyle( 	SkinProperty* focusedPressed,
						SkinProperty* focusedReleased,
						SkinProperty* unfocusedReleased,
						FontProperty* font);

	};

}

#endif /* _SE_MSAB_MAUI_BUTTON_H_ */

