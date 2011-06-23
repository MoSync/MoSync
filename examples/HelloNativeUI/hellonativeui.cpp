/* Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, write to the Free Software Foundation,
59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

/** @file hellonativeui.cpp
*
* This application provides a very basic example of how to work with Native UI
* to position and and manipulate graphical user interface elements. The code is
* very well commented so that you can see what's happening at each step.
* The application uses one screen and a some widgets to provide a password
* entry box and some control buttons.
*
* NOTE: this example application has the same functionality as our example
* application "HelloMAUI". Compare the two examples to see how we do the same
* things in MAUI and in Native UI.
*
* @author Mattias Frånberg & Chris Hughes
*/

//Include the header files for MoSync Moblets and Native UI extensions so that
//we can access their library code from our application. Include the enviroment
//so that we can listen for widget events.
#include <MAUtil/Moblet.h>
#include <MAUtil/Environment.h>
#include <IX_WIDGET.h>

//Define a new class to use as a blueprint for our application's screens.
class AppScreen : public MAUtil::CustomEventListener
{
//Define the public methods of our new class.
public:

	//First, the constructor, which will be called whenever we need an
	//instance of AppScreen.
	AppScreen()
	{
		//Make the AppScreen listen for events coming from widgets.
		MAUtil::Environment::getEnvironment().addCustomEventListener(this);

		//Create a Native UI screen using the IOCTL function maWidgetCreate.
		mScreen = maWidgetCreate(MAW_SCREEN);

		//Check if this is supported on the device it's running on
		if(mScreen == IOCTL_UNAVAILABLE)
		{
			maPanic(0,"Sorry but this program does not run currently on this platform");
		}
	   
		//Create the screen's main layout widget. As it is a member variable
		//we give its name an "m" prefix.
		mMainLayoutWidget = maWidgetCreate(MAW_VERTICAL_LAYOUT);
		maWidgetSetProperty(mMainLayoutWidget, MAW_WIDGET_WIDTH, "-1");
		maWidgetSetProperty(mMainLayoutWidget, MAW_WIDGET_HEIGHT, "-1");

		//Identify the main layout widget as root of the screen's widget tree.
		maWidgetAddChild(mScreen, mMainLayoutWidget);

		//Now we create four child widgets within the main layout widget.

		//The first widget is a label that we'll use to present instructions.
		//We set its width and height, and we initialize it with some text.
		//Finally we locate it inside the screen widget.
		mInstructions = maWidgetCreate(MAW_LABEL);
		maWidgetSetProperty(mInstructions, MAW_WIDGET_WIDTH, "-1");
		maWidgetSetProperty(mInstructions, MAW_WIDGET_HEIGHT, "-2");
		maWidgetSetProperty(mInstructions, MAW_LABEL_TEXT, "Enter a password:");
		maWidgetAddChild(mMainLayoutWidget, mInstructions);

		//The second widget is an edit box that we'll use to capture text. We use
		//password mode to hide text after entry. We locate the password box
		//inside the screen widget. It will appear after the instructions widget.
		mPasswordBox = maWidgetCreate(MAW_EDIT_BOX);
		maWidgetSetProperty(mPasswordBox, MAW_WIDGET_WIDTH, "-1");
		maWidgetSetProperty(mPasswordBox, MAW_WIDGET_HEIGHT, "-2");
		maWidgetSetProperty(mPasswordBox, "editMode", "password");
		maWidgetAddChild(mMainLayoutWidget, mPasswordBox);

		//The third and fourth widgets are labels that we will use as buttons.
		//We centre the text vertically and horizontally within the buttons.
		mClearButton = maWidgetCreate(MAW_BUTTON);
		maWidgetSetProperty(mClearButton, MAW_WIDGET_WIDTH, "-1");
		maWidgetSetProperty(mClearButton, MAW_WIDGET_HEIGHT, "-2");
		maWidgetSetProperty(mClearButton, MAW_BUTTON_TEXT_VERTICAL_ALIGNMENT, "center");
		maWidgetSetProperty(mClearButton, MAW_BUTTON_TEXT_HORIZONTAL_ALIGNMENT, "center");
		maWidgetSetProperty(mClearButton, MAW_BUTTON_TEXT, "Clear");
		maWidgetAddChild(mMainLayoutWidget, mClearButton);

		mSubmitButton = maWidgetCreate(MAW_BUTTON);
		maWidgetSetProperty(mSubmitButton, MAW_WIDGET_WIDTH, "-1");
		maWidgetSetProperty(mSubmitButton, MAW_WIDGET_HEIGHT, "-2");
		maWidgetSetProperty(mSubmitButton, MAW_BUTTON_TEXT_VERTICAL_ALIGNMENT, "center");
		maWidgetSetProperty(mSubmitButton, MAW_BUTTON_TEXT_HORIZONTAL_ALIGNMENT, "center");
		maWidgetSetProperty(mSubmitButton, MAW_BUTTON_TEXT, "Submit");
		maWidgetAddChild(mMainLayoutWidget, mSubmitButton);

	//That's the constructor finished.
	}

