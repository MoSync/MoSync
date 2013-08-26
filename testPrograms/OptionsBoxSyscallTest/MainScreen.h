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
 * @file MainScreen.h
 * @author Bogdan Iusco.
 */

#ifndef MAINSCREEN_H_
#define MAINSCREEN_H_

// Include all the wrappers.
#include <NativeUI/Widgets.h>

#include <IX_PIM.h>
#include <maapi.h>
#include <MAUtil/util.h>

#include "OptionsBox.h"
#include "OptionsBoxListener.h"

using namespace NativeUI;

class MainScreen:
	public Screen,
	public ButtonListener,
	public OptionsBoxListener
{

public:
	/**
	 * Constructor.
	 */
		MainScreen();

	/**
	 * Destructor.
	 */
	~MainScreen();

    /**
     * This method is called when there is an touch-down event for
     * a button.
     * @param button The button object that generated the event.
     */
    virtual void buttonPressed(Widget* button);

    /**
     * This method is called when there is an touch-up event for
     * a button.
     * @param button The button object that generated the event.
     */
    virtual void buttonReleased(Widget* button);

    /**
     * This method is called if the touch-up event was inside the
     * bounds of the button.
     * @param button The button object that generated the event.
     */
    virtual void buttonClicked(Widget* button);

    /**
     * This method is called when the destructive button from options
     * dialog was clicked.
     */
    virtual void optionsBoxDestructiveButtonClicked();

    /**
     * This method is called when the cancel button from options dialog was
     * clicked.
     */
    virtual void optionsBoxCancelButtonClicked();

    /**
     * This method is called when a button from options dialog was
     * clicked.
     * This method is not called if the destructive or cancel button were
     * clicked.
     * @param buttonIndex The index of the button that was clicked.
     * @param buttonTitle The title of the button that was clicked.
     */
    virtual void optionsBoxButtonClicked(
			const int buttonIndex,
			const MAUtil::WString& buttonTitle);

private:

	/**
	 * Creates and adds main layout to the screen.
	 */
	void createMainLayout();

	/**
	 * Write n wchar-arrays to buffer.
	 * @param buffer The given buffer.
	 * The first value in buffer will be the number of wchar-arrays(n argument).
	 * @param src Contains the n wchar-arrays that will be written into buffer.
	 * @return The number of written bytes.
	 */
	int MainScreen::writeWCharArraysToBuf(
	    void* buffer,
	    const wchar_t** src,
	    const int n);
private:
	/**
	 * Main layout.
	 */
	VerticalLayout* mMainLayout;

	Button* mButton;
	Label* mLabel;
};


#endif /* MAINSCREEN_H_ */
