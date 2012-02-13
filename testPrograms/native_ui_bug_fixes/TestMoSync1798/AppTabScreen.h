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
 * @file AppTabScreen.h
 * @author Bogdan Iusco.
 * @brief Application's tab screen class.
 */

#ifndef APPTABSCREEN_H_
#define APPTABSCREEN_H_

#include <NativeUI/Widgets.h>

using namespace NativeUI;

/**
 * Application's tab screen class.
 */
class AppTabScreen:
	public TabScreen
{
public:

	/**
	 * Constructor.
	 */
	AppTabScreen();

	/**
	 * Destructor.
	 */
	~AppTabScreen();

private:
	/**
	 * Create and initialize first screen.
	 */
	void createFirstScreen();

	/**
	 * Create and initialize second screen.
	 */
	void createSecondScreen();

private:
	/**
	 * First screen in tab screen.
	 */
	Screen* mFirstScreen;

	/**
	 * Second screen in tab screen.
	 */
	Screen* mSecondScreen;
};

#endif /* APPTABSCREEN_H_ */