	//Now a method for showing the screen.
	void show()
	{
		//Show screen using the IOCTL function maWidgetShowScreen.
		maWidgetScreenShow(mScreen);
	}

	//Next a method for handling events from widgets that have been clicked.
	void widgetClicked(MAHandle widgetHandle)
	{
		//If the clear button was clicked...
		if(widgetHandle == mClearButton)
		{
			//...clear the edit box.
			maWidgetSetProperty(mPasswordBox, MAW_EDIT_BOX_TEXT, "");
		}
		//If the submit button was clicked...
		else if(widgetHandle == mSubmitButton)
		{
			//Declare a buffer for the text contained in the edit box, since
			//there is no high level API available.
			char passwordTextBuffer[256];

			//Get the text from the password box and put it into the buffer.
			int textLength = maWidgetGetProperty(mPasswordBox, MAW_EDIT_BOX_TEXT,
					passwordTextBuffer, 256);

			//If the text does not fit in the buffer, textLength will be set
			//to -1, therefore we need to check that the length is greater than
			//or equal to 0, otherwise we just ignore the event.
			if(textLength == MAW_RES_INVALID_STRING_BUFFER_SIZE)
			{
				return;
			}

			//Check that the password is at least 6 characters long.
			if(textLength < 6)
			{
				//If the password is too short we use the instructions label
				//to inform the user. Note how C automatically
				//concatenates strings split over multiple lines.
				maWidgetSetProperty(mInstructions, MAW_LABEL_TEXT, "Password too short. "
					"Please enter a password of at least 6 characters:");
			}
			else
			{
				//If the password is at least 6 characters long, we congratulate
				//user.
				maWidgetSetProperty(mInstructions, MAW_LABEL_TEXT, "Password OK");
			}
		}
	}

	//Right now there is no high level framework for widget events, so
	//we have to listen to them manually.
	void customEvent(const MAEvent& event)
	{
		//If the event does not come from a widget, we just ignore it.
		if(event.type != EVENT_TYPE_WIDGET)
		{
			return;
		}

		//Get the information sent by the widget.
		MAWidgetEventData *widgetEventData = (MAWidgetEventData *) event.data;

		//Check that the event was sent from a widget...
		if(widgetEventData->eventType == MAW_EVENT_CLICKED)
		{
			//...if so, passed it to the widgetClicked method.
			widgetClicked(widgetEventData->widgetHandle);
		}
		return;
	}

	//The last method of the AppScreen class is the destructor, it is used
	//to release the memory used by the native UI widgets. In this case we
	//just need to delete the main widget: all its children will automatically
	//be deleted too.
	virtual ~AppScreen()
	{
		maWidgetDestroy(mMainLayoutWidget);
	}

//Next, the private methods of our AppScreen class.
private:

	//Here we declare the data types of our widgets so that the constructor,
	//destructor, and other methods of the class can work with them.
	MAHandle mScreen;
	MAHandle mMainLayoutWidget;
	MAHandle mInstructions;
	MAHandle mClearButton;
	MAHandle mSubmitButton;
	MAHandle mPasswordBox;
};

//That's the screen class finished, now we move on to the event handling
//wrapper for the entire application. To create the wrapper we make our own
//implementation of the MoSync Moblet base class.
class HelloNativeUIMoblet : public MAUtil::Moblet
{
public:
	//The constructor method. (Note that constructors have the same
	//name as the class.)
	HelloNativeUIMoblet()
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
	~HelloNativeUIMoblet()
	{
		delete screen;
	}

//Here we declare the data types of our widgets so that the constructor,
//destructor and pointer methods can work with them.
private:
	AppScreen * screen;

};

//Here is the real start of our program.
extern "C" int MAMain()
{
	//Create an instance of HelloNativeUIMoblet.
	HelloNativeUIMoblet mainMoblet;

	//Run the Moblet to start the application.
	MAUtil::Moblet::run( &mainMoblet );

	//HelloNativeUIMoblet will run until it is closed by the user pressing key 0.
	//When it's closed we end our program in a well-behaved way, returning zero.
	return 0;
};
