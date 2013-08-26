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
 * @author Emma Tresanszki
 *
 * The main screen of the application.
 * This is the main entry point of the user interface of the application.
 * When the application is launched there is an input box available for
 * password input. After the edit box is filled, or after the Submit button is
 * clicked, the input content is checked.
 * The clear button clears the edit box content.
 */

#ifndef MAINSCREEN_H_
#define MAINSCREEN_H_

#include <maapi.h>
#include <MAUtil/util.h>

// Include all the wrappers.
#include <NativeUI/Widgets.h>

using namespace NativeUI;

class MainScreen :
	public Screen,
	public ButtonListener,
	public EditBoxListener
{
public:
	/**
	 * Constructor.
	 * Creates a Native UI screen. During this call a check if NativeUI is
	 * supported by the runtime platform is made. For example, MoRE does not
	 * support NativeUI at the time of writing this program/
	 */
	MainScreen();

	/**
	 * Destructor.
	 */
	~MainScreen();

	/**
	 * This method is called when the Clear button is clicked.
	 */
	void clearButtonClicked();

	/**
	 * This method is called when the Submit button is clicked, or edit box
	 * return button was hit.
	 */
	void submitEditBoxContent();
private:

	/**
	 * Here we create the user interface widgets.
	 */
	void createUI();

    /**
     * This method is called if the touch-up event was inside the
     * bounds of the button.
     * @param button The button object that generated the event.
     */
    virtual void buttonClicked(Widget* button);

    /**
     * This method is called when the return button was pressed.
     * On iphone platform the virtual keyboard is not hidden after
     * receiving this event.
     * @param editBox The edit box object that generated the event.
     */
    virtual void editBoxReturn(EditBox* editBox);

private:

    /** The main layout that holds the other widgets. */
	VerticalLayout* mMainLayout;

	/** A text label that displays instructions. */
	Label* mInstructions;

	/** Text editor box for user input. */
	EditBox* mPasswordBox;

	/** The Clear button. */
	Button* mClearButton;

	/** The Submit button. */
	Button* mSubmitButton;

};

#endif /* MAINSCREEN_H_ */
