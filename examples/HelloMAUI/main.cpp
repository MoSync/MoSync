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
 * @file hellomaui.cpp
 * @author Chris Hughes
 *
 * This application provides a very basic example of how to work
 * with MAUI to position and and manipulate graphical user interface
 * elements. The code is very well commented so that you can see
 * what's happening at each step. The application uses one screen
 * and a some widgets to provide a password entry box and some
 * control buttons. For more advanced use of MAUI, see our MAUIEx
 * example application.
 *
 * NOTE: "MA" is short for MoSync API. "MAUI" stands for the MoSync
 * user interface API. This is a completely cross platform API that
 * works on a wide range of devices. There is also a native widget
 * API, also cross platform, but targeted for high-end devices,
 * currently Android and iPhone.
 *
 * This example application has the same functionality as our example
 * application "HelloNativeUI". Compare the two examples to see how we
 * do similar things in MAUI and in Native UI.
 */

// Include the header files for MoSync Moblets and MAUI widgets
// so that we can use them in our code.
#include <MAUtil/Moblet.h>
#include <MAUI/Screen.h>
#include <MAUI/Label.h>
#include <MAUI/Layout.h>
#include <MAUI/EditBox.h>
#include <MAUI/Font.h>

// Include also the header file for the application's font and
// image resources which are defined in the file called
// "resources.lst". Note that the include file "MAHeaders.h"
// is automatically generated in the background by MoSync and
// is not visible in the project's directory listing.
#include "MAHeaders.h"

/**
 * This class defines the screen of our application. We inherit
 * the Screen base class in the MAUI library. We'll only be
 * using one screen instance in this application, but you could
 * have multiple instances in your application, perhaps inheriting
 * their characteristics from other screen classes in a structured
 * hierarchy.
 *
 * We also implement the TextBoxListener interface to be able to
 * process events sent from the maTextBox syscall. This syscall
 * displays a virtual keyboard and an input text field, and is
 * very useful on devices that don't have a physical keypad.
 */
