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
 * @file BasicScreen.cpp
 *
 * This file contains the base screen of the application.
 * It contains the base screen constructors, and the creation of a main layout that is common for some screens.
 *
 * @author Emma Tresanszki
 */

// Include library for string conversions.
#include <mastdlib.h>

// Include util class for integer to string conversions.
#include <MAUtil/util.h>

// Include the header file of the class.
#include "BasicScreen.h"

// Utility functions for creating native UI widgets.
#include "Util.h"

namespace WikiNativeUI
{

/**
 * Constructor for the basic screen class.
 * Here instance variables are initialized.
 */
BasicScreen::BasicScreen():
	mScreen(-1),
	mMainLayout(-1),
	mTopLabel(-1),
	mTopButtonPrevious(-1),
	mTopButtonNext(-1),
	mScreenWidth(0),
	mScreenHeight(0)
{
	// Set the screen size, available for each screen.
	MAExtent screenSize = maGetScrSize();
	mScreenWidth = EXTENT_X(screenSize);
	mScreenHeight = EXTENT_Y(screenSize);

	// Set the necessary size for fonts and padding.
	setSizes();

	// Initialize the native UI.
	setupUI();
}

/**
 * Destructor.
 * Delete the main widget, and all it's children will be deleted also.
 * The base class destructor will be called after the derived class
 * destructor is called.
 */
BasicScreen::~BasicScreen()
{
	maWidgetDestroy(mMainLayout);
}

/**
 * Sets the font size and the padding size depending on the screen size.
 */
void BasicScreen::setSizes()
{
	// For small screens, use small fonts and padding.
	if (mScreenHeight < 600 )
	{
		mFontSize = FONT_SMALL;
		mPaddingSize = PADDING_SMALL;
	}
	else
	{
		mFontSize = FONT_LARGE;
		mPaddingSize = PADDING;
	}
}

/**
 * Show the screen.
 */
void BasicScreen::showScreen()
{
	maWidgetScreenShow( mScreen );
}

/**
 * Return a handle to the screen widget.
 * @return Handle to the screen widget.
 */
MAWidgetHandle BasicScreen::getScreen() const
{
	return mScreen;
}

/**
 * Return a handle to the main layout.
 * @return Handle to the main layout.
 */
MAWidgetHandle BasicScreen::getMainLayout()
{
	return mMainLayout;
}

/**
 * Return a handle to the top label.
 * @return Handle to the top label.
 */
MAWidgetHandle BasicScreen::getTopLabel()
{
	return mTopLabel;
}

/**
 * Return a handle to one of the top buttons (Back).
 * @return handle to the top button.
 */
MAWidgetHandle BasicScreen::getTopButtonLeft()
{
	return mTopButtonPrevious;
}

/**
 * Return a handle to one of the top buttons (Next).
 * @return handle to the top button.
 */
MAWidgetHandle BasicScreen::getTopButtonRight()
{
	return mTopButtonNext;
}

/**
 * Create the main layout, to fit the screen.
 * @return Handle to the main layout.
 */
MAWidgetHandle BasicScreen::createBaseMainLayout()
{
	// Create the screen layout.
	MAWidgetHandle screenLayout = maWidgetCreate(MAW_VERTICAL_LAYOUT);

	// Create the top layout, that is common for most screens.
	MAWidgetHandle topLayout = createTopLayout();
	maWidgetAddChild(screenLayout, topLayout);

	// Add a small spacer after the top layout.
	maWidgetAddChild(screenLayout, createSpacer(mScreenWidth, mPaddingSize));

	// Set the main layout size to fit the entire screen.
	setWidgetSize(
		screenLayout,
		MAW_CONSTANT_FILL_AVAILABLE_SPACE,
		MAW_CONSTANT_FILL_AVAILABLE_SPACE);

	return screenLayout;

}

/**
 * Utility function to create a layout that is used in all
 * screens, and it is contained in the main layout.
 * It is a top layout that contains Back and Next buttons.
 * The default text for the buttons is null,
 * and can be later set.
 * @return Access to the handle of the created layout.
 */
MAWidgetHandle BasicScreen::createTopLayout()
{
	MAWidgetHandle topLayout = maWidgetCreate(MAW_HORIZONTAL_LAYOUT);

	// Use a common background color in all the screens that
	// inherit from this class.
	setWidgetProperty(
		topLayout,
		MAW_WIDGET_BACKGROUND_COLOR,
		QUARTZ,
		16);

	// Arrange all it's children widgets at center.
	maWidgetSetProperty(
		topLayout,
		MAW_HORIZONTAL_LAYOUT_CHILD_VERTICAL_ALIGNMENT,
		MAW_ALIGNMENT_CENTER);
	maWidgetSetProperty(
		topLayout,
		MAW_HORIZONTAL_LAYOUT_CHILD_HORIZONTAL_ALIGNMENT,
		MAW_ALIGNMENT_CENTER);

	// Fully visible.
	maWidgetSetProperty(topLayout, MAW_WIDGET_ALPHA, "1.0");

	// Add info note & two buttons.
	mTopButtonPrevious = createButton("");
	maWidgetAddChild(topLayout, mTopButtonPrevious);

	mTopLabel = createLabel(
		MAW_CONSTANT_FILL_AVAILABLE_SPACE,
		" ",
		BLUE,
		mFontSize);
	maWidgetAddChild(topLayout,mTopLabel);

	mTopButtonNext = createButton("");
	maWidgetAddChild(topLayout, mTopButtonNext);

	// Set layout's size.
	setWidgetSize(
		topLayout,
		MAW_CONSTANT_FILL_AVAILABLE_SPACE,
		mScreenHeight / 8);

	return topLayout;
}

/**
 * Lay out the widgets (portrait mode).
 * These widgets are common for most screens.
 */
void BasicScreen::setupUI()
{
	// Create the main screen.
	mScreen = maWidgetCreate(MAW_SCREEN);

	// Create the main layout.
	mMainLayout = createBaseMainLayout();

	// Fill the background.
	setWidgetProperty(
		mMainLayout,
		MAW_WIDGET_BACKGROUND_COLOR,
		SLATE_GRAY,
		16);

	maWidgetAddChild(mScreen, mMainLayout);
}

} // namespace WikiNativeUI
