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
 * @file main.cpp
 * @author Mattias Frånberg and Chris Hughes
 *
 * This application provides a very basic example of how to work
 * with Native UI to position and and manipulate graphical user
 * interface elements. The code is very well commented so that you
 * can see what's happening at each step. The application uses one
 * screen and a some widgets to provide a password entry box and
 * some control buttons.
 *
 * NOTE: This example application has the same functionality as
 * our example application "HelloMAUI". Compare the two examples
 * to see how we do similar things in MAUI and in Native UI.
 */

// Include MoSync syscalls.
#include <maapi.h>

// Include NativeUI extensions.
#include <IX_WIDGET.h>

// Include header file for Moblets.
#include <MAUtil/Moblet.h>

// Include widget utility functions. These functions simplify
// getting and setting widget properties.
#include "WidgetUtil.h"

/**
 * Class that wraps a NativeUI screen widget. We implement
 * the CustomEventListener interface to be able to process
 * widget events.
 */
class AppScreen : public MAUtil::CustomEventListener
{
public:
	/**
	 * In the constructor, we create the user interface.
	 */
	AppScreen()
	{
		createUI();
	}

	/**
	 * In the destructor, we destroy the widgets to release memory.
	 * Destroying a widget automatically destroys all child widgets.
	 */
	virtual ~AppScreen()
	{
		// Destroying the screen widget destroys all of its children.
		maWidgetDestroy(mScreen);
	}

	/**
	 * Here we create the user interface widgets.
	 */
	void createUI()
	{
		// Make the AppScreen listen for events coming from widgets.
		MAUtil::Environment::getEnvironment().addCustomEventListener(this);

		// Create a Native UI screen. As the screen is a member
		// variable (also called instance variable) we have
		// prefixed the variable name with "m".
		mScreen = maWidgetCreate(MAW_SCREEN);

		// Check if NativeUI is supported by the runtime platform.
		// For example, MoRE does not support NativeUI at the time
		// of writing this program.
		if (-1 == mScreen)
		{
			maPanic(0,
				"NativeUI is only available on Android and iPhone. "
				"You must run directly on the device or devices emulator.");
		}
	   
		// Create the screen's main layout widget.
		mMainLayoutWidget = maWidgetCreate(MAW_VERTICAL_LAYOUT);

		// Make the layout fill the entire screen. For properties that
		// take an integer parameter we use the widgetSetPropertyInt
		// function, for properties that takes a string parameter
		// we use the maWidgetSetProperty function.
		widgetSetPropertyInt(
			mMainLayoutWidget,
			MAW_WIDGET_WIDTH,
			MAW_CONSTANT_FILL_AVAILABLE_SPACE);
		widgetSetPropertyInt(
			mMainLayoutWidget,
			MAW_WIDGET_HEIGHT,
			MAW_CONSTANT_FILL_AVAILABLE_SPACE);

		// Add the layout as the root of the screen's widget tree.
		maWidgetAddChild(mScreen, mMainLayoutWidget);

		// Next, we create four child widgets within the main layout widget.

		// The first widget is a label that we'll use to present instructions.
		mInstructions = maWidgetCreate(MAW_LABEL);

		// Make the label fill the width of the parent layout and
		// adjust its height to "shrink wrap" the size of the text.
		widgetSetPropertyInt(
			mInstructions,
			MAW_WIDGET_WIDTH,
			MAW_CONSTANT_FILL_AVAILABLE_SPACE);
		widgetSetPropertyInt(
			mInstructions,
			MAW_WIDGET_HEIGHT,
			MAW_CONSTANT_WRAP_CONTENT);

		// Set the label text.
		maWidgetSetProperty(
			mInstructions,
			MAW_LABEL_TEXT,
			"Enter a password:");

		// Add the label to the main layout.
		maWidgetAddChild(mMainLayoutWidget, mInstructions);

		// The second widget is an edit box that we'll use to input
		// text. We use the password mode to hide text after entry.
		mPasswordBox = maWidgetCreate(MAW_EDIT_BOX);
		widgetSetPropertyInt(
			mPasswordBox,
			MAW_WIDGET_WIDTH,
			MAW_CONSTANT_FILL_AVAILABLE_SPACE);
		widgetSetPropertyInt(
			mPasswordBox,
			MAW_WIDGET_HEIGHT,
			MAW_CONSTANT_WRAP_CONTENT);
		maWidgetSetProperty(
			mPasswordBox,
			MAW_EDIT_BOX_EDIT_MODE,
			"password");
		maWidgetAddChild(mMainLayoutWidget, mPasswordBox);

		// The third widget is a button that clears the input edit box.
		// We center the text vertically and horizontally within the button.
		mClearButton = maWidgetCreate(MAW_BUTTON);
		widgetSetPropertyInt(
			mClearButton,
			MAW_WIDGET_WIDTH,
			MAW_CONSTANT_FILL_AVAILABLE_SPACE);
		widgetSetPropertyInt(
			mClearButton,
			MAW_WIDGET_HEIGHT,
			MAW_CONSTANT_WRAP_CONTENT);
		maWidgetSetProperty(
			mClearButton,
			MAW_BUTTON_TEXT_VERTICAL_ALIGNMENT,
			MAW_ALIGNMENT_CENTER);
		maWidgetSetProperty(
			mClearButton,
			MAW_BUTTON_TEXT_HORIZONTAL_ALIGNMENT,
			MAW_ALIGNMENT_CENTER);
		maWidgetSetProperty(
			mClearButton,
			MAW_BUTTON_TEXT,
			"Clear");
		maWidgetAddChild(mMainLayoutWidget, mClearButton);

		// Finally, the fourth widget is a the submit button.
		mSubmitButton = maWidgetCreate(MAW_BUTTON);
		widgetSetPropertyInt(
			mSubmitButton,
			MAW_WIDGET_WIDTH,
			MAW_CONSTANT_FILL_AVAILABLE_SPACE);
		widgetSetPropertyInt(
			mSubmitButton,
			MAW_WIDGET_HEIGHT,
			MAW_CONSTANT_WRAP_CONTENT);
		maWidgetSetProperty(
			mSubmitButton,
			MAW_BUTTON_TEXT_VERTICAL_ALIGNMENT,
			MAW_ALIGNMENT_CENTER);
		maWidgetSetProperty(
			mSubmitButton,
			MAW_BUTTON_TEXT_HORIZONTAL_ALIGNMENT,
			MAW_ALIGNMENT_CENTER);
		maWidgetSetProperty(
			mSubmitButton,
			MAW_BUTTON_TEXT,
			"Submit");
		maWidgetAddChild(mMainLayoutWidget, mSubmitButton);
	}

