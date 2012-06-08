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
 * @file MainScreen.h
 * @author emma
 */

#ifndef MAINSCREEN_H_
#define MAINSCREEN_H_

#include <maapi.h>

#include <MAUtil/util.h>

// Include all the wrappers.
#include <NativeUI/Widgets.h>

#include "FirstScreen.h"
#include "SecondScreen.h"
#include "ThirdScreen.h"

using namespace NativeUI;

/**
 * Class that creates a screen that displays all the contacts.
 */
class MainScreen:
	public TabScreen
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
	FirstScreen* mFirstScreen;
	SecondScreen* mSecondScreen;
	ThirdScreen* mThirdScreen;
};


#endif /* MAINSCREEN_H_ */
