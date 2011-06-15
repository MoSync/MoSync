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
 * @file ScreenColorList.h
 * @author Mikael Kindborg
 *
 * Screen that displays a list of colors. When selecting an item
 * in the list, another screen with that color is displayed.
 */

#ifndef SCREEN_COLOR_LIST_H_
#define SCREEN_COLOR_LIST_H_

#include "UIWrapper/Widgets.h"

using namespace MoSync::UI;

/**
 * Class that creates the UI for the color list screen.
 */
class ScreenColorList
{
public:
	/**
	 * Create the UI.
	 */
	static Screen* create();
};

#endif
