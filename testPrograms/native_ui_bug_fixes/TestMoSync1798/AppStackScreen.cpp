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
 * @file StackScreen.cpp
 * @author Bogdan Iusco.
 * @brief Application's stack screen class.
 */

#include "AppStackScreen.h"
#include "HomeScreen.h"
#include "AppTabScreen.h"

/**
 * Constructor.
 */
AppStackScreen::AppStackScreen():
	StackScreen()
{
	mHomeScreen = new HomeScreen(this);
	mTabScreen = new AppTabScreen();

	this->push(mHomeScreen);
	this->show();
}

/**
 * Destructor.
 */
AppStackScreen::~AppStackScreen()
{
	delete mHomeScreen;
	delete mTabScreen;
}

/**
 * Show the tab screen.
 */
void AppStackScreen::showTabScreen()
{
	this->push(mTabScreen);
}
