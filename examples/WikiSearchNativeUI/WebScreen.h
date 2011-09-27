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
 * @file WebScreen.h
 * @author Emma Tresanszki
 *
 * This file contains the last screen of the application
 * It contains a top layout with a hint label and a button, and a web view.
 */

#ifndef WEBSCREENNUI_H_
#define WEBSCREENNUI_H_

// Include the library for event handling listener.
#include "MAUtil/Environment.h"

// Include the base screen class.
#include "BasicScreen.h"

namespace WikiNativeUI
{
// Forward declaration.
class SummaryScreen;

/**
 * WebScreen: native UI screen
 * It contains a web view with the user selected article, and buttons
 * for Back and NewSearch actions.
 */
class WebScreen : public BasicScreen,
				  public MAUtil::CustomEventListener
{
public:

	/**
	 * Constructor.
	 * Pass the parent screen.
	 */
	WebScreen(SummaryScreen *parentScreen);

	/**
	 * Destructor.
	 */
    ~WebScreen();

	/**
	 * Show the screen.
	 */
	void showScreen();

	/**
	 * from CustomEventListener
	 * The custom event listener interface.
	 */
	void customEvent(const MAEvent& event);

	/**
	 * Handle events on screen's widgets.
	 */
	void widgetClicked(MAHandle widgetHandle);

	/**
	 * Open a web view for a certain title
	 * compose the url and display
	 * @param The article title for which we want to open a wikipedia definition.
	 */
	void openWebView(MAUtil::String title);

private: // methods

	/**
	 * Lay out the widgets (portrait mode).
	 */
	void setupUI();


private: // members
	/** Previous screen. */
	SummaryScreen* mPrevScreen;

	// NativeUI widgets handles
	/** The screen. */
	MAWidgetHandle mScreen;

	/** Main vertical layout. */
	MAWidgetHandle mMainLayout;

	/** Label in the top layout. */
	MAWidgetHandle mLabel;

	/** Back button - to SummaryScreen. */
	MAWidgetHandle mBackButton;

	/** Button for New Search - initiated from the HomeScreen. */
	MAWidgetHandle mNewSearchButton;

	/** The web view. */
	MAWidgetHandle mWebView;
};

} // namespace WikiNativeUI

#endif /* WEBSCREENNUI_H_ */
