/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/**
 * @file BasicScreen.cpp
 * @author Emma Tresanszki
 *
 * This file contains the base screen of the application.
 * It contains the base screen constructors, and the creation of a main layout
 * that is common for some screens.
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
	mScreenWidth(0),
	mScreenHeight(0),
	mFontSize(0),
	mPaddingSize(0),
	mScreen(-1),
	mMainLayout(-1),
	mTopLabel(-1),
	mTopButtonPrevious(-1),
	mTopButtonNext(-1)
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
	if ( mScreenHeight < 600 )
	{
		mFontSize = FONT_SMALL;
		mPaddingSize = PADDING_SMALL;
	}
	else if( mScreenHeight < 800 )
	{
		mFontSize = FONT_LARGE;
		mPaddingSize = PADDING;
	}
	else
	{
		mFontSize = FONT_EXTRA_LARGE;
		mPaddingSize = PADDING;
	}

	// Set the font for iPhone devices.
	if (getPlatform() == IOS && mScreenHeight < 1000)
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
		mScreenHeight / 5);

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

	// Error handling for devices that do not support NativeUI.
	if ( -1 == mScreen )
	{
		maPanic(0, "NativeUI is only available on Android, iOS and Windows Phone.");
	}

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
