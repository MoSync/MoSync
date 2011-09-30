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
