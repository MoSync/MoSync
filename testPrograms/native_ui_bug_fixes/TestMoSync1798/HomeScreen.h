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
 * @file HomeScreen.h
 * @author Bogdan Iusco.
 * @brief Application's home screen class.
 * Contain a button that shows the tab screen.
 */

#ifndef HOMESCREEN_H_
#define HOMESCREEN_H_

#include <NativeUI/Widgets.h>

using namespace NativeUI;
using namespace MAUtil;

// Forward declaration.
class AppTabScreen;
class HomeScreenListener;

/**
 * @brief Application's home screen class.
 * Contain a button that shows the tab screen.
 */
class HomeScreen:
	public Screen,
	public ButtonListener
{
public:
	/**
	 * Contructor.
	 * @param listener Notifier for this class.
	 */
	HomeScreen(HomeScreenListener* listener);

	/**
	 * Destructor.
	 */
	~HomeScreen();

private:
	/**
	 * Create screen's widgets.
	 */
	void createUI();

    /**
     * This method is called if the touch-up event was inside the
     * bounds of the button.
     * Platform: iOS and Android.
     * @param button The button object that generated the event.
     */
    virtual void buttonClicked(Widget* button);
private:
	/**
	 * A button used for showing the tab screen.
	 */
	Button* mShowTabScreenButton;

	/**
	 * Notifies when show tab screen button was clicked.
	 */
	HomeScreenListener* mListener;
};


#endif /* HOMESCREEN_H_ */
