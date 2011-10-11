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
 * @file MainScreen.cpp
 * @author Emma Tresanszki
 *
 * The main screen of the application.
 * This is the main entry point of the user interface of the application.
 * When the application is launched there is an input box available for
 * password input. After the edit box is filled, or after the Submit button is
 * clicked, the input content is checked.
 * The clear button clears the edit box content.
 */

#include "MainScreen.h"

	/**
	 * Constructor.
	 * Creates a Native UI screen. During this call a check if NativeUI is
	 * supported by the runtime platform is made. For example, MoRE does not
	 * support NativeUI at the time of writing this program/
	 */
	MainScreen::MainScreen():
	Screen()
	{
		// Create the UI widgets.
		createUI();

		// Make the AppScreen listen for events coming from widgets.
		mClearButton->addButtonListener(this);
		mSubmitButton->addButtonListener(this);
		mPasswordBox->addEditBoxListener(this);
	}

	/**
	 * Destructor.
	 */
	MainScreen::~MainScreen()
	{
		// Remove the widget listeners.
		mClearButton->removeButtonListener(this);
		mSubmitButton->removeButtonListener(this);
		mPasswordBox->removeEditBoxListener(this);
	}

	/**
	 * Here we create the user interface widgets.
	 */
	void MainScreen::createUI()
	{

		// Create the screen's main layout widget.
		mMainLayout = new VerticalLayout();
		// Make the layout fill the entire screen.
		mMainLayout->fillSpaceHorizontally();
		mMainLayout->fillSpaceVertically();
		// Add the layout as the root of the screen's widget tree.
		Screen::setMainWidget(mMainLayout);

		// Next, we create four child widgets within the main layout widget.

		// The first widget is a label that we'll use to present instructions.
		mInstructions = new Label();
		// Make the label fill the width of the parent layout and
		// adjust its height to "shrink wrap" the size of the text.
		mInstructions->fillSpaceHorizontally();
		mInstructions->wrapContentVertically();
		// Set the label text.
		mInstructions->setText("Enter a password:");
		// Add the label to the main layout.
		mMainLayout->addChild(mInstructions);

		// The second widget is an edit box that we'll use to input
		// text. We use the password mode to hide text after entry.
		mPasswordBox = new EditBox();
		mPasswordBox->fillSpaceHorizontally();
		mPasswordBox->wrapContentVertically();
		mPasswordBox->setEditMode(EDIT_BOX_MODE_PASSWORD);
		mMainLayout->addChild(mPasswordBox);

		// The third widget is a button that clears the input edit box.
		// We center the text vertically and horizontally within the button.
		mClearButton = new Button();
		mClearButton->fillSpaceHorizontally();
		mClearButton->wrapContentVertically();
		mClearButton->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
		mClearButton->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
		mClearButton->setText("Clear");
		mMainLayout->addChild(mClearButton);

		// Finally, the fourth widget is a the submit button.
		mSubmitButton = new Button();
		mSubmitButton->fillSpaceHorizontally();
		mSubmitButton->wrapContentVertically();
		mSubmitButton->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
		mSubmitButton->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
		mSubmitButton->setText("Submit");
		mMainLayout->addChild(mSubmitButton);
	}

    /**
     * This method is called if the touch-up event was inside the
     * bounds of the button.
     * @param button The button object that generated the event.
     */
    void MainScreen::buttonClicked(Widget* button)
    {
    	if ( mClearButton == button )
    	{
    		clearButtonClicked();
    	}
    	else if ( mSubmitButton == button )
    	{
    		// Hide the keyboard if it isn't already dismissed.
    		mPasswordBox->hideKeyboard();
    		submitEditBoxContent();
    	}

    }

    /**
     * This method is called when the return button was pressed.
     * On iphone platform the virtual keyboard is not hidden after
     * receiving this event.
     * @param editBox The edit box object that generated the event.
     */
    void MainScreen::editBoxReturn(EditBox* editBox)
    {
    	submitEditBoxContent();
    }

	/**
	 * This method is called when the Clear button is clicked.
	 */
	void MainScreen::clearButtonClicked()
	{
		// Empty the contents of the edit box.
		mPasswordBox->setText("");
		submitEditBoxContent();
	}

	/**
	 * This method is called when the Submit button is clicked, or edit box
	 * return button was hit.
	 */
	void MainScreen::submitEditBoxContent()
	{
		// Get the text from the password box.
		MAUtil::String password = mPasswordBox->getText();

		// Check if the text doesn't fit the buffer( the default size is 256).
		if ( mPasswordBox->getLastError().errorCode ==
				MAW_RES_INVALID_STRING_BUFFER_SIZE )
		{
			// If the password is too long we use the instructions label
			// to inform the user. Note that C automatically concatenates
			// strings split over multiple lines.
			mInstructions->setText("Password too long. "
				"Please enter a shorter password:");
		}
		// Check that the password is at least 6 characters long.
		else if ( password.length() < 6 )
		{
			// If the password is too short we use the instructions label
			// to inform the user. Note that C automatically concatenates
			// strings split over multiple lines.
			mInstructions->setText("Password too short. "
				"Please enter a password of at least 6 characters:");
		}
		else
		{
			// The password is at least 6 characters long,
			// we congratulate user.
			mInstructions->setText("Password OK");
		}
	}
