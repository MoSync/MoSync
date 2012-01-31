/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

/*
 * TextScreen.h
 */

#ifndef TEXTSCREEN_H_
#define TEXTSCREEN_H_

#include <MAUtil/String.h>

class NativeUI::Label;
class NativeUI::VerticalLayout;
class NativeUI::Button;

#include "NativeUI/ButtonListener.h"
#include "FacebookDemoScreen.h"

namespace FacebookDemoGUI
{

class TextScreen: public FacebookDemoScreen, public NativeUI::ButtonListener
{
public:
	TextScreen(FacebookDemoScreen *prev);

	void setText(const MAUtil::String &text);
	void clear();

	//			ButtonListener overrides
	/**
	 * This method is called when there is an touch-down event for
	 * a button.
	 * Only for iphone platform.
	 * @param button The button object that generated the event.
	 */
	virtual void buttonPressed(Widget* button);

	/**
	 * This method is called when there is an touch-up event for
	 * a button.
	 * Only for iphone platform.
	 * @param button The button object that generated the event.
	 */
	virtual void buttonReleased(Widget* button);

	/**
	 * This method is called if the touch-up event was inside the
	 * bounds of the button.
	 * @param button The button object that generated the event.
	 */
	virtual void buttonClicked(Widget* button);
private:
	void initialize();

private:
	NativeUI::Label 			*mLabel;
	NativeUI::VerticalLayout 	*mLayout;
	NativeUI::Button			*mBackButton;
};

}//namespace FacebookDemoGUI

#endif /* TEXTSCREEN_H_ */