class AppScreen :
	public MAUI::Screen,
	public MAUtil::TextBoxListener
{
public:
	/**
	 * In the constructor, we create the user interface.
	 */
	AppScreen()
	{
		// Create user interface widgets.
		createUI();
	}

	/**
	 * In the destructor, we destroy the widgets to release memory.
	 * Destroying a widget automatically destroys all child widgets.
	 */
	virtual ~AppScreen()
	{
		delete mMainLayoutWidget;
	}

	/**
	 * Here we create the user interface widgets.
	 */
	void createUI()
	{
		// Create a font object. The font resource and other
		// resources are defined in the file "resources.lst".
		MAUI::Font* font = new MAUI::Font(RES_FONT);

		// Create a "skin" for the buttons. This will give the
		// buttons a customised appearance. The skin is divided
		// into nine parts ("nine patch image"), where the center
		// part will be stretched to fit the size of the button.
		MAUI::WidgetSkin* buttonSkin = new MAUI::WidgetSkin(
			RES_BUTTON_SELECTED,   // Image for selected state.
			RES_BUTTON_UNSELECTED, // Image for unselected state.
			16, // X coordinate for start of center patch.
			32, // X coordinate for end of center patch.
			16, // Y coordinate for start of center patch.
			32, // Y coordinate for end of center patch.
			true,  // Is selected image transparent?
			true); // Is unselected image transparent?

		// Get the width of the display. We use this
		// to size our widgets.
		int displayWidth = EXTENT_X(maGetScrSize());

		// Create the screen's main widget -- a layout with
		// one column and four rows. Because this is the top-level
		// widget (parent=NULL) we don't need to set its position,
		// width, or height: it will fill the entire screen.
		// As it is a member variable (instance variable) that will
		// be used in other places in this class we will give its
		// name an "m" prefix.
		mMainLayoutWidget = new MAUI::Layout(
			0,  // Left coordinate.
			0,  // Top coordinate.
			0,  // Width.
			0,  // Height.
			NULL, // Parent widget.
			1,  // Number of columns in the layout.
			4); // Number of rows in the layout.

		// Add the layout as the root of the screen's widget tree.
		setMain(mMainLayoutWidget);

		// Next, we create four child widgets within the main layout widget.

		// The first widget is a label that we'll use to present instructions.
		// We don't need to specify the position of the widget, positioning
		// will be handled by the layout.
		mInstructions = new MAUI::Label(
			0, // Left.
			0, // Top.
			displayWidth, // Width.
			60, // Height.
			mMainLayoutWidget,   // Parent widget.
			"Enter a password:", // Text label.
			0x000000, // Background color.
			font);    // Font.

		// We want the instructions label to be able to display
		// multiple lines.
		mInstructions->setMultiLine(true);

		// The second widget is an edit box that we'll use to input
		// text. We set its multiline property to "false" because we
		// just want it to be one line. (For an EditBox, the multiline
		// property can be set both by providing it as an initial
		// attribute (as here) and through the "setMultiLine" method.)
		mPasswordBox = new MAUI::EditBox(
			0, // Left.
			0, // Top.
			displayWidth, // Width.
			30, // Height.
			mMainLayoutWidget, // Parent widget.
			"",    // Initial text content.
			0x555555, // Background color.
			font,  // Font.
			true,  // Respond to keypresses when selected.
			false, // Not multiline.
			60);   // Max number of characters in the edit box.

		// Make the EditBox listen to key events so that its content
		// gets updated when keys are pressed.
		mPasswordBox->activate();

		// Use password mode to hide text after entry.
		mPasswordBox->setPasswordMode(true);

		// The third widget is the Clear button. We use a label
		// for as a button widget.
		mClearButton = new MAUI::Label(
			0, // Left.
			0, // Top.
			displayWidth, // Width.
			30, // Height.
			mMainLayoutWidget, // Parent widget.
			"Clear", // Button text.
			0,     // Background color (we will use the skin).
			font); // Font.

		// Set the skin of the widget to make it look like a button.
		mClearButton->setSkin(buttonSkin);

		// Centre the button text horizontally and vertically.
		mClearButton->setHorizontalAlignment(MAUI::Label::HA_CENTER);
		mClearButton->setVerticalAlignment(MAUI::Label::VA_CENTER);

		// The fourth widget is the Submit button.
		mSubmitButton = new MAUI::Label(
			0, // Left.
			0, // Top.
			displayWidth, // Width.
			30, // Height.
			mMainLayoutWidget, // Parent widget.
			"Submit", // Button text.
			0,     // Background color (we will use the skin).
			font); // Font.

		// Set the skin of the widget to make it look like a button.
		mSubmitButton->setSkin(buttonSkin);

		// Centre the button text horizontally and vertically.
		mSubmitButton->setHorizontalAlignment(MAUI::Label::HA_CENTER);
		mSubmitButton->setVerticalAlignment(MAUI::Label::VA_CENTER);

		// Now the user interface widgets are created.
	}

	// We need to define two methods to make the buttons work
	// with touch events. To do this we overload (i.e. create
	// our own implementation of) the pointer event methods
	// in the Screen base class.

	/**
	 * This method is called on touch down events. Here we provide
	 * visual feedback that show the buttons in selected state.
	 */
	void pointerPressEvent(MAPoint2d point)
	{
		// If the Clear button is clicked...
		if (mClearButton->contains(point.x, point.y))
		{
			// ...set the button's state to "selected". This
			// automatically changes the button's skin.
			mClearButton->setSelected(true);
		}
		else
		// Do the same for the Submit button.
		if (mSubmitButton->contains(point.x, point.y))
		{
			mSubmitButton->setSelected(true);
		}
	}

	/**
	 * This method is called on touch up events. Here we restore
	 * the buttons to the unselected state, and perform button actions.
	 */
	void pointerReleaseEvent(MAPoint2d point)
	{
		// Deselect the buttons so that the "unselected" skins
		// are redisplayed.
		mClearButton->setSelected(false);
		mSubmitButton->setSelected(false);

		// If the pointer is inside the Clear button we call the
		// action method for the button.
		if (mClearButton->contains(point.x, point.y))
		{
			clearButtonClicked();
		}

		// If the pointer is inside the Submit button we call the
		// action method for the button.
		if (mSubmitButton->contains(point.x, point.y))
		{
			submitButtonClicked();
		}

		// If the edit box was touched we display a textbox.
		// This primarily intended for devices that do not
		// have a key pad for text input.
		if (mPasswordBox->contains(point.x, point.y))
		{
			editBoxClicked();
		}
	}

	/**
	 * This method is called when the Clear button is clicked.
	 */
	void clearButtonClicked()
	{
		// Empty the contents of the edit box.
		mPasswordBox->clearText();
	}

	/**
	 * This method is called when the Submit button is clicked.
	 */
	void submitButtonClicked()
	{
		// Get the text of the edit box and check that the password
		// is at least 6 characters long.
		MAUtil::String passord = mPasswordBox->getText();
		checkPasswordLength(passord.length());
	}

	/**
	 * Here we validate the length of the password and display
	 * a message to provide feedback.
	 */
	void checkPasswordLength(int length)
	{
		if (length < 6)
		{
			// If the validation failed, update the instructions label.
			// Note that this string is a lot longer than the original
			// caption -- which is why we needed the label to have the
			// multiline setting. Note too, how C automatically
			// concatenates strings split over multiple lines.
			mInstructions->setCaption(
				"Password too short. "
				"Please enter a password of at least 6 characters:");
		}
		else
		{
			// The password is at least 6 characters long,
			// we congratulate user.
			mInstructions->setCaption("Password OK");
		}
	}

	/**
	 * This method is called when the Password edit box is clicked.
	 */
	void editBoxClicked()
	{
		// Open a text box input screen. This input method
		// is especially useful if the device has no physical
		// keypad/keyboard, in which case a virtual keyboard
		// is displayed. Note that maTextBox uses wide strings
		// (16 bytes per character). When the user closes the
		// textbox, the textBoxClosed method is called.
		maTextBox(
			L"Enter a password:", // Title as a wide string.
			L"", // Initial text as a wide string.
			mTextBoxBuffer, // Input text buffer.
			256, // Buffer length.
			MA_TB_FLAG_PASSWORD); // Password input mode.
	}

	/**
	 * This method is called when the user closes a textbox
	 * opened by calling maTextBox.
	 */
    void textBoxClosed(int result, int textLength)
    {
		// If the user press Ok in the textbox...
    	if (MA_TB_RES_OK == result)
    	{
			// ...check the length of the text and provide user feedback.
			checkPasswordLength(textLength);

			// Note: If you wish to use the text the user entered
			// you probably need to convert the wide string in
			// mTextBoxBuffer to a byte string. This is a very
			// simple way of doing this that will only work for
			// the basic ASCII character set.
			char buffer[256];
			int i;
			for (i = 0; i < textLength; ++i)
			{
				// Convert to byte size char.
				buffer[i] = (char) mTextBoxBuffer[i];
			}
			// Null terminate string.
			buffer[i] = 0;

			// Uncomment this line to display the text in
			// the instructions label.
			//mInstructions->setCaption(buffer);
    	}
    	else
    	{
    		// The user pressed cancel, we handle this
    		// as a password of length zero.
    		checkPasswordLength(0);
    	}
    }

private:
	// Here we put the instance variables (member variables
	// used in this class. These variables can be accessed
	// by all methods in the class.

	/** The main layout that holds the other widgets. */
	MAUI::Layout * mMainLayoutWidget;

	/** A text label that displays instructions. */
	MAUI::Label * mInstructions;

	/** Text editor box for user input. */
	MAUI::EditBox * mPasswordBox;

	/** The Clear button. */
	MAUI::Label * mClearButton;

	/** The Submit button. */
	MAUI::Label * mSubmitButton;

	/** Buffer that holds the result from the maTextBox syscall. */
	wchar_t mTextBoxBuffer[256];
};

// That's the screen class finished, now we move on to the Moblet that
// is the main object in the application.

/**
 * A Moblet is the main object of MoSync application. In the Moblet
 * we manage the application and handle events. Our Moblet inherits
 * the Moblet base class in the MAUtil library.
 */
class HelloMAUIMoblet : public MAUtil::Moblet
{
public:
	/**
	 * The user interface is created in the constructor method.
	 * (Note that constructors and destructors have the same
	 * name as the class.)
	 */
	HelloMAUIMoblet()
	{
		// Create the main (and only) screen used in the application.
		mAppScreen = new AppScreen();

		// Make the screen listen to events generated by the
		// maTextBox syscall. (See the AppScreen class for
		// further details.)
		addTextBoxListener(mAppScreen);

		// Display the screen.
		mAppScreen->show();
	}

	/**
	 * The destructor deletes the screen object.
	 */
	virtual ~HelloMAUIMoblet()
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

// Here is the real start of our program.
extern "C" int MAMain()
{
	// Create and run the Moblet to start the application.
	MAUtil::Moblet::run(new HelloMAUIMoblet());

	// The Moblet will run until it is closed by the user.
	// When it is closed we end our program in a well-behaved
	// way, returning zero.
	return 0;
}
