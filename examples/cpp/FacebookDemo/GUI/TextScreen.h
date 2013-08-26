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
