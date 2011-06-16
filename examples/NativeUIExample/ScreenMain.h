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
 * @file ScreenMain.h
 * @author Mikael Kindborg
 *
 * This is the main entry point of the user interface
 * of the application. Here the application screens
 * are created and connected to the main tab screen.
 */

#ifndef SCREEN_MAIN_H_
#define SCREEN_MAIN_H_

#include "UIWrapper/Widgets.h"

#define COLOR_TAB		0
#define WEB_TAB			1
#define SWIPER_TAB		2
#define CUBE_TAB		3

using namespace MoSync::UI;

/**
 * This is the main screen of the application.
 */
class ScreenMain : public TabScreen
{
public:
	/**
	 * Constructor.
	 */
	ScreenMain();

	/**
	 * Destructor.
	 */
	virtual ~ScreenMain();

	/**
	 * Handle keypresses.
	 */
	virtual void handleKeyPress(int keyCode) = 0;

	/**
	 * Handle pointer presses.
	 */
	virtual void handlePointerPressed(MAPoint2d p) = 0;

	/**
	 * Handle pointer moves.
	 */
	virtual void handlePointerMoved(MAPoint2d p) = 0;

	/**
	 * Handle pointer releases.
	 */
	virtual void handlePointerReleased(MAPoint2d p) = 0;


	/**
	 * Create UI with four tabs
	 */
	static ScreenMain* createFourTabUI();

	/**
	 * Create UI with five tabs
	 */
	static ScreenMain* createFiveTabUI();
};

#endif
