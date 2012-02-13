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
