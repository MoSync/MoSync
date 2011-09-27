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
 * @file Main.cpp
 * @author Mikael Kindborg
 *
 * This is the main entry point for the example application
 * that demonstrates NativeUI on MoSync.
 */
#include <ma.h> 				// Syscalls
#include <MAUtil/String.h>		// C++ String class
#include <MAUtil/Moblet.h>		// Moblet class
#include <conprint.h>			// lprintfln for logging

using namespace MAUtil;

/**
 * Moblet for the  application.
 */
class TestMoblet : public Moblet
{
public:
	/**
	 * Constructor.
	 */
	TestMoblet()
	{
		// Call the maAlert syscall with all 3 buttons.
		maAlert("Title", "This is a message", "Ok", "Maybe", "Cancel");
	}

	/**
	 * Destructor.
	 */
	virtual ~TestMoblet()
	{
	}

	/**
	 * This method is called when the application is closed.
	 */
	void TestMoblet::closeEvent()
	{
		// Exit the app.
		close();
	}

	/**
	* This function is called when an event that Moblet doesn't recognize is recieved.
	*/
	void TestMoblet::customEvent(const MAEvent& event)
	{
		if ( event.type == EVENT_TYPE_ALERT)
		{
//			printf("\n =========== Alert Event received ======\n");
			MAUtil::String temp = "";
			switch( event.alertButtonIndex)
			{
			case 0:
				temp += "First ";
				break;
			case 1:
				temp += "Second ";
				break;
			case 2:
				temp += "Third ";
				break;
			default:
				temp = "err";
			}
			temp += " button was clicked";
//			maMessageBox("Alert Event received", temp.c_str());
			printf(temp.c_str());
			printf("\n ------------- This was all ------------- \n");
		}

	}

	/**
	 * Method called when a key is pressed.
	 */
	void TestMoblet::keyPressEvent(int keyCode, int nativeCode)
	{
	    // Close the application if the back key is pressed.
	    if(MAK_BACK == keyCode)
	    {
	        closeEvent();
	    }
	}
};

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	// Create a moblet.
	TestMoblet* moblet = new TestMoblet();

	// Run the moblet event loop.
	Moblet::run(moblet);

	// Deallocate objects.
	delete moblet;

	return 0;
}
