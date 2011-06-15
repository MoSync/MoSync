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

#include <MAUtil/Vector.h>
#include "UIWrapper/Widgets.h"

using namespace MoSync::UI;

/**
 * The main screen for the color list.
 */
class ScreenColorList : public StackScreen
{
public:
	/**
	 * Create the UI for the color list screen.
	 * @return The main screen for the color list UI
	 * (an instance of this class).
	 */
	static StackScreen* create();

	/**
	 * Constructor.
	 */
	ScreenColorList();

	/**
	 * Destructor.
	 */
	virtual ~ScreenColorList();

	/**
	 * Create the UI for the color list screen.
	 */
	void createUI();

	/**
	 * Create a colored image.
	 * @param color The color of the image (a hex value).
	 * @param width The width of the image.
	 * @param height The height of the image.
	 * @return Handle to the image. The image needs to be
	 * deallocated with maDestoryObject.
	 */
	MAHandle createColorImage(int color, int width, int height);

	/**
	 * This method is called when an item is clicked in the list widget.
	 * @param listItemIndex The index of the list item that was clicked.
	 */
	void openColorScreen(int listItemIndex);
};

#endif
