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
#include "Util.h"
#include "ScreenColorList.h"

/**
 * Struct used to hold color data.
 */
struct ColorData
{
	const char* name;
	int color;
};

/**
 * Array of color data.
 */
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

/**
 * The number of colors in the array.
 */
static const int sNumColors = sizeof(sColors) / sizeof(ColorData);

/**
 * Constructor.
 */
ScreenColorList::ScreenColorList():
	StackScreen()
{
	createUI();
}

/**
 * Destructor.
 */
ScreenColorList::~ScreenColorList()
{
	mListView->removeListViewListener(this);
	// TODO: Deallocate the images we create!
}

/**
 * Create the UI for the color list screen.
 */
void ScreenColorList::createUI()
{
	// Set title and icon of the stack screen.
	setTitle("Colors");
	if (getPlatform() == 0)
	{
		setIcon(RES_TAB_ICON_COLORS_ANDROID);
	}
	else if(getPlatform() == 1)
	{
		setIcon(RES_TAB_ICON_COLORS);
	}

	// Create the top screen in the stack.
	Screen* screen = new Screen();

	// Also set the title of this screen.
	screen->setTitle("Colors");

	// Create the list view widget.
	mListView = new ListView();

	// Add items to the list view.
	for (int i = 0; i < sNumColors; i++)
	{
		ListViewItem* colorItem = new ListViewItem();
		colorItem->setText(sColors[i].name);
		// TODO: Fix hard coded value of image size.
		colorItem->setIcon(createColorImage(sColors[i].color, 40, 40));
		mListViewItems.add(colorItem);
		mListView->addChild(colorItem);
	}

	// Set event listener.
	mListView->addListViewListener(this);

	// Set the list view as the main widget of the screen.
	screen->setMainWidget(mListView);

	// Push the top screen.
	push(screen);
}

/**
 * Create a colored image.
 * @param color The color of the image (a hex value).
 * @param width The width of the image.
 * @param height The height of the image.
 * @return Handle to the image. The image needs to be
 * deallocated with maDestoryObject.
 */
MAHandle ScreenColorList::createColorImage(int color, int width, int height)
{
	MAHandle image = maCreatePlaceholder();
	maCreateDrawableImage(image, width, height);
	MAHandle previousDrawTarget = maSetDrawTarget(image);
	maSetColor(color);
	maFillRect(0, 0, width, height);
	maSetDrawTarget(previousDrawTarget);
	return image;
}

/**
 * This method is called when an item is clicked in the list widget.
 * @param listItemIndex The index of the list item that was clicked.
 */
void ScreenColorList::openColorScreen(int listItemIndex)
{
	// Create a screen that will show the color.
	Screen* screen = new Screen();

	// The title of this screen is displayed on the
	// navigation bar on iPhone/iPad.
	screen->setTitle(sColors[listItemIndex].name);
	VerticalLayout* widget = new VerticalLayout();
	widget->setBackgroundColor(sColors[listItemIndex].color);
	screen->setMainWidget(widget);
	this->push(screen);
}

/**
 * This method is called when a list view item is clicked.
 * @param listView The list view object that generated the event.
 * @param listViewItem The ListViewItem object that was clicked.
 */
void ScreenColorList::listViewItemClicked(
    ListView* listView,
    ListViewItem* listViewItem)
{
	if ( listView == mListView )
	{
		// Get the index of the clicked item.
		for (int i=0; i< mListViewItems.size(); i++)
		{
			if ( mListViewItems[i] == listViewItem )
			{
				openColorScreen(i);
				break;
			}
		}
	}
}
