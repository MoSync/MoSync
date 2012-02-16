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
 * @file StackScreen.h
 * @author Bogdan Iusco.
 * @brief Application's stack screen class.
 */

#ifndef APPSTACKSCREEN_H_
#define APPSTACKSCREEN_H_

#include <NativeUI/Widgets.h>

#include "HomeScreenListener.h"

using namespace NativeUI;

// Forward declaration
class HomeScreen;
class AppTabScreen;

/**
 * @brief Application's stack screen class.
 */
class AppStackScreen:
	public StackScreen,
	public HomeScreenListener
{
public:
	/**
	 * Constructor.
	 */
	AppStackScreen();

	/**
	 * Destructor.
	 */
	~AppStackScreen();

private:
	/**
	 * Show the tab screen.
	 */
	void showTabScreen();

private:
	/**
	 * First screen shown by stack screen.
	 */
	HomeScreen* mHomeScreen;

	/**
	 * Second screen shown by stack screen.
	 */
	AppTabScreen* mTabScreen;
};

#endif /* APPSTACKSCREEN_H_ */
