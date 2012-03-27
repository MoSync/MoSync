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
 * @file SecondScreen.h.
 * @author emma
 */

#ifndef SECONDSCREEN_H_
#define SECONDSCREEN_H_

#include <maapi.h>
#include <MAUtil/util.h>

// Include all the wrappers.
#include <NativeUI/Widgets.h>

using namespace NativeUI;

class SecondScreen:
	public Screen,
	public ScreenListener
{

public:
	/**
	 * Constructor.
	 */
	SecondScreen();

	/**
	 * Destructor.
	 */
	~SecondScreen();

private:

    /**
     * This method is called when the OptionsMenu is being closed
     * (either by the user canceling the menu with the back/menu
     * button, or when an item is selected.
     * @param Screen The screen that generated the event.
     */
    virtual void optionsMenuClosed(Screen* screen);

    /**
     * This method is called when an options menu item is selected.
     * @param index The index on which the item is placed into the
     * OptionsMenu.
     *  @param Screen The screen that generated the event.
     */
    virtual void optionsMenuItemSelected(Screen* screen, int index);

private:
	/**
	 * Main layout.
	 */
	VerticalLayout* mMainLayout;
	ListView* mEventsList;
};

#endif /* SECONDSCREEN_H_ */
