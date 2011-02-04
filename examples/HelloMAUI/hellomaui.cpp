/* Copyright (C) 2010 MoSync AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

/** @file hellomaui.cpp
*
* This application provides a very basic example of how to work with MAUI
* to position and and manipulate graphical user interface elements. The code is
* very well commented so that you can see what's happening at each step.
* The application uses one screen and a some widgets to provide a password
* entry box and some control buttons. For more advanced use of MAUI, see our
* MAUIEx example application.
*
* @author Chris Hughes
*/


//Include the header files for MoSync Moblets and MAUI widgets so that we can
//access them from our application.
#include <MAUtil/Moblet.h>
#include <MAUI/Screen.h>
#include <MAUI/Label.h>
#include <MAUI/Layout.h>
#include <MAUI/EditBox.h>
#include <MAUI/Font.h>
//Include also the header file for the application's font and image resources
//which are defined in the project file called "res.lst".
#include "MAHeaders.h"

//Define a new class to use as a blueprint for all our application's screens,
//building it upon the MAUI Screen base class. We'll only be using one screen
//instance in this application, but you could have multiple instances in your
//application, perhaps inheriting their characteristics from other screen
//classes in a structured hierarchy.
class AppScreen : public MAUI::Screen
{
//Define the public methods of our new class.
public:

	//First, the constructor, which will be called whenever we need an
	//instance of AppScreen.
	AppScreen()
	{
		//Set up some pointers to the font and widget-skins
		//defined in the project file "res.lst".
		MAUI::Font * myFont = new MAUI::Font(RES_FONT);
		MAUI::WidgetSkin * myBack = new MAUI::WidgetSkin(RES_SELECTED, RES_UNSELECTED,
			16, 32, 16, 32, true, true);

		//Create the screen's main widget -- a layout with one column and four
		//rows. Because it is the top-level widget (parent=NULL) we don't need
		//to set its position, width, or height: it will fill the entire screen.
		//As it is a member variable that will be used in other places in this
		//class we will give its name an "m" prefix.
		mMainLayoutWidget = new MAUI::Layout(0, 0, 0, 0, NULL, 1, 4);

		//Identify the main layout widget as root of the screen's widget tree.
		setMain(mMainLayoutWidget);

		//Add a top margin of 20 pixels to each cell of the main layout so
		//that child widgets are nicely spaced. To do this we call the method
		//"setMarginY" which is available for all Layout widgets. (In the IDE,
		//see Help > MoSync API Reference > Classes > MAUI::Layout).
		mMainLayoutWidget->setMarginY(20);

		//Now we create four child widgets within the main layout widget.

		//The first widget is a label that we'll use to present instructions.
		//We'll use our bitmap font to display the text.
		mInstructions = new MAUI::Label(0, 0, 150, 60, mMainLayoutWidget,
			"Enter a password:", 0x000000, myFont);

		//Set the instructions label to be multiline. We will need this
		//property later.
		mInstructions->setMultiLine(true);

		//The second widget is an edit box that we'll use to capture text.
		//We set its multiline property to "false" because we just want it
		//to be one line. (For an EditBox, the multiline property
		//can be set both by providing it as an initial attribute (as here)
		//and through the "isMultiLine" method.)
		mPasswordBox = new MAUI::EditBox(0, 0, 150, 30, mMainLayoutWidget,
			"", 0x555555, myFont, true, false, 60);

		//Activate the EditBox as a KeyListener so that its content gets updated
		//when keys are pressed.
		mPasswordBox->activate();

		//Use password mode to hide text after entry.
		mPasswordBox->setPasswordMode(true);

		//The third and fourth widgets are labels that we will use as buttons.
		mClearButton = new MAUI::Label(0, 0, 150, 30, mMainLayoutWidget,
			"Clear", 0, myFont);
		mSubmitButton = new MAUI::Label(0, 0, 150, 30, mMainLayoutWidget,
			"Submit", 0, myFont);

		//Skin mClearButton and mSubmitButton to make them look like buttons.
		mClearButton->setSkin(myBack);
		mSubmitButton->setSkin(myBack);

		//Centre the button texts horizontally and vertically.
		mClearButton->setHorizontalAlignment(MAUI::Label::HA_CENTER);
		mClearButton->setVerticalAlignment(MAUI::Label::VA_CENTER);
		mSubmitButton->setHorizontalAlignment(MAUI::Label::HA_CENTER);
		mSubmitButton->setVerticalAlignment(MAUI::Label::VA_CENTER);

	//That's the constructor finished.
	}

