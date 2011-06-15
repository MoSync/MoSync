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
 * @file ScreenColorList.cpp
 * @author Mikael Kindborg
 *
 * Screen that displays a list of colors. When selecting an item
 * in the list, another screen with that color is displayed.
 */

#include "MAHeaders.h"
#include "ScreenColorList.h"

struct ColorData
{
	const char* name;
	int color;
};

static ColorData sColors[] =
{
	{ "Red", 0xff0000 },
	{ "Green", 0x00ff00 },
	{ "Blue", 0x0000ff },
	{ "Air Force Blue", 0x5d8aa8 },
	{ "Amber", 0xFFBF00 },
	{ "Aqua", 0x00FFFF },
	{ "Bright Pink", 0xfb607f },
	{ "Cadet", 0x536872 },
	{ "Cadmium Yellow", 0xFFF600 },
	{ "Capri", 0x00BFFF },
	{ "Caribbean green", 0x00CC99 },
	{ "Chartreuse", 0xDFFF00 },
	{ "Crimson", 0xDC143C },
	{ "Dark orange", 0xFF8C00 },
	{ "Dark spring green", 0x177245 },
	{ "Debian red", 0xD70A53 }
};

static const int sNumColors = sizeof(sColors) / sizeof(ColorData);


/**
 * Create the UI for the color list screen.
 * @return The main screen for the color list UI.
 */
Screen* ScreenColorList::create()
{
	// Create the color screen.
	Screen* screen = new Screen();
	screen->setTitle("Colors");
	screen->setIcon(RES_TAB_ICON_COLORS);

	// Create the list view widget.
	ListView* listView = new ListView();

	// Add items to the list view.
	for (int i = 0; i < sNumColors; i++)
	{
		ListViewItem* colorItem = new ListViewItem();
		colorItem->setText(sColors[i].name);
		colorItem->setBackgroundColor(sColors[i].color);
		listView->addChild(colorItem);
	}

	// Add the list view to the screen.
	screen->setMainWidget(listView);

	// Return the list view.
	return screen;
}
