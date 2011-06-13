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
 * @file MainUI.cpp
 * @author Mikael Kindborg
 *
 * This class handles the creation of the application user interface.
 */

#ifndef MAINUI_H_
#define MAINUI_H_

#include <ma.h>
#include <mavsprintf.h>
#include "UIWrapper/WidgetManager.h"
#include "UIWrapper/WidgetEventListener.h"
#include "UIWrapper/Screen.h"
#include "UIWrapper/Layout.h"
#include "UIWrapper/Button.h"
#include "MainUI.h"

/**
 * This class handles the creation of the application user interface.
 */
class MainUI
{
public:
	/**
	 * Create the user interface of the application.
	 */
	void createUI();
};

#endif