	//Now we need some methods to make the mClearButton and mSubmitButton buttons
	//work. To do this we overload (i.e. create our own implementation of) some
	//of the MAUI::Screen base class's pointer listeners.

	//First, let's give the user some feedback if the buttons are clicked.
	//To do this we overload the pointer press listener. This method is called
	//whenever the pointer is pressed on the screen.
	void pointerPressEvent(MAPoint2d point)
	{

		//If the Clear button is clicked...
		if( mClearButton->contains( point.x, point.y ) )
		{
			//...set the button's state to "selected". This
			//automatically changes the button's skin.
			mClearButton->setSelected(true);
		}

		//Do the same for the Submit button.
		if( mSubmitButton->contains( point.x, point.y ) )
		{
			mSubmitButton->setSelected(true);
		}

	}

	//Now we define what happens when the pointer is lifted from the screen.
	void pointerReleaseEvent(MAPoint2d point)
	{

		//If the pointer is inside the Clear button...
		if( mClearButton->contains( point.x, point.y ) )
		{
			//...clear the edit box.
			mPasswordBox->clearText();
		}

		//If the pointer is inside the Submit button...
		if( mSubmitButton->contains( point.x, point.y ) )
		{
			//...check that the password is at least 6 characters long.
			//Here we read the current text in the EditBox using the
			//"getText" method and assigning it to "pw".
			const String &pw = mPasswordBox->getText( );
			if ( pw.length() < 6 )
			{
				//If the validation failed, update the instructions label.
				//Note that this string is a lot longer than the original
				//caption -- which is why we needed the label to have that
				//multiline setting. Note too, how C automatically
				//concatenates strings split over multiple lines.
				mInstructions->setCaption("Password too short. "
					"Please enter a password of at least 6 characters:");
			}
			else
			{
				mInstructions->setCaption("Password OK");
			}
		}

		//Let's also de-select the buttons so that the "unselected" skins
		//are re-displayed.
		mClearButton->setSelected(false);
		mSubmitButton->setSelected(false);

	}

	//The last method of the AppScreen class is the destructor, which will
	//be used to free up resources when the screen gets deleted by the
	//Moblet framework. In this case we just need to delete the main widget:
	//all its children will automatically be deleted too.
	~AppScreen()
	{
		delete mMainLayoutWidget;
	}
private:
	//Here we declare the data types of our widgets so that the constructor,
	//destructor, pointer and other methods of the class can work with them.
	MAUI::Layout * mMainLayoutWidget;
	MAUI::Label * mInstructions;
	MAUI::Label * mClearButton;
	MAUI::Label * mSubmitButton;
	MAUI::EditBox * mPasswordBox;
};

//That's the screen class finished, now we move on to the event handling
//wrapper for the entire application. To create the wrapper we make our own
//implementation of the MoSync Moblet base class.
class HelloMAUIMoblet : public Moblet
{
public:
	//The constructor method. (Note that constructors have the same
	//name as the class.)
	HelloMAUIMoblet()
	{
		//Create the main (and only) screen by calling AppScreen's constructor.
		screen = new AppScreen();
		//Display that screen.
		screen->show();
	}

	//Next, the method for detecting key presses, which we have inherited from
	//the Moblet base class. We will override this inherited method with some
	//processing of our own.
	void keyPressEvent(int keyCode, int nativeCode)
	{
		//Close the application if key 0 is pressed.
		if(keyCode == MAK_0)
		{
			close();
		}
	}

	//The destructor method.
	~HelloMAUIMoblet()
	{
		delete screen;
	}

//Here we declare the data types of our widgets so that the constructor,
//destructor and pointer methods can work with them.
private:
	AppScreen * screen;

};

// Here is the real start of our program.
extern "C" int MAMain()
{
	//Create an instance of HelloMAUIMoblet.
	HelloMAUIMoblet mainMoblet;

	//Run the Moblet to start the application.
	Moblet::run( &mainMoblet );

	//HelloMAUIMoblet will run until it is closed by the user pressing key 0.
	//When it's closed we end our program in a well-behaved way, returning zero.
	return 0;
};
