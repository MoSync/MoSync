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

/** @file main.cpp
*
* This application provides a very basic example of how to use MoSync's Moblet
* framework to detect key events and use timers. It displays the text "Hello
* Moblet!" The text can be moved around using the joystick or you can click on
* the screen and the text will jump to that position. The code is
* very well commented so that you can see what's happening at each step.
*
* @author The MoSync Team
*/

//Include the header files for MoSync Moblets so that we can
//access the Moblet library code from our application.
#include <MAUtil/Moblet.h>

//Define how many pixels will be traversed each timer tick.
#define SPEED 1

//Create the wrapper for the entire application. It is here that we will manage
//the application and handle events. To create the wrapper we make our own
//implementation of the MoSync Moblet base class. There can be only one Moblet
//in an application. We also make our application wrapper inherit from
//MAUtil::TimerListener so that it can handle timer events.
class MyMoblet : public MAUtil::Moblet, MAUtil::TimerListener
{

//Define our new class's public methods.
public:

	//First, the constructor, which we will call when we need an instance
	//of MyMoblet. We set some initial values for the parameters mDirX, mDirY,
	//mX and mY which we will be using to store the direction of movement and
	//the current location. As they are member variables, they have an "m"
	//prefix; their declaration can be found in the private section of the class.
	MyMoblet() : mDirx(0), mDirY(0), mX(30), mY(40)
	{
		//Initialize the mText variable with our display string.
		mText = "Hello Moblet!";

		//Get the screen dimensions using the maGetScrSize syscall
		//and save it in a local variable called "screenSize". As it is a local
		//variable it has no "m" prefix, but we do need to declare its
		//data type: MAExtent.
		MAExtent screenSize = maGetScrSize();

		//MAExtent is actually a special type of integer value that contains
		//both the height and width of the screen. We can extract the dimensions
		//using the macros EXTENT_X() and EXTENT_Y().
		mScreenWidth = EXTENT_X(screenSize);
		mScreenHeight = EXTENT_Y(screenSize);

		//Call the draw() function to display the text on the screen. The draw()
		//function is defined later in this class.
		draw();
	}

	//Next, the method that gets called when a key is pressed. This is a method
	//we have inherited from the Moblet base class, and here we will override
	//that method with some processing of our own.
	void keyPressEvent(int keyCode, int nativeCode)
	{

		//We'll use the C/C++ "switch" statement to select between different
		//possible values of the keyCode that has been passed to our application
		//by the runtime and record the movement in a direction variable.
		switch(keyCode) {
			case MAK_LEFT:	mDirx = -1;	break;
			case MAK_RIGHT: mDirx = 1;	break;
			case MAK_UP:	mDirY = -1;	break;
			case MAK_DOWN:	mDirY = 1;	break;

			//If the key pressed is the soft-right key OR the "0" key, just
			//exit the application.
			case MAK_SOFTRIGHT:
			case MAK_0:
				maExit(0);

			//If it is any other key, do nothing.
			default:
				return;
		}

		//Add a timer that, every 20 milliseconds, generates a timer event
		//and send it to the TimerListener. Remember that MyMoblet is itself the
		//TimerListener, hence it refers to itself ("this"). The timer will run
		//indefinitely (-1) until it is removed in response to a keyReleaseEvent.
		addTimer(this, 20, -1);
	}

	//Now, the method that gets called when a key is released which we will use
	//to reset our movement and location parameters, and remove the timer.
	void keyReleaseEvent(int keyCode, int nativeCode)
	{

		//We only need to reset the direction if it is the current direction.
		switch(keyCode) {
			case MAK_LEFT:	if(mDirx == -1) mDirx = 0; break;
			case MAK_RIGHT: if(mDirx ==  1) mDirx = 0; break;
			case MAK_UP:	if(mDirY == -1) mDirY = 0; break;
			case MAK_DOWN:	if(mDirY ==  1) mDirY = 0; break;
			default:
				return;
		}

		//Here we remove the timer created by the keyPressEvent if nothing is
		//still moving.
		if(mDirx == 0 && mDirY == 0)
			removeTimer(this);
	}

	//Next, the method that gets called when a pointer is pressed on the screen.
	//The MoSync runtime supplies the pointer coordinates in the parameter "p",
	//from which we can extract the individual x and y coordinates.
	//Why virtual? !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	virtual void pointerPressEvent(MAPoint2d p)
	{
		mX = p.x;
		mY = p.y;

		//Use the draw function to redraw the screen.
		draw();
	}

	//Similarly for a pointer movement to another location on the screen.
	//Why virtual? !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	virtual void pointerMoveEvent(MAPoint2d p)
	{
		mX = p.x;
		mY = p.y;
		draw();
	}

	//We don't need to do anything when the pointer is lifted.
	//Why virtual? !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	virtual void pointerReleaseEvent(MAPoint2d p)
	{
	}

	//Now the function that will draws the background and our text. It is
	//made as a separate function because both MyMoblet's constructor and its
	//timer callback need to call it.
	void draw()
	{
		//We use a set of syscalls to do the drawing. First we set the background
		//colour and paint the screen. Then we reset the drawing colour and draw
		//the text.
		maSetColor(0);
		maFillRect(0, 0, mScreenWidth, mScreenHeight);
		maSetColor(0xffffff);
		maDrawText(mX, mY, mText);

		//We've been painting and drawing to the backbuffer, so now we need to
		//show our work on the screen.
		maUpdateScreen();
	}

	//Finally in our MyMoblet class we define the Timer callback. This is the
	//function that gets called whenever our timer (see addTimer, above)
	//generates an event. While the key is pressed, the timer is sending events
	//50 times per second, thus we are redrawing the screen 50 frames-per-second.
	void runTimerEvent()
	{
		//Get the dimensions of our text using the maTextScrSize syscall and
		//extract the width and height in pixels using the EXTENT_X() and
		//EXTENT_Y() macros.
		MAExtent stringSize = maGetTextSize(mText);
		int stringSizeX = EXTENT_X(stringSize);
		int stringSizeY = EXTENT_Y(stringSize);

		//Check if the text location is inside the boundaries of the screen,
		//and if not reset it so that it is.
		if((mX + stringSizeX) > mScreenWidth)  mX = mScreenWidth - stringSizeX;
		if((mY + stringSizeY) > mScreenHeight) mY = mScreenHeight - stringSizeY;
		if(mX < 0) mX = 0;
		if(mY < 0) mY = 0;

		//Call the function that draws the background and our text string.
		draw();

		//Record the new location of our text string.
		mX += mDirx * SPEED;
		mY += mDirY * SPEED;
	}

//Define our new class's private methods.
private:
	//Here we declare the data types of our member variables so that the
	//constructor, event handlers, etc. can work with them.
	const char *mText;
	int mDirx, mDirY;
	int mX, mY;
	int mScreenWidth, mScreenHeight;
};

//Now we get to the entry point for the application - the place where
//processing starts.
extern "C" int MAMain()
{
	//Create the instance of MyMoblet.
	MyMoblet myMoblet;

	//Run the Moblet to start the application.
	MyMoblet::run(&myMoblet);

	//MyMoblet will run until it is closed by the user pressing key 0. When
	//it's closed we end our program in a well-behaved way by returning zero.
	return 0;
}