	/**
	 * Method that makes the screen visible.
	 */
	void show()
	{
		// Display the NativeUI screen.
		// Note: This would hide any previously visible screen.
		maWidgetScreenShow(mScreen);

		// Note: If you wish to display the MoSync drawing area after
		// having displayed a NativeUI screen, you can use this call:
		// maWidgetScreenShow(MAW_CONSTANT_MOSYNC_SCREEN_HANDLE);
	}

	/**
	 * This method implements a custom event listener.
	 * Widget events are sent as custom events.
	 */
	void customEvent(const MAEvent& event)
	{
		// Check if this is a widget event.
		if (EVENT_TYPE_WIDGET == event.type)
		{
			// Get the widget event data structure.
			MAWidgetEventData* eventData = (MAWidgetEventData*) event.data;

			// Here we handle clicked events.
			if (MAW_EVENT_CLICKED == eventData->eventType)
			{
				if (mClearButton == eventData->widgetHandle)
				{
					clearButtonClicked();
				}
				else if (mSubmitButton == eventData->widgetHandle)
				{
					submitButtonClicked();
				}
			}
		}
	}

	/**
	 * This method is called when the Clear button is clicked.
	 */
	void clearButtonClicked()
	{
		// Empty the contents of the edit box.
		maWidgetSetProperty(mPasswordBox, MAW_EDIT_BOX_TEXT, "");
	}

	/**
	 * This method is called when the Submit button is clicked.
	 */
	void submitButtonClicked()
	{
		// Buffer used to get the text in the password edit box.
		char passwordTextBuffer[256];

		// Get the text from the password box and put it into the buffer.
		int textLength = maWidgetGetProperty(
			mPasswordBox,
			MAW_EDIT_BOX_TEXT,
			passwordTextBuffer,
			256);

		// If the text does not fit in the buffer, textLength will be
		// an error code.
		if (MAW_RES_INVALID_STRING_BUFFER_SIZE == textLength)
		{
			// If the password is too long we use the instructions label
			// to inform the user. Note that C automatically concatenates
			// strings split over multiple lines.
			maWidgetSetProperty(
				mInstructions,
				MAW_LABEL_TEXT,
				"Password too long. "
				"Please enter a shorter password:");
		}
		// Check that the password is at least 6 characters long.
		else if(textLength < 6)
		{
			// If the password is too short we use the instructions label
			// to inform the user. Note that C automatically concatenates
			// strings split over multiple lines.
			maWidgetSetProperty(
				mInstructions,
				MAW_LABEL_TEXT,
				"Password too short. "
				"Please enter a password of at least 6 characters:");
		}
		else
		{
			// The password is at least 6 characters long,
			// we congratulate user.
			maWidgetSetProperty(
				mInstructions,
				MAW_LABEL_TEXT,
				"Password OK");
		}
	}

private:
	/** The screen widget. */
	MAHandle mScreen;

	/** The main layout that holds the other widgets. */
	MAHandle mMainLayoutWidget;

	/** A text label that displays instructions. */
	MAHandle mInstructions;

	/** Text editor box for user input. */
	MAHandle mPasswordBox;

	/** The Clear button. */
	MAHandle mClearButton;

	/** The Submit button. */
	MAHandle mSubmitButton;
};

// That's the screen class finished, now we move on to the Moblet that
// is the main object in the application.

/**
 * A Moblet is the main object of MoSync application. In the Moblet
 * we manage the application and handle events. Our Moblet inherits
 * the Moblet base class in the MAUtil library.
 */
class HelloNativeUIMoblet : public MAUtil::Moblet
{
public:
	/**
	 * The user interface is created in the constructor method.
	 */
	HelloNativeUIMoblet()
	{
		// Create the main (and only) screen used in the application.
		mAppScreen = new AppScreen();

		// Display the screen.
		mAppScreen->show();
	}

	/**
	 * The destructor deletes the screen object.
	 */
	virtual ~HelloNativeUIMoblet()
	{
		delete mAppScreen;
	}

	/**
	 * This method is called when a key is pressed. The method
	 * is inherited from the Moblet class, and is overridden here.
	 */
	void keyPressEvent(int keyCode, int nativeCode)
	{
		// Close the application if the back key or key 0 is pressed.
		if (MAK_BACK == keyCode || MAK_0 == keyCode)
		{
			// Call close to exit the application.
			close();
		}
	}

private:
	/**
	 * The screen of our application.
	 */
	AppScreen* mAppScreen;
};

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	// Create and run the Moblet to start the application.
	MAUtil::Moblet::run(new HelloNativeUIMoblet());

	// The Moblet will run until it is closed by the user.
	// When it is closed we end our program in a well-behaved
	// way, returning zero.
	return 0;
}
