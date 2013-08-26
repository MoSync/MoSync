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
 * @file MainScreen.h.
 * @author Bogdan Iusco.
 */

#ifndef MAINSCREEN_H_
#define MAINSCREEN_H_

#include <maapi.h>
#include <MAUtil/util.h>

// Include all the wrappers.
#include <NativeUI/Widgets.h>

using namespace NativeUI;

class MainScreen:
	public StackScreen,
	private ButtonListener,
	private StackScreenListener
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

private:

    virtual void createFirstScreen();
    virtual void createSecondScreen();
    virtual void createThirdScreen();

    /**
     * This method is called when there is an touch-down event for
     * a button.
     * @param button The button object that generated the event.
     */
    virtual void buttonPressed(Widget* button) {};

    /**
     * This method is called when there is an touch-up event for
     * a button.
     * @param button The button object that generated the event.
     */
    virtual void buttonReleased(Widget* button) {};

    /**
     * This method is called if the touch-up event was inside the
     * bounds of the button.
     * @param button The button object that generated the event.
     */
    virtual void buttonClicked(Widget* button);

    /**
     * This method is called when a screen has been popped from a stack
     * screen.
     * @param stackScreen The stack screen object that generated the event.
     * @param fromScreen The screen that was popped from the stack screen.
     * @param toScreen The screen that will be shown.
     */
    virtual void stackScreenScreenPopped(
        StackScreen* stackScreen,
        Screen* fromScreen,
        Screen* toScreen);

private:
    Screen* mFirstScreen;
    Screen* mSecondScreen;
    Screen* mThirdScreen;
    Button* mPushButton;
    Button* mSecondPushButton;
};

#endif /* MAINSCREEN_H_ */
